/*
 * @Author: ELEGOO
 * @Date: 2019-07-10 16:46:17
 * @LastEditTime: 2020-06-18 18:17:46
 * @LastEditors: Changhua
 * @Description: OwlBot Car _00
 * @FilePath: 
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

#if _Test_DeviceDriverSet
void DeviceDriverSet_Motor::DeviceDriverSet_Motor_Test(void)
{
  //digitalWrite(PIN_Motor_STBY, HIGH);
  digitalWrite(PIN_Motor_AIN, LOW);
  digitalWrite(PIN_Motor_BIN, HIGH);
 analogWrite(PIN_Motor_PWMA, 150);
  analogWrite(PIN_Motor_PWMB, 150);
}
#endif

/*
 Motor_control：AB / 方向、速度
*/
void DeviceDriverSet_Motor::DeviceDriverSet_Motor_control(boolean direction_A, uint8_t speed_A, //A组电机参数
                                                          boolean direction_B, uint8_t speed_B, //B组电机参数
                                                          boolean controlED                     //AB使能允许 true
                                                          )                                     //电机控制
{
   if (controlED == control_enable) //使能允许？
  {
    digitalWrite(PIN_Motor_STBY, HIGH); //开启
    {                                   //A...
      switch (direction_A)              //方向控制
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
    { //B...
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
    digitalWrite(PIN_Motor_STBY, LOW); //关闭
    analogWrite(PIN_Motor_PWMA, 0);
    analogWrite(PIN_Motor_PWMB, 0);
    return;
  }
}



/*STM8S003F3_IR*/
void DeviceDriverSet_STM8S003F3_IR::DeviceDriverSet_STM8S003F3_IR_Init(void)
{
  Wire.begin();
  Wire.beginTransmission(STM8S003F3_IR_devAddr);
  Wire.write(110);
  Wire.endTransmission();
}

void DeviceDriverSet_STM8S003F3_IR::DeviceDriverSet_STM8S003F3_IR_Get(uint16_t *STM8S003F3_IRL /*out*/, uint16_t *STM8S003F3_IRM /*out*/, uint16_t *STM8S003F3_IRR /*out*/)
{
  uint8_t STM8S003F3_IR_IIC_buff[8];
  uint8_t a = 0;
  static uint8_t cout = 0;
  Wire.requestFrom(STM8S003F3_IR_devAddr, 8); // request 6 bytes from slave device #2
  while (Wire.available())                    // slave may send less than requested
  {
    STM8S003F3_IR_IIC_buff[a++] = Wire.read(); // receive a byte as character
  }
  if ((STM8S003F3_IR_IIC_buff[0] == 0XA0) && (STM8S003F3_IR_IIC_buff[7] == 0XB0))
  {
    *STM8S003F3_IRR = (STM8S003F3_IR_IIC_buff[1] << 8) | (STM8S003F3_IR_IIC_buff[2]);
    *STM8S003F3_IRL = (STM8S003F3_IR_IIC_buff[3] << 8) | (STM8S003F3_IR_IIC_buff[4]);
    *STM8S003F3_IRM = (STM8S003F3_IR_IIC_buff[5] << 8) | (STM8S003F3_IR_IIC_buff[6]);
    cout = 0;
  }
  else
  {
    /* code */
    //Serial.println("Contact Changhua :STM8S003F3_IR data error"); // print the character
    cout += 1;
    if (cout > 250)
    {
      Wire.beginTransmission(STM8S003F3_IR_devAddr);
      Wire.write(110);
      Wire.endTransmission();
    }
    return;
  }
}

#if _Test_DeviceDriverSet
void DeviceDriverSet_STM8S003F3_IR::DeviceDriverSet_STM8S003F3_IR_Test(void)
{
  uint8_t STM8S003F3_IR_IIC_buff[8];
  uint8_t a = 0;
  Wire.requestFrom(STM8S003F3_IR_devAddr, 8); // request 6 bytes from slave device #2
  while (Wire.available())                    // slave may send less than requested
  {
    STM8S003F3_IR_IIC_buff[a++] = Wire.read(); // receive a byte as character
  }

  if ((STM8S003F3_IR_IIC_buff[0] == 0XA0) && (STM8S003F3_IR_IIC_buff[7] == 0XB0))
  {

    Serial.print("R:");
    Serial.print((STM8S003F3_IR_IIC_buff[1] << 8) | (STM8S003F3_IR_IIC_buff[2]));
    Serial.print("\t");

    Serial.print("L:");
    Serial.print((STM8S003F3_IR_IIC_buff[3] << 8) | (STM8S003F3_IR_IIC_buff[4]));
    Serial.print("\t");

    Serial.print("M:");
    Serial.print((STM8S003F3_IR_IIC_buff[5] << 8) | (STM8S003F3_IR_IIC_buff[6]));

    Serial.println(""); // print the character
  }
  else
  {
    /* code */
    Serial.println("STM8S003F3_IR data error"); // print the character
  }
}
#endif
