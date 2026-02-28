/*
 * @Author: ELEGOO
 * @Date: 2019-07-10 16:46:17
 * @LastEditTime: 2020-06-19 10:30:03
 * @LastEditors: Changhua
 * @Description: OwlBot Car _00
 * @FilePath: 
 */
#include <hardwareSerial.h>
#include <stdio.h>
#include <string.h>
#include <arduino.h>

#include "MsTimer2.h"
#include "ApplicationFunctionSet_xxx0.h"
#include "DeviceDriverSet_xxx0.h"
#define _is_print 1
#define _Test_print 0 //When testing, remember to set 0 after using the test to save controller resources and load.
ApplicationFunctionSet Application_FunctionSet;

/*硬件设备成员对象序列*/
DeviceDriverSet_STM8S003F3_IR AppSTM8S003F3_IR;
DeviceDriverSet_Motor AppMotor;

/*f(x) int */
static boolean
function_xxx(long x, long s, long e) //f(x)
{
  if (s <= x && x <= e)
    return true;
  else
    return false;
}

/*运动方向控制序列*/
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
};               //direction:前行（1）、后退（2）、 左前（3）、右前（4）、后左（5）、后右（6）

/*模式控制序列*/
enum OwlBotFunctionalModel
{
  Standby_mode,           /*空闲模式*/
  TraceBased_mode,        /*循迹模式*/
  ObstacleAvoidance_mode, /*避障模式*/
  Rocker_mode,            /*摇杆模式*/
  Exploration_mode,

  CMD_Programming_mode,                   /*编程模式*/
  CMD_ClearAllFunctions_Standby_mode,     /*清除所有功能：进入空闲模式*/
  CMD_ClearAllFunctions_Programming_mode, /*清除所有功能：进入编程模式*/
  CMD_MotorControl,                       /*电机控制模式*/
  CMD_CarControl_TimeLimit,               /*小车方向控制：有时间限定模式*/
  CMD_CarControl_NoTimeLimit,             /*小车方向控制：无时间限定模式*/
  CMD_MotorControl_Speed,                 /*电机控制:控制转速模式*/
  CMD_ServoControl,                       /*舵机控制:模式*/
  CMD_VoiceControl,                       /*声音控制:模式*/
  CMD_ledExpressionControl,               /*矩阵表情控制:模式*/
  CMD_ledNumberControl,                   /*矩阵数字控制:模式*/
  CMD_LightingControl_TimeLimit,          /*灯光控制:模式*/
  CMD_LightingControl_NoTimeLimit,        /*灯光控制:模式*/
  CMD_TrajectoryControl,                  /*轨迹控制:模式*/
};

/*控制管理成员*/
struct Application_xxx
{
  OwlBotMotionControl Motion_Control;
  OwlBotFunctionalModel Functional_Mode;
  unsigned long CMD_CarControl_Millis;
  unsigned long CMD_LightingControl_Millis;
  float AppMPU6050getdata_yaw;
};
Application_xxx Application_OwlBotxxx0;
static bool ApplicationFunctionSet_OwlBotLeaveTheGround(void);
static void ApplicationFunctionSet_MetalCarLinearMotionControl(OwlBotMotionControl direction, uint8_t directionRecord, uint8_t speed, uint8_t Kp, uint8_t UpperLimit);
static void ApplicationFunctionSet_OwlBotMotionControl(OwlBotMotionControl direction, uint8_t speed);

void ApplicationFunctionSet::ApplicationFunctionSet_Init(void)
{
  Serial.begin(9600);
  AppSTM8S003F3_IR.DeviceDriverSet_STM8S003F3_IR_Init();
  AppMotor.DeviceDriverSet_Motor_Init();
  Application_OwlBotxxx0.Functional_Mode = TraceBased_mode;
}



