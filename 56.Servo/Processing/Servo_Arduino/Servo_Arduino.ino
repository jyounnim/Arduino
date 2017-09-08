// Read data from the serial port and set the position of a servomotor 
// according to the value
#include <Servo.h>

Servo myServo;                   // Create servo object to control a servo
int servoPin = 4;                // Connect yellow servo wire to digital I/O pin 4 
int val = 0;                     // Data received from the serial port

void setup() {
  Serial.begin(9600);    	 // Start serial communication at 9600 bps
  myServo.attach(servoPin);      // Attach the servo to the PWM pin
  
  for(val=0; val<=360; val++)
  {
    myServo.write(val);
    delay(2);
  }
  delay(20);
  for(val=360; val>=0; val--)
  {
    myServo.write(val);
    delay(2);
  }

}

void loop() {  
  if( Serial.available() ) 
  {      // If data is available to read,
    val = Serial.read();         // read it and store it in val 
    myServo.write(val);           // Set the servo position
  } 
  
  delay(20);                     // Wait for the servo to get there
}

