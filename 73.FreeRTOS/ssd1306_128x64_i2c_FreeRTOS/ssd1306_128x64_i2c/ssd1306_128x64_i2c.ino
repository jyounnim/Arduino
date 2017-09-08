/*********************************************************************
This is an example for our Monochrome OLEDs based on SSD1306 drivers

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/category/63_98

This example is for a 128x64 size display using I2C to communicate
3 pins are required to interface (2 I2C and one reset)

Adafruit invests time and resources providing this open source code, 
please support Adafruit and open-source hardware by purchasing 
products from Adafruit!

Written by Limor Fried/Ladyada  for Adafruit Industries.  
BSD license, check license.txt for more information
All text above, and the splash screen must be included in any redistribution
*********************************************************************/

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <FreeRTOS_AVR.h>

#define OLED_RESET 12
Adafruit_SSD1306 display(OLED_RESET);

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2

// The LED is attached to pin 13 on Arduino.
const uint8_t LED_PIN = 13;

#define LOGO16_GLCD_HEIGHT 16 
#define LOGO16_GLCD_WIDTH  16 
static const unsigned char PROGMEM logo16_glcd_bmp[] =
{ B00000000, B11000000,
  B00000001, B11000000,
  B00000001, B11000000,
  B00000011, B11100000,
  B11110011, B11100000,
  B11111110, B11111000,
  B01111110, B11111111,
  B00110011, B10011111,
  B00011111, B11111100,
  B00001101, B01110000,
  B00011011, B10100000,
  B00111111, B11100000,
  B00111111, B11110000,
  B01111100, B11110000,
  B01110000, B01110000,
  B00000000, B00110000 };

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

//------------------------------------------------------------------------------
/*
 * Thread 1
 */
// Declare the thread function for thread 1.
static void Thread1(void* arg) {

  // draw a single pixel
  display.drawPixel(10, 10, WHITE);
  // Show the display buffer on the hardware.
  // NOTE: You _must_ call ddelayisplay after making any drawing commands
  // to make them visible on the display hardware!
  display.display();
  vTaskDelay((200L * configTICK_RATE_HZ) / 1000L);
  display.clearDisplay();
  
  // draw many lines
  testdrawline();
  display.display();
  vTaskDelay((200L * configTICK_RATE_HZ) / 1000L);
  display.clearDisplay();

  // draw rectangles
  testdrawrect();
  display.display();
  vTaskDelay(2L * configTICK_RATE_HZ);
  display.clearDisplay();

  // draw multiple rectangles
  testfillrect();
  display.display();
  vTaskDelay(2L * configTICK_RATE_HZ);
  display.clearDisplay();

  // draw mulitple circles
  testdrawcircle();
  display.display();
  vTaskDelay(2L * configTICK_RATE_HZ);
  display.clearDisplay();

  // draw a white circle, 10 pixel radius
  display.fillCircle(display.width()/2, display.height()/2, 10, WHITE);
  display.display();
  vTaskDelay(2L * configTICK_RATE_HZ);
  display.clearDisplay();

  testdrawroundrect();
  vTaskDelay(2L * configTICK_RATE_HZ);
  display.clearDisplay();

  testfillroundrect();
  vTaskDelay(2L * configTICK_RATE_HZ);
  display.clearDisplay();

  testdrawtriangle();
  vTaskDelay(2L * configTICK_RATE_HZ);
  display.clearDisplay();

  testfilltriangle();
  vTaskDelay(2L * configTICK_RATE_HZ);
  display.clearDisplay();

  // draw the first ~12 characters in the font
  testdrawchar();
  display.display();
  vTaskDelay(2L * configTICK_RATE_HZ);
  display.clearDisplay();

  // draw scrolling text
  testscrolltext();
  vTaskDelay(2L * configTICK_RATE_HZ);
  display.clearDisplay();

  // text display tests
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("Hello, world!");
  display.setTextColor(BLACK, WHITE); // 'inverted' text
  display.println(3.141592);
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.print("0x"); display.println(0xDEADBEEF, HEX);
  display.display();
  vTaskDelay(2L * configTICK_RATE_HZ);

  // miniature bitmap display
  display.clearDisplay();
  display.drawBitmap(30, 16,  logo16_glcd_bmp, 16, 16, 1);
  display.display();

  // invert the display
  display.invertDisplay(true);
  vTaskDelay(1L * configTICK_RATE_HZ);
  display.invertDisplay(false);
  vTaskDelay(1L * configTICK_RATE_HZ);

  // draw a bitmap icon and 'animate' movement
  testdrawbitmap(logo16_glcd_bmp, LOGO16_GLCD_HEIGHT, LOGO16_GLCD_WIDTH);

  while(1);
  {}
}

