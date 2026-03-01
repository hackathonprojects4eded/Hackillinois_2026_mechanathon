/*
 * @Description: In User Settings Edit
 * @Author:  HOU Changhua
 * @Date: 2019-07-12 18:30:51
 * @LastEditTime: 2019-10-15 10:35:23
 * @LastEditors: Please set LastEditors
 */
#pragma once
#ifndef _ApplicationFunctionSet_xxx0_H_
#define _ApplicationFunctionSet_xxx0_H_

#include <Arduino.h>

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

struct Application_xxx
{
  OwlBotMotionControl Motion_Control;
  OwlBotFunctionalModel Functional_Mode;
};

class ApplicationFunctionSet
{
public:
  void ApplicationFunctionSet_Init(void);
  void ApplicationFunctionSet_Rocker(void);
  void ApplicationFunctionSet_SerialPortDataAnalysis(void);

  void CMD_ClearAllFunctionsXXX(void);

public:
  String CommandSerialNumber;
};

#endif