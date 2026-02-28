/*
 * @Author: ELEGOO
 * @Date: 2019-07-10 16:46:17
 * @LastEditTime: 2020-06-11 17:45:42
 * @LastEditors: Changhua
 * @Description: OwlBot Car _00
 * @FilePath: 
 */
#ifndef _DeviceDriverSet_xxx0_H_
#define _DeviceDriverSet_xxx0_H_

#define _Test_DeviceDriverSet 0
#include <arduino.h>
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
#define speed_Max 255
#define direction_just true
#define direction_back false
#define direction_void 3

#define Duration_enable true
#define Duration_disable false
#define control_enable true
#define control_disable false
};

/*STM8S003F3 IR IIC*/
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
