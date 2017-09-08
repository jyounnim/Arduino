#define LED1 7
#define LED2 8
#define LED3 9

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin 7,8,9 as an output.
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  
  Serial.begin(115200);
  Serial.println("***** LED blink 02 *****");
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(LED1, HIGH);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(LED2, HIGH);
  digitalWrite(LED3, HIGH);
  Serial.println("LED ON");
  delay(1000);               // wait for a second
  
  digitalWrite(LED1, LOW);    // turn the LED off by making the voltage LOW
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, LOW);
  Serial.println("LED OFF");
  delay(1000);               // wait for a second
}
