/* Originally posted on www.bbangpan.com

   Program Description: FM module test code
   Needed library : TinyGPS https://github.com/mikalhart/TinyGPS
   Tested on : Arduino 1.7.4, Arduino Mega ADK, blox Neo 6M GPS Module, TigyGPS
   
   Copyright (c) 2015 www.bbangpan.com. All rights reserved.
   This program can be used for any non-commercial purpose freely.   */

#include <SoftwareSerial.h>
#include <TinyGPS.h>
#include <Adafruit_MLX90614.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 12
Adafruit_SSD1306 display(OLED_RESET);

SoftwareSerial mySerial(4,3); // RX(<-TX of GPS module), TX(<-RX of GPS module)
TinyGPS gps;

void gpsdump(TinyGPS &gps);
void printFloat(double f, int digits = 2);

void setup()  
{
  Serial.begin(115200);
  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, SSD1306_I2C_ADDRESS);  // initialize with the I2C   

  // set the data rate for the SerialPort(use SoftwareSerial port on Arduino UNO)
  mySerial.begin(38400);  // Serial2->mySerial on Arduino UNO
  
  delay(1000);
  Serial.print("uBlox Neo 6M / TinyGPS testing - ");
  Serial.println(TinyGPS::library_version());

  display.setTextSize(1);
  display.setTextColor(WHITE);
  
  display.clearDisplay();
  display.setCursor(0,0);
  display.println("GPS testing");
  display.print("Ver:"); display.println(TinyGPS::library_version());
  display.display();

  delay(2000);
}

void draw(float lat, float lon, unsigned long l_date, unsigned long l_time, float f_alt, float f_speed) 
{
  display.clearDisplay();
  display.setCursor(0,0);
  display.print("A:"); display.println(lat);  // latitude
  display.print("O:"); display.print(lon); // longitude
  display.display();
}

void loop() // run over and over
{
  bool newdata = false;
  unsigned long start = millis();
  float flat, flon, falt, fspeed;
  unsigned long date, time, age;

  // Every 2 seconds we print an update
  while (millis() - start < 5000) {
    if (mySerial.available() && gps.encode(mySerial.read())) {
        newdata = true;
        // break;  // uncomment to print new data immediately!
    }
  }
  
  if (newdata) {
    gps.f_get_position(&flat, &flon, &age);
    gps.get_datetime(&date, &time, &age);
    fspeed = gps.f_speed_kmph();
    falt = gps.f_altitude();
    draw(flat, flon, date, time, falt,fspeed);
  }
}  




