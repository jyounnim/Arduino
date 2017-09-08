/*
  Temperature test board
  Jonathan You v1.0
  
 * Temperature Sensor DS18B20 and DHT11
  ** DS18B20 : Pin 2
  ** DHT11 : Pin 3

 * SD card attached to SPI bus as follows: Arduino UNO
 (Vcc 5V)
  ** MOSI - pin 11
  ** MISO - pin 12
  ** CLK - pin 13
  ** CS - pin 4
 * SD card attached to SPI bus as follows:Arduino Mega2560
 ** MOSI - pin 51
 ** MISO - pin 50
 ** CLK - pin 52
 ** CS - pin 4
 
 * OLED Display to I2C
  ** CLK - CLK
  ** SDA - SDA
*/

#include <OneWire.h>
#include <DallasTemperature.h>
#include <DHT.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SD.h>

#define __USING_OLED_DISPLAY_
//#define __USING_SD_CARD_

// Define Pins
#define ONE_WIRE_BUS 2 // DS18B20
#define DHTPIN 3       // DHT11

// Uncomment whatever type you're using!
#define DHTTYPE DHT11   // DHT 11

/* OLED reserved */
#define OLED_RESET 9

/* SD card */
#define SD_CARD 4

// Define global values
#ifdef __USING_OLED_DISPLAY_
Adafruit_SSD1306 display(OLED_RESET);
#endif

OneWire ourWire(ONE_WIRE_BUS);
DallasTemperature sensors(&ourWire);
DHT dht(DHTPIN, DHTTYPE);

#ifdef __USING_SD_CARD_
File myFile;
#endif

void setup()
{
  Serial.begin(115200);
  Serial.println("Humax Thermal Test Program");
  Serial.println("Temperature Sensor DS18B20");

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
  display.println("Temp. test v1.0");  
  display.println("Jonathan You ");  
  display.display();
  delay(1000);
#endif

#ifdef __USING_SD_CARD_
  pinMode(10, OUTPUT);
  
  if (!SD.begin(SD_CARD)) 
  {
    Serial.println("[Err] SD ram initialization failed!");
    delay(1000);
    return;    
  }  
  Serial.println("SD ram initialization done.");

  myFile = SD.open("result.txt", FILE_WRITE);
  // if the file opened okay, write to it:
  if (myFile) 
  {
    myFile.println(" ");
    myFile.println("*****************************");  
    myFile.println(" Humax Temperature test v1.0");  
    myFile.println(" Jonathan You ");  
    myFile.println("*****************************"); 
    myFile.println("Env temp | Env Hum | Dev temp");  
    myFile.close();
  }else{
    // if the file didn't open, print an error:
    Serial.println("[Err] error opening result.txt(01)");
    delay(1000);
    return;
  }
#endif

  // Start up the DallasTemperature library
  sensors.begin();
  
}

void loop()
{
  float h0=0;
  float t0=0;
  float t1C=0;
  float t1F=0;
    
  Serial.println();
  Serial.print("Requesting temperature...");
  sensors.requestTemperatures(); // Send the command to get temperatures
  Serial.println("DONE");

  h0 = dht.readHumidity();
  t0 = dht.readTemperature();
  t1C = sensors.getTempCByIndex(0);
  t1F = sensors.getTempFByIndex(0);

#ifdef __USING_OLED_DISPLAY_
  display.setCursor(0,0);
  display.clearDisplay();
  display.print("Env = ");
  display.print(t0,0);
  display.print("C / ");
  display.print(h0,0);
  display.println("%");
  display.print("Dev = ");
  display.print(t1C);
  display.println(" C");
  display.display();
#endif

#ifdef __USING_SD_CARD_
  myFile = SD.open("result.txt", FILE_WRITE);
  if (myFile) 
  {
    myFile.print(t0,0);
    myFile.print(",");
    myFile.print(h0,0);
    myFile.print(",");    
    myFile.println(t1C);    
    myFile.close();
  }else{
    // if the file didn't open, print an error:
    Serial.println("[Err] error opening result.txt");
    delay(1000);
    return;
  }   
#endif

  Serial.print("Device 1 (index 0) = ");
  Serial.print(t1C);
  Serial.println(" C");
  Serial.print("Device 1 (index 0) = ");
  Serial.print(t1F);
  Serial.println(" F");

  Serial.print("Device 2 (index 1) = ");
  Serial.print(t0);
  Serial.println(" C");
  Serial.print("Device 2 (index 1) = ");
  Serial.print(t0*9/5+32);
  Serial.println(" F");

  Serial.print("Device 2 (index 1) = ");
  Serial.print(h0);
  Serial.println(" % Humidity");
  
  delay(1000);
}
