/*********
  Complete project details at http://randomnerdtutorials.com  
*********/

#include <SoftwareSerial.h>

// Configure software serial port
SoftwareSerial SIM900(7, 8); 

void setup() {
  // Arduino communicates with SIM900 GSM shield at a baud rate of 19200
  // Make sure that corresponds to the baud rate of your module
  SIM900.begin(19200);
  // Give time to your GSM shield log on to network
  delay(20000);   
  
  // Send the SMS
  sendSMS_test();

  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() { 

digitalWrite(LED_BUILTIN, HIGH);
delay(1000);
digitalWrite(LED_BUILTIN, LOW);
delay(1000);

digitalWrite(LED_BUILTIN, HIGH);
delay(1000);
digitalWrite(LED_BUILTIN, LOW);
delay(1000);

digitalWrite(LED_BUILTIN, HIGH);
delay(1000);
digitalWrite(LED_BUILTIN, LOW);
delay(1000);

}

// ========================
//   SEND SMS TEST
// ========================
void sendSMS_test() {
  // AT command to set SIM900 to SMS mode
  SIM900.print("AT+CMGF=1\r"); 
  delay(100);

  // REPLACE THE X's WITH THE RECIPIENT'S MOBILE NUMBER
  // USE INTERNATIONAL FORMAT CODE FOR MOBILE NUMBERS
  SIM900.println("AT + CMGS = \"+38641000111\""); 
  delay(100);
  
  // REPLACE WITH YOUR OWN SMS MESSAGE CONTENT
  SIM900.println("Test."); 
  delay(100);

  // End AT command with a ^Z, ASCII code 26
  SIM900.println((char)26); 
  delay(100);
  SIM900.println();
  // Give module time to send SMS
  delay(5000); 
}

// ========================
//   SEND SMS STATUS
// ========================
void sendSMS_status() {
  SIM900.print("AT+CMGF=1\r");
  delay(100);
  SIM900.println("AT + CMGS = \"+38641000111\"");
  delay(100);
  SIM900.println("Status.");
  delay(100);
  SIM900.println((char)26);
  delay(100);
  SIM900.println();
  delay(5000);
}

// ========================
//   SEND SMS STOPED
// ========================
// ========================
//   SEND SMS ARMED
// ========================
// ========================
//   SEND SMS TRIGERED
// ========================
