void setup() {
  Serial.begin(9600);
}
 
void loop() {
  float vol; 
  int WaterSensorValue;
 
  WaterSensorValue = analogRead(A0);
  Serial.print("WaterSensorValue = ");
  Serial.println(WaterSensorValue);

  delay(1000);
}
