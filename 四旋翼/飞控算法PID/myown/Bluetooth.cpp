#include <Arduino.h>
#include "Bluetooth.h"

char select = 's';
String comdata = "";
char symbol = 'a';
int length = 0;
int count = 5;
boolean sign = false;
boolean data_updated = false;
boolean wait_for_data = false;        //stop showing the data when reading
boolean select_set = false;           //stop reading select when reading data
boolean data_is_updating = false;     
boolean previous_updated = false;//to check the set of number
extern uint16_t cycleTimefirst;
extern uint16_t cycleTimesecond;
extern float target_ypr[3];
extern float intg_err_ypr[3];

void Bluetooth_Show() {
  Serial.print('s');
  for (int i = 0; i < 3; i++) {
    Serial.print(pid[i]);
    if (i != 2)
      Serial.print(' ');
  }
  Serial.print('%');
  Serial.print(cycleTime);
  Serial.print('%');
  Serial.print(motorSpeed_error[FRONT]); 
  Serial.print(' ');
  Serial.print(motorSpeed_error[BACK]);
  Serial.print('%');
  Serial.print(motorSpeed_error[LEFT]); 
  Serial.print(' ');
  Serial.print(motorSpeed_error[RIGHT]);
  Serial.print('%');
  Serial.print(throttle); 
  Serial.print('%');
  for (int i = 0; i < 3; i++) {
    Serial.print(intg_err_ypr[i]);
    if (i != 2)
      Serial.print(' ');
  } 
  Serial.print('%');
  for (int i = 0; i < 4; i++) {
    Serial.print(motorSpeed[i]);
    if (i != 3)
      Serial.print(' ');
  }
  Serial.print("%");
  for (int i = 0; i < 3; i++) {
    Serial.print(ypr[2-i]);
    Serial.print(' ');
  }
    Serial.print(throttle);
  Serial.print('%');
  for (int i = 0; i < 3; i++) {
    Serial.print(target_ypr[2-i]);
    Serial.print(' ');
  }
  Serial.print(throttle);
  Serial.print('%');
  Serial.print(0);
  Serial.print('%');
  Serial.print(0);
  Serial.print('%');
  Serial.print(0);
  Serial.print(' ');
  Serial.print(0);
  Serial.print(' ');
  Serial.print(0);
  Serial.println();
}

void Bluetooth_Update() {
  //selection of the value being changed
  if (Serial.available() > 0 && !select_set) {
    select = char(Serial.read());
    wait_for_data = true;
    select_set = true;
  }
  //read the value to be changed
  if(select != 's') {
    while (Serial.available() > 0 && select_set == true) {
      if (Serial.peek() != 'S' || Serial.peek() !='x' || Serial.peek() !='s' || Serial.peek() !='q') {
        comdata += Serial.read();
        data_updated = true;
      }
      else break;
    }
  if (data_updated) {
    length = comdata.length();
    int num = 0, update_data = 0, j = 0, *dataset = new int[length / 2];
    int angle[2] = {0, 0}, error[2] = {0, 0};
    for (int i = 1; i <= length; i += 2) {
      dataset[j] = (comdata[i - 1] - 48) * 10 * (i % 2) + comdata[i] - 48; 
      j++;  
    }
    j = 0;
    //determine which value to be changed
    Bluetooth_Show();
    switch (select) {
      case 'p':
        for (int i = 0; i < length / 2 - 1; i++) {
          update_data = update_data * 10 + dataset[i] - 48;
        }
        pid[0] = update_data/100.0;
        update_data = 0;
        break;
      case 'i':
        for (int i = 0; i < length / 2 - 1; i++) {
          update_data = update_data * 10 + dataset[i] - 48;
        }
        pid[1] = update_data/100.0;
        update_data = 0;
        break;
      case 'd':
        for (int i = 0; i < length / 2 - 1; i++) {
          update_data = update_data * 10 + dataset[i] - 48;
        }
        pid[2] = update_data/100.0;
        update_data = 0;
        break;
      case 't':
        for (int i = 0; i < length / 2 - 1; i++) {
          update_data = update_data * 10 + dataset[i] - 48;
        }
        throttle = update_data;
        update_data = 0;
        break;
      case 'a':
        for (int i = 0; i < length / 2 - 1; i++) {
          update_data = update_data * 10 + dataset[i] - 48;
        }
        motorSpeed_error[FRONT] = update_data;
        update_data = 0;
        break;
      case 'e':    
        for (int i = 0; i < length / 2 - 1; i++) {
          update_data = update_data * 10 + dataset[i] - 48;
        }
        motorSpeed_error[BACK] = update_data;
        update_data = 0;
        break;
      case 'n':
        for (int i = 0; i < length / 2 - 1; i++) {
          update_data = update_data * 10 + dataset[i] - 48;
        }
        motorSpeed_error[LEFT] = update_data;
        update_data = 0;
        break;
      case 'r':
        for (int i = 0; i < length / 2 - 1; i++) {
          update_data = update_data * 10 + dataset[i] - 48;
        }
        motorSpeed_error[RIGHT] = update_data;
        update_data = 0;
        break;
    }
    wait_for_data = false;
    comdata = "";
    select_set = false;
    data_updated = false;
    symbol = 'a';
    delete dataset;
    sign = false;
    }
  } else {
    switch (select) {
      case 's':
      Motor_Start();
      Serial.println('1');
      break;
    }
    wait_for_data = false;
    select_set = false;
  }
}
