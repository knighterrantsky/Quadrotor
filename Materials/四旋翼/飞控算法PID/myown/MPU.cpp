#include "MPU.h"
#include "def.h"
MPU6050 mpu(0x68);
 
// MPU control/status vars
bool dmpReady = false;  // set true if DMP init was successful
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer
 
// orientation/motion vars
Quaternion q;           // [w, x, y, z]         quaternion container
VectorFloat gravity;    // [x, y, z]            gravity vector
extern float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector
extern float err_ypr[3]; // last record
extern float last_err_ypr[3]; // last record
extern float diff_err_ypr[3]; // difference
extern float intg_err_ypr[3]; // integration                                                                                                                                                                          
extern float target_ypr[3];
extern float Ez;

// ================================================================
// ===               INTERRUPT DETECTION ROUTINE                ===
// ================================================================
 
volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high
void dmpDataReady() {
  mpuInterrupt = true;
}
 
 
 
// ================================================================
// ===                      INITIAL SETUP                       ===
// ================================================================
 
void MPU_Setup() {
  // join I2C bus (I2Cdev library doesn't do this automatically)
  Serial.println("[ Setting up MPU... ]");
  int i;
  for(i=0;i<3;++i){
    ypr[i] = 0;
    err_ypr[i] = 0;    
    last_err_ypr[i] = 0;
    diff_err_ypr[i] = 0;
    intg_err_ypr[i] = 0;
  }
  // initialize device
  Serial.println("Initializing I2C devices...");
  mpu.initialize();
 
  // verify connection
  Serial.println("Testing device connections...");
  Serial.println(mpu.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");

    // wait for ready
  delay(2);
 
  // load and configure the DMP
  Serial.println("Initializing DMP...");
  devStatus = mpu.dmpInitialize();

    // supply your own gyro offsets here, scaled for min sensitivity
    mpu.setXGyroOffset(220);
    mpu.setYGyroOffset(76);
    mpu.setZGyroOffset(-85);
    mpu.setZAccelOffset(1788); // 1688 factory default for my test chip

 
  // make sure it worked (returns 0 if so)
  if (devStatus == 0) {
    // turn on the DMP, now that it's ready
    Serial.println("Enabling DMP...");
    mpu.setDMPEnabled(true);
 
    // enable Arduino interrupt detection
    Serial.println("Enabling interrupt detection (Arduino external interrupt 0)...");
    attachInterrupt(0, dmpDataReady, RISING);
    mpuIntStatus = mpu.getIntStatus();
 
    // set our DMP Ready flag so the main loop() function knows it's okay to use it
    Serial.println("DMP ready! Waiting for first interrupt...");
    dmpReady = true;
 
    // get expected DMP packet size for later comparison
    packetSize = mpu.dmpGetFIFOPacketSize();
    Serial.println("[ MPU is ready ]");
  } 
  else {
    // ERROR!
    // 1 = initial memory load failed
    // 2 = DMP configuration updates failed
    // (if it's going to break, usually the code will be 1)
    Serial.print("DMP Initialization failed (code ");
    Serial.print(devStatus);
    Serial.println(")");
  }
}
 
void MPU_Loop()
{
  // if programming failed, don't try to do anything
  if (!dmpReady) 
    return;
 
  // wait for MPU interrupt or extra packet(s) available
  if (!mpuInterrupt && fifoCount < packetSize) 
    return;
 
  // reset interrupt flag and get INT_STATUS byte
  mpuInterrupt = false;
  mpuIntStatus = mpu.getIntStatus();
 
  // get current FIFO count
  fifoCount = mpu.getFIFOCount();
 
  // check for overflow (this should never happen unless our code is too inefficient)
  if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
    // reset so we can continue cleanly
    mpu.resetFIFO();
    Serial.println("FIFO overflow!");
 
    // otherwise, check for DMP data ready interrupt (this should happen frequently)
  } 
  else if (mpuIntStatus & 0x02) {
    // wait for correct available data length, should be a VERY short wait
    while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();
 
    // read a packet from FIFO
    mpu.getFIFOBytes(fifoBuffer, packetSize);
 
    // track FIFO count here in case there is > 1 packet available
    // (this lets us immediately read more without waiting for an interrupt)
    fifoCount -= packetSize;
 
    mpu.dmpGetQuaternion(&q, fifoBuffer);
    mpu.dmpGetGravity(&gravity, &q);
    mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);  //从DMP中取出Yaw、Pitch、Roll三个轴的角度，放入数组ypr。单位：弧度
//    alpha=-ypr[2] * 180/M_PI;
    
//    omiga=mpu.getRotationX()/16.4;        //配置是16位表示正负2000°/s, 65536/4000


    //[ HORISUN ] for testing PITCH and ROLL, don't think about yaw
    //target_ypr[YAW] = ypr[YAW];
    int i;
    ypr[2] = ypr[2] - 0.01;
    ypr[1] = ypr[1] - 0.05;
    for(i=0;i<3;++i){      
      diff_err_ypr[i] = target_ypr[i] - ypr[i] - err_ypr[i];      
      err_ypr[i] += diff_err_ypr[i];
      if(abs(err_ypr[i]) <= 0.2) {
        intg_err_ypr[i] += err_ypr[i];
      } else {
        intg_err_ypr[i] += 0;
      }
    }
    //Serial.print("Alpha: ");
    //Serial.print(alpha);
    //Serial.print("\t Omiga: ");
    //Serial.print(omiga);    
  }
}

#define ABS(a) ((a)>0?(a):(0-(a)))

void MPU_Stable(){
  float last_ypr[3] = {0,0,0};
  float diff_ypr[3] = {255,255,255};
  int i;
  while(ABS(diff_ypr[0]) > 0.0005 || ABS(diff_ypr[1]) > 0.0005 || ABS(diff_ypr[2]) > 0.0005){
    Serial.print("diff_ypr: ");Serial.print(diff_ypr[YAW]);\
    Serial.print("\t");Serial.print(diff_ypr[PITCH]);\
    Serial.print("\t");Serial.print(diff_ypr[ROLL]);\
    Serial.println("");

    for(i=0;i<3;++i){
      last_ypr[i] = ypr[i];
    }
    MPU_Loop();
    for(i=0;i<3;++i){
      diff_ypr[i] = ypr[i]-last_ypr[i];
    }    
    delay(10);
  }
}  
