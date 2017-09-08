void setup() {
  Serial.begin(9600);
}
 
void loop() {
  float vol; 
  int WaterSensorValue;
  int WaterLevel;
 
  WaterSensorValue = analogRead(A0);
//  Serial.print("WaterSensorValue = ");
  WaterLevel = WaterSensorValue/4;
  Serial.write(WaterLevel);

  delay(100);
}
