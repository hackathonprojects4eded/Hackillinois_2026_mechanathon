#ifndef ULTRASONIC_CONTROL_H
#define ULTRASONIC_CONTROL_H

#include <Arduino.h>

#define _Test_DeviceDriverSet 0

#define TimeCompensation 4 
extern unsigned long _millis();
extern void _delay(unsigned long ms);

#include "I2Cdev.h"
class DeviceDriverSet_ULTRASONIC
{
public:
  void DeviceDriverSet_ULTRASONIC_Init(void);
#if _Test_DeviceDriverSet
  void DeviceDriverSet_ULTRASONIC_Test(void);
#endif
  void DeviceDriverSet_ULTRASONIC_Get(uint16_t *ULTRASONIC_Get /*out*/);
};

#endif