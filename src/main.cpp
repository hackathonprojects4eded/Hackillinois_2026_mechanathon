#include <Arduino.h>

#include "Robot.h"

bool MANUAL_MODE = true;
Robot robot(MANUAL_MODE);

void setup()
{
  Serial.begin(9600);

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

  robot.led.DeviceDriverSet_RBGLED_xxx((uint16_t)(0), 5, CRGB::LawnGreen);
  delay(1000);
  robot.led.DeviceDriverSet_RBGLED_xxx((uint16_t)(0), 5, CRGB::Black);

  // delay(5000);

  // robot.moveToWall(30, 180);

  // robot.turnToAngle(-180, 3.0, false);
  // robot.moveToWall(30, 180);

  // robot.turnToAngle(0, 3.0, false);
  // robot.moveToWall(30, 180);

  // robot.stop();
}

void loop()
{
  robot.update();
  Serial.println(robot.getRow());

  // float roll = robot.getRow(); // this is actually pitch
  // float pitch = roll;          // fix sensor frame

  // const float centerAngle = 90.0; // servo mechanical center
  // const float gain = 5.0;         // tune this

  // float servoAngle = centerAngle + gain * pitch;

  // // constrain to left-half-circle limits
  // servoAngle = constrain(servoAngle, 0, 180);

  // Serial.println(servoAngle);

  // robot.servo.write(servoAngle);
  // robot.servo.refresh();
}

// #include "DeviceDriverSet_xxx0.h"
// #include "ApplicationFunctionSet_xxx0.h"

// ApplicationFunctionSet Application_FunctionSet;

// void setup()
// {
//   // Serial.begin(115200);
//   Application_FunctionSet.ApplicationFunctionSet_Init();
// }

// void loop()
// {
//   // put your main code here, to run repeatedly:
//   Application_FunctionSet.ApplicationFunctionSet_SerialPortDataAnalysis();
//   Application_FunctionSet.ApplicationFunctionSet_Rocker();
//   Application_FunctionSet.CMD_ClearAllFunctionsXXX();
// }