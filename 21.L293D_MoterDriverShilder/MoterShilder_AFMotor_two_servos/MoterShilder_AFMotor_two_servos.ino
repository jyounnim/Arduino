#include <Servo.h>

// define PWM pin for servo
int servoPin1 = 9; 
int servoPin2 = 10;
int val1 = 0;                     // Data received from the serial port
int val2 = 180;                     // Data received from the serial port
int dir1 = 0;
int dir2 = 1;

// Create servo object to control a servo
Servo myServo1;
Servo myServo2;

void setup()
{  
  Serial.begin(115200);           // set up Serial library at 115200 bps
  Serial.println("Two Servos test!");

  // Attach the servo to the PWM pin
  myServo1.attach(servoPin1);
  myServo2.attach(servoPin2);      

  for(val1=0; val1<=180; val1++)
  {
    myServo1.write(val1);
    myServo2.write(180-val1);
    delay(2);
  }
  
  delay(20);
  for(val1=180; val1>=0; val1--)
  {
    myServo1.write(val1);
    myServo2.write(180-val1);
    delay(2);
  }
}

void loop()
{
    if( val1 <= 0 )
      dir1 = 0;  
    else if( val1 >= 180 )
      dir1 = 1;

    if( dir1 == 0 )
        val1++;  
    else
        val1--;

    if( val2 <= 0 )
      dir2 = 0;  
    else if( val2 >= 180 )
      dir2 = 1;

    if( dir2 == 0 )
        val2++;  
    else
        val2--;
      
    myServo1.write(val1);
    myServo2.write(val2);

    Serial.print(val1);
    Serial.print("   ");
    Serial.println(val2);
    
    delay(20);
}
