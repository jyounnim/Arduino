void setup() {
  Serial.begin(9600);
}
 
void loop() {
  float vol; 
  int OptocouplerCircuitValue;
 
  OptocouplerCircuitValue = analogRead(A0);
  Serial.print("OptocouplerCircuitValue = ");
  Serial.println(OptocouplerCircuitValue);

  delay(1000);
}
