int switchPin = 7;  		       // Switch connected to pin 4 

#define SW_PIN 7
#define RED_LED     12
#define BLUE_LED    11
#define YELLOW_LED  10
 
void setup() { 
  Serial.begin(9600);         	       // Start serial communication at 9600 bps 

  // initialize pins mode
  pinMode(SW_PIN, INPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  
  digitalWrite(RED_LED, LOW);   
  digitalWrite(BLUE_LED, LOW);
  digitalWrite(YELLOW_LED, LOW);  
} 
 
void loop() { 
  if (digitalRead(SW_PIN) == HIGH) 
  {  // If switch is ON, 
    Serial.write(1);               // send 1 to Processing
     digitalWrite(RED_LED, HIGH);   
    digitalWrite(BLUE_LED, HIGH);
    digitalWrite(YELLOW_LED, HIGH);  
  } else {                               // If the switch is not ON,
    Serial.write(0);               // send 0 to Processing 
    digitalWrite(RED_LED, LOW);   
    digitalWrite(BLUE_LED, LOW);
    digitalWrite(YELLOW_LED, LOW);    
  } 
  delay(100);                            // Wait 100 milliseconds 
} 

