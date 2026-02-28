#include <Arduino.h>

// #include "Robot.h"
// #include "MPU6050Wrapper.h"

// Robot robot;
// MPU6050Wrapper imu;

// void setup()
// {
//   Serial.begin(115200);

//   // Initialize robot (motors and IR sensors)
//   robot.init();

//   // Simple movement
//   robot.move(Robot::FORWARD, 200); // Move forward at speed 200

//   robot.move(Robot::LEFT, 255); // Turn left at max speed
//   robot.stop();
// }

// void loop()
// {
//   imu.update();    // reads raw + DMP FIFO if interrupt fired
//   imu.printData(); // prints whatever OUTPUT_* flags are enabled

//   // Or access data directly:
//   // imu.raw().ax, imu.raw().gy, etc.
//   // imu.orientation().yaw, .pitch, .roll, etc.
// }

#include "DeviceDriverSet_xxx0.h"
#include "ApplicationFunctionSet_xxx0.h"

ApplicationFunctionSet Application_FunctionSet;

void setup()
{
  // put your setup code here, to run once:
  Application_FunctionSet.ApplicationFunctionSet_Init();
}

void loop()
{
  // put your main code here, to run repeatedly:
  Application_FunctionSet.ApplicationFunctionSet_SerialPortDataAnalysis();
  Application_FunctionSet.ApplicationFunctionSet_Rocker();
  Application_FunctionSet.CMD_ClearAllFunctionsXXX();
}
