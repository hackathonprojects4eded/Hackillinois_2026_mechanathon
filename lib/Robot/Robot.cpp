#include "Robot.h"
#include <DeviceDriverSet_xxx0.h>
#include "ApplicationFunctionSet_xxx0.h"

extern unsigned long lastPacketTime;
extern unsigned long WATCHDOG_TIMEOUT_MS;
extern DeviceDriverSet_Motor AppMotor;
extern Application_xxx Application_OwlBotxxx0;

Robot::Robot(bool &manualMode)
    : _manualMode(manualMode), _isMoving(false), _targetDistance(0), _moveBaseSpeed(180),
      _isTurning(false), _targetTurnAngle(0), _turnAngleOffset(5.0f), _turnBothWheels(true),
      _headReversed(true) // mea_e=2cm, est_e=5cm, q=2cm (process noise)
{
}

bool Robot::begin()
{
    robotTargetYaw = 0;

    Application_FunctionSet.ApplicationFunctionSet_Init();
    Application_FunctionSet.CMD_ClearAllFunctionsXXX();
    // motor.DeviceDriverSet_Motor_Init();
    // ultrasonic.DeviceDriverSet_ULTRASONIC_Init();

    // bool ret = imu.begin();
    // if (!ret)
    // {
    //     // Serial.println("failed to init imu");
    //     return false;
    // }
    return true;
}

void Robot::update()
{
    Application_FunctionSet.ApplicationFunctionSet_SerialPortDataAnalysis();

    Application_FunctionSet.ApplicationFunctionSet_Rocker();
    Application_FunctionSet.CMD_ClearAllFunctionsXXX();

    // if (millis() - lastPacketTime > WATCHDOG_TIMEOUT_MS)
    // {
    //     AppMotor.DeviceDriverSet_Motor_control(
    //         direction_void, 0,
    //         direction_void, 0,
    //         control_enable);
    //     Application_OwlBotxxx0.Functional_Mode = Standby_mode;
    //     Serial.println("Watch dog called");
    // }

    // imu.update();
    Serial.println("Update");
}

// void Robot::moveToWall(uint16_t distanceToWall, uint8_t baseSpeed)
// {
//     if (_manualMode)
//         return;
//     // Validate parameters
//     if (distanceToWall >= 40)
//     {
//         // Serial.println("Error: distanceToWall must be < 40 cm");
//         return;
//     }

//     _targetDistance = distanceToWall;
//     _moveBaseSpeed = baseSpeed;
//     _isMoving = true;
//     while (_isMoving)
//     {

//         update();
//         if (_manualMode)
//             return;
//         uint16_t currentDist = _lastFilteredDistance;

//         // Serial.println(currentDist);

//         // Check if we've reached the target distance
//         if (currentDist <= _targetDistance && currentDist != 0)
//         {
//             stop();
//             _isMoving = false;
//             //  Serial.print("Reached wall at distance: ");
//             //  Serial.print(currentDist);
//             //  Serial.println(" cm");
//             return;
//         }

//         // Get current yaw and apply heading correction
//         float currentYaw = imu.getFilteredYaw();
//         uint8_t speedA = _moveBaseSpeed;
//         uint8_t speedB = _moveBaseSpeed;

//         _applyHeadingCorrection(speedA, speedB, currentYaw - robotTargetYaw);

//         // Serial.println("Speeds");
//         // Serial.println(speedA);
//         // Serial.println(speedB);

//         // Determine forward direction (true = forward, false = backward)
//         bool forwardDir = _reverseDirection(true);

//         // Move forward with heading correction
//         motor.DeviceDriverSet_Motor_control(
//             forwardDir, speedA, // Motor A: forward with corrected speed
//             forwardDir, speedB, // Motor B: forward with corrected speed
//             true);              // Control enabled

//         // delay(20); // Small delay for sensor polling
//     }
// }

// void Robot::turnToAngle(float targetAngle, float angleOffset, bool bothWheels)
// {
//     if (_manualMode)
//         return;
//     float adjustedTargetAngle = _reverseAngle(targetAngle);
//     robotTargetYaw = _reverseAngle(targetAngle);

//     _targetTurnAngle = adjustedTargetAngle;
//     _turnAngleOffset = angleOffset;
//     _turnBothWheels = bothWheels;
//     _isTurning = true;

