"""
Elegoo Owl Tank Bot - Bluetooth Control Interface
Python/Tkinter GUI for controlling the robot via Bluetooth
"""

import tkinter as tk
from tkinter import ttk, messagebox
import threading
import json
import time

try:
    import bluetooth

    BLUETOOTH_AVAILABLE = True
except ImportError:
    BLUETOOTH_AVAILABLE = False
    print("Warning: pybluez not installed. Install with: pip install pybluez")


class RobotController:
    """Handles Bluetooth communication with the robot"""

    # Motion directions (must match the robot firmware)
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
        self.socket = None
        self.connected = False
        self.command_number = 0

    def find_robot_devices(self):
        """Scan for nearby Bluetooth devices"""
        if not BLUETOOTH_AVAILABLE:
            return []

        print("Scanning for Bluetooth devices...")
        try:
            nearby_devices = bluetooth.discover_devices(duration=8, lookup_names=True)
            print(f"Found {len(nearby_devices)} devices")
            return nearby_devices
        except Exception as e:
            print(f"Error during device scan: {e}")
            return []

    def connect(self, device_address):
        """Connect to the robot via Bluetooth"""
        if not BLUETOOTH_AVAILABLE:
            raise Exception("PyBluez not available. Install with: pip install pybluez")

        try:
            self.socket = bluetooth.BluetoothSocket(bluetooth.RFCOMM)
            self.socket.connect((device_address, 1))
            self.socket.settimeout(2.0)
            self.connected = True
            print(f"Connected to {device_address}")
            return True
        except Exception as e:
            self.connected = False
            print(f"Connection failed: {e}")
            raise

    def disconnect(self):
        """Disconnect from the robot"""
        if self.socket:
            try:
                self.socket.close()
            except:
                pass
            self.connected = False
            print("Disconnected from robot")

    def send_motion_command(self, direction, speed=200):
        """
        Send a motion control command to the robot

        Args:
            direction: Motion direction (Forward, Backward, etc.)
            speed: Speed value 0-255 (currently robot uses fixed speed in Rocker mode)
        """
        if not self.connected or not self.socket:
            raise Exception("Not connected to robot")

        if direction not in self.DIRECTION_MAP:
            raise ValueError(f"Invalid direction: {direction}")

        self.command_number += 1
        direction_code = self.DIRECTION_MAP[direction]

        # Send JSON command: N=102 is rocker/joystick control mode
        command = {"N": 102, "D1": direction_code, "H": self.command_number % 100}

        command_str = json.dumps(command)
        print(f"Sending: {command_str}")

        try:
            self.socket.send(command_str)
        except Exception as e:
            print(f"Error sending command: {e}")
            self.connected = False
            raise

    def send_stop_command(self):
        """Send stop/clear command to the robot"""
        if not self.connected or not self.socket:
            raise Exception("Not connected to robot")

        self.command_number += 1
        command = {"N": 100, "H": self.command_number % 100}

        command_str = json.dumps(command)
        print(f"Sending: {command_str}")

        try:
            self.socket.send(command_str)
        except Exception as e:
            print(f"Error sending stop command: {e}")
            self.connected = False
            raise


