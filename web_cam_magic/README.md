# Elegoo Owl Tank Bot - Bluetooth Control Suite

Complete Python/Tkinter solution for controlling your Elegoo Owl Tank bot via Bluetooth.

## Quick Start

### 1. Install Dependencies
```bash
pip install -r requirements.txt
```

### 2. Choose Your Version

#### **`robot_controller.py`** - Standard Version
- ✅ Lightweight and straightforward
- ✅ Perfect for basic control
- ✅ Minimal dependencies
- **Best for:** First-time users, simple control tasks

**Start with:**
```bash
python robot_controller.py
```

#### **`robot_controller_advanced.py`** - Advanced Version
- ✅ Tabbed interface with multiple features
- ✅ Speed control slider (0-255)
- ✅ Command recording and playback
- ✅ Sensor telemetry display
- **Best for:** Complex tasks, automation, development

**Start with:**
```bash
python robot_controller_advanced.py
```

### 3. Test Commands (Optional)
Before connecting to the robot, test the command format:

```bash
python test_commands.py
```

This validates all JSON commands and shows you exactly what gets sent to the robot.

## Control Guide

### Connecting to Your Robot

1. **Power on the robot**
   - LED on Bluetooth module should blink
   
2. **Click "Scan Devices"**
   - Takes ~8 seconds
   - Your robot should appear in the list
   
3. **Select and Connect**
   - Choose the robot from the dropdown
   - Click "Connect"

### Controlling the Robot

**Button Controls:**
- Use the directional pad (8 directions + stop)

**Keyboard Controls:**
- `↑` / `↓` / `←` / `→` - Cardinal directions
- `W` / `A` / `E` / `D` - Diagonal movements
- `Space` - Stop

**Speed Control (Advanced only):**
- Slider from 0-255
- Changes speed of all subsequent commands

## Command Protocol

The application sends JSON commands to your robot:

```json
{
  "N": 102,
  "D1": 1,
  "H": 1
}
```

**Fields:**
- `N`: Command type (102 = joystick mode, 100 = stop/clear)
- `D1`: Direction (1-9)
- `H`: Command sequence number

**Direction Codes:**
```
1 = Forward
2 = Backward
3 = Left
4 = Right
5 = Left-Forward
6 = Left-Backward
7 = Right-Forward
8 = Right-Backward
9 = Stop
```

## Troubleshooting

### Installation Issues

**"PyBluez not found"**
```bash
pip install pybluez
```

**macOS specific:**
```bash
brew install bluez
pip install --pre pybluez
```

**Linux (Ubuntu/Debian):**
```bash
sudo apt-get install libbluetooth-dev
pip install pybluez
```

### Connection Issues

1. **No devices found**
   - Ensure robot is powered on
   - Check Bluetooth is enabled on your computer
   - Try restarting the robot

2. **Connection refused**
   - Try pairing the device in system settings first
   - Check if another app is using the connection
   - Verify the device address is correct

3. **Commands not received**
   - Check robot is in correct mode
   - Try getting closer to the robot
   - Verify Bluetooth signal strength
   - Check the command log for errors

## File Reference

```
web_cam_magic/
├── robot_controller.py              # Standard version (START HERE)
├── robot_controller_advanced.py     # Advanced version with extra features
├── test_commands.py                 # Command validation and testing
├── requirements.txt                 # Python dependencies
├── README_ROBOT_CONTROLLER.md       # Detailed documentation
└── README.md                        # This file
```

## Features by Version

| Feature | Standard | Advanced |
|---------|----------|----------|
| Motion Control | ✅ | ✅ |
| Direction Buttons | ✅ | ✅ |
| Keyboard Control | ✅ | ✅ |
| Device Scanning | ✅ | ✅ |
| Command Logging | ✅ | ✅ |
| Speed Control | ❌ | ✅ |
| Recording | ❌ | ✅ |
| Playback | ❌ | ✅ |
| Sensor Display | ❌ | ✅ |
| Tabbed Interface | ❌ | ✅ |

## Advanced Features Guide

### Recording Commands (Advanced Version Only)

1. **Start Recording**: Click "Start Recording" tab
2. **Send Commands**: Use the control buttons/keyboard
3. **Stop Recording**: Click "Stop Recording"
4. **Save**: Click "Save Recording" and choose a file

Commands are saved as CSV with timestamps.

### Playback Commands

1. **Load Recording**: Open a previously saved CSV file
2. **Playback**: Click "Playback" to execute the sequence
3. Commands will execute with the same timing as recorded

## Python Requirements

- Python 3.6+
- Tkinter (usually included)
- PyBluez (see installation)

## Common Command Sequences

### Center Square
```
Forward (5s) → Right (2s) → Forward (5s) → Stop
Left (2s) → Forward (5s) → Stop
```

### Obstacle Avoidance Test
```
Forward → Detect obstacle → Backward → Right turn → Forward
```

### Spin Test
```
Right (full 360°) → Stop
```

## Adding to Your Project

You can also import the classes in your own Python code:

```python
from robot_controller import RobotController

robot = RobotController()
robot.connect("00:1A:7D:DA:71:13")  # Your robot's MAC address
robot.send_motion_command("Forward")
robot.disconnect()
```

## Next Steps

1. ✅ Install dependencies: `pip install -r requirements.txt`
2. ✅ Run the standard version: `python robot_controller.py`
3. ✅ Power on your robot
4. ✅ Click "Scan Devices"
5. ✅ Select your robot and connect
6. ✅ Start controlling!

## Getting Help

- Check the command log for detailed error messages
- Run `python test_commands.py` to validate commands
- Read `README_ROBOT_CONTROLLER.md` for detailed documentation
- Check that your robot firmware supports the JSON protocol (N=102 for motion)

## Notes

- Commands are sent as JSON strings over Bluetooth RFCOMM channel 1
- The robot should echo back sensor data if available
- Command sequence numbers help track command order
- All timestamps in advanced version are in milliseconds

---

**Happy controlling!** 🤖
