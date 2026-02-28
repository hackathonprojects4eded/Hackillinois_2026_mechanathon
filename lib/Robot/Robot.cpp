#include "Robot.h"

Robot::Robot()
    : _isMoving(false), _targetDistance(0), _moveBaseSpeed(180),
      _isTurning(false), _targetTurnAngle(0), _turnAngleOffset(5.0f), _turnBothWheels(true),
      _lastFilteredDistance(69),
      ultrasonicFilter(2.0f, 5.0f, 2.0f) // mea_e=2cm, est_e=5cm, q=2cm (process noise)
{
}

bool Robot::begin()
{
    motor.DeviceDriverSet_Motor_Init();
    ultrasonic.DeviceDriverSet_ULTRASONIC_Init();
    bool ret = imu.begin();
    if (!ret)
    {
        Serial.println("failed to init imu");
        return false;
    }
    return true;
}

void Robot::update()
{
    uint16_t rawDist = ultrasonic.DeviceDriverSet_ULTRASONIC_GetDistanceCm();
    delay(20);
    rawDist += ultrasonic.DeviceDriverSet_ULTRASONIC_GetDistanceCm();
    rawDist /= 2;
    // Serial.println("----");
    // Serial.println(rawDist);
    if (!(rawDist > 5000 || rawDist == 150 || rawDist == 149 || rawDist == 0))
    {
        _lastFilteredDistance = (uint16_t)ultrasonicFilter.updateEstimate((float)rawDist);
    }
    else
    {
        _lastFilteredDistance = 69;
    }
    // Serial.println(_lastFilteredDistance);

    imu.update();
    delay(20);
}

void Robot::moveToWall(uint16_t distanceToWall, uint8_t baseSpeed)
{
    // Validate parameters
    if (distanceToWall >= 40)
    {
        Serial.println("Error: distanceToWall must be < 40 cm");
        return;
    }

    _targetDistance = distanceToWall;
    _moveBaseSpeed = baseSpeed;
    _isMoving = true;
    while (_isMoving)
    {
        update();
        uint16_t currentDist = _lastFilteredDistance;

        Serial.println(currentDist);

        // Check if we've reached the target distance
        if (currentDist <= _targetDistance && currentDist != 0)
        {
            stop();
            _isMoving = false;
            Serial.print("Reached wall at distance: ");
            Serial.print(currentDist);
            Serial.println(" cm");
            return;
        }

        // Get current yaw and apply heading correction
        float currentYaw = imu.getFilteredYaw();
        uint8_t speedA = _moveBaseSpeed;
        uint8_t speedB = _moveBaseSpeed;

        _applyHeadingCorrection(speedA, speedB, currentYaw);

        Serial.println("Speeds");
        Serial.println(speedA);
        Serial.println(speedB);
        // Move forward with heading correction
        motor.DeviceDriverSet_Motor_control(
            true, speedA, // Motor A: forward with corrected speed
            true, speedB, // Motor B: forward with corrected speed
            true);        // Control enabled

        // delay(20); // Small delay for sensor polling
    }
}

