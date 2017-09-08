int LED=13;
int sw = 0;

//Setup usb serial connection to computer
void setup() 
{ 
  Serial.begin(9600); 
  delay(100);
  Serial3.begin(9600); //Setup Bluetooth serial connecti/on to android
  Serial.println("hello !!");
  Serial3.write("hello !!");
  pinMode(LED, OUTPUT);
}

void loop() 
{

  if(Serial3.available()) 
  {
    char cmd = (char)Serial3.read();
    if(cmd == 'n' & sw == 0) 
    {
      Serial.println("Red LED ON!");
      Serial3.write("Red LED ON\r\n");
      digitalWrite(LED, HIGH); 
      sw = 1;
    }
    
    if(cmd == 'f' & sw == 1) 
    {
      Serial.println("Red LED OFF");
      Serial3.write("Red LED OFF\r\n");
      digitalWrite(LED, LOW);
      sw = 0;
    }
  }
}
