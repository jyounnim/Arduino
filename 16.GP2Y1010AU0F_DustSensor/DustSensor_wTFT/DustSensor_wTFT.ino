#if (1)
/*
 Standalone Sketch to use with a Arduino UNO and a
 Sharp Optical Dust Sensor GP2Y1010AU0F
*/
#include <Adafruit_GFX.h>    // Core graphics library
#include "SWTFT.h" // Hardware-specific library

// Assign human-readable names to some common 16-bit color values:
#define	BLACK   0x0000
#define	BLUE    0x001F
#define	RED     0xF800
#define	GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

SWTFT tft;
  
int measurePin = 8; //Connect dust sensor to Arduino A8 pin
int ledPower = 50;   //Connect 50 led driver pins of dust sensor to Arduino D2
  
int samplingTime = 280;
int deltaTime = 40;
int sleepTime = 9680;
  
float voMeasured = 0;
float calcVoltage = 0;
float dustDensity = 0;
  
void setup(){
  uint16_t identifier;

  Serial.begin(115200);
  pinMode(ledPower,OUTPUT);

  tft.reset();
  identifier = tft.readID(); 
  tft.begin(identifier);  

}
  
void loop(){
  tft.fillScreen(BLACK);  
 
  digitalWrite(ledPower,LOW); // power on the LED
  delayMicroseconds(samplingTime);
  
  voMeasured = analogRead(measurePin); // read the dust value
  
  delayMicroseconds(deltaTime);
  digitalWrite(ledPower,HIGH); // turn the LED off
  delayMicroseconds(sleepTime);
  
  // 0 - 5V mapped to 0 - 1023 integer values
  // recover voltage
  calcVoltage = voMeasured * (5.0 / 1024.0);
  
  // linear eqaution taken from http://www.howmuchsnow.com/arduino/airquality/
  // Chris Nafis (c) 2012
  dustDensity = 0.17 * calcVoltage - 0.1;
  if( dustDensity < 0)
    dustDensity = 0;
  
  Serial.print("Raw Signal Value (0-1023): ");
  Serial.print(voMeasured);
  
  Serial.print(" - Voltage: ");
  Serial.print(calcVoltage);
  
  Serial.print(" - Dust Density: ");
  Serial.println(dustDensity); // unit: mg/m3

  tft.setCursor(0, 7);
  tft.setTextColor(BLUE);  
  tft.setTextSize(2);
  tft.println(F("GP2Y1010AU0F"));
  tft.println(F("DustSensor test \n"));    

  tft.setTextColor(WHITE);     
  tft.print("Raw Signal Value \n(0-1023): ");
  tft.println(voMeasured);
  tft.print("Voltage: ");
  tft.println(calcVoltage);   
  tft.print("Dust: ");
  tft.print(dustDensity); // unit: mg/m3
  tft.println(F(" mg/m^3"));
  delay(1000);
}

#else

#include <SharpDust.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include "SWTFT.h" // Hardware-specific library

#define DUST_LED_PIN		50
#define DUST_MEASURE_PIN	8

// Assign human-readable names to some common 16-bit color values:
#define	BLACK   0x0000
#define	BLUE    0x001F
#define	RED     0xF800
#define	GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

SWTFT tft;

void setup() {
  uint16_t identifier;
    
  Serial.begin(115200);
  Serial.println(F("GP2Y1010AU0F DustSensor test with 2.4 inch TFT"));

  tft.reset();
  identifier = tft.readID(); 
  Serial.print(F("LCD driver chip: "));
  Serial.println(identifier, HEX);
  tft.begin(identifier);
  
  SharpDust.begin(DUST_LED_PIN, DUST_MEASURE_PIN);
}

void loop() {
  float val;

  val = SharpDust.measure();
  tft.fillScreen(BLACK);  
  tft.setCursor(0, 7);
  tft.setTextColor(WHITE);  
  tft.setTextSize(2);
  tft.println(F("GP2Y1010AU0F"));
  tft.println(F("DustSensor test"));
    
  while (1) 
  { 
    tft.fillScreen(BLACK);  
    val = SharpDust.measure();
    if( val < 0)
      val = 0;
  
    tft.fillScreen(BLACK);  
    tft.setCursor(0, 7);
    tft.setTextColor(BLUE);  
    tft.setTextSize(2);
    tft.println(F("GP2Y1010AU0F"));
    tft.println(F("DustSensor test \n"));    

    tft.setTextColor(WHITE);     
    tft.print(F("Dust Raw Data:"));
    tft.println(analogRead(DUST_MEASURE_PIN));
    tft.print(F("Dust :"));
    tft.print(val,2);
    tft.println(F(" mg/m^3"));

    Serial.print(F("Dust Raw Data:"));
    Serial.println(analogRead(DUST_MEASURE_PIN));
    
    Serial.print(F("Dust :"));
    Serial.print(val);
    Serial.println(F(" mg/m^3"));

    delay(5000);
  }
}
#endif