/*
 --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
@ Function prototype:   static void ApplicationFunctionSet_OwlBotMotionControl(OwlBotMotionControl direction, uint8_t speed)
@ Functional function:  Owl Car 运动状态控制
@ Input parameters:     1# direction:前行（1）、后退（2）、 左前（3）、右前（4）、后左（5）、后右（6）、停止（6）
                        2# speed速度(0--255)
@ Output parameters:    none
@ Other Notes:          此为ApplicationFunctionSet层静态方法<调用硬件驱动层DeviceDriverSet_xxx0  Motor_control接口实现>
$ Elegoo & OwlBot & 2019-09
 --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
*/
static void
ApplicationFunctionSet_OwlBotMotionControl(OwlBotMotionControl direction, uint8_t speed)
{
  ApplicationFunctionSet Application_FunctionSet;
  static uint8_t directionRecord = 0;

  switch (direction)
  {
  case /* constant-expression */
      Forward:
    /* code */
    //if (Application_OwlBotxxx0.Functional_Mode == TraceBased_mode)
    if (Application_OwlBotxxx0.Functional_Mode == TraceBased_mode || Application_OwlBotxxx0.Functional_Mode == CMD_TrajectoryControl)
    {
      AppMotor.DeviceDriverSet_Motor_control(/*direction_A*/ direction_just, /*speed_A*/ speed,
                                             /*direction_B*/ direction_just, /*speed_B*/ speed, /*controlED*/ control_enable); //Motor control
    }
    
    break;
  case /* constant-expression */ Backward:
    /* code */
    if (Application_OwlBotxxx0.Functional_Mode == TraceBased_mode || Application_OwlBotxxx0.Functional_Mode == CMD_TrajectoryControl)
    //if (Application_OwlBotxxx0.Functional_Mode == TraceBased_mode)
    {
      AppMotor.DeviceDriverSet_Motor_control(/*direction_A*/ direction_back, /*speed_A*/ speed,
                                             /*direction_B*/ direction_back, /*speed_B*/ speed, /*controlED*/ control_enable); //Motor control
    }

    break;
  case /* constant-expression */ Left:
    /* code */
    directionRecord = 3;
    AppMotor.DeviceDriverSet_Motor_control(/*direction_A*/ direction_just, /*speed_A*/ speed,
                                           /*direction_B*/ direction_back, /*speed_B*/ speed, /*controlED*/ control_enable); //Motor control
    break;
  case /* constant-expression */ Right:
    /* code */
    directionRecord = 4;
    AppMotor.DeviceDriverSet_Motor_control(/*direction_A*/ direction_back, /*speed_A*/ speed,
                                           /*direction_B*/ direction_just, /*speed_B*/ speed, /*controlED*/ control_enable); //Motor control
    break;
  case /* constant-expression */ LeftForward:
    /* code */
    directionRecord = 5;
    AppMotor.DeviceDriverSet_Motor_control(/*direction_A*/ direction_just, /*speed_A*/ speed,
                                           /*direction_B*/ direction_just, /*speed_B*/ speed - 130, /*controlED*/ control_enable); //Motor control

    break;
  case /* constant-expression */ LeftBackward:
    /* code */
    directionRecord = 6;
    AppMotor.DeviceDriverSet_Motor_control(/*direction_A*/ direction_back, /*speed_A*/ speed,
                                           /*direction_B*/ direction_back, /*speed_B*/ speed - 130, /*controlED*/ control_enable); //Motor control
    break;
  case /* constant-expression */ RightForward:
    /* code */
    directionRecord = 7;
    AppMotor.DeviceDriverSet_Motor_control(/*direction_A*/ direction_just, /*speed_A*/ speed - 130,
                                           /*direction_B*/ direction_just, /*speed_B*/ speed, /*controlED*/ control_enable); //Motor control
    break;
  case /* constant-expression */ RightBackward:
    /* code */
    directionRecord = 8;
    AppMotor.DeviceDriverSet_Motor_control(/*direction_A*/ direction_back, /*speed_A*/ speed - 130,
                                           /*direction_B*/ direction_back, /*speed_B*/ speed, /*controlED*/ control_enable); //Motor control
    break;
  case /* constant-expression */ stop_it:
    /* code */
    directionRecord = 9;
    AppMotor.DeviceDriverSet_Motor_control(/*direction_A*/ direction_void, /*speed_A*/ 0,
                                           /*direction_B*/ direction_void, /*speed_B*/ 0, /*controlED*/ control_enable); //Motor control
    break;
  default:
    directionRecord = 8;
    break;
  }
}
/*
 --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
@ Function prototype:   void ApplicationFunctionSet::ApplicationFunctionSet_Tracking(void)
@ Functional function:  Owl Car 循迹
@ Input parameters:     none
@ Output parameters:    none
@ Other Notes:          此为ApplicationFunctionSet层公共方法
  模式进入：ApplicationFunctionSet_KeyCommand():1 / ApplicationFunctionSet_SerialPortDataAnalysis()：N 101
  1#可通过串口命令及模式按键切换进入当前工作模式，即：循迹
  2#获取两个模拟量的红外光电传感器数据（循迹模块:0--1024）
  3#设定阈值（线上：200 -- 860）
$ Elegoo & OwlBot & 2019-09
 --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 循迹功能编码表：2E3=8（三个传感器组合编程8个状态）
 --------------------------------|
  L/   0  1  0  1  0  1  0  1    |
 --------------------------------|
  M/   0  0  1  1  0  0  1  1    |
 --------------------------------|
  R/   0  0  0  0  1  1  1  1    |
 --------------------------------|
       0  1  2  3  4  5  6  7    |
 --------------------------------|
 前进：2、7     /    左转：1、3     /     右转：4、6      /      盲扫：0
 --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
*/
void ApplicationFunctionSet::ApplicationFunctionSet_Tracking(void)
{
  static boolean timestamp = true;
  static boolean BlindDetection = true;
  static unsigned long MotorRL_time = 0;
  static uint8_t DirectionRecording = 0;

  if (Application_OwlBotxxx0.Functional_Mode == TraceBased_mode)
  {
//     if (Car_LeaveTheGround == false)
//    {
//      ApplicationFunctionSet_OwlBotMotionControl(stop_it, 0);
//      return;
//    }   

    AppSTM8S003F3_IR.DeviceDriverSet_STM8S003F3_IR_Get(&TrackingData_L /*uint16_t * STM8S003F3_IRL out*/,
                                                       &TrackingData_M /*uint16_t * STM8S003F3_IRM out*/,
                                                       &TrackingData_R /*uint16_t * STM8S003F3_IRR out*/);

#if _Test_print
    static unsigned long print_time = 0;
    if (_millis() - print_time > 500)
    {
      print_time = _millis();
      Serial.print("TrackingData_L=");
      Serial.println(TrackingData_L);
      Serial.print("TrackingData_M=");
      Serial.println(TrackingData_M);
      Serial.print("TrackingData_R=");
      Serial.println(TrackingData_R);
    }
#endif

    if (1 == function_xxx(TrackingData_M, TrackingDetection_S, TrackingDetection_E)) //前进 2==010
    {
      timestamp = true;
      BlindDetection = true;
      /*控制左右电机转动：实现匀速直行*/
      ApplicationFunctionSet_OwlBotMotionControl(Forward, 200);
      DirectionRecording = 0;
    }
    else if (1 == function_xxx(TrackingData_L, TrackingDetection_S, TrackingDetection_E) &&
             // 0 == function_xxx(TrackingData_M, TrackingDetection_S, TrackingDetection_E) &&
             0 == function_xxx(TrackingData_R, TrackingDetection_S, TrackingDetection_E)) //左转 1==001 || 3==011  可以不关心 M
    {
      timestamp = true;
      BlindDetection = true;
      /*控制左右电机转动：前左*/
      ApplicationFunctionSet_OwlBotMotionControl(Left, 255);
      DirectionRecording = 1;
    }
    else if (0 == function_xxx(TrackingData_L, TrackingDetection_S, TrackingDetection_E) &&
             // 0 == function_xxx(TrackingData_M, TrackingDetection_S, TrackingDetection_E) &&
             1 == function_xxx(TrackingData_R, TrackingDetection_S, TrackingDetection_E)) //右转 4==100 || 6==110  可以不关心 M
    {
      timestamp = true;
      BlindDetection = true;
      /*控制左右电机转动：前右*/
      ApplicationFunctionSet_OwlBotMotionControl(Right, 255);
      DirectionRecording = 2;
    }
    else //不在黑线上的时候。。。执行盲循迹   0==0000
    {

      ApplicationFunctionSet_OwlBotMotionControl(stop_it, 0);
      //获取时间戳 timestamp
      if (timestamp == true)
      {
        timestamp = false;
        MotorRL_time = _millis();
        ApplicationFunctionSet_OwlBotMotionControl(stop_it, 0);
      }
      /*Blind Detection*/
      if ((function_xxx((_millis() - MotorRL_time), 0, 200) || function_xxx((_millis() - MotorRL_time), 600, 1000) || function_xxx((_millis() - MotorRL_time), 1500, 3600)) && BlindDetection == true)
      {
        if (1 == DirectionRecording)
        {
          ApplicationFunctionSet_OwlBotMotionControl(Right, 255);
        }
        else
        {
          ApplicationFunctionSet_OwlBotMotionControl(Left, 255);
        }
      }
      else if (((function_xxx((_millis() - MotorRL_time), 200, 600)) || function_xxx((_millis() - MotorRL_time), 1000, 1400)) && BlindDetection == true)
      {
        if (1 == DirectionRecording)
        {
          ApplicationFunctionSet_OwlBotMotionControl(Left, 255);
        }
        else
        {
          ApplicationFunctionSet_OwlBotMotionControl(Right, 255);
        }
      }
      else if (((function_xxx((_millis() - MotorRL_time), 1400, 1500))) && BlindDetection == true)
      {
        ApplicationFunctionSet_OwlBotMotionControl(Forward, 200);
      }
      else if ((function_xxx((_millis() - MotorRL_time), 5000, 5500))) // Blind Detection ...s ?
      {
        BlindDetection == false;
        ApplicationFunctionSet_OwlBotMotionControl(stop_it, 0);
      }
    }
  }
  else
  {
    timestamp = true;
    BlindDetection = true;
    MotorRL_time = 0;
  }
}

