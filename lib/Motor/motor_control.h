#pragma once

#ifndef MOTOR_CONTROL_H
#define MOTOR_CONTROL_H

#include <Arduino.h>
#define TimeCompensation 4
extern unsigned long _millis();
extern void _delay(unsigned long ms);

/*Motor*/
class DeviceDriverSet_Motor
{
public:
    void DeviceDriverSet_Motor_Init(void);

    void DeviceDriverSet_Motor_control(boolean direction_A, uint8_t speed_A, // motor A
                                       boolean direction_B, uint8_t speed_B, // motor B
                                       boolean controlED                     // enable
    );                                                                       // motor control
private:
#define PIN_Motor_STBY 4
#define PIN_Motor_PWMA 9
// #define PIN_Motor_PWMB 3 //测试版用
// #define PIN_Motor_PWMB 11
#define PIN_Motor_PWMB 6
#define PIN_Motor_AIN 8
#define PIN_Motor_BIN 7

public:
#define speed_Max 255
#define direction_just true
#define direction_back false
#define direction_void 3

#define Duration_enable true
#define Duration_disable false
#define control_enable true
#define control_disable false
};

#endif
