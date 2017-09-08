int Led = 13 ;      // define LED Interface
int buttonpin = 51; // define the Reed sensor interfaces
int val,temp;       // define numeric variables val

void setup ()
{
  // initialize serial port
  Serial.begin(115200);
    
  pinMode (Led, OUTPUT) ;// define LED as output interface
  pinMode (buttonpin, INPUT) ;// output interface as defined Reed sensor

  Serial.println("------- Magnetron Sensor -------"); 
}
void loop ()
{
  temp = val;
  val = digitalRead (buttonpin) ;// digital interface will be assigned a value of 3 to read val
  
  if (val == HIGH) // When the Reed sensor detects a signal, LED flashes
  {
    digitalWrite (Led, HIGH);
    if( val != temp)
      Serial.println("HIGH"); 
  }
  else
  {
    digitalWrite (Led, LOW);
    if( val != temp)
      Serial.println("LOW"); 
  }
}
