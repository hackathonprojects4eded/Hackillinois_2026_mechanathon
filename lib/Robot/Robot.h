#pragma once

#include "MPU6050Wrapper.h"
#include "SimpleKalmanFilter.h"

#include "ApplicationFunctionSet_xxx0.h"
#include "LED_control.h"

class Robot
{
private:
    ApplicationFunctionSet Application_FunctionSet;

    // Manual mode reference
    bool &_manualMode;

    // Movement state
    bool _isMoving;
    uint16_t _targetDistance;
    uint8_t _moveBaseSpeed;

    // Turning state
    bool _isTurning;
    float _targetTurnAngle;
    float _turnAngleOffset;
    bool _turnBothWheels;

    // Head orientation
    bool _headReversed; // If true, reverses all forward/backward directions and inverts angles

    // yaw stuff
    float robotTargetYaw;

    // Helper functions
    // float _normalizeAngle(float angle);
    // bool _isAtTargetAngle(float currentYaw, float targetYaw, float offset);
    // void _applyHeadingCorrection(uint8_t &speedA, uint8_t &speedB, float yaw);
    // bool _reverseDirection(bool direction); // Reverse a direction if head is reversed
    // float _reverseAngle(float angle);       // Reverse an angle if head is reversed

public:
    DeviceDriverSet_RBGLED led;

    // DeviceDriverSet_Motor motor;

    Robot(bool &manualMode);

    // Initialize robot components
    bool begin();

    // Move forward until reaching distanceToWall.
    // Automatically stops when distance <= distanceToWall.
    // Applies heading correction using yaw to maintain straight movement.
    // baseSpeed: starting PWM speed (0-255, recommended 150-200)
    //  void moveToWall(uint16_t distanceToWall, uint8_t baseSpeed = 180);

    // Turn to face a target angle using yaw.
    // targetAngle: -90 (left), 90 (right), or custom angle
    // angleOffset: tolerance range for target angle (default ±5 degrees)
    // bothWheels: if true, both wheels rotate; if false, only outer wheel rotates
    // void turnToAngle(float targetAngle, float angleOffset = 5.0f, bool bothWheels = true);

    // Stop all motors
    // void stop();

    // Update IMU and sensor data (call this in main loop if needed)
    void update();

    // Set head orientation (false = normal, true = reversed 180 degrees)
    // When true, forwards/backwards are reversed and angles are inverted
    void setHeadReversed(bool reversed) { _headReversed = reversed; }

    // Accessors
    // float getYaw() const { return imu.getFilteredYaw(); }
    // float getPitch() const { return imu.getFilteredPitch(); }
    // float getRow() const { return imu.getFilteredRoll(); }
};