//------------------------------------------------------------------------------
/*
 * Thread 2, turn the LED blinking
 */
// Declare the thread function for thread 2.
void Thread2(void* arg) {
  pinMode(LED_PIN, OUTPUT);

  while (1) {
    
    // Turn LED on.
    digitalWrite(LED_PIN, HIGH);
    
    // Sleep for 200 milliseconds.
    vTaskDelay((200L * configTICK_RATE_HZ) / 1000L);

    // Turn LED off.
    digitalWrite(LED_PIN, LOW);

    // Sleep for 200 milliseconds.
    vTaskDelay((200L * configTICK_RATE_HZ) / 1000L);
    
  }
}


void setup()   {                
  portBASE_TYPE s1, s2;
  
  Serial.begin(115200);
  Serial.println("Start");

  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3D);  // initialize with the I2C addr 0x3D (for the 128x64)
  // init done
  
  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.
  display.display();
  delay(2000);
  // Clear the buffer.
  display.clearDisplay();

  // create task at priority two
  s1 = xTaskCreate(Thread1, NULL, 2048, NULL, 2, NULL);

  // create task at priority one
  s2 = xTaskCreate(Thread2, NULL, configMINIMAL_STACK_SIZE, NULL, 1, NULL);

  // check for creation errors
  if ( s1 != pdPASS || s1 != pdPASS ) {
    Serial.println(F("Creation problem"));
    while(1);
  }
  
  // start scheduler
  vTaskStartScheduler();
  
  Serial.println(F("Insufficient RAM"));
  while(1);
}

//------------------------------------------------------------------------------
// WARNING idle loop has a very small stack (configMINIMAL_STACK_SIZE)
// loop must never block
void loop() {
  // Not used.
}

void testdrawbitmap(const uint8_t *bitmap, uint8_t w, uint8_t h) {
  uint8_t icons[NUMFLAKES][3];
 
  // initialize
  for (uint8_t f=0; f< NUMFLAKES; f++) {
    icons[f][XPOS] = random(display.width());
    icons[f][YPOS] = 0;
    icons[f][DELTAY] = random(5) + 1;
#if (0)    
    Serial.print("x: ");
    Serial.print(icons[f][XPOS], DEC);
    Serial.print(" y: ");
    Serial.print(icons[f][YPOS], DEC);
    Serial.print(" dy: ");
    Serial.println(icons[f][DELTAY], DEC);
#endif    
  }

  while (1) {
    // draw each icon
    for (uint8_t f=0; f< NUMFLAKES; f++) {
      display.drawBitmap(icons[f][XPOS], icons[f][YPOS], logo16_glcd_bmp, w, h, WHITE);
    }
    display.display();
    vTaskDelay((200L * configTICK_RATE_HZ) / 1000L);
    
    // then erase it + move it
    for (uint8_t f=0; f< NUMFLAKES; f++) {
      display.drawBitmap(icons[f][XPOS], icons[f][YPOS],  logo16_glcd_bmp, w, h, BLACK);
      // move it
      icons[f][YPOS] += icons[f][DELTAY];
      // if its gone, reinit
      if (icons[f][YPOS] > display.height()) {
	icons[f][XPOS] = random(display.width());
	icons[f][YPOS] = 0;
	icons[f][DELTAY] = random(5) + 1;
      }
    }
   }
}


void testdrawchar(void) {
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);

  for (uint8_t i=0; i < 168; i++) {
    if (i == '\n') continue;
    display.write(i);
    if ((i > 0) && (i % 21 == 0))
      display.println();
  }    
  display.display();
}

void testdrawcircle(void) {
  for (int16_t i=0; i<display.height(); i+=2) {
    display.drawCircle(display.width()/2, display.height()/2, i, WHITE);
    display.display();
  }
}

void testfillrect(void) {
  uint8_t color = 1;
  for (int16_t i=0; i<display.height()/2; i+=3) {
    // alternate colors
    display.fillRect(i, i, display.width()-i*2, display.height()-i*2, color%2);
    display.display();
    color++;
  }
}

