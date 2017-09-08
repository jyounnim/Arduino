int pirPin = 3;
int val;

void setup() 
{
  // initialize serial port
  Serial.begin(9600); 

  pinMode(13, OUTPUT);

  Serial.println("------- PIR test -------"); 
  
}

void loop() 
{
  val = digitalRead(pirPin); //read state of the PIR
  if (val == LOW) 
  {
    Serial.println("LOW");     
    digitalWrite(13, LOW);
  } else {
    Serial.println("HIGH"); 
    digitalWrite(13, HIGH);
  }
  
  delay(1000); 
} 


