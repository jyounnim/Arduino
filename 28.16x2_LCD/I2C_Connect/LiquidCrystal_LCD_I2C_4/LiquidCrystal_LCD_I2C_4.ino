#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
 
LiquidCrystal_I2C lcd(0x3F,16,2); // set the LCD address to 0x27 or 0x3F
 
void setup()
{
  lcd.init();
}
 
void loop()
{
  //Backlight on/off
  lcd.backlight();

  //cursor
  lcd.setCursor(0,0);
   
  //write function
  lcd.autoscroll();
  lcd.print("ASCII CODE WRITE:"); 
  for( int i=0x21 ; i<0x7F ; i++ )
  {
    lcd.write(i);
    delay(500);
  }
  delay(5000);
  
  lcd.clear();
  lcd.noBacklight();
  delay(1000);
}

