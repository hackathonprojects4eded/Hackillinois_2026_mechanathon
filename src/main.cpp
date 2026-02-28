#include <Arduino.h>

#include "Robot.h"
#include "LED_control.h"

// Global Robot instance
Robot robot;
DeviceDriverSet_RBGLED led;

const uint8_t numColors = 6;
const uint8_t colors[numColors][3] = {
    {255, 0, 0},    // red
    {0, 255, 0},    // green
    {0, 0, 255},    // blue
    {255, 165, 0},  // orange
    {255, 255, 0},  // yellow
    {255, 255, 255} // white
};
// cycle through some colors every 500ms
static unsigned long lastChange = 0;
static uint8_t idx = 0;

void setup()
{
  Serial.begin(115200);
  while (!Serial)
    ;

  // initialize LEDs for app control
  led.DeviceDriverSet_RBGLED_Init(95);

  if (!robot.begin())
  {
    Serial.println("Failed to initialize robot");
    led.DeviceDriverSet_RBGLED_Color(NUM_LEDS,
                                     colors[0][0],
                                     colors[0][1],
                                     colors[0][2]);
    while (1)
      ;
  }
  Serial.println("Robot initialized successfully");

  delay(5000);
  led.DeviceDriverSet_RBGLED_Color(NUM_LEDS,
                                   colors[2][0],
                                   colors[2][1],
                                   colors[2][2]);

  robot.moveToWall(20, 180);
  robot.stop();
}

// autonomous LED pattern (ignores serial input)
void controlLED()
{
  unsigned long now = millis();

  if (now - lastChange >= 500)
  {
    lastChange = now;
    led.DeviceDriverSet_RBGLED_Color(NUM_LEDS,
                                     colors[idx][0],
                                     colors[idx][1],
                                     colors[idx][2]);
    idx = (idx + 1) % numColors;
  }
}

void loop()
{
  controlLED();

  // Example usage:
  // Move forward until reaching 20cm from wall
  // robot.moveToWall(20, 180); // distance=20cm, baseSpeed=180/255

  // // Turn to face left (-90 degrees)
  // robot.turnToAngle(-90, 5.0, true); // angle=-90, offset=5°, both wheels
  // // Turn to face left (-90 degrees)
  // robot.turnToAngle(-90, 5.0, true); // angle=-90, offset=5°, both wheels

  // // Move to wall again
  // robot.moveToWall(25, 180);
  // // Move to wall again
  // robot.moveToWall(25, 180);

  // // Turn to face right (90 degrees) using only outer wheel
  // robot.turnToAngle(90, 5.0, false); // angle=90, offset=5°, single wheel
  // // Turn to face right (90 degrees) using only outer wheel
  // robot.turnToAngle(90, 5.0, false); // angle=90, offset=5°, single wheel

  // Stop and wait
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