void Robot::turnToAngle(float targetAngle, float angleOffset, bool bothWheels)
{
    _targetTurnAngle = targetAngle;
    _turnAngleOffset = angleOffset;
    _turnBothWheels = bothWheels;
    _isTurning = true;

    // Normalize target angle to [-180, 180]
    _targetTurnAngle = _normalizeAngle(_targetTurnAngle);

    const float PROPORTIONAL_GAIN = 2.0f; // Controls how aggressive the turn is
    const uint8_t MAX_SPEED = 255;
    const uint8_t MIN_SPEED = 150;

    while (_isTurning)
    {
        imu.update();
        float currentYaw = imu.getFilteredYaw();
        Serial.println(currentYaw);
        Serial.println(_targetTurnAngle);

        // Check if we're at target angle
        if (_isAtTargetAngle(currentYaw, _targetTurnAngle, _turnAngleOffset))
        {
            stop();
            _isTurning = false;
            Serial.print("Reached target angle: ");
            Serial.println(currentYaw);
            return;
        }

        // Calculate angle error
        float angleDiff = _targetTurnAngle - currentYaw;

        // Normalize the difference to [-180, 180]
        if (angleDiff > 180)
            angleDiff -= 360;
        else if (angleDiff < -180)
            angleDiff += 360;

        // Proportional control: error to speed conversion
        float err = angleDiff * PROPORTIONAL_GAIN;

        // Clamp to speed limits
        if (fabs(err) > MAX_SPEED)
        {
            err = (err / fabs(err)) * MAX_SPEED;
        }
        if (fabs(err) < MIN_SPEED && fabs(err) > 0.1f)
        {
            err = (err / fabs(err)) * MIN_SPEED;
        }

        // Convert error to motor speeds
        // Positive err = turn right: left motor forward, right motor backward
        // Negative err = turn left: left motor backward, right motor forward
        int8_t speedA = (int8_t)(-err); // Left motor
        int8_t speedB = (int8_t)(err);  // Right motor

        if (_turnBothWheels)
        {
            // Both wheels rotate in opposite directions
            motor.DeviceDriverSet_Motor_control(
                speedA >= 0, abs(speedA), // Motor A
                speedB >= 0, abs(speedB), // Motor B
                true);                    // Control enabled
        }
        else
        {
            // Single wheel (outer wheel) rotation
            if (err > 0)
            {
                // Turn right: only left wheel moves forward
                motor.DeviceDriverSet_Motor_control(true, abs(speedA), true, 0, true);
            }
            else
            {
                // Turn left: only right wheel moves forward
                motor.DeviceDriverSet_Motor_control(true, 0, true, abs(speedB), true);
            }
        }

        delay(20);
    }
}

void Robot::stop()
{
    motor.DeviceDriverSet_Motor_control(direction_void, 0, direction_void, 0, true);
    _isMoving = false;
    _isTurning = false;
}

float Robot::_normalizeAngle(float angle)
{
    // Normalize angle to [-180, 180]
    while (angle > 180)
        angle -= 360;
    while (angle < -180)
        angle += 360;
    return angle;
}

bool Robot::_isAtTargetAngle(float currentYaw, float targetYaw, float offset)
{
    // Normalize current yaw to [-180, 180]
    float normalized = _normalizeAngle(currentYaw);

    // Calculate absolute difference
    float diff = fabs(targetYaw - normalized);

    // Handle wraparound (e.g., difference between 179 and -179 is 2, not 358)
    if (diff > 180)
        diff = 360 - diff;

    return diff <= offset;
}

void Robot::_applyHeadingCorrection(uint8_t &speedA, uint8_t &speedB, float yaw)
{
    // yaw should be close to 0 for straight movement
    // Negative yaw means robot is tilted left, so speed up right motor (B)
    // Positive yaw means robot is tilted right, so speed up left motor (A)

    const float YAW_THRESHOLD = 1.0f;  // Only correct if yaw error > threshold
    const uint8_t CORRECTION_RATE = 3; // PWM adjustment per degree of yaw
    const uint8_t MIN_SPEED = 100;     // Prevent motors from stalling
    const uint8_t MAX_CORRECTION = 50; // Max PWM adjustment

    if (fabs(yaw) > YAW_THRESHOLD)
    {
        uint8_t correction = min((uint8_t)(fabs(yaw) * CORRECTION_RATE), MAX_CORRECTION);

        if (yaw < 0)
        {
            // Tilted left, speed up right motor (B)
            speedA = max((uint8_t)(speedA - correction), MIN_SPEED);
            speedB = min((uint8_t)(speedB + correction), 255);
        }
        else
        {
            // Tilted right, speed up left motor (A)
            speedA = min((uint8_t)(speedA + correction), 255);
            speedB = max((uint8_t)(speedB - correction), MIN_SPEED);
        }
    }
}
