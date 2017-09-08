/*
 * Dust sensor
  ** Sensor - A0
  ** Sensor_LED - 6
 * OLED Display to I2C
  ** CLK - CLK
  ** SDA - SDA
  ** Reset - 9
 * RTC (I2C)
  ** CLK - CLK
  ** SDA - SDA

*/
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SPI.h>
#include <Wire.h>
#include <DS3232RTC.h>
#include <Time.h>
#include <DHT.h>
#include <EtherCard.h>

#define LED_DUST  6
#define PIN_DUST  0  // A0
#define OLED_RESET 9
#define DHTPIN 3   
#define STATIC 1  // set to 1 to disable DHCP (adjust myip/gwip values below)
#define CS_PIN 10

#define SAM_TIME    280
#define DELTA_TIME  40
#define OFF_TIME    9680

#define DHTTYPE DHT11   // DHT 11 

// Define global values
Adafruit_SSD1306 display(OLED_RESET);
DHT dht(DHTPIN, DHTTYPE);
// ethernet interface mac address, must be unique on the LAN
static byte mymac[] = { 0x74,0x69,0x69,0x2D,0x30,0x31 };
#if STATIC
// ethernet interface ip address
static byte myip[] = { 192,168,30,185 };
// gateway ip address
static byte gwip[] = { 192,168,30,254 };
#endif

byte Ethernet::buffer[500];
BufferFiller bfill;

int gSecond; 
int gMinute; 
int gHour; 
int gWday;   // day of week, sunday is day 1
int gDay;
int gMonth; 
int gYear;   // offset from 1970; 
int oldSecond;

float voMeasured = 0;
float calcVoltage = 0;
float dustDensity = 0;
float h=0;
float t=0;
  
void setup()
{
  Serial.begin(115200);
  pinMode(LED_DUST, OUTPUT);

#if (0)
    tmElements_t t;
    time_t ti;
    
    t.Year = 2015-1970;
    t.Month = 6;
    t.Day = 9;
    t.Wday = 2;
    t.Hour = 9;
    t.Minute = 25;
    t.Second = 00;
    
    ti = makeTime(t);
    
    RTC.set(ti);
#endif

  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, SSD1306_I2C_ADDRESS);  // initialize with the I2C   
  // init done
  
  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.
  // Clear the buffer.
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.println("Dust Sensor v1");  
  display.println("Jonathan You ");  
  display.display();
  delay(1000);

  setSyncProvider(RTC.get);   // the function to get the time from the RTC
  if(timeStatus() != timeSet) 
    Serial.println("Unable to sync with the RTC");
  else
    Serial.println("RTC has set the system time");      

  dht.begin();

  if (ether.begin(sizeof Ethernet::buffer, mymac, CS_PIN) == 0)
    Serial.println(F("Failed to access Ethernet controller"));

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

void getTime()
{
  gSecond = second(); 
  gMinute = minute(); 
  gHour = hour(); 
  gWday = weekday();   
  gDay = day();
  gMonth = month(); 
  gYear = year();
}

void printDigits(int digits)
{
  // utility function for digital clock display: prints preceding colon and leading 0
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}

void digitalClockDisplay(void)
{
  // digital clock display of the time
  Serial.print(gYear); 
  Serial.print('/');
  printDigits(gMonth);
  Serial.print('/');
  printDigits(gDay);
  Serial.print(' ');
  if( gHour >= 12 )
  {
    Serial.print("PM ");
    Serial.print(gHour-12);
  } else {
    Serial.print("AM ");
    Serial.print(gHour);
  }
  Serial.print(':');
  printDigits(gMinute);
  Serial.print(':'); 
  printDigits(gSecond);
  Serial.println(); 
}

static word homePage() {
  char d1,d2,d3;
  
  d1 = (char)dustDensity;
  d2 = (char)( (char)(dustDensity*10)%10);
  d3 = (char)( (char)(dustDensity*100)%100 - d2*10);

  bfill = ether.tcpOffset();

  bfill.emit_p(PSTR(
    "HTTP/1.0 200 OK\r\n"
    "Content-Type: text/html\r\n"
    "Pragma: no-cache\r\n"
    "\r\n"
    "<meta http-equiv='refresh' content='2'/>"
    "<title>Env. monitor server</title>" 
    "<h1>Env. temperature = $D C </h1>"
    "<h1>Env. humidity = $D %</h1>"
    "<h1>Env. dust = $D.$D$D mg/m^3</h1>"),
      (char)t, (char)h, (char)d1, (char)d2, (char)d3);

  return bfill.position();
}

void loop()
{
  word len = ether.packetReceive();
  word pos = ether.packetLoop(len);

  // Sensor LED power is any digital pin on the arduino connected to Pin 3 on the sensor
  digitalWrite(LED_DUST,LOW);         // power on the LED
  delayMicroseconds(SAM_TIME);
  voMeasured = analogRead(PIN_DUST); // read the dust value via pin 5 on the sensor
  delayMicroseconds(DELTA_TIME);
  digitalWrite(LED_DUST,HIGH);        // turn the LED off
  delayMicroseconds(OFF_TIME);

  // 0 - 5V mapped to 0 - 1023 integer values
  // recover voltage
  calcVoltage = voMeasured * (5.0 / 1024.0);
  dustDensity = 0.17 * calcVoltage - 0.1;	 
  if( dustDensity < 0 )
    dustDensity = 0;

  h = dht.readHumidity();
  t = dht.readTemperature();

  getTime();
  if ( oldSecond != gSecond )  
    digitalClockDisplay();  

  if ( oldSecond != gSecond )  
  {
    Serial.print("Dust Density: ");
    Serial.print(dustDensity);
    Serial.println(" mg/m^3");
  }  

  if ( oldSecond != gSecond )  
  {
    // check if returns are valid, if they are NaN (not a number) then something went wrong!
    if (isnan(t) || isnan(h)) {
      Serial.println("Failed to read from DHT");
    } else {
      Serial.print("Humidity: ");
      Serial.print(h);
      Serial.print(" %\t");
      Serial.print("Temperature: ");
      Serial.print(t);
      Serial.println(" *C");
    }
    Serial.println(" ");
  
    display.setCursor(0,0);
    display.clearDisplay();
    display.print("Env = ");
    display.print(t,0);
    display.print("C / ");
    display.print(h,0);
    display.println("%");
    display.print("Dust:");
    display.print(dustDensity);
    display.println("mg/m^3"); 
    display.display();
  }

  if (pos)  // check if valid tcp data is received
    ether.httpServerReply(homePage()); // send web page data 

  oldSecond = gSecond;
}
