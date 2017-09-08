//char message; //string that stores the incoming message
String message;
String tmp;

void setup()
{
  Serial.begin(9600); 
  delay(100);
  Serial3.begin(9600); //Setup Bluetooth serial connecti/on to android
  Serial.println("hello !!");
  tmp = message;
}

void loop()
{
 
  while(Serial3.available())
  {
    message += char(Serial3.read());//store string from serial command
    Serial.print(message);
    Serial3.print(message);
    message = tmp;
  }
  delay(5); //delay
}
    
