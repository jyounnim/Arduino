#define LED1 7
#define LED2 8
#define LED3 9

#define SW1 A0
#define SW2 A1

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin 7,8,9 as an output.
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  digitalWrite(LED3, HIGH);
  
  // initialize analog pin 0,1 as an input.
  pinMode(SW1, INPUT);
  pinMode(SW2, INPUT);
  
  Serial.begin(115200);
  Serial.println("***** LED OnOff  *****");

}

// the loop function runs over and over again forever
void loop() {
  if( digitalRead(SW1) == LOW )
  {
    digitalWrite(LED1, HIGH);   // turn the LED1 on 
  }else{
    digitalWrite(LED1, LOW);   // turn the LED1 OFF 
  }

  if( digitalRead(SW2) == LOW )
  {
    digitalWrite(LED2, HIGH);   // turn the LED2 on 
  }else{
    digitalWrite(LED2, LOW);   // turn the LED2 OFF 
  }
}
