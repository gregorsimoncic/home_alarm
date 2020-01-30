const int sensor = 4;

bool changed = false;
int tmp = 0;
int state = 0; // 0 close - 1 open switch

void setup(){
  Serial.begin(9600);
  pinMode(sensor, INPUT_PULLUP);
}

void loop(){
  state = digitalRead(sensor);
  
  if(tmp == state){
    changed = false;
  }
  if(tmp != state){
    changed = true;
    tmp = state;
  }
  
  switch(state){
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
  delay(10);
}
