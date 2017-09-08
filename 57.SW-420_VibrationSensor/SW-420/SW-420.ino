#define AinPin A0
#define DinPin 2

unsigned char st = 0;
 
void setup() { 
  /*-(start serial port to see results )-*/
  delay(100);
  Serial.begin(9600);
  Serial.println("Vibration Sensor DS18B20");
  delay(100);
} 
 
void loop() { 
  int vibrationValue;

  if( digitalRead(DinPin) == 0 )
  {
    if( st == 0)
      Serial.println("Vibration is not detected");  
    st = 1;
  }
  else
  {
    Serial.print("Vibration is detected : value - ");  
    vibrationValue = analogRead(AinPin);
    Serial.println( vibrationValue);
    st = 0;
  }  
  delay(100);                            // Wait 100 milliseconds 
} 

