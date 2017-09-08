void setup() {
  Serial.begin(9600);
}
 
void loop() {
  float vol; 
  int SoilMoistureSensorValue;
 
  SoilMoistureSensorValue = analogRead(A0);
  SoilMoistureSensorValue = (0x3ff - SoilMoistureSensorValue)/4;
  
  if( SoilMoistureSensorValue <= 84 )
    SoilMoistureSensorValue = 0;
  else if( SoilMoistureSensorValue >= 184 )
    SoilMoistureSensorValue = 100;
  else
    SoilMoistureSensorValue -= 84;
    
//  Serial.print("SoilMoistureSensorValue = ");
//  Serial.print(SoilMoistureSensorValue);
  Serial.write(SoilMoistureSensorValue);
  
  delay(100);
}
