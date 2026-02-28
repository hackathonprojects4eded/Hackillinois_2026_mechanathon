#include "Ultrasonic_control.h"

/*ULTRASONIC*/
void DeviceDriverSet_ULTRASONIC::DeviceDriverSet_ULTRASONIC_Init(void)
{
  // Wire.begin();
}

void DeviceDriverSet_ULTRASONIC::DeviceDriverSet_ULTRASONIC_Get(uint16_t *ULTRASONIC_Get /*out*/)
{
  unsigned dat[2] = {0};
  Wire.requestFrom(0x07, 1); // 从器件读取一位数
  if (Wire.available() > 0)
  {
    dat[0] = Wire.read();
  }
  Wire.requestFrom(0x07, 1); // 从器件读取一位数
  if (Wire.available() > 0)
  {
    dat[1] = Wire.read();
  }

  *ULTRASONIC_Get = ((dat[0] << 8) | dat[1]);
}

float DeviceDriverSet_ULTRASONIC::DeviceDriverSet_ULTRASONIC_GetDistanceCm(void)
{
  uint16_t raw = 0;
  DeviceDriverSet_ULTRASONIC_Get(&raw);

  // Raw value is already in centimeters (from sensor's I2C output)
  // Apply compensation to adjust for sensor calibration
  int32_t distance_cm_raw = (int32_t)raw - (int32_t)TimeCompensation;
  if (distance_cm_raw < 0)
    distance_cm_raw = 0;

  return (float)distance_cm_raw;
}

float DeviceDriverSet_ULTRASONIC::DeviceDriverSet_ULTRASONIC_GetDistanceMm(void)
{
  uint16_t raw = 0;
  DeviceDriverSet_ULTRASONIC_Get(&raw);

  // Raw value is in centimeters, convert to millimeters (1 cm = 10 mm)
  int32_t distance_cm_raw = (int32_t)raw - (int32_t)TimeCompensation;
  if (distance_cm_raw < 0)
    distance_cm_raw = 0;

  float distance_mm = ((float)distance_cm_raw) * 10.0f;

  return distance_mm;
}

#if _Test_DeviceDriverSet
void DeviceDriverSet_ULTRASONIC::DeviceDriverSet_ULTRASONIC_Test(void)
{
  unsigned dat[2] = {0};
  Wire.requestFrom(0x07, 1); // 从器件读取一位数
  if (Wire.available() > 0)
  {
    dat[0] = Wire.read();
  }

  Wire.requestFrom(0x07, 1); // 从器件读取一位数
  if (Wire.available() > 0)
  {
    dat[1] = Wire.read();
  }
  _delay(100);
  Serial.print("ULTRASONIC=");
  Serial.println((dat[0] << 8) | dat[1]);
}
#endif