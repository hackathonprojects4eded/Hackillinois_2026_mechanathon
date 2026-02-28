
/*
   @Description: Application Function Set
   @Author: HOU Changhua
   @Date: 2019-07-12 18:30:51
   @LastEditTime: 2019-10-14 18:10:30
   @LastEditors: Please set LastEditors
*/

#include "ApplicationFunctionSet_xxx0.h"
#include "DeviceDriverSet_xxx0.h"

/*Hardware device member object*/
DeviceDriverSet_Motor AppMotor;

/*Motion direction control sequence*/
enum OwlBotMotionControl
{
  Forward,       //(1)
  Backward,      //(2)
  Left,          //(3)
  Right,         //(4)
  LeftForward,   //(5)
  LeftBackward,  //(6)
  RightForward,  //(7)
  RightBackward, //(8)
  stop_it        //(9)
};            //direction

void ApplicationFunctionSet::ApplicationFunctionSet_Init(void)
{
  Serial.begin(9600);
  AppMotor.DeviceDriverSet_Motor_Init();
}

/*
  --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  @ Function prototype:   static void ApplicationFunctionSet_OwlBotMotionControl(OwlBotMotionControl direction, uint8_t speed)
  @ Functional function:  Owl Car motion state control
  @ Input parameters:     1# direction:Forward（1）、Backward（2）、 LeftForward（3）、RightForward（4）、LeftBackward（5）、RightBackward（6）、stop_it（6）
                        2# speed(0--255)
  @ Output parameters:    none
  @ Other Notes:          This is the static method of the applicationfunctionset layer < call the hardware driver layer devicedriverset > Motor > control interface implementation >
  $ Shenzhen, China:Elegoo & HOU Changhua & 2019-09
  --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
*/
static void ApplicationFunctionSet_OwlBotMotionControl(OwlBotMotionControl direction, uint8_t speed)
{
  ApplicationFunctionSet Application_FunctionSet;
  switch (direction)
  {
    case /* constant-expression */ Forward:
      /* code */
      AppMotor.DeviceDriverSet_Motor_control(/*direction_A*/ direction_just, /*speed_A*/ speed,
          /*direction_B*/ direction_just, /*speed_B*/ speed, /*controlED*/ control_enable); //Motor control
      break;
    case /* constant-expression */ Left:
      /* code */
      AppMotor.DeviceDriverSet_Motor_control(/*direction_A*/ direction_just, /*speed_A*/ speed,
          /*direction_B*/ direction_back, /*speed_B*/ speed, /*controlED*/ control_enable); //Motor control
      break;
    case /* constant-expression */ Right:
      /* code */
      AppMotor.DeviceDriverSet_Motor_control(/*direction_A*/ direction_back, /*speed_A*/ speed,
          /*direction_B*/ direction_just, /*speed_B*/ speed, /*controlED*/ control_enable); //Motor control
      break;
    case /* constant-expression */ Backward:
      /* code */
      AppMotor.DeviceDriverSet_Motor_control(/*direction_A*/ direction_back, /*speed_A*/ speed,
          /*direction_B*/ direction_back, /*speed_B*/ speed, /*controlED*/ control_enable); //Motor control
      break;
    case /* constant-expression */ LeftForward:
      /* code */
      AppMotor.DeviceDriverSet_Motor_control(/*direction_A*/ direction_just, /*speed_A*/ speed - 130,
          /*direction_B*/ direction_just, /*speed_B*/ speed, /*controlED*/ control_enable); //Motor control
      break;
    case /* constant-expression */ RightForward:
      /* code */
      AppMotor.DeviceDriverSet_Motor_control(/*direction_A*/ direction_just, /*speed_A*/ speed - 130,
          /*direction_B*/ direction_just, /*speed_B*/ speed, /*controlED*/ control_enable); //Motor control
      break;
    case /* constant-expression */ LeftBackward:
      /* code */
      AppMotor.DeviceDriverSet_Motor_control(/*direction_A*/ direction_back, /*speed_A*/ speed,
          /*direction_B*/ direction_back, /*speed_B*/ speed - 130, /*controlED*/ control_enable); //Motor control
      break;
    case /* constant-expression */ RightBackward:
      /* code */
      AppMotor.DeviceDriverSet_Motor_control(/*direction_A*/ direction_back, /*speed_A*/ speed - 130,
          /*direction_B*/ direction_back, /*speed_B*/ speed, /*controlED*/ control_enable); //Motor control
      break;
    case /* constant-expression */ stop_it:
      /* code */
      AppMotor.DeviceDriverSet_Motor_control(/*direction_A*/ direction_void, /*speed_A*/ 0,
          /*direction_B*/ direction_void, /*speed_B*/ 0, /*controlED*/ control_enable); //Motor control
      break;
    default:
      break;
  }
}

#define Max_speed 255
void ApplicationFunctionSet::Test(void)
{
  for (int i = 0; i <= stop_it; i++)
  {
    ApplicationFunctionSet_OwlBotMotionControl(i, Max_speed);
    delay(1000);
  }
}
