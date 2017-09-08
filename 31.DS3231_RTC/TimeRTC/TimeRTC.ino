/*
 * TimeRTC.pde
 * Example code illustrating Time library with Real Time Clock.
 
  * RTC(DS3231) to I2C
  ** CLK - CLK
  ** SDA - SDA

  * OLED Display to I2C
  ** CLK - CLK
  ** SDA - SDA
 */

#include <DS3232RTC.h>    //http://github.com/JChristensen/DS3232RTC
#include <Time.h>         //http://www.arduino.cc/playground/Code/Time  
#include <Wire.h>         //http://arduino.cc/en/Reference/Wire (included with Arduino IDE)
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SPI.h>

#define __USING_OLED_DISPLAY_

/* OLED reserved */
#define OLED_RESET 9

#ifdef __USING_OLED_DISPLAY_
Adafruit_SSD1306 display(OLED_RESET);
#endif

int gSecond; 
int gMinute; 
int gHour; 
int gWday;   // day of week, sunday is day 1
int gDay;
int gMonth; 
int gYear;   // offset from 1970; 

void setup(void)
{
    Serial.begin(115200);
#if (0)
    tmElements_t t;
    time_t ti;
    
    t.Year = 2015-1970;
    t.Month = 5;
    t.Day = 26;
    t.Wday = 2;
    t.Hour = 23;
    t.Minute = 51;
    t.Second = 00;
    
    ti = makeTime(t);
    
    RTC.set(ti);
#endif

    setSyncProvider(RTC.get);   // the function to get the time from the RTC
    if(timeStatus() != timeSet) 
        Serial.println("Unable to sync with the RTC");
    else
        Serial.println("RTC has set the system time");      
        
#ifdef __USING_OLED_DISPLAY_
  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, SSD1306_I2C_ADDRESS);  // initialize with the I2C   
  // init done
  
  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.
  // Clear the buffer.
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.println("RTC v1.0");  
  display.println("Jonathan You ");  
  display.display();
  delay(1000);
#endif
        
}

void loop(void)
{
    getTime();
    digitalClockDisplay();  
#ifdef __USING_OLED_DISPLAY_    
    oledClockDisplay();
#endif    
    delay(1000);
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

#ifdef __USING_OLED_DISPLAY_
void oledClockDisplay()
{
  display.setCursor(0,0);
  display.clearDisplay();

  // digital clock display of the time
  display.print(gYear); 
  display.print('/');
  printOledDigits(gMonth);
  display.print('/');
  printOledDigits(gDay);
  display.println();

  if( gHour >= 12 )
  {
    display.print("PM ");
    display.print(gHour-12);
  } else {
    display.print("AM ");
    display.print(gHour);
  }
  display.print(':');
  printOledDigits(gMinute);
  display.print(':');  
  printOledDigits(gSecond);
  display.println();
  
  display.display();
}
#endif

void printDigits(int digits)
{
    // utility function for digital clock display: prints preceding colon and leading 0
    if(digits < 10)
        Serial.print('0');
    Serial.print(digits);
}

void printOledDigits(int digits)
{
    // utility function for digital clock display: prints preceding colon and leading 0
    if(digits < 10)
        display.print('0');
    display.print(digits);
}