void testdrawtriangle(void) {
  for (int16_t i=0; i<min(display.width(),display.height())/2; i+=5) {
    display.drawTriangle(display.width()/2, display.height()/2-i,
                     display.width()/2-i, display.height()/2+i,
                     display.width()/2+i, display.height()/2+i, WHITE);
    display.display();
  }
}

void testfilltriangle(void) {
  uint8_t color = WHITE;
  for (int16_t i=min(display.width(),display.height())/2; i>0; i-=5) {
    display.fillTriangle(display.width()/2, display.height()/2-i,
                     display.width()/2-i, display.height()/2+i,
                     display.width()/2+i, display.height()/2+i, WHITE);
    if (color == WHITE) color = BLACK;
    else color = WHITE;
    display.display();
  }
}

void testdrawroundrect(void) {
  for (int16_t i=0; i<display.height()/2-2; i+=2) {
    display.drawRoundRect(i, i, display.width()-2*i, display.height()-2*i, display.height()/4, WHITE);
    display.display();
  }
}

void testfillroundrect(void) {
  uint8_t color = WHITE;
  for (int16_t i=0; i<display.height()/2-2; i+=2) {
    display.fillRoundRect(i, i, display.width()-2*i, display.height()-2*i, display.height()/4, color);
    if (color == WHITE) color = BLACK;
    else color = WHITE;
    display.display();
  }
}
   
void testdrawrect(void) {
  for (int16_t i=0; i<display.height()/2; i+=2) {
    display.drawRect(i, i, display.width()-2*i, display.height()-2*i, WHITE);
    display.display();
  }
}

void testdrawline() {  
  for (int16_t i=0; i<display.width(); i+=4) {
    display.drawLine(0, 0, i, display.height()-1, WHITE);
    display.display();
  }
  for (int16_t i=0; i<display.height(); i+=4) {
    display.drawLine(0, 0, display.width()-1, i, WHITE);
    display.display();
  }
  vTaskDelay((250L * configTICK_RATE_HZ) / 1000L);
  
  display.clearDisplay();
  for (int16_t i=0; i<display.width(); i+=4) {
    display.drawLine(0, display.height()-1, i, 0, WHITE);
    display.display();
  }
  for (int16_t i=display.height()-1; i>=0; i-=4) {
    display.drawLine(0, display.height()-1, display.width()-1, i, WHITE);
    display.display();
  }
  vTaskDelay((250L * configTICK_RATE_HZ) / 1000L);
  
  display.clearDisplay();
  for (int16_t i=display.width()-1; i>=0; i-=4) {
    display.drawLine(display.width()-1, display.height()-1, i, 0, WHITE);
    display.display();
  }
  for (int16_t i=display.height()-1; i>=0; i-=4) {
    display.drawLine(display.width()-1, display.height()-1, 0, i, WHITE);
    display.display();
  }
  vTaskDelay((250L * configTICK_RATE_HZ) / 1000L);

  display.clearDisplay();
  for (int16_t i=0; i<display.height(); i+=4) {
    display.drawLine(display.width()-1, 0, 0, i, WHITE);
    display.display();
  }
  for (int16_t i=0; i<display.width(); i+=4) {
    display.drawLine(display.width()-1, 0, i, display.height()-1, WHITE); 
    display.display();
  }
  vTaskDelay((250L * configTICK_RATE_HZ) / 1000L);
}

void testscrolltext(void) {
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(10,0);
  display.clearDisplay();
  display.println("scroll");
  display.display();
 
  display.startscrollright(0x00, 0x0F);
  vTaskDelay(2L * configTICK_RATE_HZ);
  display.stopscroll();
  vTaskDelay(1L * configTICK_RATE_HZ);
  display.startscrollleft(0x00, 0x0F);
  vTaskDelay(2L * configTICK_RATE_HZ);
  display.stopscroll();
  vTaskDelay(1L * configTICK_RATE_HZ);    
  display.startscrolldiagright(0x00, 0x07);
  vTaskDelay(2L * configTICK_RATE_HZ);
  display.startscrolldiagleft(0x00, 0x07);
  vTaskDelay(2L * configTICK_RATE_HZ);
  display.stopscroll();
}
