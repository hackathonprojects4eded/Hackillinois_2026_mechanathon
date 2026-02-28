# Elegoo Owl Tank Bot - Bluetooth Controller

A Python/Tkinter GUI application for controlling your Elegoo Owl Tank bot via Bluetooth.

## Features

- **Bluetooth Device Discovery**: Scan for nearby Bluetooth devices
- **Easy Connection Management**: Connect/disconnect with a single click
- **Intuitive Motion Controls**: 8-directional movement + stop
  - Forward, Backward, Left, Right
  - Diagonal movements (LeftForward, LeftBackward, RightForward, RightBackward)
- **Keyboard Support**: Control the robot with arrow keys and WASD
- **Command Logging**: View all sent commands with timestamps
- **JSON Protocol**: Sends commands in the same JSON format your robot firmware expects

## Installation

### 1. Install Python Dependencies

```bash
pip install pybluez
```

**On macOS**, you might need to install via homebrew first:
```bash
brew install bluez
pip install pybluez
```

**On Linux (Ubuntu/Debian)**:
```bash
sudo apt-get install libbluetooth-dev
pip install pybluez
```

**On Windows**, PyBluez should work directly, but you need Windows 10+.

### 2. Run the Application

```bash
python robot_controller.py
```

## Usage

### Connecting to Your Robot

1. **Power on your Elegoo Owl Tank bot**
   - Make sure the Bluetooth module (HC-05 or similar) is powered
   - The bot should enter pairing mode (usually indicated by a blinking LED)

2. **Scan for Devices**
   - Click "Scan Devices" - this will take ~8 seconds
   - Your robot should appear in the device list with its Bluetooth address

3. **Select and Connect**
   - Select your robot from the dropdown
   - Click "Connect"
   - You should see "Connected" status and a success message

### Controlling the Robot

#### Using Buttons
Click any directional button to send commands to your robot:
- **↑ Forward** / **↓ Backward**
- **← Left** / **Right →**
- **↖ L-Fwd** / **↗ R-Fwd** / **↙ L-Back** / **↘ R-Back**
- **Stop** - Halts all movement

#### Using Keyboard
When connected, you can also use:
- **Arrow Keys**: Forward, Backward, Left, Right
- **W**: Left-Forward diagonal
- **A**: Left-Backward diagonal
- **E**: Right-Forward diagonal
- **D**: Right-Backward diagonal
- **Space**: Stop

## Command Protocol

The application sends JSON commands that match your robot firmware's protocol:

```json
{
  "N": 102,
  "D1": 1,
  "H": <command_number>
}
```

Where:
- **N**: Command type (102 = joystick/rocker control mode)
- **D1**: Direction code
  - 1 = Forward
  - 2 = Backward
  - 3 = Left
  - 4 = Right
  - 5 = LeftForward
  - 6 = LeftBackward
  - 7 = RightForward
  - 8 = RightBackward
  - 9 = Stop
- **H**: Command sequence number

For a complete stop and mode reset:
```json
{
  "N": 100,
  "H": <command_number>
}
```

## Troubleshooting

### "PyBluez not installed" Error
Install the required package:
```bash
pip install pybluez
```

### No Devices Found
- Ensure your robot is powered on
- Make sure Bluetooth is enabled on your computer
- Check that the Bluetooth module on the robot is functioning
- Try rescanning after a few seconds

### Connection Fails
- Try pairing the device with your OS first (in system Bluetooth settings)
- Check the device address is correct
- Ensure nothing else is using the Bluetooth connection to that device
- Try disconnecting and reconnecting

### Commands Not Being Received
- Verify the robot is in the correct mode (should accept N=102 commands)
- Check the Bluetooth signal strength (try getting closer)
- Verify the robot's firmware supports the JSON protocol
- Check the Command Log for any error messages

## Extending the Application

### Adding Speed Control
Currently, the robot firmware uses a fixed speed. You can modify the GUI to add a speed slider:

```python
# In setup_ui(), add after motion_frame:
speed_frame = ttk.Frame(motion_frame)
speed_frame.pack(pady=10, fill="x")
ttk.Label(speed_frame, text="Speed:").pack(side="left", padx=5)
self.speed_var = tk.IntVar(value=200)
speed_slider = ttk.Scale(speed_frame, from_=0, to=255, variable=self.speed_var)
speed_slider.pack(side="left", padx=5, fill="x", expand=True)

# Then modify send_command to use the speed:
self.robot.send_motion_command(direction, self.speed_var.get())
```

### Adding Sensor Display
If you want to display sensor data (like distance from ultrasonic):

```python
# Add a receive thread in RobotController
def receive_data(self):
    """Receive data from the robot"""
    while self.connected:
        try:
            data = self.socket.recv(1024).decode('utf-8')
            # Parse and handle incoming data
        except:
            break
```

## File Structure

```
web_cam_magic/
├── robot_controller.py          # Main application (this file)
├── requirements.txt             # Python dependencies
├── README_ROBOT_CONTROLLER.md   # This file
└── algorithm.py                 # Existing computer vision code
```

## Notes

- The application sends commands continuously while buttons are held (in button-based control)
- Keyboard shortcuts send a single command per key press
- All commands are logged with timestamps for debugging
- The command sequence number (H field) increments with each command to help track command order

## Compatibility

This controller should work with:
- **Elegoo Owl Tank Robot** (original hardware)
- **HC-05 or HC-06 Bluetooth modules**
- Any robot firmware that accepts the JSON protocol format shown above

## Future Enhancements

Potential additions to this controller:
- [ ] Sensor telemetry display (distance, battery, orientation)
- [ ] Speed adjustment slider
- [ ] Recording and playback of command sequences
- [ ] Obstacle avoidance mode toggle
- [ ] LED control commands
- [ ] Real-time video streaming integration
- [ ] Path planning and autonomous navigation
- [ ] Multi-robot control support

## License

This code is provided as-is for controlling your Elegoo robot.
