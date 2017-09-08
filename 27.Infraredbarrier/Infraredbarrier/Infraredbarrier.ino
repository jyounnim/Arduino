#define DIN_PIN   7
#define AIN_PIN   A0
#define OUT_LED  13
 
unsigned char st = 0;
 
void setup() { 
  delay(100);
  Serial.begin(9600);         	       // Start serial communication at 9600 bps 

  // initialize pins mode
  pinMode(DIN_PIN, INPUT);
  pinMode(OUT_LED, OUTPUT);

  Serial.println("** Avoidance Sensor Test **"); 
  digitalWrite(OUT_LED, HIGH);  
  
  delay(100);
} 
 
void loop() { 
  int aValue;
  
  if (digitalRead(DIN_PIN) == HIGH) 
  {
    if( st == 0 )  
    {
//      Serial.write(1);  
      Serial.println("Detected Nothing");
      digitalWrite(OUT_LED, LOW);
      st = 1;   
    }
  } else {          
//    Serial.write(0);
    Serial.print("Detected object : ");
    aValue = analogRead(AIN_PIN);
    Serial.println(aValue);
    digitalWrite(OUT_LED, HIGH);   
    st = 0;
  } 
  delay(100);        
} 

