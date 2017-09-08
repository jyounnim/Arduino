#define LED 13

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin 13(LED_BUILTIN) as an output.
  pinMode(LED, OUTPUT);
  Serial.begin(115200);
  Serial.println("***** LED blink 01 *****");
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(LED, HIGH);   // turn the LED on (HIGH is the voltage level)
  Serial.println("LED ON");
  delay(1000);               // wait for a second
  
  digitalWrite(LED, LOW);    // turn the LED off by making the voltage LOW
  Serial.println("LED OFF");
  delay(1000);               // wait for a second
}
