#include <Arduino.h>

#include "ApplicationFunctionSet_xxx0.h"
// put function declarations here:

ApplicationFunctionSet Application_FunctionSet;

void setup()
{
  // put your setup code here, to run once:
  Application_FunctionSet.ApplicationFunctionSet_Init();
}

void loop()
{
  // put your main code here, to run repeatedly:
  Application_FunctionSet.Test();
}
