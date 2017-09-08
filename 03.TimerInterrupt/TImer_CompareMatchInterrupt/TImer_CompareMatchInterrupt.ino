#define ledPin 13

void setup()
{
  pinMode(ledPin, OUTPUT);
  Serial.begin(115200);
  
  // initialize timer1 
  noInterrupts();           // disable all interrupts
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;

  OCR1A = 31250;            // compare match register 16MHz/256/2Hz
  TCCR1B |= (1 << WGM12);   // CTC mode
  TCCR1B |= (1 << CS12);    // 256 prescaler 
  TIMSK1 |= (1 << OCIE1A);  // enable timer compare interrupt
  interrupts();             // enable all interrupts
}

// The interrupt will blink the LED, and keep
// track of how many times it has blinked.
volatile unsigned long blinkCount = 0; // use volatile for shared variables

ISR(TIMER1_COMPA_vect)          // timer compare interrupt service routine
{
  if ( digitalRead(ledPin) == LOW) {
    digitalWrite(ledPin, HIGH);
    blinkCount = blinkCount + 1;  // increase when LED turns on
  } else {
    digitalWrite(ledPin, LOW);
  }  
}

// The main program will print the blink count
// to the Arduino Serial Monitor
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

