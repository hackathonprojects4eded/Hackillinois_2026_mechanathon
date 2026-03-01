"""
Elegoo Owl Tank Bot - BLE Bluetooth Control Interface
Python/Tkinter GUI for controlling the robot via BLE (bleak)
Works on macOS/Windows/Linux without pairing or serial ports.

Features:
  - BLE connection via bleak (no pairing / serial port needed)
  - Idle packet {"N":110} sent automatically when no key is held
  - Live orientation panel rendering the robot as a rectangle from above
    driven by quaternion data streamed from the robot over BLE
"""

import tkinter as tk
from tkinter import ttk, messagebox
import threading
import asyncio
import json
import math
import time

from bleak import BleakClient, BleakScanner

# BLE UUIDs discovered from the DX-BT16
BLE_SERVICE_UUID = "0000ffe0-0000-1000-8000-00805f9b34fb"
BLE_WRITE_UUID = "0000ffe1-0000-1000-8000-00805f9b34fb"
BLE_NOTIFY_UUID = "0000ffe1-0000-1000-8000-00805f9b34fb"

IDLE_INTERVAL_MS = 250  # how often to send idle packet (ms)
MOTION_REPEAT_MS = 200  # how often to re-send a held direction (ms)


# ======================================================================
# Robot controller (BLE backend)
# ======================================================================


class RobotController:
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
        self.on_data_received = None  # callback(str)

    def _start_loop(self):
        self._loop = asyncio.new_event_loop()
        self._thread = threading.Thread(target=self._loop.run_forever, daemon=True)
        self._thread.start()

    def _run_async(self, coro, timeout=10):
        if self._loop is None:
            self._start_loop()
        return asyncio.run_coroutine_threadsafe(coro, self._loop).result(
            timeout=timeout
        )

    def find_robot_devices(self):
        async def _scan():
            devices = await BleakScanner.discover(timeout=5.0)
            return [(d.address, d.name or "Unknown") for d in devices]

        if self._loop is None:
            self._start_loop()
        return asyncio.run_coroutine_threadsafe(_scan(), self._loop).result(timeout=15)

    def connect(self, address):
        self.device_address = address

        async def _connect():
            self.client = BleakClient(address)
            await self.client.connect()
            await self.client.start_notify(BLE_NOTIFY_UUID, self._notification_handler)
            return self.client.is_connected

        result = self._run_async(_connect(), timeout=15)
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
            self._run_async(_disconnect(), timeout=5)
        except Exception:
            pass
        self.connected = False

    def _notification_handler(self, sender, data: bytearray):
        text = data.decode("utf-8", errors="replace")
        if self.on_data_received:
            self.on_data_received(text)

    def _write(self, command_dict):
        if not self.connected or self.client is None:
            raise Exception("Not connected")
        payload = json.dumps(command_dict, separators=(",", ":")).encode("utf-8")

        async def _send():
            await self.client.write_gatt_char(BLE_WRITE_UUID, payload, response=False)

        asyncio.run_coroutine_threadsafe(_send(), self._loop).result(timeout=5)

    def send_motion_command(self, direction):
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
        self._write({"N": 100, "H": self.command_number % 100})

    def send_idle_command(self):
        """Sent continuously while no movement key is held."""
        self._write({"N": 110})

    def send_mode_command(self):
        self.command_number += 1
        self._write({"N": 105, "H": self.command_number % 100})


# ======================================================================
# Orientation canvas
# ======================================================================


