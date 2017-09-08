void setup()
{
  Serial.begin(115200); 
  Serial3.begin(9600); //Setup Bluetooth serial connecti/on to android

  Serial.println("***** AT-09 Bluetooth *****");
}

void loop()
{
  if(Serial3.available())
    Serial.write(Serial3.read());
  if(Serial.available())
    Serial3.write(Serial.read());
}
    
