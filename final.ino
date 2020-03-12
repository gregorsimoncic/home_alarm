#include <SoftwareSerial.h>
/*  ==============================
 *  
 *  NAVODILA
 *  
 *  prvi pin STIKALA priklopi na GND
 *  drugi pin STIKALA priklopi na DIGITAL 4
 *  SIM900 TX/RX = pin 7/8
 *  
 *  ===============================
 */ 

// common settings
SoftwareSerial GPRS(7, 8);
String textMessage; // ARM, DISABLE, TRIGGER, STATE, DELETE
enum ALARM_STATE_ENUM { DISABLED, ARMED, TRIGGERED }; // status 0, 1, 2
ALARM_STATE_ENUM alarm_state = DISABLED;

// settings for switch
const int sensor = 4; // sensor PIN number
bool changed = false;
int tmp = 0;
int switch_state = 0; // 0 close - 1 open switch

// settings for finite state machine
bool alarm_state_change = false;

void setup() {
  pinMode(sensor, INPUT_PULLUP);
  delay(2000);
  SIM900power();
  Serial.begin(19200);
  GPRS.begin(19200);
  
  delay(20000);
  Serial.print("Initializing GSM module...\r\n");
  GPRS.print("AT+CMGF=1\r\n"); 
  delay(1000);
  GPRS.print("AT+CNMI=2,2,0,0,0\r\n");
  delay(1000);
}

// ========================
//  POWER ON SIM900
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
//  MAIN LOOP
// ========================
void loop() {
  if(GPRS.available()>0) {
    textMessage = GPRS.readString();
    Serial.print(textMessage);
    delay(10);
  }
  change_alarm_state();
  read_switch();
  delay(10);
  send_status();
  delete_sms();
}

// ========================
//  CHANGE ALARM STATE
// ========================
void change_alarm_state() {
  switch(alarm_state) {
    case DISABLED:
      if (textMessage.indexOf("ARM")>=0) {
        alarm_state = ARMED;
        textMessage = "";
        Serial.println("Alarm is ARMED\r\n");
        delay(10);
        alarm_state_change = true;
        GPRS.println("AT+CMGS=\"+38640311222\"");
        delay(500);
        GPRS.print("Alarm is ARMED\r");
        GPRS.write( 0x1a );
        delay(1000);
        break;
      }
    alarm_state_change = false;
    break;
    case ARMED:
      if (textMessage.indexOf("DISABLE")>=0) {
        alarm_state = DISABLED;
        textMessage = "";
        Serial.println("Alarm is DISABLED\r\n");
        delay(10);
        alarm_state_change = true;
        GPRS.println("AT+CMGS=\"+38640311222\"");
        delay(500);
        GPRS.print("Alarm is DISABLED\r");
        GPRS.write( 0x1a );
        delay(1000);
        break;
      }
      if ((textMessage.indexOf("TRIGGER")>=0) or (switch_state == HIGH)) {
        alarm_state = TRIGGERED;
        textMessage = "";
        Serial.println("Alarm is TRIGGERED\r\n");
        delay(10);
        alarm_state_change = true;
        GPRS.println("ATD + +38640311222;");
        delay(100);
        GPRS.println();
        delay(30000);
        GPRS.println("ATH");
        delay(500);
        GPRS.println("AT+CMGS=\"+38640311222\"");
        delay(500);
        GPRS.print("Alarm is TRIGGERED\r");
        GPRS.write( 0x1a );
        delay(1000);
        break;
      }
    alarm_state_change = false;
    break;
    case TRIGGERED:
      if (textMessage.indexOf("DISABLE")>=0) {
        alarm_state = DISABLED;
        textMessage = "";
        Serial.println("Alarm is DISABLED\r\n");
        delay(10);
        alarm_state_change = true;
        GPRS.println("AT+CMGS=\"+38640311222\"");
        delay(500);
        GPRS.print("Alarm is DISABLED\r");
        GPRS.write( 0x1a );
        delay(1000);
        break;
      }
    alarm_state_change = false;
    break;
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
      Serial.println("narazen\r\n");
      changed = false;
      break;
    }
    else{
      break;
    }
    case LOW:
    if(changed){
      Serial.println("skupaj\r\n");
      changed = false;
      break;
    }
    else{
      break;
    }
  }
}

// ========================
//  SEND STATUS
// ========================
void send_status() {
  if(textMessage.indexOf("STATE")>=0) {
    switch(alarm_state) {
    case 0:
      Serial.println("STATUS is: DISABLED");
      delay(10);
      textMessage = "";
      GPRS.println("AT+CMGS=\"+38640311222\"");
      delay(500);
      GPRS.print("STATUS is: DISABLED\r");
      GPRS.write( 0x1a );
      delay(1000);
      break;
    case 1:
      Serial.println("STATUS is: ARMED");
      delay(10);
      textMessage = "";
      GPRS.println("AT+CMGS=\"+38640311222\"");
      delay(500);
      GPRS.print("STATUS is: ARMED\r");
      GPRS.write( 0x1a );
      delay(1000);
      break;
    case 2:
      Serial.println("STATUS is: TRIGGERED");
      delay(10);
      textMessage = "";
      GPRS.println("AT+CMGS=\"+38640311222\"");
      delay(500);
      GPRS.print("STATUS is: TRIGGERED\r");
      GPRS.write( 0x1a );
      delay(1000);
      break;
    }
  }
}

// ========================
//  DELETE SMS
// ========================
void delete_sms() {
  if(textMessage.indexOf("DELETE")>=0) {
    Serial.println("Deleting SMS\r\n");
    delay(10);
    textMessage = "";
    GPRS.println("AT+CMGD=1,4");
    delay(1000);
  }
}
