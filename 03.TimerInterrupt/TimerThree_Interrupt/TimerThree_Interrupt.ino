#include <TimerThree.h>

// This example uses the timer interrupt to blink an LED
// and also demonstrates how to share a variable between
// the interrupt and the main program.


const int led = 13;  // the pin with a LED

void setup(void)
{
  pinMode(led, OUTPUT);
  
  Timer3.initialize(500000);        // Initialize timer1, and set a 1/2 second period  
  Timer3.attachInterrupt(blinkLED); // blinkLED to run every 0.15 seconds
  Serial.begin(115200);
}


// The interrupt will blink the LED, and keep
// track of how many times it has blinked.
volatile unsigned long blinkCount = 0; // use volatile for shared variables

void blinkLED(void)
{
  if ( digitalRead(led) == LOW) {
    digitalWrite(led, HIGH);
    blinkCount = blinkCount + 1;  // increase when LED turns on
  } else {
    digitalWrite(led, LOW);
  }  
}


// The main program will print the blink count
// to the Arduino Serial Monitor
void loop(void)
{
  static unsigned long blinkCopy;  // holds a copy of the blinkCount

  // to read a variable which the interrupt code writes, we
  // must temporarily disable interrupts, to be sure it will
  // not change while we are reading.  To minimize the time
  // with interrupts off, just quickly make a copy, and then
  // use the copy while allowing the interrupt to keep working.
  if( blinkCopy != blinkCount)
  {
    noInterrupts();
    blinkCopy = blinkCount;
    interrupts();

    Serial.print("blinkCount = ");
    Serial.println(blinkCopy);
  }
  
  delay(100);
}