class OrientationDisplay(tk.Canvas):
    """
    Top-down rectangle representing the robot, rotated by yaw from IMU.

    Serial format expected:  quat:W,X,Y,Z
    """

    SIZE = 210  # canvas width & height (square)
    RW = 32  # robot half-width  (pixels)
    RL = 52  # robot half-length (pixels)

    def __init__(self, parent, **kwargs):
        super().__init__(
            parent,
            width=self.SIZE,
            height=self.SIZE,
            bg="#1a1a2e",
            highlightthickness=0,
            **kwargs,
        )
        self._yaw_deg = 0.0
        self.raw_var = tk.StringVar(value="W=—  X=—  Y=—  Z=—")
        self._draw()

    def update_quaternion(self, w, x, y, z):
        yaw_rad = math.atan2(2.0 * (w * z + x * y), 1.0 - 2.0 * (y * y + z * z))
        self._yaw_deg = math.degrees(yaw_rad)
        self.raw_var.set(f"W={w:.3f}  X={x:.3f}  Y={y:.3f}  Z={z:.3f}")
        self._draw()

    def _draw(self):
        self.delete("all")
        cx = cy = self.SIZE / 2
        a = math.radians(self._yaw_deg)
        ca, sa = math.cos(a), math.sin(a)

        def rot(px, py):
            return cx + px * ca - py * sa, cy + px * sa + py * ca

        # compass ring (fixed, not rotated)
        r = 88
        self.create_oval(
            cx - r, cy - r, cx + r, cy + r, outline="#2a2a4a", width=1, dash=(3, 5)
        )
        for lbl, lx, ly in [
            ("N", 0, -r + 10),
            ("S", 0, r - 10),
            ("W", -r + 10, 0),
            ("E", r - 10, 0),
        ]:
            self.create_text(
                cx + lx, cy + ly, text=lbl, fill="#44446a", font=("Arial", 8, "bold")
            )

        # robot body
        corners = [
            rot(-self.RW, -self.RL),
            rot(self.RW, -self.RL),
            rot(self.RW, self.RL),
            rot(-self.RW, self.RL),
        ]
        flat = [c for pt in corners for c in pt]
        self.create_polygon(flat, fill="#2d5fa6", outline="#6aaae8", width=2)

        # front stripe
        self.create_line(
            *rot(-self.RW, -self.RL), *rot(self.RW, -self.RL), fill="#e8e8ff", width=3
        )

        # forward arrow
        self.create_line(
            *rot(0, -self.RL + 6),
            *rot(0, -self.RL - 20),
            fill="#f5c542",
            width=2,
            arrow=tk.LAST,
            arrowshape=(10, 13, 4),
        )

        # track wheels (visual detail)
        for side in (-1, 1):
            tx = side * (self.RW + 5)
            for ty_top, ty_bot in [(-self.RL, -self.RL // 2), (self.RL // 2, self.RL)]:
                t1, t2 = rot(tx - 4, ty_top), rot(tx + 4, ty_top)
                b1, b2 = rot(tx - 4, ty_bot), rot(tx + 4, ty_bot)
                wheel = [c for pt in [t1, t2, b2, b1] for c in pt]
                self.create_polygon(wheel, fill="#555577", outline="#8888aa", width=1)

        # yaw label
        self.create_text(
            cx,
            self.SIZE - 10,
            text=f"Yaw  {self._yaw_deg:+.1f}°",
            fill="#9999bb",
            font=("Arial", 9),
        )


# ======================================================================
# GUI
# ======================================================================


class RobotControllerGUI:
    def __init__(self, root):
        self.root = root
        self.root.title("Elegoo Owl Tank Bot — BLE Controller")
        self.root.resizable(False, False)

        self.robot = RobotController()
        self.robot.on_data_received = self._on_robot_data

        self.status_var = tk.StringVar(value="Disconnected")
        self.device_var = tk.StringVar()
        self._devices = []
        self._auto_mode = False

        # key-hold state
        self._held_direction = None
        self._last_sent_direction = None  # track what was last sent to avoid repeats

        # serial buffer (BLE frames may be partial)
        self._serial_buf = ""

        self._setup_ui()
        self._idle_loop()  # start the idle/held ticker

    # ------------------------------------------------------------------
    # UI
    # ------------------------------------------------------------------
    def _setup_ui(self):
        outer = ttk.Frame(self.root)
        outer.pack(fill="both", expand=True)

        left = ttk.Frame(outer)
        left.pack(side="left", fill="both", expand=True, padx=(10, 4), pady=10)

        right = ttk.LabelFrame(outer, text="Orientation (top-down)", padding=8)
        right.pack(side="right", fill="y", padx=(4, 10), pady=10)

        # title
        ttk.Label(left, text="Elegoo Owl Tank Bot", font=("Arial", 14, "bold")).pack(
            pady=(0, 5)
        )

        # connection
        cf = ttk.LabelFrame(left, text="Connection", padding=8)
        cf.pack(fill="x", pady=3)

        br = ttk.Frame(cf)
        br.pack(pady=3)
        ttk.Button(br, text="Scan BLE", command=self._scan).pack(side="left", padx=3)
        ttk.Button(br, text="Connect", command=self._connect).pack(side="left", padx=3)
        ttk.Button(br, text="Disconnect", command=self._disconnect).pack(
            side="left", padx=3
        )

        dr = ttk.Frame(cf)
        dr.pack(pady=3, fill="x")
        ttk.Label(dr, text="Device:").pack(side="left", padx=3)
        self.device_combo = ttk.Combobox(
            dr, textvariable=self.device_var, state="readonly", width=36
        )
        self.device_combo.pack(side="left", padx=3, fill="x", expand=True)

        sr = ttk.Frame(cf)
        sr.pack(pady=3, fill="x")
        ttk.Label(sr, text="Status:").pack(side="left", padx=3)
        self._status_lbl = ttk.Label(
            sr,
            textvariable=self.status_var,
            foreground="red",
            font=("Arial", 10, "bold"),
        )
        self._status_lbl.pack(side="left", padx=3)

        # motion
        mf = ttk.LabelFrame(left, text="Motion Control", padding=8)
        mf.pack(fill="x", pady=3)

        pad = ttk.Frame(mf)
        pad.pack(pady=8)

        def dpad_btn(parent, label, direction, **pkw):
            b = tk.Button(parent, text=label, width=8, relief="raised", cursor="hand2")
            b.pack(**pkw)
            b.bind("<ButtonPress-1>", lambda e, d=direction: self._key_press(d))
            b.bind("<ButtonRelease-1>", lambda e: self._key_release())
            return b

        r1 = ttk.Frame(pad)
        r1.pack()
        dpad_btn(r1, "↖ L-Fwd", "LeftForward", side="left", padx=4)
        dpad_btn(r1, "↑ Forward", "Forward", side="left", padx=4)
        dpad_btn(r1, "↗ R-Fwd", "RightForward", side="left", padx=4)

        r2 = ttk.Frame(pad)
        r2.pack(pady=7)
        dpad_btn(r2, "← Left", "Left", side="left", padx=4)
        tk.Button(
            r2,
            text="⏹ Stop",
            width=8,
            relief="raised",
            cursor="hand2",
            command=self._stop,
        ).pack(side="left", padx=4)
        dpad_btn(r2, "Right →", "Right", side="left", padx=4)

        r3 = ttk.Frame(pad)
        r3.pack()
        dpad_btn(r3, "↙ L-Back", "LeftBackward", side="left", padx=4)
        dpad_btn(r3, "↓ Backward", "Backward", side="left", padx=4)
        dpad_btn(r3, "↘ R-Back", "RightBackward", side="left", padx=4)

        mr = ttk.Frame(mf)
        mr.pack(pady=5)
        self._mode_btn = tk.Button(
            mr,
            text="🤖  Switch to Auto Mode",
            width=22,
            bg="#4a90d9",
            fg="white",
            font=("Arial", 10, "bold"),
            relief="raised",
            cursor="hand2",
            command=self._toggle_mode,
        )
        self._mode_btn.pack()

        # keyboard shortcuts info
        kf = ttk.LabelFrame(left, text="Keyboard Shortcuts", padding=5)
        kf.pack(fill="x", pady=3)
        for line in [
            "Hold arrow keys = move  |  Release = idle (N:110)",
            "W=L-Fwd  A=L-Back  E=R-Fwd  D=R-Back",
            "Space = stop once",
        ]:
            ttk.Label(kf, text=line, font=("Arial", 9)).pack(anchor="w")

        # log
        lf = ttk.LabelFrame(left, text="Log", padding=4)
        lf.pack(fill="both", expand=True, pady=3)
        self.log_text = tk.Text(lf, height=7, width=50, state="disabled")
        sb = ttk.Scrollbar(lf, command=self.log_text.yview)
        self.log_text.configure(yscrollcommand=sb.set)
        self.log_text.pack(side="left", fill="both", expand=True)
        sb.pack(side="right", fill="y")

        # orientation panel
        self._orient = OrientationDisplay(right)
        self._orient.pack()
        ttk.Label(
            right,
            textvariable=self._orient.raw_var,
            font=("Courier", 8),
            foreground="#888",
        ).pack(pady=(4, 0))

        # keyboard bindings (press + release pairs)
        key_dir = {
            "Up": "Forward",
            "Down": "Backward",
            "Left": "Left",
            "Right": "Right",
            "w": "LeftForward",
            "a": "LeftBackward",
            "e": "RightForward",
            "d": "RightBackward",
        }
        for k, d in key_dir.items():
            self.root.bind(f"<KeyPress-{k}>", lambda e, dd=d: self._key_press(dd))
            self.root.bind(f"<KeyRelease-{k}>", lambda e: self._key_release())
        self.root.bind("<KeyPress-space>", lambda e: self._stop())

    # ------------------------------------------------------------------
    # Idle / held-key loop (runs every IDLE_INTERVAL_MS via root.after)
    # ------------------------------------------------------------------
    def _idle_loop(self):
        if self.robot.connected:
            try:
                if self._held_direction is not None:
                    self.robot.send_motion_command(self._held_direction)
                else:
                    self.robot.send_idle_command()
            except Exception:
                pass
        self.root.after(IDLE_INTERVAL_MS, self._idle_loop)

    def _key_press(self, direction):
        if self.robot.connected:
            self._held_direction = direction

    def _key_release(self):
        self._held_direction = None

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
        self._status_lbl.config(foreground=color)

    def _on_robot_data(self, text):
        self.root.after(0, lambda t=text: self._process_serial(t))

    def _process_serial(self, chunk):
        self._serial_buf += chunk
        while "\n" in self._serial_buf:
            line, self._serial_buf = self._serial_buf.split("\n", 1)
            line = line.strip()
            if not line:
                continue
            if line.startswith("quat:"):
                self._parse_quaternion(line[5:])
            else:
                self._log(f"← {line}")

    def _parse_quaternion(self, data):
        try:
            parts = data.split(",")
            if len(parts) != 4:
                return
            w, x, y, z = (float(p) for p in parts)
            self._orient.update_quaternion(w, x, y, z)
        except ValueError:
            pass

    # ------------------------------------------------------------------
    # Actions
    # ------------------------------------------------------------------
    def _scan(self):
        self._log("Scanning for BLE devices (5 s)…")
        self._set_status("Scanning…", "orange")

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
        selected = self.device_var.get()
        address = next((addr for addr, _ in self._devices if addr in selected), None)
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
        self._held_direction = None
        self._set_status("Disconnected")
        self._log("Disconnected")

    def _toggle_mode(self):
        if not self.robot.connected:
            messagebox.showwarning("Not connected", "Connect to the robot first.")
            return
        try:
            self.robot.send_mode_command()
            self._auto_mode = not self._auto_mode
            if self._auto_mode:
                self._mode_btn.config(text="🕹  Switch to Manual Mode", bg="#e07b39")
                self._log("→ Mode: Auto (N:105)")
            else:
                self._mode_btn.config(text="🤖  Switch to Auto Mode", bg="#4a90d9")
                self._log("→ Mode: Manual (N:105)")
        except Exception as e:
            self._log(f"Error: {e}")

    def _stop(self):
        if not self.robot.connected:
            return
        self._held_direction = None
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
