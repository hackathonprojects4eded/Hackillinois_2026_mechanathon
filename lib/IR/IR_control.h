#ifndef IR_CONTROL_H
#define IR_CONTROL_H

#define _Test_DeviceDriverSet 0
#include <Arduino.h>
#define TimeCompensation 4 
extern unsigned long _millis();
extern void _delay(unsigned long ms);

#include <Wire.h>
class DeviceDriverSet_STM8S003F3_IR
{
public:
  void DeviceDriverSet_STM8S003F3_IR_Init(void);
#if _Test_DeviceDriverSet
  void DeviceDriverSet_STM8S003F3_IR_Test(void);
#endif
  void DeviceDriverSet_STM8S003F3_IR_Get(uint16_t *STM8S003F3_IRL /*out*/, uint16_t *STM8S003F3_IRM /*out*/, uint16_t *STM8S003F3_IRR /*out*/);

private:
#define STM8S003F3_IR_devAddr 0XA0
};

#endif