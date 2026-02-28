
/*
 * @Description: Application Function Set
 * @Author: HOU Changhua
 * @Date: 2019-07-12 18:30:51
 * @LastEditTime: 2019-10-15 10:32:19
 * @LastEditors: Please set LastEditors
 */
#include <HardwareSerial.h>
#include <stdio.h>
#include <string.h>
#include "ApplicationFunctionSet_xxx0.h"
#include "DeviceDriverSet_xxx0.h"

#include "ArduinoJson-v6.11.1.h" //ArduinoJson
#include <SoftwareSerial.h>
#include "Ultrasonic_control.h"

#define _is_print 1
#define _Test_print 0 // When testing, remember to set 0 after using the test to save controller resources and load.

/*硬件设备成员对象序列*/
DeviceDriverSet_Motor AppMotor;
DeviceDriverSet_ULTRASONIC AppUltrasonic;

// Bluetooth instance (RX=10, TX=11)
SoftwareSerial BTSerial(10, 11);

#define OBSTACLE_DISTANCE_CM 20  // Obstacle detected if closer than 20cm
bool autonomous_mode = true;

/*f(x) int */
static boolean function_xxx(long x, long s, long e) // f(x)
{
  if (s <= x && x <= e)
    return true;
  else
    return false;
}

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
}; // direction:前行（1）、后退（2）、 左前（3）、右前（4）、后左（5）、后右（6）

/*Mode control sequence*/
enum OwlBotFunctionalModel
{
  Standby_mode,           /*idle mode*/
  TraceBased_mode,        /* tracking mode*/
  ObstacleAvoidance_mode, /*obstacle mode*/
  Rocker_mode,            /*rocker mode*/

  CMD_Programming_mode,                   /*programming mode*/
  CMD_ClearAllFunctions_Standby_mode,     /*clear all functions: enter idle mode*/
  CMD_ClearAllFunctions_Programming_mode, /* clear all functions: enter programming mode*/
  CMD_MotorControl,                       /* motor control mode*/
  CMD_CarControl_TimeLimit,               /*car direction control: time limited mode*/
  CMD_CarControl_NoTimeLimit,             /* car direction control: no time limit mode*/
  CMD_MotorControl_Speed,                 /* motor control: control speed mode*/
  CMD_ServoControl,                       /* servo control: mode*/
  CMD_VoiceControl,                       /*voice control: mode*/
  CMD_ledExpressionControl,               /*matrix expressioncontrol: mode*/
  CMD_ledNumberControl,                   /*matrix digital control: mode*/
  CMD_LightingControl_TimeLimit,          /*light control: mode*/
  CMD_LightingControl_NoTimeLimit,        /*light control: mode*/
  CMD_TrajectoryControl,                  /*Trajectory control: mode*/
};

/*Control management member*/
struct Application_xxx
{
  OwlBotMotionControl Motion_Control;
  OwlBotFunctionalModel Functional_Mode;
};
Application_xxx Application_OwlBotxxx0;

void ApplicationFunctionSet::ApplicationFunctionSet_Init(void)
{
  Serial.begin(9600);
  AppMotor.DeviceDriverSet_Motor_Init();

  // Initialize Bluetooth
  BTSerial.begin(9600);
  delay(50);

  // Initialize Ultrasonic sensor
  AppUltrasonic.DeviceDriverSet_ULTRASONIC_Init();

  Serial.println("System initialized: Motor, Bluetooth, Ultrasonic ready.");
}

