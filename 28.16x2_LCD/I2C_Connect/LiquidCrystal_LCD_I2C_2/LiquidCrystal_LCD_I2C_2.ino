#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#if defined(ARDUINO) && ARDUINO >= 100
#define printByte(args) write(args);
#else
#define printByte(args) print(args,BYTE);
#endif

LiquidCrystal_I2C lcd(0x3F,16,2); // set the LCD address to 0x27 or 0x3F

void setup()
{
  lcd.init(); // initialize the lcd
  lcd.backlight();
  lcd.clear();
  delay(100);
  
  for(int i = 0; i< 3; i++)
  {
    lcd.backlight();
    delay(250);
    lcd.noBacklight();
    delay(250);
  }
  lcd.backlight();
}

void loop()
{
  int x=0;

  lcd.clear();
  lcd.setCursor(2,0); //Start at character 0 on line 0
  lcd.print("Hello World");
  lcd.setCursor(0,1); //Start at character 0 on line 1
  lcd.print("Jaeyoun You");
  delay(3000); //Wait 3 seconds

  lcd.clear();
  lcd.setCursor(0,0); //Start at character 0 on line 0
  lcd.print("Cursor Blink");
  lcd.blink();
  delay(2000);

  lcd.setCursor(0,0);
  lcd.print("Cursor noBlink");
  lcd.noBlink();
  delay(2000);
}
