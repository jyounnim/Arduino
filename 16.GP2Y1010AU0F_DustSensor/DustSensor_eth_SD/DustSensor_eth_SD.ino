/*
 * Dust sensor
  ** Sensor - A0
  ** Sensor_LED - 6
 * OLED Display to I2C
  ** CLK - CLK
  ** SDA - SDA
  ** Reset - 9
 * SD card to SP:I
  ** MOSI - pin 11
  ** MISO - pin 12
  ** CLK - pin 13
  ** CS - pin 10



*/
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define LED_DUST  6
#define PIN_DUST  0  // A0
#define OLED_RESET 9
#define CS_PIN 10

#define SAM_TIME    280
#define DELTA_TIME  40
#define OFF_TIME    9680

// Define global values
Adafruit_SSD1306 display(OLED_RESET);

File myFile;

float voMeasured = 0;
float calcVoltage = 0;
float dustDensity = 0;
float temp0=0, temp1=0, temp2=0;
  
void setup()
{
  Serial.begin(9600);
  pinMode(LED_DUST, OUTPUT);

  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, SSD1306_I2C_ADDRESS);  // initialize with the I2C   
  // init done

  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.
  // Clear the buffer.
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.println("Env. Sensor v1.0");  
  display.println("Jonathan You ");  
  display.display();
  
  // Init SD card
  if (!SD.begin(10)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
  
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  myFile = SD.open("result.txt", FILE_WRITE);
  // if the file opened okay, write to it:
  if (myFile) 
  {
    myFile.println(" ");
    myFile.println("*****************************");  
    myFile.println(" Jyoun Dust and Temp v1.0");  
    myFile.println(" Jonathan You ");  
    myFile.println("*****************************"); 
    myFile.println("  Dust ");  
    myFile.close();
  }else{
    // if the file didn't open, print an error:
    Serial.println("[Err] error opening result.txt(01)");
    delay(1000);
    return;
  }
  
  delay(1000);
}

void printDigits(int digits)
{
  // utility function for digital clock display: prints preceding colon and leading 0
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}


void loop()
{
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
  dustDensity = dustDensity*1000;	 
  if( dustDensity < 0 )
    dustDensity = 0;

  temp2 = temp1;
  temp1 = temp0;
  temp0 = dustDensity;
  
  dustDensity = (temp0+temp1+temp2) / 3;

  myFile = SD.open("result.txt", FILE_WRITE);
  if (myFile) 
  {
    myFile.print(dustDensity);
    myFile.println(" ug/m^3 : ");
    myFile.close();
  }else{
    // if the file didn't open, print an error:
    Serial.println("[Err] error opening result.txt");
    delay(1000);
    return;
  }   

#if (0)
  Serial.print("Dust Density: ");
  Serial.print(dustDensity);
  Serial.println(" ug/m^3");

  display.setCursor(0,0);
  display.clearDisplay();
  display.println("[JyounDust v1]");
  display.print("Dust:");
  display.print(dustDensity);
  display.println(" ug/m^3"); 
  display.display();
#else
  Serial.print(temp1);
  Serial.print(", "); 
  Serial.println(dustDensity);

  display.setCursor(0,0);
  display.clearDisplay();
  display.println("[JyounDust v1]");
  display.print("Dust:");
  display.print(dustDensity);
  display.println(" ug/m^3"); 

  display.display();  
  
#endif
  delay(1000);
}
