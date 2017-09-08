void setup() {
  Serial.begin(115200);
}
 
void loop() {
  float vol; 
  int AlcoholSensorValue;

#if (0)
  // under 1.0 : Normal
  // 1.0~2.5 : Warning
  // over 2.5 : Danger 
  AlcoholSensorValue = analogRead(A1);
  vol = (float)AlcoholSensorValue/1024*5.0;  
  Serial.print("AlcoholSensorRaw = ");
  Serial.println(AlcoholSensorValue);
  Serial.print("AlcoholSensorValue = ");
  Serial.println(vol);
#else

  /* http://nootropicdesign.com/projectlab/2010/09/17/arduino-breathalyzer/
  It’s really hard to calibrate this sensor for even an approximate BAC reading. 
  It’s even difficult to correlate readings to looked-up BAC values. 
  There are many environmental factors that affect the resistance within the sensor
  (humidity, temperature, oxygen concentration), and this is only a $5 device anyway. 
  And as evidenced by the lack of consistency between online BAC calculators, there's not 
  even concensus about how to compute BAC. 
  Law enforcement agencies have much more sophisticated breathalyzers, and often 
  rely on actual blood tests or urinalysis for evidence.
  Despite these disappointments, the MQ-3 is still fun to play with, and maybe 
  it’s good enough to just make some LEDs light up when you blow on the sensor. 
  So just enjoy your beverages and have fun. Cheers!
  */
  
  AlcoholSensorValue = analogRead(A1);
  if ( (AlcoholSensorValue>700) & (AlcoholSensorValue<=900) )
    vol = (AlcoholSensorValue-700) * 7/250 /100;
  else if ( AlcoholSensorValue>=900 )
    vol = ((AlcoholSensorValue-900) * 15/250 /100) + 0.056;
  else
    vol = 0;
  Serial.print("AlcoholSensorRaw = ");
  Serial.println(AlcoholSensorValue);
  Serial.print("BAC = ");
  Serial.println(vol,3);

#endif
  delay(1000);
}
