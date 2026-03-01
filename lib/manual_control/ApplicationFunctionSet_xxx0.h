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

#include <arduino.h>

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