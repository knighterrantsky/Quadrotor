#ifndef _MOTOR_H_
#define _MOTOR_H_


//#include <Arduino.h>
//#include <Servo.h>
#include "def.h"
//extern uint16_t cycleTimefirst;
//extern uint16_t cycleTimesecond;
extern bool Motor_flag;
//extern uint8_t motorSpeed[4];
//extern uint8_t motorPins[4];

#define Motor_Write() {\
	analogWrite(motorPins[0], motorSpeed[0]);\
	analogWrite(motorPins[1], motorSpeed[1]);\
	analogWrite(motorPins[2], motorSpeed[2]);\
	analogWrite(motorPins[3], motorSpeed[3]);\
}//end of loop

#define Motor_Write_All(spd){\
  	analogWrite(motorPins[0], spd);\
	analogWrite(motorPins[1], spd);\
	analogWrite(motorPins[2], spd);\
	analogWrite(motorPins[3], spd);\
}

#define Motor_Write_One(pin,spd){\
        analogWrite(motorPins[pin], spd);\
}

#define Motor_Setup() {\
  Serial.println("[ Setting up Motor... ]");\
  motorPins[0] = MOTOR_PIN_0;\
  motorPins[1] = MOTOR_PIN_1;\
  motorPins[2] = MOTOR_PIN_2;\
  motorPins[3] = MOTOR_PIN_3;\  
  motorSpeed[0] = 0;\
  motorSpeed[1] = 0;\
  motorSpeed[2] = 0;\
  motorSpeed[3] = 0;\
  pinMode(motorPins[0], OUTPUT);\
  pinMode(motorPins[1], OUTPUT);\
  pinMode(motorPins[2], OUTPUT);\
  pinMode(motorPins[3], OUTPUT);\ 
  Serial.println("[ Motor is ready ]");\  
}

#define Motor_Start() {\
  Serial.println("[ Preparing... ]");\
  Motor_Write_All(0);\
  delay(1000);\
  Motor_Write_All(125);\
  delay(1000);\
  takeOff[1] = true;\
  Motor_flag = true;\
  Serial.println("[ Ready to take up ]");\
  cycleTimefirst = cycleTimesecond = millis();\
}

#define Motor_Stop() {\
  Motor_Write_All(0);\
  Motor_flag = false;  \
}

#endif /* _MOTOR_H_ */
