const char* ssid = "HUSLA_2G";
const char* password = "Hus_Network12@!";

// set pin numbers:
const int ledPin =  13;      // the number of the LED pin
const int ESP8266_CHPD = 4;

// Variables will change:
int ledState = HIGH;             // ledState used to set the LED

#define BUFFER_SIZE 128
char buffer[BUFFER_SIZE];

void setup() 
{ 
  pinMode(ledPin, OUTPUT);  
  pinMode(ESP8266_CHPD, OUTPUT);  
      
  Serial.begin(115200);    // Serial monitor
  Serial2.begin(9600);     // ESP8266
    
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

  Serial.println("ESP8266 Web Server.");

  while(1)
  {
    if( hardReset() == false )
      continue;
    delay(100);
    clearSerialBuffer();
  
    //connect to router
    if(connectWiFi(ssid, password) == true)
    {
      break;
    }
    delay(1000);
  }
 
  //test if the module is ready
  Serial.print("AT : ");
  Serial.println( GetResponse("AT",100) );
    
  //Change to mode 1
  Serial.print("AT+CWMODE=1 : ");
  Serial.println( GetResponse("AT+CWMODE=1",10) );
        
  //set the multiple connection mode
  Serial.print(F("AT+CIPMUX=1 : "));
  Serial.println( GetResponse("AT+CIPMUX=1",10) );
  
  //set the server of port 80 check "no change" or "OK"
  Serial.print(F("AT+CIPSERVER=1,8888 : "));
  Serial.println( GetResponse("AT+CIPSERVER=1,8888", 10) );
 
  //set time out
  Serial.print("AT+CIPSTO=15 : ");
  Serial.println( GetResponse("AT+CIPSTO=15",10) );
  
   //print the ip addr
  Serial.print(F("ip address : "));
  Serial.println( GetResponse("AT+CIFSR", 10) );
  delay(200);

   
  Serial.println();
  Serial.println(F("Start Webserver"));

  digitalWrite(ledPin,ledState);  
}

void loop() {
  int ch_id, packet_len;
  char *pb;  
  Serial2.readBytesUntil('\n', buffer, BUFFER_SIZE);
  
  if(strncmp(buffer, "+IPD,", 5)==0) {
    // request: +IPD,ch,len:data
    sscanf(buffer+5, "%d,%d", &ch_id, &packet_len);
    if (packet_len > 0) {
      // read serial until packet_len character received
      // start from :
      pb = buffer+5;
      while(*pb!=':') pb++;
      pb++;
      if (strncmp(pb, "GET /led", 8) == 0) {
        Serial.print(millis());
        Serial.print(" : ");
        Serial.println(buffer);
        Serial.print( "get led from ch :" );
        Serial.println(ch_id);
 
        delay(100);
        clearSerialBuffer();
        
       if (ledState == LOW)
          ledState = HIGH;
       else
          ledState = LOW;
        digitalWrite(ledPin, ledState);
        
        homepage(ch_id);

      } else if (strncmp(pb, "GET / ", 6) == 0) {
        Serial.print(millis());
        Serial.print(" : ");
        Serial.println(buffer);
        Serial.print( "get Status from ch:" );
        Serial.println(ch_id);
        
        delay(100);
        clearSerialBuffer();

        homepage(ch_id);
      }
    }
  }
  clearBuffer();
}

void homepage(int ch_id) {
  String Header;

  Header =  "HTTP/1.1 200 OK\r\n";
  Header += "Content-Type: text/html\r\n";
  Header += "Connection: close\r\n";  
  //Header += "Refresh: 5\r\n";
  
  String Content;
  Content = "D";
  Content += String(ledState);
  
  Header += "Content-Length: ";
  Header += (int)(Content.length());
  Header += "\r\n\r\n";
  
  
  Serial2.print("AT+CIPSEND=");
  Serial2.print(ch_id);
  Serial2.print(",");
  Serial2.println(Header.length()+Content.length());
  delay(10);
  
  // for debug buffer serial error
  //while (Serial2.available() >0 )  {
  //  char c = Serial2.read();
  //  Serial.write(c);
  //  if (c == '>') {
  //      Serial2.print(Header);
  //      Serial2.print(Content);
  //  }
  //}
  
  if (Serial2.find(">")) {
      Serial2.print(Header);
      Serial2.print(Content);
      delay(10);
   }
 
//  Serial1.print("AT+CIPCLOSE=");
//  Serial1.println(ch_id);


}

// Get the data from the WiFi module and send it to the debug serial port
String GetResponse(String AT_Command, int wait){
  String tmpData;
  
  Serial2.println(AT_Command);
  delay(10);
  while (Serial2.available() >0 )  {
    char c = Serial2.read();
    tmpData += c;
    
    if ( tmpData.indexOf(AT_Command) > -1 )         
      tmpData = "";
    else
      tmpData.trim();       
          
   }
   return tmpData;
}

boolean hardReset() 
{
  String tmpData;
  
#if (1)
  Serial2.println("AT+RST");
  delay(1000);
#else     
  digitalWrite(ESP8266_CHPD,LOW);
  delay(200);
  digitalWrite(ESP8266_CHPD,HIGH);
  delay(1000);
#endif  

#if (1)
  if(Serial2.find("ready"))
  {
    Serial.println("Wi-Fi Module is ready");
    return true;
  } else {
    Serial.println("Wi-Fi Module have no response.");
    return false;
  }
#else
  while ( Serial2.available() > 0 ) {
    char c = Serial2.read();
    tmpData +=c;
    Serial2.write(c);
    if ( tmpData.indexOf("Ready") > -1 ) {
        Serial.println("Ready");
        clearBuffer();
        return 1;
    } 
  }
#endif
}

void clearSerialBuffer(void) {
       while ( Serial2.available() > 0 ) {
         Serial2.read();
       }
}

void clearBuffer(void) {
       for (int i =0;i<BUFFER_SIZE;i++ ) {
         buffer[i]=0;
       }
}
         
boolean connectWiFi(String NetworkSSID,String NetworkPASS) 
{
  String cmd = "AT+CWJAP=\"";
  cmd += NetworkSSID;
  cmd += "\",\"";
  cmd += NetworkPASS;
  cmd += "\"";  
  Serial.println(cmd);
  Serial2.println(cmd);
//  Serial.println(GetResponse(cmd,10));
  
  delay(2000);
  if(Serial2.find("OK"))
  {
    Serial.println("OK, Connected to WiFi.");
    return true;
  } else  {
    Serial.println("Can not connect to the WiFi.");
    return false;
 }  
}
