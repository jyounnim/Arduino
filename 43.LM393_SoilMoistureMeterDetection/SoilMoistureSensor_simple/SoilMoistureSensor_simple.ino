void setup() {
  Serial.begin(9600);
}
 
void loop() {
  float vol; 
  int SoilMoistureSensorValue;
 
  SoilMoistureSensorValue = analogRead(A0);
  SoilMoistureSensorValue = 0x3ff - SoilMoistureSensorValue;
  Serial.print("SoilMoistureSensorValue = ");
  Serial.println(SoilMoistureSensorValue);

  delay(1000);
}
