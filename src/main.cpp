#include <Arduino.h>

#include "Robot.h"

bool MANUAL_MODE = false;
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

  robot.servo.write(90);
  robot.servo.refresh();
  // robot.servo.write(45);
  // robot.servo.refresh();
  // delay(1000);
  // robot.servo.write(90);
  // robot.servo.refresh();
  // delay(1000);
  // robot.servo.write(135);
  // robot.servo.refresh();
  // delay(1000);
  // robot.servo.write(180);
  // robot.servo.refresh();
  // delay(1000);
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
  // Serial.println(robot.getRow());

  // float roll = robot.getRow(); // this is actually pitch
  // float pitch = -roll;         // fix sensor frame

  // const float centerAngle = 90.0; // servo mechanical center
  // const float gain = 1.0;         // tune this

  // float servoAngle = centerAngle + gain * pitch;

  // // constrain to left-half-circle limits
  // servoAngle = constrain(servoAngle, 0, 180);

  // robot.servo.write(servoAngle);
  // robot.servo.refresh(); // if using software servo

  // robot.servo.write(robot.getRow());
  // robot.servo.refresh();

  // Serial.println("----");
  // Serial.println(robot.getRow()); //thjis one matters (and up is negative cuz its flipped rip)
  // Serial.println(robot.getPitch());
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