#define HC05_EN 8

String message;
String tmp;

void setup()
{
  pinMode(HC05_EN, OUTPUT);
  digitalWrite(HC05_EN, LOW); // Open EN ping
  
  Serial.begin(115200); 
//  Serial3.begin(38400); //Setup Bluetooth serial connecti/on to android
  Serial3.begin(9600); //Setup Bluetooth serial connecti/on to android

  if( digitalRead(HC05_EN) == HIGH)
    Serial.println("ATcommand!!");
  else
    Serial.println("Normal mode!");

  tmp = message;    
}

void loop()
{
  if(Serial3.available())
    Serial.write(Serial3.read());
  if(Serial.available())
    Serial3.write(Serial.read());
}
    
