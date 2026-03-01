"""
Elegoo Owl Tank Bot - BLE Bluetooth Control Interface
Python/Tkinter GUI for controlling the robot via BLE (bleak)
Works on macOS/Windows/Linux without pairing or serial ports.
"""

import tkinter as tk
from tkinter import ttk, messagebox
import threading
import asyncio
import json
import time

from bleak import BleakClient, BleakScanner

# BLE UUIDs discovered from the DX-BT16
BLE_SERVICE_UUID = "0000ffe0-0000-1000-8000-00805f9b34fb"
BLE_WRITE_UUID = "0000ffe1-0000-1000-8000-00805f9b34fb"  # notify + read + write
BLE_NOTIFY_UUID = "0000ffe1-0000-1000-8000-00805f9b34fb"  # same char handles both


class RobotController:
    """Handles BLE communication with the robot (DX-BT16)"""

    DIRECTION_MAP = {
        "Forward": 1,
        "Backward": 2,
        "Left": 3,
        "Right": 4,
        "LeftForward": 5,
        "LeftBackward": 6,
        "RightForward": 7,
        "RightBackward": 8,
        "Stop": 9,
    }

    def __init__(self):
        self.client = None
        self.connected = False
        self.command_number = 0
        self.device_address = None
        self._loop = None
        self._thread = None
        self.on_data_received = None  # optional callback(str)

    # ------------------------------------------------------------------
    # Internal: run a coroutine on the background event loop
    # ------------------------------------------------------------------
    def _run(self, coro):
        if self._loop is None or not self._loop.is_running():
            raise Exception("Event loop not running")
        future = asyncio.run_coroutine_threadsafe(coro, self._loop)
        return future.result(timeout=10)

    def _start_loop(self):
        """Start a dedicated asyncio event loop in a background thread."""
        self._loop = asyncio.new_event_loop()
        self._thread = threading.Thread(target=self._loop.run_forever, daemon=True)
        self._thread.start()

    # ------------------------------------------------------------------
    # Scan
    # ------------------------------------------------------------------
    def find_robot_devices(self):
        """Scan for BLE devices and return list of (address, name)."""

        async def _scan():
            devices = await BleakScanner.discover(timeout=5.0)
            return [(d.address, d.name or "Unknown") for d in devices]

        if self._loop is None:
            self._start_loop()
        future = asyncio.run_coroutine_threadsafe(_scan(), self._loop)
        return future.result(timeout=15)

    # ------------------------------------------------------------------
    # Connect / Disconnect
    # ------------------------------------------------------------------
    def connect(self, address):
        if self._loop is None:
            self._start_loop()

        self.device_address = address

        async def _connect():
            self.client = BleakClient(address)
            await self.client.connect()
            # Subscribe to notifications so we can read responses
            await self.client.start_notify(BLE_NOTIFY_UUID, self._notification_handler)
            return self.client.is_connected

        result = asyncio.run_coroutine_threadsafe(_connect(), self._loop).result(
            timeout=15
        )
        self.connected = result
        return result

    def disconnect(self):
        if self.client is None:
            self.connected = False
            return

        async def _disconnect():
            try:
                await self.client.stop_notify(BLE_NOTIFY_UUID)
            except Exception:
                pass
            await self.client.disconnect()

        try:
            asyncio.run_coroutine_threadsafe(_disconnect(), self._loop).result(
                timeout=5
            )
        except Exception:
            pass
        self.connected = False

    def _notification_handler(self, sender, data: bytearray):
        """Called when the robot sends data back."""
        text = data.decode("utf-8", errors="replace")
        if self.on_data_received:
            self.on_data_received(text)

    # ------------------------------------------------------------------
    # Commands
    # ------------------------------------------------------------------
    def _write(self, command_dict):
        if not self.connected or self.client is None:
            raise Exception("Not connected")
        payload = json.dumps(command_dict).encode("utf-8")
        print(f"Sending: {command_dict}")

        async def _send():
            await self.client.write_gatt_char(BLE_WRITE_UUID, payload, response=False)

        asyncio.run_coroutine_threadsafe(_send(), self._loop).result(timeout=5)

    def send_motion_command(self, direction, speed=200):
        if direction not in self.DIRECTION_MAP:
            raise ValueError(f"Invalid direction: {direction}")
        self.command_number += 1
        self._write(
            {
                "N": 102,
                "D1": self.DIRECTION_MAP[direction],
                "H": self.command_number % 100,
            }
        )

    def send_stop_command(self):
        self.command_number += 1
        self._write(
            {
                "N": 100,
                "H": self.command_number % 100,
            }
        )


