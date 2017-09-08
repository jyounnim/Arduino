int Led = 13;// define LED Interface 
int Shock = 3;// define the percussion Sensor Interface 
int val;// define numeric variables val 
boolean state = false;  // interrupt state

void setup () 
{
  // initialize serial port
  Serial.begin(115200); 
  
  pinMode (Led, OUTPUT) ;// define LED as output interface 
  pinMode (Shock, INPUT) ;// define knock sensor output interface 

  attachInterrupt(digitalPinToInterrupt(Shock), ShockIsr, FALLING );
  
  Serial.println("------- Knock Sensor test -------");
} 

void ShockIsr()
{
  if( state == true )
    return;
    
  state = true;
}

void loop () 
{ 
  if (state == true) // When the percussion when the sensor detects a signal, LED flashes 
  {
    Serial.println("Knock is detected");
    digitalWrite(Led, HIGH); 
    delay(500);
    state = false;
  }else{
    val = digitalRead (Shock);
    if( val == HIGH )
         digitalWrite(Led, LOW); 
  }
} 

