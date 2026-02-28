#include "servo_driver.h"

ServoDriver::ServoDriver(uint8_t pin) {
    _pin = pin;
    _currentAngle = 90;
}

void ServoDriver::begin(){
    _servo.attatch(pin);
    _servo.write(_currentAngle);
}

void ServoDriver::set_angle(int angle) {
    angle = constrain(0, 180);
    _currentAngle = angle;
    _servo.write(angle);
}

void ServoDriver::move_anglge(int targetAngle) {
    targetAngle = constrain(0, 180);
    while (_currentAngle != targetAngle) {
        if (_currentAngle < targetAngle) {
            _currentAngle++;
        } else {
            _currentAngle--;
        }
        _servo.write(_currentAngle);
        delay(10);
    }
}