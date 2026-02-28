#include "Ultrasonic_control.h"

/*ULTRASONIC*/
void DeviceDriverSet_ULTRASONIC::DeviceDriverSet_ULTRASONIC_Init(void)
{
  Wire.begin();
}

void DeviceDriverSet_ULTRASONIC::DeviceDriverSet_ULTRASONIC_Get(uint16_t *ULTRASONIC_Get /*out*/)
{
  unsigned dat[2] = {0};
  Wire.requestFrom(0x07, 1); //从器件读取一位数
  if (Wire.available() > 0)
  {
    dat[0] = Wire.read();
  }
  Wire.requestFrom(0x07, 1); //从器件读取一位数
  if (Wire.available() > 0)
  {
    dat[1] = Wire.read();
  }

  *ULTRASONIC_Get = ((dat[0] << 8) | dat[1]);
}

#if _Test_DeviceDriverSet
void DeviceDriverSet_ULTRASONIC::DeviceDriverSet_ULTRASONIC_Test(void)
{
  unsigned dat[2] = {0};
  Wire.requestFrom(0x07, 1); //从器件读取一位数
  if (Wire.available() > 0)
  {
    dat[0] = Wire.read();
  }

  Wire.requestFrom(0x07, 1); //从器件读取一位数
  if (Wire.available() > 0)
  {
    dat[1] = Wire.read();
  }
  _delay(100);
  Serial.print("ULTRASONIC=");
  Serial.println((dat[0] << 8) | dat[1]);
}
#endif