static bool ApplicationFunctionSet_OwlBotLeaveTheGround(void)
{
  /*STM8S003F3红外对管数据更新：作用于循迹*/
  AppSTM8S003F3_IR.DeviceDriverSet_STM8S003F3_IR_Get(&Application_FunctionSet.TrackingData_L /*uint16_t * STM8S003F3_IRL out*/,
                                                     &Application_FunctionSet.TrackingData_M /*uint16_t * STM8S003F3_IRM out*/,
                                                     &Application_FunctionSet.TrackingData_R /*uint16_t * STM8S003F3_IRR out*/);

  /*R循迹状态更新*/
  Application_FunctionSet.TrackingDetectionStatus_R = function_xxx(Application_FunctionSet.TrackingData_R, Application_FunctionSet.TrackingDetection_S, Application_FunctionSet.TrackingDetection_E);
  /*M循迹状态更新*/
  Application_FunctionSet.TrackingDetectionStatus_M = function_xxx(Application_FunctionSet.TrackingData_M, Application_FunctionSet.TrackingDetection_S, Application_FunctionSet.TrackingDetection_E);
  /*L循迹状态更新*/
  Application_FunctionSet.TrackingDetectionStatus_L = function_xxx(Application_FunctionSet.TrackingData_L, Application_FunctionSet.TrackingDetection_S, Application_FunctionSet.TrackingDetection_E);
  //检测小车是否离开地面
  if (Application_FunctionSet.TrackingData_L > Application_FunctionSet.TrackingDetection_CarLeaveTheGround &&
      Application_FunctionSet.TrackingData_M > Application_FunctionSet.TrackingDetection_CarLeaveTheGround &&
      Application_FunctionSet.TrackingData_R > Application_FunctionSet.TrackingDetection_CarLeaveTheGround)
  {
    Application_FunctionSet.Car_LeaveTheGround = false;
    return false;
  }
  else
  {
    Application_FunctionSet.Car_LeaveTheGround = true;
    return true;
  }
}
