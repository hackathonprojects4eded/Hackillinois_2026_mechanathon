#include "IR_control.h"

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