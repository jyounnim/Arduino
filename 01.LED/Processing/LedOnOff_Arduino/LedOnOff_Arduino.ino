/*
  Blink
  Turns on/off LEDs 
*/

#define RED_LED     12
#define BLUE_LED    11
#define YELLOW_LED  10

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize serial port
  Serial.begin(9600); 
  
  // initialize LED pins as an output.
  pinMode(RED_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  
  digitalWrite(RED_LED, LOW);   
  digitalWrite(BLUE_LED, LOW);
  digitalWrite(YELLOW_LED, LOW);
}

// the loop function runs over and over again forever
void loop() {
  char val;
  
  // Check serial input
  while( Serial.available() )
  {
    val = Serial.read();
  }
  
  if( val == 'H' )
  {
    // turn the LED on (HIGH is the voltage level)
    digitalWrite(RED_LED, HIGH);   
    digitalWrite(BLUE_LED, HIGH);
    digitalWrite(YELLOW_LED, HIGH);  
  } 
  else if ( val == 'L') 
  {
    // turn the LED on (HIGH is the voltage level)
    digitalWrite(RED_LED, LOW);   
    digitalWrite(BLUE_LED, LOW);
    digitalWrite(YELLOW_LED, LOW);
  }
  delay(100);
}
