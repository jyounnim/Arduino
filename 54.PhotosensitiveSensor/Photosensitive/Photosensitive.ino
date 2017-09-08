void setup() {
  Serial.begin(9600);
}
 
void loop() {
  float vol; 
  int PhotosensitiveSensorValue;
 
  PhotosensitiveSensorValue = analogRead(A0);
  Serial.print("PhotosensitiveSensorValue = ");
  Serial.println(PhotosensitiveSensorValue);

  delay(1000);
}
