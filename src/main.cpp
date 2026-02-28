#include <Arduino.h>

#include "Robot.h"

bool MANUAL_MODE = false;
Robot robot(MANUAL_MODE);

void setup()
{
  Serial.begin(115200);

  if (!robot.begin())
  {
    Serial.println("Failed to initialize robot");
    while (1)
      ;
  }
  Serial.println("Robot initialized successfully");

  // robot.buzzer.DeviceDriverSet_passiveBuzzer_Scale_c8(100);

  for (int i = 0; i < 10; i++)
  {
    robot.update();
    Serial.println(robot.getDistance());
  }

  // robot.led.DeviceDriverSet_RBGLED_xxx((uint16_t)(0), 5, CRGB::Blue);
  // delay(1000);
  // robot.led.DeviceDriverSet_RBGLED_xxx((uint16_t)(0), 5, CRGB::Black);

  // robot.moveToWall(20, 180);
  // robot.turnToAngle(-90, 5.0, true);
  // robot.update();

  // delay(3000);

  // robot.moveToWall(20, 180);
  // robot.turnToAngle(-180, 5.0, true);
  // robot.stop();
}

void loop()
{
  robot.update();

  Serial.println("----");
  Serial.println(robot.getRow());
  Serial.println(robot.getPitch());
}

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
// Application_FunctionSet.ApplicationFunctionSet_SerialPortDataAnalysis();
// Application_FunctionSet.ApplicationFunctionSet_Rocker();
// Application_FunctionSet.ApplicationFunctionSet_ObstacleAvoidance();  // Enable obstacle avoidance
// Application_FunctionSet.CMD_ClearAllFunctionsXXX();
// }