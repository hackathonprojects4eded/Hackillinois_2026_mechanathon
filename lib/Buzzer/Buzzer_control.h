/*
 * @Description: 设备驱动封装
 * @Author:  HOU Changhua
 * @Date: 2019-07-11 13:40:52
 * @LastEditTime: 2019-10-15 09:24:49
 * @LastEditors:
 */

#ifndef BUZZER_CONTROL_H
#define BUZZER_CONTROL_H
#include <Arduino.h>

/*passive Buzzer*/
#include "pitches.h"
class DeviceDriverSet_passiveBuzzer
{
public:
  void DeviceDriverSet_passiveBuzzer_Init(void);

  void DeviceDriverSet_passiveBuzzer_control(void);

public:
  void DeviceDriverSet_passiveBuzzer_controlMonosyllabic(uint8_t controlMonosyllabic, uint16_t Duration);
  void DeviceDriverSet_passiveBuzzer_controlAudio(uint16_t controlAudio, uint16_t Duration);
  void DeviceDriverSet_passiveBuzzer_Scale_c8(uint16_t Duration);

private:
#define PIN_passiveBuzzer 5
  int duration = 500;                                                                       // 500 miliseconds
  int melody[8] = {NOTE_C5, NOTE_D5, NOTE_E5, NOTE_F5, NOTE_G5, NOTE_A5, NOTE_B5, NOTE_C6}; // notes in the melody
};
#endif
