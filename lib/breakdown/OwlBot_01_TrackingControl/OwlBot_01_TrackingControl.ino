/*
   @Author: ELEGOO
   @Date: 2019-07-10 16:46:17
   @LastEditTime: 2020-06-19 10:09:47
   @LastEditors: Changhua
   @Description: OwlBot Car _00
   @FilePath:
*/
#include <avr/wdt.h>
#include "ApplicationFunctionSet_xxx0.h"
#include <hardwareSerial.h>
#include "DeviceDriverSet_xxx0.h"

void setup()
{
  //Serial.begin(9600);
  // put your setup code here, to run once:
  Application_FunctionSet.ApplicationFunctionSet_Init();

}
void loop()
{
  delay(10);
  Application_FunctionSet.ApplicationFunctionSet_Tracking();
}