/*
 --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
@ Function prototype:   static void ApplicationFunctionSet_OwlBotMotionControl(OwlBotMotionControl direction, uint8_t speed)
@ Functional function:  Owl Car 运动状态控制
@ Input parameters:     1# direction:前行（1）、后退（2）、 左前（3）、右前（4）、后左（5）、后右（6）、停止（6）
                        2# speed速度(0--255)
@ Output parameters:    none
@ Other Notes:          此为ApplicationFunctionSet层静态方法<调用硬件驱动层DeviceDriverSet_xxx0  Motor_control接口实现>
$ Shenzhen, China:Elegoo & HOU Changhua & 2019-09
 --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
*/
static void ApplicationFunctionSet_OwlBotMotionControl(OwlBotMotionControl direction, uint8_t speed)
{
  ApplicationFunctionSet Application_FunctionSet;
  switch (direction)
  {
  case /* constant-expression */ Left:
    /* code */
    AppMotor.DeviceDriverSet_Motor_control(/*direction_A*/ direction_just, /*speed_A*/ speed,
                                           /*direction_B*/ direction_back, /*speed_B*/ speed, /*controlED*/ control_enable); // Motor control
    break;
  case /* constant-expression */ Right:
    /* code */
    AppMotor.DeviceDriverSet_Motor_control(/*direction_A*/ direction_back, /*speed_A*/ speed,
                                           /*direction_B*/ direction_just, /*speed_B*/ speed, /*controlED*/ control_enable); // Motor control
    break;
  case /* constant-expression */ Forward:
    /* code */
    AppMotor.DeviceDriverSet_Motor_control(/*direction_A*/ direction_just, /*speed_A*/ speed,
                                           /*direction_B*/ direction_just, /*speed_B*/ speed, /*controlED*/ control_enable); // Motor control
    break;
  case /* constant-expression */ Backward:
    /* code */
    AppMotor.DeviceDriverSet_Motor_control(/*direction_A*/ direction_back, /*speed_A*/ speed,
                                           /*direction_B*/ direction_back, /*speed_B*/ speed, /*controlED*/ control_enable); // Motor control
    break;
  case /* constant-expression */ RightForward:
    /* code */
    AppMotor.DeviceDriverSet_Motor_control(/*direction_A*/ direction_back, /*speed_A*/ speed - 130,
                                           /*direction_B*/ direction_just, /*speed_B*/ speed, /*controlED*/ control_enable); // Motor control
    break;
  case /* constant-expression */ LeftForward:
    /* code */
    AppMotor.DeviceDriverSet_Motor_control(/*direction_A*/ direction_just, /*speed_A*/ speed,
                                           /*direction_B*/ direction_back, /*speed_B*/ speed - 130, /*controlED*/ control_enable); // Motor control
    break;
  case /* constant-expression */ LeftBackward:
    /* code */
    AppMotor.DeviceDriverSet_Motor_control(/*direction_A*/ direction_back, /*speed_A*/ speed,
                                           /*direction_B*/ direction_back, /*speed_B*/ speed - 130, /*controlED*/ control_enable); // Motor control
    break;
    break;
  case /* constant-expression */ RightBackward:
    /* code */
    AppMotor.DeviceDriverSet_Motor_control(/*direction_A*/ direction_back, /*speed_A*/ speed - 130,
                                           /*direction_B*/ direction_back, /*speed_B*/ speed, /*controlED*/ control_enable); // Motor control
    break;
  case /* constant-expression */ stop_it:
    /* code */
    AppMotor.DeviceDriverSet_Motor_control(/*direction_A*/ direction_void, /*speed_A*/ 0,
                                           /*direction_B*/ direction_void, /*speed_B*/ 0, /*controlED*/ control_enable); // Motor control
    break;
  default:
    break;
  }
}

