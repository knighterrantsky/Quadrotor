#ifndef _DEF_H_
#define _DEF_H_


//The configuration of Kp, Ki, Kd
//Constraint: [0,255]
#define CONF_Kp 0.2//30
#define CONF_Ki 0
#define CONF_Kd 0.7//70


//Define the throttle which makes the quardcopter fly exactly right against the gravity
//Constraint: [0,180]********************************************************************************************************************
#define FIT_THROTTLE 0


//Define the bit operation multiple number. Now it is for int16.
//Constraint: unavailable for int8 / [0,7] for int16 / [0,23] for int32
#define ANGLE_BIT_MULTI 4


//Define the angle constraints of roll and pitch.
//The final results should be [0,180]. For the safety now the value is +-45.
//Constraint: [-90,+90]
#define MIN_ANGLE -60
#define MAX_ANGLE 60


//Define the bit operation multiple number. Now it is for int32.
//The ERROR_BIT_MULTI is depended on the ANGLE_BIT_MULTI and Kp. 11=7+4 (Kp is about pow(2,4))
//Constraint: unavailable for int8 / [0,7] for int16 / [0,23] for int32
#define ERROR_BIT_MULTI 11


//Define the error constraints of the motors error
//The final results should be [0,180]. For the safety now the value is +-25.
//Constraint: [-90,+90]**********************************************************************************************************************
#define MIN_MOTOR_ERROR 0
#define MAX_MOTOR_ERROR 30


//For the convenience of using the PID array
//[ HORISUN ] exchanged YAW and ROLL , to adapt with MPU module  
#define ROLL 2
#define PITCH 1
#define YAW 0


//Define the pin number of motots
#define MOTOR_PIN_0 3
#define MOTOR_PIN_1 11
#define MOTOR_PIN_2 9
#define MOTOR_PIN_3 10

#define FRONT 0
#define RIGHT 1
#define BACK 2
#define LEFT 3

#define LED_PIN 13

#define THROTTLE MAX_MOTOR_ERROR

#endif /* _DEF_H_ */