# ======================================================================
# GUI
# ======================================================================


class RobotControllerGUI:
    def __init__(self, root):
        self.root = root
        self.root.title("Elegoo Owl Tank Bot - BLE Controller")
        self.root.geometry("620x780")
        self.root.resizable(False, False)

        self.robot = RobotController()
        self.robot.on_data_received = self._on_robot_data

        self.status_var = tk.StringVar(value="Disconnected")
        self.device_var = tk.StringVar()
        self._devices = []  # list of (address, name)

        self._setup_ui()

    # ------------------------------------------------------------------
    # UI setup
    # ------------------------------------------------------------------
    def _setup_ui(self):
        ttk.Label(
            self.root,
            text="Elegoo Owl Tank Bot - BLE Controller",
            font=("Arial", 16, "bold"),
        ).pack(pady=10)

        # --- Connection ---
        conn_frame = ttk.LabelFrame(self.root, text="Connection", padding=10)
        conn_frame.pack(padx=10, pady=5, fill="x")

        btn_row = ttk.Frame(conn_frame)
        btn_row.pack(pady=5)
        ttk.Button(btn_row, text="Scan BLE Devices", command=self._scan).pack(
            side="left", padx=5
        )
        ttk.Button(btn_row, text="Connect", command=self._connect).pack(
            side="left", padx=5
        )
        ttk.Button(btn_row, text="Disconnect", command=self._disconnect).pack(
            side="left", padx=5
        )

        dev_row = ttk.Frame(conn_frame)
        dev_row.pack(pady=5, fill="x")
        ttk.Label(dev_row, text="Device:").pack(side="left", padx=5)
        self.device_combo = ttk.Combobox(
            dev_row, textvariable=self.device_var, state="readonly", width=52
        )
        self.device_combo.pack(side="left", padx=5, fill="x", expand=True)

        stat_row = ttk.Frame(conn_frame)
        stat_row.pack(pady=5, fill="x")
        ttk.Label(stat_row, text="Status:").pack(side="left", padx=5)
        self._status_label = ttk.Label(
            stat_row,
            textvariable=self.status_var,
            foreground="red",
            font=("Arial", 10, "bold"),
        )
        self._status_label.pack(side="left", padx=5)

        # --- Motion ---
        motion_frame = ttk.LabelFrame(self.root, text="Motion Control", padding=10)
        motion_frame.pack(padx=10, pady=5, fill="both")

        pad = ttk.Frame(motion_frame)
        pad.pack(pady=20)

        def btn(parent, label, cmd, **pack_kwargs):
            ttk.Button(parent, text=label, width=9, command=cmd).pack(**pack_kwargs)

        row1 = ttk.Frame(pad)
        row1.pack()
        btn(row1, "↖ L-Fwd", lambda: self._cmd("LeftForward"), side="left", padx=5)
        btn(row1, "↑ Forward", lambda: self._cmd("Forward"), side="left", padx=5)
        btn(row1, "↗ R-Fwd", lambda: self._cmd("RightForward"), side="left", padx=5)

        row2 = ttk.Frame(pad)
        row2.pack(pady=10)
        btn(row2, "← Left", lambda: self._cmd("Left"), side="left", padx=5)
        btn(row2, "⏹ Stop", self._stop, side="left", padx=5)
        btn(row2, "Right →", lambda: self._cmd("Right"), side="left", padx=5)

        row3 = ttk.Frame(pad)
        row3.pack()
        btn(row3, "↙ L-Back", lambda: self._cmd("LeftBackward"), side="left", padx=5)
        btn(row3, "↓ Backward", lambda: self._cmd("Backward"), side="left", padx=5)
        btn(row3, "↘ R-Back", lambda: self._cmd("RightBackward"), side="left", padx=5)

        # --- Keyboard shortcuts ---
        info_frame = ttk.LabelFrame(self.root, text="Keyboard Shortcuts", padding=8)
        info_frame.pack(padx=10, pady=5, fill="x")
        for line in [
            "Arrow keys = Forward / Backward / Left / Right",
            "W = L-Fwd  |  A = L-Back  |  E = R-Fwd  |  D = R-Back",
            "Space = Stop",
        ]:
            ttk.Label(info_frame, text=line, font=("Arial", 9)).pack(anchor="w")

        # --- Log ---
        log_frame = ttk.LabelFrame(self.root, text="Log", padding=5)
        log_frame.pack(padx=10, pady=5, fill="both", expand=True)
        self.log_text = tk.Text(log_frame, height=10, width=72, state="disabled")
        scrollbar = ttk.Scrollbar(log_frame, command=self.log_text.yview)
        self.log_text.configure(yscrollcommand=scrollbar.set)
        self.log_text.pack(side="left", fill="both", expand=True)
        scrollbar.pack(side="right", fill="y")

        # Keyboard bindings
        bindings = {
            "<Up>": "Forward",
            "<Down>": "Backward",
            "<Left>": "Left",
            "<Right>": "Right",
            "w": "LeftForward",
            "a": "LeftBackward",
            "e": "RightForward",
            "d": "RightBackward",
        }
        for key, direction in bindings.items():
            self.root.bind(
                key,
                lambda e, d=direction: self._cmd(d) if self.robot.connected else None,
            )
        self.root.bind(
            "<space>", lambda e: self._stop() if self.robot.connected else None
        )

    # ------------------------------------------------------------------
    # Helpers
    # ------------------------------------------------------------------
    def _log(self, msg):
        self.log_text.config(state="normal")
        self.log_text.insert("end", f"[{time.strftime('%H:%M:%S')}] {msg}\n")
        self.log_text.see("end")
        self.log_text.config(state="disabled")

    def _set_status(self, text, color="red"):
        self.status_var.set(text)
        self._status_label.config(foreground=color)

    def _on_robot_data(self, text):
        """Called from the BLE thread — post to Tkinter safely."""
        self.root.after(0, lambda: self._log(f"← Robot: {text.strip()}"))

    # ------------------------------------------------------------------
    # Actions
    # ------------------------------------------------------------------
    def _scan(self):
        self._log("Scanning for BLE devices (5 s)…")
        self._set_status("Scanning…", "orange")
        self.root.update()

        def do_scan():
            try:
                devices = self.robot.find_robot_devices()
                self._devices = devices
                labels = [f"{name}  ({addr})" for addr, name in devices]
                self.root.after(0, lambda: self._scan_done(labels, len(devices)))
            except Exception as e:
                self.root.after(0, lambda: self._log(f"Scan error: {e}"))
                self.root.after(0, lambda: self._set_status("Scan failed"))

        threading.Thread(target=do_scan, daemon=True).start()

    def _scan_done(self, labels, count):
        self.device_combo["values"] = labels
        if count:
            # Auto-select Elegoo if found
            for i, lbl in enumerate(labels):
                if "ELEGOO" in lbl.upper() or "BT16" in lbl.upper():
                    self.device_combo.current(i)
                    break
            else:
                self.device_combo.current(0)
            self._log(f"Found {count} device(s)")
            self._set_status("Ready to connect", "blue")
        else:
            self._log("No devices found — is the robot powered on?")
            self._set_status("No devices found")

    def _connect(self):
        if not self.device_var.get():
            messagebox.showwarning("No device", "Scan first, then select a device.")
            return

        # Find matching address
        selected = self.device_var.get()
        address = None
        for addr, name in self._devices:
            if addr in selected:
                address = addr
                break

        if not address:
            messagebox.showerror("Error", "Could not parse device address.")
            return

        self._log(f"Connecting to {selected}…")
        self._set_status("Connecting…", "orange")

        def do_connect():
            try:
                self.robot.connect(address)
                self.root.after(0, self._connect_ok)
            except Exception as e:
                self.root.after(0, lambda: self._connect_fail(str(e)))

        threading.Thread(target=do_connect, daemon=True).start()

    def _connect_ok(self):
        self._set_status("Connected ✓", "green")
        self._log("Connected!")

    def _connect_fail(self, msg):
        self._set_status("Failed")
        self._log(f"Connection failed: {msg}")
        messagebox.showerror("Connection Failed", msg)

    def _disconnect(self):
        self.robot.disconnect()
        self._set_status("Disconnected")
        self._log("Disconnected")

    def _cmd(self, direction):
        if not self.robot.connected:
            messagebox.showwarning("Not connected", "Connect to the robot first.")
            return
        try:
            self.robot.send_motion_command(direction)
            self._log(f"→ {direction}")
        except Exception as e:
            self._log(f"Error: {e}")

    def _stop(self):
        if not self.robot.connected:
            return
        try:
            self.robot.send_stop_command()
            self._log("→ Stop")
        except Exception as e:
            self._log(f"Error: {e}")


# ======================================================================
# Entry point
# ======================================================================


def main():
    root = tk.Tk()
    gui = RobotControllerGUI(root)

    def on_closing():
        if gui.robot.connected:
            gui.robot.disconnect()
        root.destroy()

    root.protocol("WM_DELETE_WINDOW", on_closing)
    root.mainloop()


if __name__ == "__main__":
    main()
