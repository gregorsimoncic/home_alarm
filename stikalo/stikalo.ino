/*  ==============================
 *  
 *  NAVODILA
 *  
 *  prvi pin STIKALA priklopi na GND
 *  drugi pin STIKALA priklopi na DIGITAL 4
 *  
 *  ===============================
 */ 
#include <SoftwareSerial.h>

// create software serial object to communicate with SIM900
SoftwareSerial mySerial(7, 8); //SIM900 Tx & Rx is connected to Arduino #7 & #8

// common settings
enum ALARM_STATE_ENUM { DISABLED, ARMED, TRIGGERED }; // status 0, 1, 2
ALARM_STATE_ENUM alarm_state = DISABLED;

// settings for switch
const int sensor = 4; // sensor PIN number
bool changed = false;
int tmp = 0;
int switch_state = 0; // 0 close - 1 open switch

// settings for reading input
const byte num_chars = 32;
char received_chars_ard[num_chars]; // an array to store received data
bool new_data = false;

// settings for finite state machine
bool alarm_state_change = false;

// common setup
void setup(){
  Serial.begin(9600);
  pinMode(sensor, INPUT_PULLUP);
  SIM900power();
  
  // begin serial communication with Arduino and SIM900
  mySerial.begin(9600);
  
  Serial.println("Initializing...");
  delay(1000);
  
  mySerial.println("AT"); //Handshaking with SIM900
  updateSerial();
  mySerial.println("AT+CSQ"); //Signal quality test, value range is 0-31 , 31 is the best
  updateSerial();
  mySerial.println("AT+CCID"); //Read SIM information to confirm whether the SIM is plugged
  updateSerial();
  mySerial.println("AT+CREG?"); //Check whether it has registered in the network
  updateSerial();
}

// ========================
//   MAIN LOOP
// ========================
void loop(){
  
  // update serial
  updateSerial();
  
  // check alarm state and change it if need to
  change_alarm_state();
  
  // read from switch
  read_switch();
  delay(10);
  
  // read from serial input
  read_serial();
  delay(10);
  print_to_serial_from_serial_input();

  // print status
  print_status();
  
}

// ========================
//   UPDATE SERIAL
// ========================
void updateSerial() {
  delay(500);
  while (Serial.available()) {
    mySerial.write(Serial.read()); // forward what Serial received to Software Serial Port
  } 
  while(mySerial.available()) {
    Serial.write(mySerial.read()); // forward what Software Serial received to Serial Port
  }
}

// ========================
//   POWER ON SIM900
// ========================
void SIM900power() {
  pinMode(9, OUTPUT);
  digitalWrite(9,LOW);
  delay(1000);
  digitalWrite(9,HIGH);
  delay(2000);
  digitalWrite(9,LOW);
  delay(3000);
}

// ========================
//   CHANGE ALARM STATE
// ========================
void change_alarm_state() {
  switch(alarm_state) {
    case DISABLED:
      if (strcmp(received_chars_ard, "ARM") == 0) {
        alarm_state = ARMED;
        Serial.println("Alarm is ARMED!");
        alarm_state_change = true;
        break;
      }
    alarm_state_change = false;
    break;
    case ARMED:
      if (strcmp(received_chars_ard, "DISABLE") == 0) {
        alarm_state = DISABLED;
        Serial.println("Alarm is DISABLED!");
        alarm_state_change = true;
        break;
      }
      if ((strcmp(received_chars_ard, "TRIGGERED") == 0) or (switch_state == HIGH)) {
        alarm_state = TRIGGERED;
        Serial.println("Alarm is TRIGGERED!");
        alarm_state_change = true;
        break;
      }
    alarm_state_change = false;
    break;
    case TRIGGERED:
      if (strcmp(received_chars_ard, "DISABLE") == 0) {
        alarm_state = DISABLED;
        Serial.println("Alarm is DISABLED!");
        alarm_state_change = true;
        break;
      }
    alarm_state_change = false;
    break;
  }
}

// ========================
//   READ FROM SERIAL
// ========================
void read_serial() {
  static byte ndx = 0;
  char end_marker = '\r';
  char rc;
  
  // if (Serial.available() > 0) {
  while (Serial.available() > 0 && new_data == false) {
    rc = Serial.read();
    
    if (rc != end_marker) {
      received_chars_ard[ndx] = rc;
      ndx++;
      if (ndx >= num_chars) {
        ndx = num_chars - 1;
      }
    }
    else {
      received_chars_ard[ndx] = '\0'; // terminate the string
      ndx = 0;
      new_data = true;
    }
  }
}

// ========================
//   PRINT TO SERIAL FROM SERIAL READ
// ========================
void print_to_serial_from_serial_input() {
  if (new_data == true) {
    Serial.println(received_chars_ard);
    new_data = false;
  }
}

// ========================
//   READ FROM SWITCH
// ========================
void read_switch() {
  switch_state = digitalRead(sensor);
  
  if(tmp == switch_state){
    changed = false;
  }
  if(tmp != switch_state){
    changed = true;
    tmp = switch_state;
  }

  // read from SWITCH
  switch(switch_state){
    case HIGH:
    if(changed){
      Serial.println("narazen");
      changed = false;
      break;
    }
    else{
      break;
    }
    case LOW:
    if(changed){
      Serial.println("skupaj");
      changed = false;
      break;
    }
    else{
      break;
    }
  }
}

// ========================
//  PRINT STATUS
// ========================
void print_status() {
  if((strcmp(received_chars_ard, "STATUS") == 0)) {
    switch(alarm_state) {
    case 0:
      Serial.println("STATUS is: DISABLED");
      memset(received_chars_ard,0,num_chars);
      break;
    case 1:
      Serial.println("STATUS is: ARMED");
      memset(received_chars_ard,0,num_chars);
      break;
    case 2:
      Serial.println("STATUS is: TRIGGERED");
      memset(received_chars_ard,0,num_chars);
      break;
    }
  }
}

// ========================
//   SEND SMS STATUS
// ========================
void send_sms_status(){
  
}

// ========================
//   SEND SMS DISABLED
// ========================
void send_sms_disable(){
  
}

// ========================
//   SEND SMS ARMED
// ========================
void send_sms_armed(){
  
}

// ========================
//   SEND SMS TRIGGERED
// ========================
void send_sms_triggered(){
  
}

// ========================
//   RECEIVE SMS STATUS
// ========================
void receive_sms_status(){
  
}

// ========================
//   RECEIVE SMS DISABLE
// ========================
void receive_sms_disable(){
  
}

// ========================
//   RECEIVE SMS ARM
// ========================
void receive_sms_arm(){
  
}
