#include <SoftwareSerial.h>

// Configure software serial port
SoftwareSerial SIM900(7, 8); 

void setup() {
  delay(2000);
  SIM900power();
  SIM900.begin(19200);
  delay(20000);

  // Make the phone call
  callSomeone();
}

void loop() {
}

void callSomeone()
{
  SIM900.println("ATD + +38641000111;");
  delay(100);
  SIM900.println();
  delay(30000);
  SIM900.println("ATH"); // hang up
}

void SIM900power()
{
  pinMode(9, OUTPUT); 
  digitalWrite(9,LOW);
  delay(1000);
  digitalWrite(9,HIGH);
  delay(2000);
  digitalWrite(9,LOW);
  delay(3000);
}
