#include "MPU.h"
#include "MOTOR.h"
#include "Bluetooth.h"

uint8_t motorSpeed[4];
uint8_t motorSpeed_error[4];
uint8_t motorPins[4];
int8_t err_ypr_sign[3];
float ypr[3];
float err_ypr[3]; // ypr error (to (dir,0,0) )
float last_err_ypr[3]; // last error record
float diff_err_ypr[3]; // error difference
float intg_err_ypr[3]; // error integration
float Ez;
#define ERR_RECORD_TIME 5
float err_ypr_record[3][ERR_RECORD_TIME]; // last 5 times of ypr error 
float speed_ypr[3];
boolean takeOff[2] = {false,false};
bool Motor_flag = false;
float pid[3] = {CONF_Kp,CONF_Ki,CONF_Kd};
uint8_t throttle = THROTTLE;

float min_angle = 0;
float max_angle = 0;
uint8_t min_motor_error = MIN_MOTOR_ERROR;
uint8_t max_motor_error = MAX_MOTOR_ERROR;

// target place of ypr, for now , do not take YAW in consideration, just test PID first
// need to add yaw control when PITCH and ROLL tests are stable.
float target_ypr[3] = {0,0,0}; 
int loopCount = 5;
uint16_t pre_hello_time = 0;
uint16_t currentTime = 0;
uint16_t previousTime = 0;
uint16_t cycleTime = 0;
uint16_t cycleTimefirst = 0;
uint16_t cycleTimesecond = 0;

#define initData(){\
                uint8_t axis;\
		for (axis = 0; axis < 3; axis ++) {\
                    err_ypr[axis] = 0; \
                    last_err_ypr[axis] = 0; \
                    diff_err_ypr[axis] = 0; \
                    intg_err_ypr[axis] = 0; \
                    speed_ypr[axis] = 0;\
                    target_ypr[axis] = 0;\
                    motorSpeed_error[axis] = 0;\
		}\
                takeOff[0] =  takeOff[1] = false;\
                throttle = THROTTLE;\
                Motor_flag = false;\
}

void setup(){
  pinMode(LED_PIN,OUTPUT);
  digitalWrite(LED_PIN,LOW);
  Wire.begin();
  Serial.begin(115200);        // opens serial port, sets data rate to 9600 bps
  MPU_Setup();
  MPU_Stable();
  Motor_Setup();
  initData();
  Ez = 0;
  digitalWrite(LED_PIN,HIGH);
}

#define print_info(){\
    Serial.print("ypr: ");Serial.print(ypr[YAW]);\
    Serial.print("\t");Serial.print(ypr[PITCH]);\
    Serial.print("\t");Serial.print(ypr[ROLL]);\
    Serial.print("speed_ypr: ");Serial.print(speed_ypr[YAW]);\
    Serial.print("\t");Serial.print(speed_ypr[PITCH]);\
    Serial.print("\t");Serial.print(speed_ypr[ROLL]);\
    Serial.print("\tMotors: ");Serial.print(motorSpeed[FRONT]);\
    Serial.print("\t");Serial.print(motorSpeed[BACK]);\
    Serial.print("\t");Serial.print(motorSpeed[LEFT]);\
    Serial.print("\t");Serial.print(motorSpeed[RIGHT]);\
    Serial.println("");\
}

#define PID(){\
    /* CONF_Kp  CONF_Ki  CONF_Kd */\
    cycleTimefirst = millis();\
    cycleTime = cycleTimefirst-cycleTimesecond;\
    int i;\
    for (i = 0; i < 3; ++i){\
      /*speed_ypr[i] = (float)min_motor_error + ((float)max_motor_error - (float)min_motor_error) * ( pid[0] * err_ypr[i]*err_ypr[i]*err_ypr_sign[0] + pid[1] * intg_err_ypr[i] - pid[2] * diff_err_ypr[i]*diff_err_ypr[i]/(cycleTimefirst-cycleTimesecond+1)); */\
      if (abs(err_ypr[i]) >= 0.1) {\
        speed_ypr[i] = 30 * ( pid[0] * err_ypr[i] + pid[1] * intg_err_ypr[i]/100 + pid[2] * (diff_err_ypr[i])/(cycleTimefirst-cycleTimesecond+1));\
      } else {\
        speed_ypr[i] = 30 * ( pid[0] * err_ypr[i] + pid[1] * intg_err_ypr[i]/150 + pid[2] * (diff_err_ypr[i])/(cycleTimefirst-cycleTimesecond+1));\
      }\
      constrain(speed_ypr[i],-max_motor_error,max_motor_error);\
    }\
    /*[ HORISUN ] YAW not included yet */\
    motorSpeed[FRONT] = throttle + speed_ypr[1]+motorSpeed_error[FRONT]-speed_ypr[0]; \
    motorSpeed[BACK] = throttle - speed_ypr[1]+motorSpeed_error[BACK]-speed_ypr[0];   \ 
    motorSpeed[LEFT] = throttle + speed_ypr[2]+motorSpeed_error[LEFT]+speed_ypr[0]; \
    motorSpeed[RIGHT] = throttle - speed_ypr[2]+motorSpeed_error[RIGHT]+speed_ypr[0]; \ 
    cycleTimesecond = cycleTimefirst;  \
}

