/*
  Web Server
 
 A simple web server that shows the value of the analog input pins.
 using an Arduino Wiznet Ethernet shield. 
 
 Circuit:
 * Ethernet shield attached to pins 8, 11, 12, 13
 * Analog inputs attached to pins A0 through A5 (optional)
 
 created 18 Dec 2009
 by David A. Mellis
 modified 9 Apr 2012
 by Tom Igoe
 
 */

#include <SPI.h>
#include <EtherCard.h>

#define STATIC 1  // set to 1 to disable DHCP (adjust myip/gwip values below)

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
static byte mymac[] = { 0x74,0x69,0x69,0x2D,0x30,0x31 };
#if STATIC
// ethernet interface ip address
static byte myip[] = { 192,168,30,185 };
// gateway ip address
static byte gwip[] = { 192,168,30,254 };
#endif

byte Ethernet::buffer[500]; // tcp/ip send and receive buffer

void setup() {
 // Open serial communications and wait for port to open:
  Serial.begin(115200);
   while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

  pinMode(4, OUTPUT);
  digitalWrite(4, HIGH);

#if STATIC
  ether.staticSetup(myip, gwip);
#else
  if (!ether.dhcpSetup())
    Serial.println("DHCP failed");
#endif

  ether.printIp("IP:  ", ether.myip);
  ether.printIp("GW:  ", ether.gwip);  
  ether.printIp("DNS: ", ether.dnsip);  
}


void loop() {
  // listen for incoming clients
  if (ether.packetLoop(ether.packetReceive())) 
  {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;

#if (1)
#else
    while (client.connected()) 
    {
      if (client.available()) 
      {
        char c = client.read();
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) 
        {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
	  client.println("Refresh: 5");  // refresh the page automatically every 5 sec
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          // output the value of each analog input pin
          for (int analogChannel = 0; analogChannel < 6; analogChannel++) 
          {
            int sensorReading = analogRead(analogChannel);
            client.print("analog input ");
            client.print(analogChannel);
            client.print(" is ");
            client.print(sensorReading);
            client.println("<br />");       
          }
          client.println("</html>");
          break;
        }
        
        if (c == '\n') 
        {
          // you're starting a new line
          currentLineIsBlank = true;
        } 
        else if (c != '\r') 
        {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
  }
#endif  
}


