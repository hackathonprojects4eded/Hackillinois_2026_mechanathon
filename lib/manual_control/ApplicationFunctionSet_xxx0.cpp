
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
#include "Buzzer_control.h"
#include <Adafruit_SoftServo.h>
#include "LED_control.h"
extern Adafruit_SoftServo servo;
extern DeviceDriverSet_RBGLED led;
#define _is_print 1
#define _Test_print 0
DeviceDriverSet_Motor AppMotor;
extern DeviceDriverSet_passiveBuzzer buzzer;

/*f(x) int */
static boolean function_xxx(long x, long s, long e) // f(x)
{
  if (s <= x && x <= e)
    return true;
  else
    return false;
}

Application_xxx Application_OwlBotxxx0;

void ApplicationFunctionSet::ApplicationFunctionSet_Init(void)
{
  Serial.begin(9600);
  AppMotor.DeviceDriverSet_Motor_Init();
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
    servo.write(180);
    servo.refresh();
    buzzer.DeviceDriverSet_passiveBuzzer_controlMonosyllabic(0, 100);
    led.DeviceDriverSet_RBGLED_xxx(0, 3, CRGB::Yellow);
    delay(50);

    AppMotor.DeviceDriverSet_Motor_control(/*direction_A*/ direction_just, /*speed_A*/ speed,
                                           /*direction_B*/ direction_back, /*speed_B*/ speed, /*controlED*/ control_enable); // Motor control
    break;
  case /* constant-expression */ Right:
    /* code */
    servo.write(180);
    servo.refresh();
    buzzer.DeviceDriverSet_passiveBuzzer_controlMonosyllabic(0, 100);
    led.DeviceDriverSet_RBGLED_Color(0, 255, 0, 255);
    led.DeviceDriverSet_RBGLED_Color(2, 255, 0, 255);
    led.DeviceDriverSet_RBGLED_Color(3, 255, 0, 255);
    delay(50);

    AppMotor.DeviceDriverSet_Motor_control(/*direction_A*/ direction_back, /*speed_A*/ speed,
                                           /*direction_B*/ direction_just, /*speed_B*/ speed, /*controlED*/ control_enable); // Motor control
    break;
  case /* constant-expression */ Forward:
    /* code */
    servo.write(180);
    servo.refresh();
    buzzer.DeviceDriverSet_passiveBuzzer_controlMonosyllabic(0, 200);

    led.DeviceDriverSet_RBGLED_xxx(0, 5, CRGB::Salmon);
    delay(50);

    AppMotor.DeviceDriverSet_Motor_control(/*direction_A*/ direction_just, /*speed_A*/ speed,
                                           /*direction_B*/ direction_just, /*speed_B*/ speed, /*controlED*/ control_enable); // Motor control
    break;
  case /* constant-expression */ Backward:
    /* code */
    servo.write(75);
    servo.refresh();
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
          //  Application_OwlBotxxx0.Motion_Control = Forward;
          Application_OwlBotxxx0.Motion_Control = Backward;
          break;
        case 2:
          //  Application_OwlBotxxx0.Motion_Control = Backward;
          Application_OwlBotxxx0.Motion_Control = Forward;
          break;
        case 3:
          //  Application_OwlBotxxx0.Motion_Control = Left;
          Application_OwlBotxxx0.Motion_Control = Left;
          break;
        case 4:
          // Application_OwlBotxxx0.Motion_Control = Right;
          Application_OwlBotxxx0.Motion_Control = Right;
          break;
        case 5:
          // Application_OwlBotxxx0.Motion_Control = LeftForward;
          Application_OwlBotxxx0.Motion_Control = RightBackward;
          break;
        case 6:
          // Application_OwlBotxxx0.Motion_Control = LeftBackward;
          Application_OwlBotxxx0.Motion_Control = RightForward;
          break;
        case 7:
          // Application_OwlBotxxx0.Motion_Control = RightForward;
          Application_OwlBotxxx0.Motion_Control = LeftBackward;
          break;
        case 8:
          // Application_OwlBotxxx0.Motion_Control = RightBackward;
          Application_OwlBotxxx0.Motion_Control = LeftForward;
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