void Bluetooth_Loop(){
        static uint8_t axis;
	while(takeOff[1] == false || Serial.available() > 0) {
                Serial.println('2');
		//          {1.3.1} disable all the motors and some variables.
                if(Motor_flag && !takeOff[1] && !(Serial.available() > 0)) {
                    Motor_Stop();
                }
                
		digitalWrite(13, LOW);

		//          {1.3.3} show the state of data for debug
		if (loopCount) {
			loopCount --;
		} else {
			loopCount = 5;
			Bluetooth_Show();
		}
		
		//          {1.3.4} detect whether the LOCK state is changed
		while (Serial.available()) {
  			char c = Serial.peek();
                        switch(c) {
                          case 'x':c = Serial.read();
                                   takeOff[1] = true;
                                   target_ypr[0] = ypr[0];
				   break;
                          case 'S':c = Serial.read();
                                   pre_hello_time = millis();
			           break;
                          case 'q':
                                    if(Motor_flag) {
		                    motorSpeed[0] = 0;
		                    motorSpeed[1] = 0;
		                    motorSpeed[2] = 0;
		                    motorSpeed[3] = 0;
		                    Motor_Write();
                                   }
                                   c = Serial.read();
                                   Motor_flag = false;
                                   takeOff[1] = false;
                                   break;
                           case 'u':
                                   throttle = throttle + 1;
                                   intg_err_ypr[0] = 0;
                                   intg_err_ypr[1] = 0;
                                   intg_err_ypr[2] = 0;
                                   c = Serial.read();
                                   break;
                           case 'l':
                                   throttle = throttle - 1;
                                   c = Serial.read();
                                   break;
                           case 'f': target_ypr[2] = target_ypr[2] - 0.01;c = Serial.read();break;
                           case 'b': target_ypr[2] = target_ypr[2] + 0.01;c = Serial.read();break;
                           case 'z': target_ypr[1] = target_ypr[1] - 0.01;c = Serial.read();break;
                           case 'y': target_ypr[1] = target_ypr[1] + 0.01;c = Serial.read();break;
                           case 'r': target_ypr[1] = 0;
                                     target_ypr[2] = 0;
                                     c = Serial.read();
                                     break;
                           default :Bluetooth_Update();break;
                      }
		}
                MPU_Loop();
        	currentTime = millis();
  	        pre_hello_time = currentTime;
        }
}





void loop(){  
  if(!takeOff[1] && !takeOff[0]) {
    digitalWrite(LED_PIN,HIGH);
  }
  else if(!takeOff[0] && Motor_flag){
    digitalWrite(LED_PIN, LOW);
  }
  takeOff[0] = takeOff[1];
  	
        if (loopCount) {
		loopCount --;
	} else {
		loopCount = 5;
                Serial.println('1');
		Bluetooth_Show();
	}
	//     {1.2} upper monitor update data or detect symbol of periodic "saying hello"
	if (Serial.available() > 0){
		while(Serial.peek() == 'S'){
                        pre_hello_time = millis();
			Serial.read();
                }
	}
	//if having not receive any data for more than 1 sec, then lock.
	//the time variable must be unsigned. This can solve the overflow problem
	else if ((uint16_t)millis() - pre_hello_time > 2000)
		takeOff[1] = false;
  
  
  Bluetooth_Loop();
  MPU_Loop();
  if(abs(ypr[1]) > 0.8 || abs(ypr[2]) > 0.8) {
    takeOff[1] = false;
    Motor_Stop();
  } 
  PID();
  
  if(Motor_flag){
      Motor_Write();
  }
  //print_info();
}
