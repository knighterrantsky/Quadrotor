// Arduino Wire library is required if I2Cdev I2CDEV_ARDUINO_WIRE implementation
// is used in I2Cdev.h
#include <Wire.h>
 
// I2Cdev and MPU6050 must be installed as libraries, or else the .cpp/.h files
// for both classes must be in the include path of your project
#include "I2Cdev.h"
 
#include "MPU6050_6Axis_MotionApps20.h"
void MPU_Setup();
void MPU_Loop();
void MPU_Stable();
