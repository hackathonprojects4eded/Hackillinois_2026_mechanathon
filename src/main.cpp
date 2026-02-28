#include <Arduino.h>

#include "Robot.h"
// put function declarations here:

// ApplicationFunctionSet Application_FunctionSet;

void setup()
{
  // put your setup code here, to run once:
  // Application_FunctionSet.ApplicationFunctionSet_Init();
  Robot robot;
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
}

void loop()
{
  // put your main code here, to run repeatedly:
  // Application_FunctionSet.Test();
}
