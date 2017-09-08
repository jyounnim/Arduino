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

#define __USING_VCC_FEEDBACK_  0
#define __PWR_INVERTED_        0

// Define PINs
#define PWR_ONOFF  6
#define PWR_LED    7
#define PASS_LED   8
#define FAIL_LED   9
#define POWER_DETECT  A5

// User define 
#define POWER_OFF_TIME  10000  // 10 seconds
#define POWER_ON_TIME   60000  // 60 seconds
#define PASS_LINE_NUMBER 5      // Minimum serial log line number

// Define errro code
#define RETURN_OK             0
#define ERROR_CODE_POWER_ON   0x01
#define ERROR_CODE_POWER_OFF  0x02
#define ERROE_CODE_FILE_OPEN  0x10
#define ERROE_CODE_SERIAL     0x20

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
                
// Define global values
File myFile;
String message;
unsigned int gTestCount=0;
unsigned int gPassCount=0;
unsigned int gFailCount=0;
unsigned int gSerialLine = 0;
unsigned long gTime=0;
unsigned long gPowerOnTime=0;
unsigned long g1stLogTime=0;

void setup()
{
    byte data[8]={0,0,0,0,0,0,0x3F,0};
    
// 1. initialize UART
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  Serial.setTimeout(100);

// 2. initialize LED pins
  pinMode(PWR_LED, OUTPUT);
  pinMode(PASS_LED, OUTPUT);
  pinMode(FAIL_LED, OUTPUT);
  digitalWrite(PASS_LED, HIGH);
  
// 3. initialize Relay pin
  pinMode(PWR_ONOFF, OUTPUT);

// 4. initialize 7-segment
  pinMode(LED_DIO, OUTPUT);
  pinMode(LED_CLK, OUTPUT);
  pinMode(LED_STR, OUTPUT);  
 
  spiTransferCommand(COMMAND1);
  spiTransferCommand(COMMAND2);
  spiTransferData(data, 8) ;
  spiTransferCommand(COMMAND4);

// 5. initialize SD RAM
  Serial.println("Initializing SD card...");
  // On the Ethernet Shield, CS is pin 4. It's set as an output by default.
  // Note that even if it's not used as the CS pin, the hardware SS pin 
  // (10 on most Arduino boards, 53 on the Mega) must be left as an output 
  // or the SD library functions will not work. 
  pinMode(10, OUTPUT);
   
  if (!SD.begin(4)) 
  {
    Serial.println("SD ram initialization failed!");
    error_state();    
  }  
  Serial.println("SD ram initialization done.");
  
  myFile = SD.open("result.txt", FILE_WRITE);
  // if the file opened okay, write to it:
  if (myFile == NULL) 
  {
    // if the file didn't open, print an error:
    myFile.close();
    error_state(); 
  }   
  myFile.println(" ");
  myFile.println("*****************************");  
  myFile.println(" Humax Power On/Off test v1.0");  
  myFile.println(" Jonathan You ");  
  myFile.println("*****************************");  
  myFile.close();
  
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

unsigned char power_on()
{
  int vVal;

#if __PWR_INVERTED_  
  digitalWrite(PWR_ONOFF, LOW);  // Turn ON STB
#else
  digitalWrite(PWR_ONOFF, HIGH);  // Turn ON STB
#endif
  gPowerOnTime = millis();
  delay(100);


#if __USING_VCC_FEEDBACK_
  vVal = analogRead(POWER_DETECT);
  if(  vVal <= 512 )
    return ERROR_CODE_POWER_ON;
#endif

  digitalWrite(PWR_LED, HIGH);
  return RETURN_OK;
}

unsigned char power_off()
{
  int vVal;

#if __PWR_INVERTED_  
  digitalWrite(PWR_ONOFF, HIGH);  // Turn OFF STB
#else
  digitalWrite(PWR_ONOFF, LOW);  // Turn OFF STB
#endif
  delay(100);

#if __USING_VCC_FEEDBACK_
  vVal = analogRead(POWER_DETECT);
  if(  vVal >= 512 )
    return ERROR_CODE_POWER_OFF;
#endif

  digitalWrite(PWR_LED, LOW);
  return RETURN_OK;
}

void  save_serial()
{
  gSerialLine= 0;
  
  while(1)
  {
    message = Serial.readString();
    if( message != NULL )
    {
      Serial.println(message); 
      
      myFile = SD.open("result.txt", FILE_WRITE);
      if (myFile == NULL) {
        myFile.close();
        return;
      }
      if( gSerialLine == 0 )
        g1stLogTime = millis();   
      gSerialLine++;
    
      myFile.println(message);
      myFile.close();
    }

    if( (millis() - gTime) >= POWER_ON_TIME )
      return;      
  }
  
}

unsigned char save_test_result()
{
  // Save test result
  gTime = millis();
  byte data[8]={0,0,0,0,0,0,0,0};
  
  Serial.println("*****************************");  
  Serial.print("Test Total Time : ");
  Serial.print(gTime);
  Serial.println("ms");  
  Serial.print("Reset Time : ");
  Serial.print(g1stLogTime-gPowerOnTime);
  Serial.println("ms");  
  Serial.print("Test Count : ");
  Serial.println(gTestCount);  
  Serial.print("Pass : ");
  Serial.println(gPassCount);
  Serial.print("Fail : ");
  Serial.println(gFailCount); 
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
  
  // Save thre result on SD card  
  myFile = SD.open("result.txt", FILE_WRITE);
  // if the file opened okay, write to it:
  if (myFile == NULL) {
    // if the file didn't open, print an error:
    myFile.close();
    return ERROE_CODE_FILE_OPEN;
  }   
  myFile.println("*****************************");  
  myFile.print("Test Total Time : ");
  myFile.print(gTime);
  myFile.println("ms");
  myFile.print("Reset Time : ");
  myFile.print(g1stLogTime-gPowerOnTime);
  myFile.println("ms");
  myFile.print("Test Count : ");
  myFile.println(gTestCount);  
  myFile.print("Pass : ");
  myFile.println(gPassCount);
  myFile.print("Fail : ");
  myFile.println(gFailCount); 
  myFile.println("*****************************");  
  myFile.close();
    
  return RETURN_OK;
}

unsigned char error_state()
{
  while(1)
  {
    digitalWrite(PASS_LED, HIGH);
    digitalWrite(FAIL_LED, HIGH);
    delay(500); 
    
    digitalWrite(PASS_LED, LOW);
    digitalWrite(FAIL_LED, LOW);
    delay(500); 
  } 
}

void error_code(unsigned char errCode)
{
  digitalWrite(PASS_LED, LOW);   // turn OFF PASS_LED
  digitalWrite(FAIL_LED, HIGH);  // turn ON FAIL_LED

  Serial.println("*****************************");  
  Serial.print("[Err] Errcode : ");
  Serial.println(errCode);
  if( errCode == ERROE_CODE_SERIAL )
  {
    Serial.print("[Err] Serial log was not enough : ");    
    Serial.println( gSerialLine);
  }
  Serial.println("*****************************");  

  myFile = SD.open("result.txt", FILE_WRITE);
  // if the file opened okay, write to it:
  if (myFile == NULL) {
     myFile.close();
     return;
   }
  myFile.println("*****************************");  
  myFile.print("[Err] Errcode : ");
  myFile.println(errCode);
  if( errCode == ERROE_CODE_SERIAL )
  {
    myFile.print("[Err] Serial log was not enough : ");    
    myFile.println( gSerialLine);    
  }
  myFile.println("*****************************");  
  myFile.close();

  save_test_result();
  
  return;
}

void loop()
{
  unsigned char ret;
  
  gTestCount ++;

// Step 1) Power on STB  
  ret = power_on();
  if( ret != RETURN_OK )
  {
    gFailCount++;
    error_code(ret);
    return;    
  }

// Step 2) Get STB serial log  
  save_serial();

// Step 3) Power off STB  
  ret = power_off();
  if( ret != RETURN_OK )
  {
    gFailCount++;
    error_code(ret);
    return;    
  }

// Step 4) Check  serial log
  if( gSerialLine <= PASS_LINE_NUMBER )
  {
    gFailCount++;
    error_code(ERROE_CODE_SERIAL);
    return;
  } else {
    gPassCount++;
    digitalWrite(PASS_LED, HIGH);  // turn ON PASS_LED
  }

// Step 5) Save result  
  save_test_result();
  delay(POWER_OFF_TIME);
  
#if (0) // Serial input test
  message = Serial.readString();
  if( message != NULL )
  {
    Serial.println(message); 
    
    myFile = SD.open("result.txt", FILE_WRITE);
    if (myFile == NULL) {
      myFile.close();
      return;
    }   
  
    myFile.println(message);
    myFile.close();
  }
#endif

#if (0) // LED test, SD card, relay
  Serial.println("LEDs ON"); 
  digitalWrite(PASS_LED, HIGH);  // turn ON PASS_LED
  digitalWrite(FAIL_LED, LOW);   // turn OFF FAIL_LED
  digitalWrite(PWR_ONOFF, LOW);  // Turn ON STB
  delay(100);
  if( digitalRead(PWR_LED) == HIGH )
    gPassCount++;
  else
    gFailCount++;
  delay(2000);                  // wait for a second

  Serial.println("LEDs OFF"); 
  digitalWrite(PASS_LED, LOW);   // turn OFF PASS_LED
  digitalWrite(FAIL_LED, HIGH);  // turn ON FAIL_LED
  digitalWrite(PWR_ONOFF, HIGH); // Turn OFF STB
  delay(100);
  if( digitalRead(PWR_LED) == LOW )
    gPassCount++;
  else
    gFailCount++;
  delay(2000);                  // wait for a second
  
  if(save_test_result() != RETURN_OK)
  {
    error_state();  
  }
#endif  

}