//     _targetTurnAngle = _normalizeAngle(_targetTurnAngle);

//     uint8_t SPEED = 255;

//     while (_isTurning)
//     {
//         update();
//         if (_manualMode)
//             return;
//         float currentYaw = imu.getFilteredYaw();

//         if (_headReversed)
//         {
//             currentYaw = _normalizeAngle(currentYaw + 180.0f);
//         }

//         if (_isAtTargetAngle(currentYaw, _targetTurnAngle, _turnAngleOffset))
//         {
//             stop();
//             _isTurning = false;
//             // Serial.print("Reached target angle: ");
//             //  Serial.println(currentYaw);
//             return;
//         }

//         float angleDiff = _targetTurnAngle - currentYaw;
//         angleDiff = _normalizeAngle(angleDiff);

//         if (abs(angleDiff) < 20)
//         {
//             SPEED = 150;
//         }

//         if (_turnBothWheels)
//         {
//             bool dirA = (angleDiff < 0);
//             uint8_t pwmA = SPEED;
//             bool dirB = (angleDiff > 0);
//             uint8_t pwmB = SPEED;

//             motor.DeviceDriverSet_Motor_control(dirA, pwmA, dirB, pwmB, true);
//         }
//         else
//         {
//             if (angleDiff > 0)
//             {
//                 motor.DeviceDriverSet_Motor_control(direction_back, SPEED, direction_back, 0, true);
//             }
//             else
//             {
//                 motor.DeviceDriverSet_Motor_control(direction_back, 0, direction_back, SPEED, true);
//             }
//         }

//         delay(20);
//     }
// }

// void Robot::stop()
// {
//     motor.DeviceDriverSet_Motor_control(direction_void, 0, direction_void, 0, true);
//     _isMoving = false;
//     _isTurning = false;
// }

// float Robot::_normalizeAngle(float angle)
// {
//     // Normalize angle to [-180, 180]
//     while (angle > 180)
//         angle -= 360;
//     while (angle < -180)
//         angle += 360;
//     return angle;
// }

// bool Robot::_isAtTargetAngle(float currentYaw, float targetYaw, float offset)
// {
//     // Normalize current yaw to [-180, 180]
//     float normalized = _normalizeAngle(currentYaw);

//     // Calculate absolute difference
//     float diff = fabs(targetYaw - normalized);

//     // Handle wraparound (e.g., difference between 179 and -179 is 2, not 358)
//     if (diff > 180)
//         diff = 360 - diff;

//     return diff <= offset;
// }

// void Robot::_applyHeadingCorrection(uint8_t &speedA, uint8_t &speedB, float yaw)
// {
//     // yaw should be close to 0 for straight movement
//     // Negative yaw means robot is tilted left, so speed up right motor (B)
//     // Positive yaw means robot is tilted right, so speed up left motor (A)

//     const float YAW_THRESHOLD = 1.0f;  // Only correct if yaw error > threshold
//     const uint8_t CORRECTION_RATE = 3; // PWM adjustment per degree of yaw
//     const uint8_t MIN_SPEED = 100;     // Prevent motors from stalling
//     const uint8_t MAX_CORRECTION = 50; // Max PWM adjustment

//     if (fabs(yaw) > YAW_THRESHOLD)
//     {
//         uint8_t correction = min((uint8_t)(fabs(yaw) * CORRECTION_RATE), MAX_CORRECTION);

//         if (yaw > 0)
//         {
//             // Tilted left, speed up right motor (B)
//             speedA = max((uint8_t)(speedA - correction), MIN_SPEED);
//             speedB = min((uint8_t)(speedB + correction), 255);
//         }
//         else
//         {
//             // Tilted right, speed up left motor (A)
//             speedA = min((uint8_t)(speedA + correction), 255);
//             speedB = max((uint8_t)(speedB - correction), MIN_SPEED);
//         }
//     }
// }

// bool Robot::_reverseDirection(bool direction)
// {
//     // If head is reversed, flip the direction
//     // true = forward, false = backward
//     if (_headReversed)
//     {
//         return !direction;
//     }
//     return direction;
// }

// float Robot::_reverseAngle(float angle)
// {
//     // If head is reversed, invert the angle (add 180 degrees)
//     if (_headReversed)
//     {
//         return angle + 180.0f;
//     }
//     return angle;
// }