/*
 --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
@ Function prototype:   void ApplicationFunctionSet::ApplicationFunctionSet_Rocker(void)
@ Functional function:  Owl Car 摇杆
@ Input parameters:     none
@ Output parameters:    none
@ Other Notes:          此为ApplicationFunctionSet层公共方法<调用ApplicationFunctionSet_OwlBotMotionControl()>
  命令N102:APP摇杆控制(控制命令接收及解析实现：ApplicationFunctionSet_SerialPortDataAnalysis())
$ Shenzhen, China:Elegoo & HOU Changhua & 2019-09
 --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
*/
void ApplicationFunctionSet::ApplicationFunctionSet_Rocker(void)
{
  if (Application_OwlBotxxx0.Functional_Mode == Rocker_mode)
  {
    // When the car is turning... Slow down slightly

    ApplicationFunctionSet_OwlBotMotionControl(Application_OwlBotxxx0.Motion_Control /*direction*/, 255 /*speed*/);
  }
}
/*
 --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
@ Function prototype:   void ApplicationFunctionSet::CMD_ClearAllFunctionsXXX(void)
@ Functional function:  Owl Car 清除功能
@ Input parameters:     none
@ Output parameters:    none
@ Other Notes:          此为ApplicationFunctionSet层公共方法 <调用硬件驱动层: DeviceDriverSet_xxx0_H >
    CMD模式：N100:指令
    0# N100 清除所有功能进入空闲模式
$ Shenzhen, China:Elegoo & HOU Changhua & 2019-09
 --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
*/
void ApplicationFunctionSet::CMD_ClearAllFunctionsXXX(void)
{
  if (Application_OwlBotxxx0.Functional_Mode == CMD_ClearAllFunctions_Standby_mode) // 0# N100 清除所有功能进入空闲模式
  {
    ApplicationFunctionSet_OwlBotMotionControl(stop_it, 0);
    Application_OwlBotxxx0.Functional_Mode = Standby_mode;
  }
}
/*
 --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
@ Function prototype:   void ApplicationFunctionSet::ApplicationFunctionSet_SerialPortDataAnalysis(void)
@ Functional function:  Owl Car 串口数据解析
@ Input parameters:     none
@ Output parameters:    none
@ Other Notes:          此为ApplicationFunctionSet层公共方法 <调用硬件驱动层: DeviceDriverSet_xxx0_H >
    1#接收串口数据流
    2#导入JsonDocument
    3#解析并更新控制命令的信号量值
$ Shenzhen, China:Elegoo & HOU Changhua & 2019-09
 --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
*/
void ApplicationFunctionSet::ApplicationFunctionSet_SerialPortDataAnalysis(void)
{
  String SerialPortData = "";
  // 1 #接收串口数据流
  while ((Serial.available() > 0) && (false == SerialPortData.endsWith("}")))
  {
    SerialPortData += char(Serial.read());
    _delay(6); /*Necessary delay to prevent data loss*/
  }
  if ((SerialPortData.length() > 0) && (SerialPortData != "") && (true == SerialPortData.endsWith("}")))
  {
#if _Test_print
    Serial.println(SerialPortData);
#endif
    // 2#导入JsonDocument
    StaticJsonDocument<200> doc;                                       // 声明一个JsonDocument对象
    DeserializationError error = deserializeJson(doc, SerialPortData); // 反序列化JSON数据
    if (!error)                                                        // 检查反序列化是否成功
    {
      int control_mode_N = doc["N"];
      char buf[3];
      uint8_t temp = doc["H"];
      sprintf(buf, "%d", temp);
      CommandSerialNumber = buf; // 获取新命令的序号
      // 3#解析并更新控制命令的信号量值
      switch (control_mode_N) /*以下代码块请结合小车通讯协议V.docx 查看*/
      {
      case 1: /*<命令：N 1> */
        break;
      case 2: /*<命令：N 2>*/
        break;
      case 3: /*<命令：N 3>*/
        break;
      case 4: /*<命令：N 4>*/
        break;
      case 5: /*<命令：N 5>*/
        break;
      case 6: /*<命令：N 6>*/

        break;
      case 7: /*<命令：N 7>*/
        break;
      case 8: /*<命令：N 8>*/
        break;
      case 9: /*<命令：N 9>*/
        break;
      case 10: /*<命令：N 10>*/
        break;
      case 21: /*<命令：N 21>*/
        break;
      case 22: /*<命令：N 22>*/
        break;
      case 23: /*<命令：N 23>*/
        break;
      case 103: /*<命令：N 103>*/
        break;
      case 110: /*<命令：N 110>*/
        break;
      case 100:                                                                      /*<命令：N 100>*/
        Application_OwlBotxxx0.Functional_Mode = CMD_ClearAllFunctions_Standby_mode; /*清除功能：进入空闲模式*/
        autonomous_mode = false;
        break;
      case 101: /*<命令：N 101>*/

        break;
      case 104: /*<命令：N 104>*/
        break;
      case 105: /*<命令：N 105>*/
        break;
      case 102: /*<命令：N 102> :摇杆控制命令*/
        Application_OwlBotxxx0.Functional_Mode = Rocker_mode;
        uint8_t Rocker_temp = doc["D1"];
        switch (Rocker_temp)
        {
        case 1:
          Application_OwlBotxxx0.Motion_Control = Forward;
          break;
        case 2:
          Application_OwlBotxxx0.Motion_Control = Backward;
          break;
        case 3:
          Application_OwlBotxxx0.Motion_Control = Left;
          break;
        case 4:
          Application_OwlBotxxx0.Motion_Control = Right;
          break;
        case 5:
          Application_OwlBotxxx0.Motion_Control = LeftForward;
          break;
        case 6:
          Application_OwlBotxxx0.Motion_Control = LeftBackward;
          break;
        case 7:
          Application_OwlBotxxx0.Motion_Control = RightForward;
          break;
        case 8:
          Application_OwlBotxxx0.Motion_Control = RightBackward;
          break;
        case 9:
          Application_OwlBotxxx0.Motion_Control = stop_it;
          break;
        default:
          Application_OwlBotxxx0.Motion_Control = stop_it;
          break;
        }
#if _is_print
// Serial.print('{' + CommandSerialNumber + "_ok}");
#endif
        break;
      default:
        break;
      }
    }
  }
}

