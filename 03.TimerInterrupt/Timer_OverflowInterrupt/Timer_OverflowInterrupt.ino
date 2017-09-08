/* 
 * Arduino 101: timer and interrupts
 * 2: Timer1 overflow interrupt example 
 * more infos: http://www.letmakerobots.com/node/28278
 * created by RobotFreak 
 */

#define ledPin 13

void setup()
{
  pinMode(ledPin, OUTPUT);
  Serial.begin(115200);

  // initialize timer1 
  noInterrupts();           // disable all interrupts
  TCCR1A = 0;
  TCCR1B = 0;

  TCNT1 = 34286;            // preload timer 65536-16MHz/256/2Hz
  TCCR1B |= (1 << CS12);    // 256 prescaler 
  TIMSK1 |= (1 << TOIE1);   // enable timer overflow interrupt
  interrupts();             // enable all interrupts
}

// The interrupt will blink the LED, and keep
// track of how many times it has blinked.
volatile unsigned long blinkCount = 0; // use volatile for shared variables

ISR(TIMER1_OVF_vect)        // interrupt service routine that wraps a user defined function supplied by attachInterrupt
{
  TCNT1 = 34286;            // preload timer
  
  if ( digitalRead(ledPin) == LOW) {
    digitalWrite(ledPin, HIGH);
    blinkCount = blinkCount + 1;  // increase when LED turns on
  } else {
    digitalWrite(ledPin, LOW);
  }  
}

void loop()
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

