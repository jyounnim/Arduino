#include "Nokia_5110.h"

// Software SPI (slower updates, more flexible pin options):
// pin 3 - Serial clock out (SCLK)
// pin 4 - Serial data out (DIN)
// pin 5 - Data/Command select (D/C)
// pin 7 - LCD chip select (CS)
// pin 6 - LCD reset (RST)
#define RST 6
#define CE 7
#define DC 5
#define DIN 4
#define CLK 3

Nokia_5110 lcd = Nokia_5110(RST, CE, DC, DIN, CLK);


void setup() {
    lcd.setContrast(60); // 60 is the default value set by the driver
}

void loop() {
    lcd.clear();
    lcd.println("`setContrast` method lets you change the LCD's Contrast.");
    delay(8000);

    lcd.clear();
    lcd.println("It accepts a value between 0 and 127 (0 >= value <= 127)");
    delay(8000);
}
