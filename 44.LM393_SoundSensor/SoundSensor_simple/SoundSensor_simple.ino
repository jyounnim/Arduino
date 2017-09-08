void setup() {
  Serial.begin(9600);
}
 
void loop() {
  float vol; 
  int SoundSensorValue;
 
  SoundSensorValue = analogRead(A0);
  SoundSensorValue = 1023 - SoundSensorValue;
  Serial.print("SoundSensorValue = ");
  Serial.println(SoundSensorValue);

  delay(100);
}
