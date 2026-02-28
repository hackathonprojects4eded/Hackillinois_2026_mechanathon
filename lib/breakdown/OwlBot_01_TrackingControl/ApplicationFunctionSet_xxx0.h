/*
 * @Author: ELEGOO
 * @Date: 2019-07-10 16:46:17
 * @LastEditTime: 2020-06-19 10:33:15
 * @LastEditors: Changhua
 * @Description: OwlBot Car _00
 * @FilePath: 
 */

#ifndef _ApplicationFunctionSet_xxx0_H_
#define _ApplicationFunctionSet_xxx0_H_

#include <arduino.h>

class ApplicationFunctionSet
{
public:
  void ApplicationFunctionSet_Init(void);
  void ApplicationFunctionSet_Bootup(void);
  void ApplicationFunctionSet_SensorDataUpdate(void);  
  void ApplicationFunctionSet_Tracking(void);               //循迹

public:
  /*传感器数据*/
  
  volatile uint16_t TrackingData_R;    //循迹数据
  volatile uint16_t TrackingData_L;
  volatile uint16_t TrackingData_M;
  //volatile float mpu6050_Yaw; //偏航
  
public:

  boolean TrackingDetectionStatus_R = false;
  boolean TrackingDetectionStatus_L = false;
  boolean TrackingDetectionStatus_M = false;
  boolean Car_LeaveTheGround = true;

public: //Tracking
  uint16_t TrackingDetection_S = 500;
  uint16_t TrackingDetection_E = 850;
  uint16_t TrackingDetection_CarLeaveTheGround = 850;


public: //motor
  uint8_t CMD_is_MotorSelection;
  uint8_t CMD_is_MotorDirection;
  uint8_t CMD_is_MotorSpeed;
  uint32_t CMD_is_MotorTimer;
  uint8_t CMD_is_MotorSpeed_L;
  uint8_t CMD_is_MotorSpeed_R;
  uint8_t CMD_is_MotorSpeed_M;

public: //car
  uint8_t CMD_is_CarDirection;
  uint8_t CMD_is_CarSpeed;
  uint32_t CMD_is_CarTimer;
};
extern ApplicationFunctionSet Application_FunctionSet;

#endif
