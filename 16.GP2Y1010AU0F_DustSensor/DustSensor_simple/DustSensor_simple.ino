/*
 * OLED Display to I2C
  ** CLK - CLK
  ** SDA - SDA
*/
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SPI.h>
#include <Wire.h>

#define __USING_OLED_DISPLAY_

#define LED_DUST  6
#define PIN_DUST  0  // A0
#ifdef __USING_OLED_DISPLAY_
#define OLED_RESET 9
#endif

#define SAM_TIME    280
#define DELTA_TIME  40
#define OFF_TIME    9680

// Define global values
#ifdef __USING_OLED_DISPLAY_
Adafruit_SSD1306 display(OLED_RESET);
#endif

void setup()
{
  Serial.begin(115200);
  pinMode(LED_DUST, OUTPUT);

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
  display.println("Dust Sensor v1.0");  
  display.println("Jonathan You ");  
  display.display();
  delay(1000);
#endif  
}
 
void loop()
{
  float voMeasured = 0;
  float calcVoltage = 0;
  float dustDensity = 0;
  
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
//  dustDensity = 0.17 * calcVoltage - 0.1;	 
  dustDensity = 170 * calcVoltage - 100;	 
  if( dustDensity < 0 )
    dustDensity = 0;

  Serial.print("Raw Signal Value (0-1023): ");
  Serial.print(voMeasured);
  Serial.print(" - Voltage: ");
  Serial.print(calcVoltage);
  Serial.print("V");
  Serial.print(" - Dust Density: ");
  Serial.print(dustDensity);
  Serial.println(" ug/m^3");

#ifdef __USING_OLED_DISPLAY_
  display.setCursor(0,0);
  display.clearDisplay();
  display.print("Vol:");
  display.print(calcVoltage);
  display.println("V");
  display.print("Dust:");
//  display.print(dustDensity);  
  display.print(dustDensity,0);
  display.println("ug/m^3");
  display.display();
#endif
  delay(1000);
}
