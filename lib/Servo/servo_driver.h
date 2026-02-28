#ifndef SERVO_DRIVER_H
#define SERVO_DRIVER_H

#include <Arduino.h>
#include <Servo.h>

class ServoDriver {
    public:
        ServoDriver(uint8_t pin);
        void begin();
        void move_angle();
        void set_angle();
    private:
        Servo _servo;
        uint8_t _pin;
        int _currentAngle;
}

#endif