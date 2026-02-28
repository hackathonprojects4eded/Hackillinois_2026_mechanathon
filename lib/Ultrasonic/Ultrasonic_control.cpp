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

float DeviceDriverSet_ULTRASONIC::DeviceDriverSet_ULTRASONIC_GetDistanceCm(void)
{
  uint16_t raw = 0;
  DeviceDriverSet_ULTRASONIC_Get(&raw);

  // Interpret raw as time-of-flight in microseconds. Apply simple compensation.
  int32_t tof_us = (int32_t)raw - (int32_t)TimeCompensation;
  if (tof_us < 0)
    tof_us = 0;

  // Speed of sound ~343 m/s = 0.0343 cm/us. Divide by 2 for round-trip.
  const float CM_PER_US_DIV2 = 0.0343f / 2.0f; // ~0.01715
  float distance_cm = ((float)tof_us) * CM_PER_US_DIV2;

  return distance_cm;
}

float DeviceDriverSet_ULTRASONIC::DeviceDriverSet_ULTRASONIC_GetDistanceMm(void)
{
  uint16_t raw = 0;
  DeviceDriverSet_ULTRASONIC_Get(&raw);

  int32_t tof_us = (int32_t)raw - (int32_t)TimeCompensation;
  if (tof_us < 0)
    tof_us = 0;

  // Speed of sound 343 m/s => 0.343 mm/us. Divide by 2 for round trip.
  const float MM_PER_US_DIV2 = 0.343f / 2.0f; // ~0.1715 mm/us
  float distance_mm = ((float)tof_us) * MM_PER_US_DIV2;

  return distance_mm;
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