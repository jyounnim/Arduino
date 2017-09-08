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
  lcd.print("BACKLIGHT ON");
  lcd.setCursor(0,1);
  lcd.print("AFTER 5SEC OFF");
  delay(5000);
  lcd.noBacklight();
  delay(3000);
  lcd.clear();
 
  //display on/off
  lcd.backlight();
  lcd.display();
  lcd.print("DISPLAY ON!");
  lcd.setCursor(0,1);
  lcd.print("AFTER 5SEC OFF");
  delay(5000);
  lcd.noDisplay();
  delay(3000);
  lcd.display();
  lcd.clear();
 
  //cursor
  lcd.print("CURSOR APPEAR");
  lcd.setCursor(0,1);  
  lcd.cursor();
  delay(5000);
  lcd.clear();
  
  //corsor move
  lcd.print("CURSOR MOVE");
  for (int i=0; i < 10 ; i++) 
  {
    lcd.setCursor(i,1);
    delay(500);
  }
  lcd.clear();
 
  //blink cursor
  lcd.print("CURSOR BLINK");
  lcd.setCursor(5,1);  
  lcd.blink();
  delay(5000);
  lcd.noCursor();
  lcd.clear();
 
  //write function
  lcd.print("ASCII CODE WRITE");
  lcd.setCursor(0,1);
  for( int i=33 ; i<49 ; i++)  
    lcd.write(i);
//  lcd.write(36);
//  lcd.write(37);
//  lcd.write(38);
  delay(5000);
  lcd.clear();  
 
  //scroll right, left
  lcd.print("SCROLL RIGHT");
  for (int positionCounter = 0; positionCounter < 16; positionCounter++) 
  {
    lcd.scrollDisplayRight();
    delay(500);
  }
  lcd.clear();
 
  lcd.print("     SCROLL LEFT");
  for (int positionCounter = 0; positionCounter < 16; positionCounter++) 
  {
    lcd.scrollDisplayLeft();
    delay(500);
  }
  lcd.clear();
 
  //autoscroll
  lcd.print("     AUTO SCROLL");
  lcd.setCursor(15,1);
  lcd.autoscroll();
  for (int i=0; i < 10 ; i++) 
  {
    lcd.print(i);
    delay(500);
  }
  lcd.clear();
  
  lcd.init();
}

