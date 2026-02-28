
#include "Robot.h"
#include "motor_control.h"
#include "IR_control.h"
#include "I2Cdev.h"

void Robot::init()
{
    _motor.DeviceDriverSet_Motor_Init();
    _ir_sensor.DeviceDriverSet_STM8S003F3_IR_Init();
}

void Robot::move(Direction direction, uint8_t speed)
{
    switch (direction)
    {
    case FORWARD:
        _motor.DeviceDriverSet_Motor_control(
            direction_just, speed, // Motor A: forward at speed
            direction_just, speed, // Motor B: forward at speed
            control_enable         // Enable control
        );
        break;

    case BACKWARD:
        _motor.DeviceDriverSet_Motor_control(
            direction_back, speed, // Motor A: backward at speed
            direction_back, speed, // Motor B: backward at speed
            control_enable);
        break;

    case LEFT:
        _motor.DeviceDriverSet_Motor_control(
            direction_just, speed, // Motor A: forward
            direction_back, speed, // Motor B: backward (creates left turn)
            control_enable);
        break;

    case RIGHT:
        _motor.DeviceDriverSet_Motor_control(
            direction_back, speed, // Motor A: backward (creates right turn)
            direction_just, speed, // Motor B: forward
            control_enable);
        break;

    case LEFT_FORWARD:
        _motor.DeviceDriverSet_Motor_control(
            direction_just, speed,       // Motor A: full speed forward
            direction_just, speed - 130, // Motor B: reduced speed forward
            control_enable);
        break;

    case LEFT_BACKWARD:
        _motor.DeviceDriverSet_Motor_control(
            direction_back, speed,       // Motor A: full speed backward
            direction_back, speed - 130, // Motor B: reduced speed backward
            control_enable);
        break;

    case RIGHT_FORWARD:
        _motor.DeviceDriverSet_Motor_control(
            direction_just, speed - 130, // Motor A: reduced speed forward
            direction_just, speed,       // Motor B: full speed forward
            control_enable);
        break;

    case RIGHT_BACKWARD:
        _motor.DeviceDriverSet_Motor_control(
            direction_back, speed - 130, // Motor A: reduced speed backward
            direction_back, speed,       // Motor B: full speed backward
            control_enable);
        break;

    case STOP:
    default:
        _motor.DeviceDriverSet_Motor_control(
            direction_void, 0, // Motor A: stop
            direction_void, 0, // Motor B: stop
            control_enable);
        break;
    }
}

void Robot::stop()
{
    move(STOP, 0);
}

void Robot::getIRSensorValues(uint16_t &left_out, uint16_t &middle_out, uint16_t &right_out)
{
    _ir_sensor.DeviceDriverSet_STM8S003F3_IR_Get(&left_out, &middle_out, &right_out);
}

boolean Robot::isOnLine(uint16_t value)
{
    return (value >= _threshold_start && value <= _threshold_end);
}

void Robot::setLineThresholds(uint16_t start, uint16_t end)
{
    _threshold_start = start;
    _threshold_end = end;
}
