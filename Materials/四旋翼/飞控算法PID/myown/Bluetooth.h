#ifndef _BLUETOOTH_H_
#define _BLUETOOTH_H_


#include <Arduino.h>
#include "MOTOR.h"
#include "def.h"

extern uint8_t motorSpeed_error[4];
extern float pid[3];
extern uint8_t throttle;
extern float min_angle;
extern float max_angle;
extern uint8_t min_motor_error;
extern uint8_t max_motor_error;
extern uint16_t cycleTime;
extern float diff_err_ypr[3];
//extern int32_t error[3];
//extern int8_t motor_error[3];
extern uint8_t motorSpeed[4];
//extern Attitude_t currentAtt, commandAtt;
//extern uint8_t angle_bit_multi;
//extern uint8_t error_bit_multi;
//extern uint16_t A, B, C;
extern boolean takeOff[2];
extern bool Motor_flag;
extern float ypr[3];
extern float err_ypr[3];

extern uint8_t motorSpeed[4];
extern uint8_t motorPins[4];

void Bluetooth_Show();
void Bluetooth_Update();


#endif /* _BLUETOOTH_H_ */
