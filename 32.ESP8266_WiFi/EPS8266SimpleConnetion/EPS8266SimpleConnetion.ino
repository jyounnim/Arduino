#define SSID "HUSLA_2G"
#define PASS "HusNetwork12@!"
#define DST_IP "220.181.111.85" //baidu.com

void setup()
{
 // Open serial communications and wait for port to open:
// Serial2.begin(57600);
 Serial2.begin(9600);
 Serial2.setTimeout(5000);

 Serial.begin(115200); //can't be faster than 19200 for softserial
 Serial.println("ESP8266 Demo");
 //test if the module is ready
 Serial2.println("AT+RST");
 delay(1000);
 if(Serial2.find("ready"))
 {
   Serial.println("Module is ready");
 }
 else
 {
   Serial.println("Module have no response.");
   while(1);
 }
 delay(1000);
 //connect to the wifi
 boolean connected=false;
 for(int i=0;i<5;i++)
 {
   if(connectWiFi())
   {
     connected = true;
     break;
   }
 }
 if (!connected){while(1);}
 delay(5000);
 //print the ip addr
 Serial2.println("AT+CIFSR");
 Serial.println("ip address:");
 while (Serial.available())
   Serial.write(Serial.read());
 //set the single connection mode
 Serial2.println("AT+CIPMUX=0");
}

void loop()
{
 String cmd = "AT+CIPSTART=\"TCP\",\"";
 cmd += DST_IP;
 cmd += "\",80";
 Serial2.println(cmd);
 Serial.println(cmd);

 if(Serial2.find("Error")) return;
 cmd = "GET / HTTP/1.0\r\n\r\n";
 Serial2.print("AT+CIPSEND=");
 Serial2.println(cmd.length());
 if(Serial2.find(">"))
 {
   Serial.print(">");
 } else {
   Serial2.println("AT+CIPCLOSE");
   Serial.println("connect timeout");
   delay(1000);
   return;
 }
 Serial2.print(cmd);
 delay(2000);
 //Serial.find("+IPD");
 while (Serial2.available())
 {
 char c = Serial2.read();
 Serial.write(c);
 if(c=='\r') Serial.print('\n');
 }
 Serial.println("====");
 delay(1000);
}

boolean connectWiFi()
{
 Serial2.println("AT+CWMODE=1");
 String cmd="AT+CWJAP=\"";
 cmd+=SSID;
 cmd+="\",\"";
 cmd+=PASS;
 cmd+="\"";
 Serial2.println(cmd);
 Serial.println(cmd);
 delay(2000);
 if(Serial2.find("OK"))
 {
   Serial.println("OK, Connected to WiFi.");
   return true;
 } else {
   Serial.println("Can not connect to the WiFi.");
   return false;
 }
}
                                                                
