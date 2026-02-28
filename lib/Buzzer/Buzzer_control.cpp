/*
 * @Description: In User Settings Edit
 * @Author: HOU Changhua 
 * @Date: 2019-07-11 13:40:52
 * @LastEditTime: 2019-10-14 19:22:49
 * @LastEditors: Please set LastEditors
 */

#include "Buzzer_control.h"
/*passive Buzzer*/
void DeviceDriverSet_passiveBuzzer::DeviceDriverSet_passiveBuzzer_Init(void)
{
}

#if _Test_DeviceDriverSet
void DeviceDriverSet_passiveBuzzer::DeviceDriverSet_passiveBuzzer_Test(void)
{
  for (int thisNote = 0; thisNote < 8; thisNote++)
  {
    tone(5, melody[thisNote], duration); // pinxx output the voice, every scale is 0.5 sencond
    delay(1000);                         // Output the voice after several minutes
  }
}
#endif

void DeviceDriverSet_passiveBuzzer::DeviceDriverSet_passiveBuzzer_controlMonosyllabic(uint8_t controlMonosyllabic, uint16_t Duration)
{
  if (controlMonosyllabic > 7) //0----7
    return;
  tone(5, melody[controlMonosyllabic], Duration); //
}

void DeviceDriverSet_passiveBuzzer::DeviceDriverSet_passiveBuzzer_controlAudio(uint16_t controlAudio, uint16_t Duration)
{
  if (controlAudio > 2500)
  {
    controlAudio = 2500;
  }
  tone(5, controlAudio, Duration); //
}

void DeviceDriverSet_passiveBuzzer::DeviceDriverSet_passiveBuzzer_Scale_c8(uint16_t Duration)
{
  for (int thisNote = 0; thisNote < 8; thisNote++)
  {
    tone(5, melody[thisNote], duration); // pinxx output the voice, every scale is 0.5 sencond
    delay(Duration);
  }
}
