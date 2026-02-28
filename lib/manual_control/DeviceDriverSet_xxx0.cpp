/*
 * @Description: In User Settings Edit
 * @Author: HOU Changhua
 * @Date: 2019-07-11 13:40:52
 * @LastEditTime: 2019-10-15 10:27:06
 * @LastEditors: Please set LastEditors
 */

#include "DeviceDriverSet_xxx0.h"

extern unsigned long _millis()
{
  return millis() * TimeCompensation;
}
extern void _delay(unsigned long ms)
{
  delay(ms / TimeCompensation);
}

/*Motor control*/
void DeviceDriverSet_Motor::DeviceDriverSet_Motor_Init(void)
{
  pinMode(PIN_Motor_PWMA, OUTPUT);
  pinMode(PIN_Motor_PWMB, OUTPUT);
  pinMode(PIN_Motor_AIN, OUTPUT);
  pinMode(PIN_Motor_BIN, OUTPUT);
  pinMode(PIN_Motor_STBY, OUTPUT);
}

/*
 Motor_control：AB / 方向、速度
*/
void DeviceDriverSet_Motor::DeviceDriverSet_Motor_control(boolean direction_A, uint8_t speed_A, // A组电机参数
                                                          boolean direction_B, uint8_t speed_B, // B组电机参数
                                                          boolean controlED                     // AB使能允许 true
                                                          )                                     // 电机控制
{
  if (controlED == control_enable) // 使能允许？
  {
    digitalWrite(PIN_Motor_STBY, HIGH); // 开启
    {                                   // A...
      switch (direction_A)              // 方向控制
      {
      case direction_just:
        digitalWrite(PIN_Motor_AIN, HIGH);
        analogWrite(PIN_Motor_PWMA, speed_A);
        break;
      case direction_back:
        digitalWrite(PIN_Motor_AIN, LOW);
        analogWrite(PIN_Motor_PWMA, speed_A);
        break;
      case direction_void:
        analogWrite(PIN_Motor_PWMA, 0);
        break;
      default:
        analogWrite(PIN_Motor_PWMA, 0);
        break;
      }
    }
    { // B...
      switch (direction_B)
      {
      case direction_just:
        digitalWrite(PIN_Motor_BIN, HIGH);
        analogWrite(PIN_Motor_PWMB, speed_B);
        break;
      case direction_back:
        digitalWrite(PIN_Motor_BIN, LOW);
        analogWrite(PIN_Motor_PWMB, speed_B);

        break;
      case direction_void:
        analogWrite(PIN_Motor_PWMB, 0);
        break;
      default:
        analogWrite(PIN_Motor_PWMB, 0);
        break;
      }
    }
  }
  else
  {
    digitalWrite(PIN_Motor_STBY, LOW); // 关闭
    analogWrite(PIN_Motor_PWMA, 0);
    analogWrite(PIN_Motor_PWMB, 0);
    return;
  }
}