class RobotControllerGUI:
    """Tkinter GUI for the robot controller"""

    def __init__(self, root):
        self.root = root
        self.root.title("Elegoo Owl Tank Bot - Bluetooth Controller")
        self.root.geometry("600x750")
        self.root.resizable(False, False)

        self.robot = RobotController()
        self.status_var = tk.StringVar(value="Disconnected")
        self.device_var = tk.StringVar()
        self.command_thread = None

        self.setup_ui()

    def setup_ui(self):
        """Create the user interface"""

        # Title
        title_frame = ttk.Frame(self.root)
        title_frame.pack(pady=10)
        ttk.Label(
            title_frame,
            text="Elegoo Owl Tank Bot Controller",
            font=("Arial", 16, "bold"),
        ).pack()

        # Connection Frame
        conn_frame = ttk.LabelFrame(self.root, text="Connection", padding=10)
        conn_frame.pack(padx=10, pady=5, fill="x")

        button_frame = ttk.Frame(conn_frame)
        button_frame.pack(pady=5)
        ttk.Button(button_frame, text="Scan Devices", command=self.scan_devices).pack(
            side="left", padx=5
        )
        ttk.Button(button_frame, text="Connect", command=self.connect_robot).pack(
            side="left", padx=5
        )
        ttk.Button(button_frame, text="Disconnect", command=self.disconnect_robot).pack(
            side="left", padx=5
        )

        device_frame = ttk.Frame(conn_frame)
        device_frame.pack(pady=5, fill="x")
        ttk.Label(device_frame, text="Device:").pack(side="left", padx=5)

        self.device_combo = ttk.Combobox(
            device_frame, textvariable=self.device_var, state="readonly", width=50
        )
        self.device_combo.pack(side="left", padx=5, fill="x", expand=True)

        status_frame = ttk.Frame(conn_frame)
        status_frame.pack(pady=5, fill="x")
        ttk.Label(status_frame, text="Status:").pack(side="left", padx=5)
        ttk.Label(
            status_frame,
            textvariable=self.status_var,
            foreground="red",
            font=("Arial", 10, "bold"),
        ).pack(side="left", padx=5)

        # Motion Control Frame
        motion_frame = ttk.LabelFrame(self.root, text="Motion Control", padding=10)
        motion_frame.pack(padx=10, pady=5, fill="both", expand=True)

        # Directional pad layout
        pad_frame = ttk.Frame(motion_frame)
        pad_frame.pack(pady=20)

        # Row 1: Diagonal forward
        row1 = ttk.Frame(pad_frame)
        row1.pack()
        ttk.Button(
            row1,
            text="↖ L-Fwd",
            width=8,
            command=lambda: self.send_command("LeftForward"),
        ).pack(side="left", padx=5)
        ttk.Button(
            row1,
            text="↑ Forward",
            width=8,
            command=lambda: self.send_command("Forward"),
        ).pack(side="left", padx=5)
        ttk.Button(
            row1,
            text="↗ R-Fwd",
            width=8,
            command=lambda: self.send_command("RightForward"),
        ).pack(side="left", padx=5)

        # Row 2: Left-right
        row2 = ttk.Frame(pad_frame)
        row2.pack(pady=10)
        ttk.Button(
            row2, text="← Left", width=8, command=lambda: self.send_command("Left")
        ).pack(side="left", padx=5)
        ttk.Button(row2, text="Stop", width=8, command=self.stop_robot).pack(
            side="left", padx=5
        )
        ttk.Button(
            row2, text="Right →", width=8, command=lambda: self.send_command("Right")
        ).pack(side="left", padx=5)

        # Row 3: Diagonal backward
        row3 = ttk.Frame(pad_frame)
        row3.pack()
        ttk.Button(
            row3,
            text="↙ L-Back",
            width=8,
            command=lambda: self.send_command("LeftBackward"),
        ).pack(side="left", padx=5)
        ttk.Button(
            row3,
            text="↓ Backward",
            width=8,
            command=lambda: self.send_command("Backward"),
        ).pack(side="left", padx=5)
        ttk.Button(
            row3,
            text="↘ R-Back",
            width=8,
            command=lambda: self.send_command("RightBackward"),
        ).pack(side="left", padx=5)

        # Keyboard shortcuts info
        info_frame = ttk.LabelFrame(
            self.root, text="Keyboard Shortcuts (when connected)", padding=10
        )
        info_frame.pack(padx=10, pady=5, fill="x")

        shortcuts = [
            "↑ = Forward  |  ↓ = Backward  |  ← = Left  |  → = Right",
            "W = L-Fwd  |  A = L-Back  |  E = R-Fwd  |  D = R-Back  |  Space = Stop",
        ]
        for shortcut in shortcuts:
            ttk.Label(info_frame, text=shortcut, font=("Arial", 9)).pack(anchor="w")

        # Log Frame
        log_frame = ttk.LabelFrame(self.root, text="Command Log", padding=5)
        log_frame.pack(padx=10, pady=5, fill="both", expand=True)

        self.log_text = tk.Text(log_frame, height=8, width=70, state="disabled")
        self.log_text.pack(fill="both", expand=True)

        # Bind keyboard events
        self.root.bind(
            "<Up>",
            lambda e: self.send_command("Forward") if self.robot.connected else None,
        )
        self.root.bind(
            "<Down>",
            lambda e: self.send_command("Backward") if self.robot.connected else None,
        )
        self.root.bind(
            "<Left>",
            lambda e: self.send_command("Left") if self.robot.connected else None,
        )
        self.root.bind(
            "<Right>",
            lambda e: self.send_command("Right") if self.robot.connected else None,
        )
        self.root.bind(
            "w",
            lambda e: (
                self.send_command("LeftForward") if self.robot.connected else None
            ),
        )
        self.root.bind(
            "a",
            lambda e: (
                self.send_command("LeftBackward") if self.robot.connected else None
            ),
        )
        self.root.bind(
            "e",
            lambda e: (
                self.send_command("RightForward") if self.robot.connected else None
            ),
        )
        self.root.bind(
            "d",
            lambda e: (
                self.send_command("RightBackward") if self.robot.connected else None
            ),
        )
        self.root.bind(
            "<space>", lambda e: self.stop_robot() if self.robot.connected else None
        )

    def log_message(self, message):
        """Add message to the log"""
        self.log_text.config(state="normal")
        timestamp = time.strftime("%H:%M:%S")
        self.log_text.insert("end", f"[{timestamp}] {message}\n")
        self.log_text.see("end")
        self.log_text.config(state="disabled")

    def scan_devices(self):
        """Scan for Bluetooth devices"""
        if not BLUETOOTH_AVAILABLE:
            messagebox.showerror(
                "Error", "PyBluez not installed.\n\nInstall with:\npip install pybluez"
            )
            return

        self.log_message("Scanning for Bluetooth devices...")
        self.root.update()

        # Run scan in a thread to avoid blocking UI
        def scan():
            try:
                devices = self.robot.find_robot_devices()
                self.devices = devices

                if devices:
                    device_names = [f"{name} ({addr})" for addr, name in devices]
                    self.device_combo["values"] = device_names
                    self.log_message(f"Found {len(devices)} devices")
                else:
                    self.log_message("No devices found")
                    messagebox.showwarning(
                        "No Devices",
                        "No Bluetooth devices found. Make sure your robot is powered on.",
                    )
            except Exception as e:
                self.log_message(f"Scan error: {str(e)}")
                messagebox.showerror("Scan Error", str(e))

        thread = threading.Thread(target=scan, daemon=True)
        thread.start()

    def connect_robot(self):
        """Connect to the selected device"""
        if self.device_var.get() == "":
            messagebox.showwarning(
                "No Device Selected", "Please scan and select a device first"
            )
            return

        try:
            # Extract address from the displayed text
            device_name = self.device_var.get()
            device_addr = device_name.split("(")[-1].rstrip(")")

            self.log_message(f"Connecting to {device_name}...")
            self.robot.connect(device_addr)

            self.status_var.set("Connected")
            self.log_message("Successfully connected!")
            messagebox.showinfo("Connected", f"Connected to {device_name}")
        except Exception as e:
            self.status_var.set("Connection Failed")
            self.log_message(f"Connection failed: {str(e)}")
            messagebox.showerror("Connection Failed", str(e))

    def disconnect_robot(self):
        """Disconnect from the robot"""
        self.robot.disconnect()
        self.status_var.set("Disconnected")
        self.log_message("Disconnected from robot")

    def send_command(self, direction):
        """Send a motion command"""
        if not self.robot.connected:
            messagebox.showwarning("Not Connected", "Please connect to the robot first")
            return

        try:
            self.robot.send_motion_command(direction)
            self.log_message(f"Sent: {direction}")
        except Exception as e:
            self.log_message(f"Error: {str(e)}")
            self.status_var.set("Error")
            messagebox.showerror("Command Error", str(e))

    def stop_robot(self):
        """Send stop command"""
        if not self.robot.connected:
            messagebox.showwarning("Not Connected", "Please connect to the robot first")
            return

        try:
            self.robot.send_stop_command()
            self.log_message("Sent: Stop")
        except Exception as e:
            self.log_message(f"Error: {str(e)}")
            self.status_var.set("Error")
            messagebox.showerror("Command Error", str(e))


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
