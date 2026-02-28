"""
Test script for validating robot commands locally
Useful for testing the command format without a physical robot
"""

import json
import time


class RobotCommandTester:
    """Test and validate robot commands"""

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
        self.command_number = 0
        self.command_log = []

    def create_motion_command(self, direction, command_id=None):
        """Generate a motion command"""
        if direction not in self.DIRECTION_MAP:
            raise ValueError(f"Invalid direction: {direction}")

        if command_id is None:
            self.command_number += 1
            command_id = self.command_number % 100

        direction_code = self.DIRECTION_MAP[direction]

        command = {"N": 102, "D1": direction_code, "H": command_id}

        return command

    def create_stop_command(self, command_id=None):
        """Generate a stop/clear command"""
        if command_id is None:
            self.command_number += 1
            command_id = self.command_number % 100

        command = {"N": 100, "H": command_id}

        return command

    def validate_command(self, command):
        """Validate command structure"""
        if not isinstance(command, dict):
            return False, "Command must be a dictionary"

        if "N" not in command:
            return False, "Missing 'N' field (command type)"

        if command["N"] == 102:
            if "D1" not in command:
                return False, "Motion commands (N=102) must have D1 field"
            if command["D1"] not in range(1, 10):
                return False, f"D1 must be 1-9, got {command['D1']}"
        elif command["N"] == 100:
            pass  # Stop command, valid
        else:
            return False, f"Unknown command type: {command['N']}"

        return True, "Valid"

    def format_command(self, command):
        """Convert command to JSON string"""
        return json.dumps(command)

    def log_command(self, command, direction_name=""):
        """Log a command"""
        timestamp = time.strftime("%H:%M:%S")
        cmd_str = json.dumps(command)
        self.command_log.append(
            {"time": timestamp, "direction": direction_name, "command": cmd_str}
        )


def test_basic_commands():
    """Test basic motion commands"""
    print("=" * 60)
    print("TEST 1: Basic Motion Commands")
    print("=" * 60)

    tester = RobotCommandTester()

    directions = [
        "Forward",
        "Backward",
        "Left",
        "Right",
        "LeftForward",
        "LeftBackward",
        "RightForward",
        "RightBackward",
        "Stop",
    ]

    for direction in directions:
        cmd = tester.create_motion_command(direction)
        valid, msg = tester.validate_command(cmd)
        cmd_str = tester.format_command(cmd)

        print(f"\n{direction}:")
        print(f"  Command: {cmd_str}")
        print(f"  Valid: {valid} - {msg}")

        tester.log_command(cmd, direction)


def test_command_sequence():
    """Test a sequence of commands"""
    print("\n" + "=" * 60)
    print("TEST 2: Command Sequence")
    print("=" * 60)

    tester = RobotCommandTester()

    sequence = [
        ("Forward", 3, 1000),
        ("Right", 2, 500),
        ("Forward", 3, 1000),
        ("Stop", 1, 500),
    ]

    print("\nCommand Sequence:")
    for direction, duration_ms, wait_ms in sequence:
        cmd = tester.create_motion_command(direction)
        valid, msg = tester.validate_command(cmd)
        cmd_str = tester.format_command(cmd)

        print(f"\n[{duration_ms}ms] {direction}:")
        print(f"  Command: {cmd_str}")
        print(f"  Valid: {valid}")
        print(f"  Wait: {wait_ms}ms")

        tester.log_command(cmd, direction)


def test_stop_command():
    """Test stop/clear command"""
    print("\n" + "=" * 60)
    print("TEST 3: Stop/Clear Command")
    print("=" * 60)

    tester = RobotCommandTester()

    cmd = tester.create_stop_command()
    valid, msg = tester.validate_command(cmd)
    cmd_str = tester.format_command(cmd)

    print(f"\nStop Command:")
    print(f"  Command: {cmd_str}")
    print(f"  Valid: {valid} - {msg}")
    print(f"  Purpose: Clear all functions and enter standby mode")

    tester.log_command(cmd, "Stop")


