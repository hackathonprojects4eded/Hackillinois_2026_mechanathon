#include <Arduino.h>

#include "Robot.h"

// Global Robot instance
Robot robot;

void setup()
{
  Serial.begin(9600);

  // Initialize robot (motors and IR sensors)
  robot.init();

  // Simple movement
  robot.move(Robot::FORWARD, 200); // Move forward at speed 200
  robot.move(Robot::LEFT, 255);    // Turn left at max speed
  robot.stop();

  // IR line tracking
  uint16_t left, middle, right;
  robot.getIRSensorValues(left, middle, right);

  if (robot.isOnLine(middle))
  {
    robot.move(Robot::FORWARD, 200); // Follow line
  }

  // Initialize MPU6050 sensor
  // uint8_t mpu_status = mpu6050_basic_init(MPU6050_ADDRESS_AD0_LOW);
  // if (mpu_status != 0)
  // {
  //   Serial.println("MPU6050 init failed!");
  //   while (1)
  //     ; // Halt if MPU6050 fails
  // }

  // Serial.println("Robot and sensors initialized!");
  // delay(500);
}

void loop()
{
  // Read MPU6050 data
  // float accel[3]; // acceleration in g (gravity)
  // float gyro[3];  // angular velocity in dps (degrees per second)
  // float temp;     // temperature in Celsius

  // // Read accelerometer and gyroscope
  // if (mpu6050_basic_read(accel, gyro) == 0)
  // {
  //   Serial.print("Accel X: ");
  //   Serial.print(accel[0], 2);
  //   Serial.print("g | Y: ");
  //   Serial.print(accel[1], 2);
  //   Serial.print("g | Z: ");
  //   Serial.print(accel[2], 2);
  //   Serial.println("g");

  //   Serial.print("Gyro X: ");
  //   Serial.print(gyro[0], 2);
  //   Serial.print("dps | Y: ");
  //   Serial.print(gyro[1], 2);
  //   Serial.print("dps | Z: ");
  //   Serial.print(gyro[2], 2);
  //   Serial.println("dps");
  // }

  // // Read temperature
  // if (mpu6050_basic_read_temperature(&temp) == 0)
  // {
  //   Serial.print("Temperature: ");
  //   Serial.print(temp, 1);
  //   Serial.println("C");
  // }

  // // IR sensor readings
  // uint16_t ir_left, ir_middle, ir_right;
  // robot.getIRSensorValues(ir_left, ir_middle, ir_right);

  // Serial.print("IR - Left: ");
  // Serial.print(ir_left);
  // Serial.print(" | Middle: ");
  // Serial.print(ir_middle);
  // Serial.print(" | Right: ");
  // Serial.println(ir_right);

  // Serial.println("---");
  // delay(500); // Read every 500ms
}

// #include "DeviceDriverSet_xxx0.h"
// #include "ApplicationFunctionSet_xxx0.h"

// ApplicationFunctionSet Application_FunctionSet;

// void setup()
// {
//   // put your setup code here, to run once:
//   Application_FunctionSet.ApplicationFunctionSet_Init();
// }

// void loop()
// {
//   // put your main code here, to run repeatedly:
//   Application_FunctionSet.ApplicationFunctionSet_SerialPortDataAnalysis();
//   Application_FunctionSet.ApplicationFunctionSet_Rocker();
//   Application_FunctionSet.CMD_ClearAllFunctionsXXX();
// }
