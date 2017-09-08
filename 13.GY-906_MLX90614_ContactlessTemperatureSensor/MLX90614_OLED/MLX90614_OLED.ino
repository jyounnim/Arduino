/*************************************************** 
  This is a library example for the MLX90614 Temp Sensor

  Designed specifically to work with the MLX90614 sensors in the
  adafruit shop
  ----> https://www.adafruit.com/products/1748
  ----> https://www.adafruit.com/products/1749

  These sensors use I2C to communicate, 2 pins are required to  
  interface
  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ****************************************************/

#include <Wire.h>
#include <Adafruit_MLX90614.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 12
Adafruit_SSD1306 display(OLED_RESET);
Adafruit_MLX90614 mlx = Adafruit_MLX90614();

void setup() {
  Serial.begin(115200);

  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, SSD1306_I2C_ADDRESS);  // initialize with the I2C   
  display.setTextSize(1);
  display.setTextColor(WHITE);

  Serial.println("Adafruit MLX90614 test");  

  mlx.begin();
}

void loop() {
  float tAmbient=mlx.readAmbientTempC();
  float tObject=mlx.readObjectTempC();
  float tAmbientF=mlx.readAmbientTempF();
  float tObjecF=mlx.readObjectTempF();
  
  Serial.print("Ambient = "); Serial.print(tAmbient); 
  Serial.print("*C\tObject = "); Serial.print(tObject); Serial.println("*C");
  Serial.print("Ambient = "); Serial.print(tAmbientF); 
  Serial.print("*F\tObject = "); Serial.print(tObjecF); Serial.println("*F");
  Serial.println();

  display.clearDisplay();
  display.setCursor(0,0);
  display.print("Amb = ");display.print(tAmbient);display.print(" ");display.write(167);display.println("C");
  display.print("Obj = ");display.print(tObject);display.print(" ");display.write(167);display.println("C");
  display.display();
  
  delay(500);
}
