// to the pins used:
const int triggerPin = 12;
const int sensorPin = 0;
int val = 0;
int counter = 0;

void setup() {
  Serial.println( "MQ-7 sensor" );
  pinMode(triggerPin, OUTPUT); 
  Serial.begin( 115200 );
}

void loop() {
  // 60s high voltage 5.0v
  Serial.print( "Heating Sensor(waiting 60s).." );
  analogWrite(triggerPin, 255);  
  for( char i=60; i; i-- )
  {
    delay(1000);
    Serial.print( '.' );
  }
  Serial.println( " " );  

  // 90s low voltage 1.4v
  analogWrite(triggerPin, 67);
  val = analogRead( sensorPin );  
  
  for ( counter = 0; counter < 90; counter++ )
  {
    Serial.print("Sensor Value: ");
    Serial.println(val);
    delay(1000);
  }
  
}
