#pragma once

#include "MPU6050Wrapper.h"
#include "Ultrasonic_control.h"
#include "motor_control.h"

class Robot
{
private:
    MPU6050Wrapper imu;
    DeviceDriverSet_ULTRASONIC ultrasonic;
    DeviceDriverSet_Motor motor;

    // Movement state
    bool _isMoving;
    uint16_t _targetDistance;
    uint8_t _moveBaseSpeed;

    // Turning state
    bool _isTurning;
    float _targetTurnAngle;
    float _turnAngleOffset;
    bool _turnBothWheels;

    // Helper functions
    float _normalizeAngle(float angle);
    bool _isAtTargetAngle(float currentYaw, float targetYaw, float offset);
    void _applyHeadingCorrection(uint8_t &speedA, uint8_t &speedB, float yaw);

public:
    Robot();

    // Initialize robot components
    bool begin();

    // Move forward until reaching distanceToWall.
    // Automatically stops when distance <= distanceToWall.
    // Applies heading correction using yaw to maintain straight movement.
    // baseSpeed: starting PWM speed (0-255, recommended 150-200)
    void moveToWall(uint16_t distanceToWall, uint8_t baseSpeed = 180);

    // Turn to face a target angle using yaw.
    // targetAngle: -90 (left), 90 (right), or custom angle
    // angleOffset: tolerance range for target angle (default ±5 degrees)
    // bothWheels: if true, both wheels rotate; if false, only outer wheel rotates
    void turnToAngle(float targetAngle, float angleOffset = 5.0f, bool bothWheels = true);

    // Stop all motors
    void stop();

    // Update IMU and sensor data (call this in main loop if needed)
    void update();

    // Accessors
    float getYaw() const { return imu.getFilteredYaw(); }
    uint16_t getDistance() { return ultrasonic.DeviceDriverSet_ULTRASONIC_GetDistanceCm(); }
};
