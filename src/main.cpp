#include <Arduino.h>

#include "Robot.h"
#include "LED_control.h"
#include "Buzzer_control.h"

// Global Robot instance
Robot robot;
DeviceDriverSet_RBGLED led;
DeviceDriverSet_passiveBuzzer buzzer;

void setup()
{
  buzzer.DeviceDriverSet_passiveBuzzer_Init();
  buzzer.DeviceDriverSet_passiveBuzzer_Scale_c8(100);

  Serial.begin(115200);
  // while (!Serial)
  //   ;

  // initialize LEDs for app control
  led.DeviceDriverSet_RBGLED_Init(95);

  if (!robot.begin())
  {
    Serial.println("Failed to initialize robot");
    // led.DeviceDriverSet_RBGLED_Color(NUM_LEDS,
    //                                  colors[0][0],
    //                                  colors[0][1],
    //                                  colors[0][2]);
    while (1)
      ;
  }
  Serial.println("Robot initialized successfully");

  for (int i = 0; i < 10; i++)
  {
    robot.update();
    Serial.println(robot.getDistance());
  }

  robot.moveToWall(20, 180);
  robot.stop();
  led.DeviceDriverSet_RBGLED_xxx((uint16_t)(0), 5, CRGB::Blue);
  delay(1000);
  led.DeviceDriverSet_RBGLED_xxx((uint16_t)(0), 5, CRGB::Black);

  robot.moveToWall(20, 180);
  robot.turnToAngle(-90, 5.0, true);
  robot.stop();
}

void loop()
{
  robot.update();

  // Example usage:
  // Move forward until reaching 20cm from wall
  // robot.moveToWall(20, 180); // distance=20cm, baseSpeed=180/255

  // // // Turn to face left (-90 degrees)
  // robot.turnToAngle(-90, 5.0, true); // angle=-90, offset=5°, both wheels

  // // // Move to wall again
  // // robot.moveToWall(25, 180);

  // // // Turn to face right (90 degrees) using only outer wheel
  // // robot.turnToAngle(90, 5.0, false); // angle=90, offset=5°, single wheel

  // // Stop and wait
  // robot.stop();
  // delay(2000);
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
//   Application_FunctionSet.ApplicationFunctionSet_SerialPortDataAnalysis();
//   Application_FunctionSet.ApplicationFunctionSet_Rocker();
//   Application_FunctionSet.ApplicationFunctionSet_ObstacleAvoidance();  // Enable obstacle avoidance
//   Application_FunctionSet.CMD_ClearAllFunctionsXXX();
// }