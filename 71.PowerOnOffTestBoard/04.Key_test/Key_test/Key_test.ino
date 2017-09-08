/*
  Humax Power On/Off test board
  Jonathan You v1.0
  
The circuit:
  * SD card attached to SPI bus as follows:
    ** MOSI - pin 11
    ** MISO - pin 12
    ** CLK  - pin 13
    ** CS   - pin 4
  * LEDs
    ** STB power - pin 7
    ** PASS(B) - pin 8
    ** FAIL(R) - pin 9
  * STB monitor
    ** PWR_Feedback(G)  - pin A5
  * Relay control
    ** PWR_ONOFF - pin 6
  * Serial
    ** Rx - pin 0 (monitor test unit)
    ** Tx - pin 1 (to watch log by serial out)
  * 7-segment
    ** DIO  2
    ** CLK  3
    ** STR  5    
*/
 
#include <SD.h>

// Define PINs
#define PWR_ONOFF  6
#define PWR_LED    7
#define PASS_LED   8
#define FAIL_LED   9
#define POWER_DETECT  A5
#define KEY_1     A0
#define KEY_2     A1
#define JUMPER_1  A2
#define JUMPER_2  A3

// User define 
unsigned int gTestCount=0;

// 7-Segment 
#define LED_DIO  2
#define LED_CLK  3
#define LED_STR  5

#define COMMAND1  B00000001
#define COMMAND2  B01000000
#define COMMAND3  B11000000
#define COMMAND4  B10001111

byte value[] ={ B00111111, // 0
                B00000110, // 1
                B01011011, // 2
                B01001111, // 3
                B01100110, // 4
                B01101101, // 5
                B01111101, // 6
                B00000111, // 7
                B01111111, // 8
                B01101111, // 9                
                B00000000};// display nothing

byte digit[] ={ B00000001, // left segment
                B00000010,
                B00000100,
                B00001000,
                B00010000,
                B00100000,
                B01000000,
                B10000000}; // right segment
                
void setup()
{
    byte data[8]={0,0,0,0,0,0,0x3F,0};
    
// 1. initialize UART
  // Open serial communications and wait for port to open:
  Serial.begin(115200);

// 2. initialize LED pins
  pinMode(PWR_LED, OUTPUT);
  pinMode(PASS_LED, OUTPUT);
  pinMode(FAIL_LED, OUTPUT);

// 3. initialize KEY pins
  pinMode(KEY_1, INPUT);
  pinMode(KEY_2, INPUT);
  pinMode(JUMPER_1, INPUT);
  pinMode(JUMPER_2, INPUT);
  
// 4. initialize Relay pin
  pinMode(PWR_ONOFF, OUTPUT);
  digitalWrite(PWR_ONOFF, HIGH);
  
// 5. initialize 7-segment
  pinMode(LED_DIO, OUTPUT);
  pinMode(LED_CLK, OUTPUT);
  pinMode(LED_STR, OUTPUT);  
 
  spiTransferCommand(COMMAND1);
  spiTransferCommand(COMMAND2);
  spiTransferData(data, 8) ;
  spiTransferCommand(COMMAND4);
}

void spiTransferData(byte data)
{
    shiftOut(LED_DIO,LED_CLK,LSBFIRST,data);
}    

void spiTransferCommand(byte data) 
{
    //enable the line 
    digitalWrite(LED_STR,LOW);
    //Now shift out the data 
    shiftOut(LED_DIO,LED_CLK,LSBFIRST,data);
    //latch the data onto the display
    digitalWrite(LED_STR,HIGH);
}

void spiTransferData(byte* data, byte n) 
{
    byte i;
    
    //enable the line 
    digitalWrite(LED_STR,LOW);
    spiTransferData(COMMAND3);
    for( i=0 ; i<n ; i++)
    {
      //Now shift out the data 
      spiTransferData(*data);
      data++;
    }
    //latch the data onto the display
    digitalWrite(LED_STR,HIGH);
}

void save_test_result()
{
  // Save test result
  byte data[8]={0,0,0,0,0,0,0,0};
  
  Serial.println("*****************************");  
  Serial.print("Test Total Time : ");
  Serial.print(millis());
  Serial.println("ms");  
  Serial.print("Test Count : ");
  Serial.println(gTestCount);  
  Serial.println("*****************************");  

  // Display test count to 7-Segment
  data[6] = value[gTestCount%10];
  if(gTestCount >= 10)
    data[4] = value[(gTestCount/10)%10];
  else
    data[4] = value[10];
  if(gTestCount >= 100)
      data[2] = value[(gTestCount/100)%10];
  else
      data[2] = value[10];
  if(gTestCount>=1000)
      data[0] = value[(gTestCount/1000)%10];
  else
      data[0] = value[10];
  spiTransferData(data, 8);

 return;
}

void loop()
{
  static unsigned char oldKey = HIGH;
  
  if ( digitalRead(KEY_1) != oldKey)
  {
    oldKey = digitalRead(KEY_1);
    
    if( oldKey == HIGH )
    {
      Serial.println("Key Released");
      digitalWrite(PWR_LED, HIGH);    // turn ON PASS_LED
      digitalWrite(PASS_LED, HIGH);   // turn ON PASS_LED
      digitalWrite(FAIL_LED, HIGH);   // turn ON FAIL_LED      
    }else{
      Serial.println("Key Pressed");
      digitalWrite(PWR_LED, LOW);    // turn ON PASS_LED
      digitalWrite(PASS_LED, LOW);   // turn ON PASS_LED
      digitalWrite(FAIL_LED, LOW);   // turn ON FAIL_LED
      gTestCount ++;
      save_test_result();
    }             
  }

//  delay(100);
}

