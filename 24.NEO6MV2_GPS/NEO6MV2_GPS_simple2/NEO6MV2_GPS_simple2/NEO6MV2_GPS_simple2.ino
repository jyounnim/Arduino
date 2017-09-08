#include <SoftwareSerial.h>
 
SoftwareSerial swSerial(4,3); // RX, TX
 
void setup() {
  Serial.begin(38400);
  swSerial.begin(38400);
  swSerial.println("Start GPS... ");
  
}
 
void loop() {
  if(Serial.available())
  {
    swSerial.write(Serial.read());
  }
}


