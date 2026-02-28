#pragma once

#include <Arduino.h>
#include <Servo.h>

#ifndef SERVO_DRIVER_H
#define SERVO_DRIVER_H


class ServoDriver {
    public:
        ServoDriver(uint8_t pin);
        void begin();
        void move_angle(int targetAngle);
        void set_angle(int angle);
    private:
        Servo _servo;
        uint8_t _pin;
        int _currentAngle;
};

#endif