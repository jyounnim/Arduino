#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x3F,16,2);  // set the LCD address to 0x27 or 0x3F for a 16 chars and 2 line display

void setup()
{
  // initialize serial port
  Serial.begin(115200); 

  // initialize the lcd
  lcd.init();                      
 
  // Print a message to the LCD.
  lcd.backlight();
  lcd.print("Hello, world!");
}

void loop()
{
  lcd.backlight();
  Serial.println("Backlight ON");
  delay(1000);                  // wait for a second
  lcd.noBacklight();
  Serial.println("Backlight OFF");
  delay(1000);                  // wait for a second
}
