/*
 * @Description: 设备驱动封装集合
 * @Author:  HOU Changhua
 * @Date: 2019-07-11 13:40:52
 * @LastEditTime: 2019-10-15 16:42:59
 * @LastEditors:
 */

#ifndef LED_CONTROL_H
#define LED_CONTROL_H

#define _Test_DeviceDriverSet 1

/*RBG LED*/
#include <FastLED.h>
class DeviceDriverSet_RBGLED
{
public:
  void DeviceDriverSet_RBGLED_Init(uint8_t set_Brightness);
  void DeviceDriverSet_RBGLED_xxx(uint16_t Duration, uint8_t Traversal_Number, CRGB colour);
#if _Test_DeviceDriverSet
  void DeviceDriverSet_RBGLED_Test(void);
#endif
  void DeviceDriverSet_RBGLED_Color(uint8_t LED_s, uint8_t r, uint8_t g, uint8_t b);

public:
private:
#define PIN_RBGLED 10
#define NUM_LEDS 5
public:
  CRGB leds[NUM_LEDS];
};

#endif
