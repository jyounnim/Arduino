/* YourDuino Electronic Brick Test
Temperature Sensor DS18B20
- Connect cable to Arduino Digital I/O Pin 2
terry@yourduino.com */

/*-----( Import needed libraries )-----*/
#include <OneWire.h>
#include <DallasTemperature.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

/*-----( Declare Constants )-----*/
#define ONE_WIRE_BUS 2 /*-(Connect to Pin 2 )-*/

/*-----( Declare objects )-----*/
/* Set up a oneWire instance to communicate with any OneWire device*/
OneWire ourWire(ONE_WIRE_BUS);

/* Tell Dallas Temperature Library to use oneWire Library */ 
DallasTemperature sensors(&ourWire);

/*-----( Declare Variables )-----*/
#define OLED_RESET 12
Adafruit_SSD1306 display(OLED_RESET);

void setup() /*----( SETUP: RUNS ONCE )----*/
{
/*-(start serial port to see results )-*/
  delay(500);
  Serial.begin(115200);
  Serial.println("YourDuino.com: Electronic Brick Test Program");
  Serial.println("Temperature Sensor DS18B20");
	
  /*-( Start up the DallasTemperature library )-*/
  sensors.begin();

  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, SSD1306_I2C_ADDRESS);  // initialize with the I2C   
  // init done
  
  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.
  display.display();
  delay(1000);

  // Clear the buffer.
  display.clearDisplay();
  
  display.setTextColor(WHITE);
}/*--(end setup )---*/

void loop() /*----( LOOP: RUNS CONSTANTLY )----*/
{
  Serial.println();
  Serial.print("Requesting temperature...");
  sensors.requestTemperatures(); // Send the command to get temperatures
  Serial.println("DONE");
	
  Serial.print("Device 1 (index 0) = ");
  Serial.print(sensors.getTempCByIndex(0));
  Serial.println(" Degrees C");
  Serial.print("Device 1 (index 0) = ");
  Serial.print(sensors.getTempFByIndex(0));
  Serial.println(" Degrees F");

  display.setCursor(0,0);
  display.clearDisplay();
  display.print("Dev = ");
  display.print(sensors.getTempCByIndex(0));
  display.println(" C");
  display.display();

  delay(1000);
}/* --(end main loop )-- */

/* ( THE END ) */