/*
 Obstacle Avoidance: Detect obstacles and turn the robot
 Sends distance data over Bluetooth and automatically avoids obstacles
*/
void ApplicationFunctionSet::ApplicationFunctionSet_ObstacleAvoidance(void)
{
  if (Application_OwlBotxxx0.Functional_Mode == ObstacleAvoidance_mode)
  {
    // Read distance from ultrasonic sensor
    float distanceCm = AppUltrasonic.DeviceDriverSet_ULTRASONIC_GetDistanceCm();

    // Send distance data to Bluetooth
    static unsigned long lastBTSend = 0;
    if (millis() - lastBTSend >= 500) // Send every 500ms
    {
      lastBTSend = millis();
      BTSerial.print("DIST:");
      BTSerial.print(distanceCm, 1);
      BTSerial.println("cm");

      Serial.print("Obstacle distance: ");
      Serial.print(distanceCm, 1);
      Serial.println(" cm");
    }

    // Obstacle avoidance logic
    if (distanceCm < OBSTACLE_DISTANCE_CM && distanceCm > 0)
    {
      // Obstacle detected - stop and turn right
      Serial.println("Obstacle detected! Turning right...");
      BTSerial.println("OBSTACLE DETECTED - TURNING");

      // Stop briefly
      ApplicationFunctionSet_OwlBotMotionControl(stop_it, 0);
      _delay(200);

      // Turn right
      ApplicationFunctionSet_OwlBotMotionControl(Right, 200);
      _delay(500);

      // Resume forward
      ApplicationFunctionSet_OwlBotMotionControl(Forward, 150);
    }
    else
    {
      // No obstacle, move forward
      ApplicationFunctionSet_OwlBotMotionControl(Forward, 180);
    }
  }
}

void ApplicationFunctionSet::SetFunctionalMode(int mode)
{
  Application_OwlBotxxx0.Functional_Mode = (OwlBotFunctionalModel)mode;
}