def test_command_parsing():
    """Test parsing received JSON commands"""
    print("\n" + "=" * 60)
    print("TEST 4: Command Parsing")
    print("=" * 60)

    test_jsons = [
        '{"N":102,"D1":1,"H":1}',
        '{"N":102,"D1":5,"H":2}',
        '{"N":100,"H":3}',
        '{"N":102,"D1":9,"H":4}',
    ]

    direction_names = {
        1: "Forward",
        2: "Backward",
        3: "Left",
        4: "Right",
        5: "LeftForward",
        6: "LeftBackward",
        7: "RightForward",
        8: "RightBackward",
        9: "Stop",
    }

    for cmd_str in test_jsons:
        try:
            cmd = json.loads(cmd_str)
            valid, msg = RobotCommandTester().validate_command(cmd)

            direction_name = ""
            if cmd["N"] == 102:
                direction_name = direction_names.get(cmd["D1"], "Unknown")
            elif cmd["N"] == 100:
                direction_name = "Stop/Clear"

            print(f"\nParsed: {cmd_str}")
            print(f"  Direction: {direction_name}")
            print(f"  Command ID: {cmd.get('H', 'N/A')}")
            print(f"  Valid: {valid}")
        except json.JSONDecodeError as e:
            print(f"\nError parsing: {cmd_str}")
            print(f"  Error: {e}")


def test_command_log():
    """Test command logging"""
    print("\n" + "=" * 60)
    print("TEST 5: Command Logging")
    print("=" * 60)

    tester = RobotCommandTester()

    # Generate some commands
    for direction in ["Forward", "Right", "Forward", "Stop"]:
        cmd = tester.create_motion_command(direction)
        tester.log_command(cmd, direction)

    print("\nCommand Log:")
    print(f"{'Time':<12} {'Direction':<15} {'Command':<35}")
    print("-" * 62)

    for log_entry in tester.command_log:
        print(
            f"{log_entry['time']:<12} {log_entry['direction']:<15} {log_entry['command']:<35}"
        )


def interactive_test():
    """Interactive command generation"""
    print("\n" + "=" * 60)
    print("INTERACTIVE TEST: Generate Custom Commands")
    print("=" * 60)

    tester = RobotCommandTester()

    directions = {
        "1": "Forward",
        "2": "Backward",
        "3": "Left",
        "4": "Right",
        "5": "LeftForward",
        "6": "LeftBackward",
        "7": "RightForward",
        "8": "RightBackward",
        "9": "Stop",
    }

    print("\nAvailable directions:")
    for key, direction in directions.items():
        print(f"  {key}: {direction}")
    print("  0: Quit")

    while True:
        choice = input("\nSelect direction (0-9): ").strip()

        if choice == "0":
            break

        if choice not in directions:
            print("Invalid choice")
            continue

        direction = directions[choice]

        try:
            cmd = tester.create_motion_command(direction)
            valid, msg = tester.validate_command(cmd)
            cmd_str = tester.format_command(cmd)

            print(f"\nGenerated Command for '{direction}':")
            print(f"  JSON: {cmd_str}")
            print(f"  Valid: {valid}")
            print(f"  Structure: N={cmd['N']}, D1={cmd.get('D1', 'N/A')}, H={cmd['H']}")

        except Exception as e:
            print(f"Error: {e}")


def main():
    """Run all tests"""
    print("\n")
    print("*" * 60)
    print("* ELEGOO OWL TANK BOT - COMMAND VALIDATION TEST SUITE *")
    print("*" * 60)

    try:
        test_basic_commands()
        test_command_sequence()
        test_stop_command()
        test_command_parsing()
        test_command_log()

        # Ask about interactive test
        interactive_choice = input("\n\nRun interactive test? (y/n): ").strip().lower()
        if interactive_choice == "y":
            interactive_test()

        print("\n" + "=" * 60)
        print("All tests completed!")
        print("=" * 60 + "\n")

    except KeyboardInterrupt:
        print("\n\nTests interrupted by user")
    except Exception as e:
        print(f"\n\nError during tests: {e}")


if __name__ == "__main__":
    main()
