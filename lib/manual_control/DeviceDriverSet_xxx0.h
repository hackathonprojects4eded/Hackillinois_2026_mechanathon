/*
 * @Description: 设备驱动封装集合
 * @Author:  HOU Changhua
 * @Date: 2019-07-11 13:40:52
 * @LastEditTime: 2019-10-15 10:16:03
 * @LastEditors:
 */

#ifndef _DeviceDriverSet_xxx0_H_
#define _DeviceDriverSet_xxx0_H_

#include <Arduino.h>

#define _Test_DeviceDriverSet 0

#define TimeCompensation 4 
extern unsigned long _millis();
extern void _delay(unsigned long ms);

/*Motor*/
class DeviceDriverSet_Motor
{
public:
  void DeviceDriverSet_Motor_Init(void);
#if _Test_DeviceDriverSet
  void DeviceDriverSet_Motor_Test(void);
#endif
  void DeviceDriverSet_Motor_control(boolean direction_A, uint8_t speed_A, //A组电机参数
                                     boolean direction_B, uint8_t speed_B, //B组电机参数
                                     boolean controlED                     //AB使能允许 true
  );                                                                       //电机控制
private:

#define PIN_Motor_STBY 4
#define PIN_Motor_PWMA 9
#define PIN_Motor_PWMB 6
#define PIN_Motor_AIN 8
#define PIN_Motor_BIN 7

public:
#define direction_just true
#define direction_back false
#define direction_void 3

#define Duration_enable true
#define Duration_disable false
#define control_enable true
#define control_disable false
};

#endif
