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
  // Convenience wrapper: read sensor and return distance to object in centimeters.
  // Assumes the raw value returned by DeviceDriverSet_ULTRASONIC_Get is
  // a time-of-flight in microseconds (typical for HC-SR04-like sensors over I2C),
  // and applies a small TimeCompensation defined above. If your sensor returns
  // a different unit (e.g., millimeters), adjust the conversion accordingly.
  float DeviceDriverSet_ULTRASONIC_GetDistanceCm(void);
  // Convenience wrapper: get distance in millimeters.
  // Returns a float distance in mm using the same raw-unit assumption
  // (time-of-flight in microseconds). If raw units differ, adjust accordingly.
  float DeviceDriverSet_ULTRASONIC_GetDistanceMm(void);
};

#endif