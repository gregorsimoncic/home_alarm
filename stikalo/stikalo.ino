/*  ==============================
 *  
 *  NAVODILA
 *  
 *  prvi pin STIKALA priklopi na GND
 *  drugi pin STIKALA priklopi na DIGITAL 4
 *  
 *  ===============================
 */ 

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


void setup(){
  Serial.begin(9600);
  pinMode(sensor, INPUT_PULLUP);
}

void loop(){
  
  // read from switch
  read_switch();
  delay(10);
  
  // read from serial input
  recv_with_end_marker();
  show_me_data();
  
  
}

// ========================
//   READ FROM SERIAL
// ========================
void recv_with_end_marker() {
  static byte ndx = 0;
  char end_marker = '\n';
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

void show_me_data() {
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
//   SEND SMS TEST
// ========================
// ========================
//   SEND SMS STATUS
// ========================
// ========================
//   SEND SMS STOPED
// ========================
// ========================
//   SEND SMS ARMED
// ========================
// ========================
//   SEND SMS TRIGERED
// ========================
