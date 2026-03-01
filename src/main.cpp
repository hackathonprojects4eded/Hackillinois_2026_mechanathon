#include <Arduino.h>

#include "Robot.h"
#include "Buzzer_control.h"
#include <Adafruit_SoftServo.h>

bool MANUAL_MODE = true;
Robot robot(MANUAL_MODE);

DeviceDriverSet_passiveBuzzer buzzer;
Adafruit_SoftServo servo;
MPU6050Wrapper imu;

unsigned long lastPacketTime = 0;
unsigned long WATCHDOG_TIMEOUT_MS = 250; // stop if no packet for 500ms

void setup()
{
  Serial.begin(9600);

  if (!robot.begin())
  {
    while (1)
      ;
  }

  bool ret = imu.begin();
  if (!ret)
  {
    // Serial.println("failed to init imu");
  }

  buzzer.DeviceDriverSet_passiveBuzzer_Scale_c8(100);

  robot.led.DeviceDriverSet_RBGLED_xxx((uint16_t)(0), 5, CRGB::DarkGreen);
  delay(1000);
  robot.led.DeviceDriverSet_RBGLED_xxx((uint16_t)(0), 5, CRGB::Black);

  servo.attach(3);
  servo.write(90);
  servo.refresh();

  // robot.servo.write(90); // 90 is down 180 is up
  // robot.servo.refresh();

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
  servo.refresh();
  imu.update();

  // Serial.print(F("quat:"));
  // Serial.print(robot.imu.getW());
  // Serial.print(F(","));
  // Serial.print(robot.imu.getX());
  // Serial.print(F(","));
  // Serial.print(robot.imu.getY());
  // Serial.print(F(","));
  // Serial.println(robot.imu.getZ());

  // Serial.println(robot.getRow());

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