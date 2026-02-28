#include <Arduino.h>

#include "MPU6050Wrapper.h"
#include "Ultrasonic_control.h"

// Ultrasonic driver instance
DeviceDriverSet_ULTRASONIC ultrasonic;

// Global Robot instance
MPU6050Wrapper imu;

static unsigned long lastDistPrint = 0;
const unsigned long DIST_PRINT_INTERVAL = 1000;

void setup()
{
  Serial.begin(115200);
  while (!Serial)
    ;
  ultrasonic.DeviceDriverSet_ULTRASONIC_Init();
  bool ret = imu.begin();
  if (!ret)
  {
    Serial.println("failed to init imu");
    while (1)
      ;
  }
}

void loop()
{
  // imu.update();    // reads raw + DMP FIFO if interrupt fired
  // imu.printData(); // prints whatever OUTPUT_* flags are enabled

  // Or access data directly:
  // imu.raw().ax, imu.raw().gy, etc.
  // imu.orientation().yaw, .pitch, .roll, etc.

  if (millis() - lastDistPrint >= DIST_PRINT_INTERVAL)
  {
    lastDistPrint = millis();
    float distCm = ultrasonic.DeviceDriverSet_ULTRASONIC_GetDistanceCm();
    float distMm = ultrasonic.DeviceDriverSet_ULTRASONIC_GetDistanceMm();
    Serial.print("Ultrasonic distance: ");
    Serial.print(distCm, 2);
    Serial.print(" cm  /  ");
    Serial.print(distMm, 1);
    Serial.println(" mm");
  }
}

// #include "DeviceDriverSet_xxx0.h"
// #include "ApplicationFunctionSet_xxx0.h"

// ApplicationFunctionSet Application_FunctionSet;

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

// #include "DeviceDriverSet_xxx0.h"
// #include "ApplicationFunctionSet_xxx0.h"

// ApplicationFunctionSet Application_FunctionSet;

// void setup()
// {
//   // put your setup code here, to run once:
//   Application_FunctionSet.ApplicationFunctionSet_Init();

//   // Enable obstacle avoidance mode (ObstacleAvoidance_mode = enum value 2)
//   Application_FunctionSet.SetFunctionalMode(2);  // ObstacleAvoidance_mode
//   Serial.println("Obstacle avoidance mode enabled!");
// }

// void loop()
// {
//   // put your main code here, to run repeatedly:
//   Application_FunctionSet.ApplicationFunctionSet_SerialPortDataAnalysis();
//   Application_FunctionSet.ApplicationFunctionSet_Rocker();
//   Application_FunctionSet.ApplicationFunctionSet_ObstacleAvoidance();  // Enable obstacle avoidance
//   Application_FunctionSet.CMD_ClearAllFunctionsXXX();
// }