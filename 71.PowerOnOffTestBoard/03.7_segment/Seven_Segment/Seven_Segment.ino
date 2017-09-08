/*
  * LEDs
    ** LED1 - pin 7
    ** LED2 - pin 8
    ** LED3 - pin 9
  * Switches
    ** SW1 - A0
    ** SW2 - A1   
  * 7-segment
    ** DIO - 2
    ** CLK - 3
    ** STR - 5    
*/

#define LED1 7
#define LED2 8
#define LED3 9

#define SW1 A0
#define SW2 A1

// 7-Segment 
#define SEVEN_DIO  2
#define SEVEN_CLK  3
#define SEVEN_STR  5

// User define 
unsigned int gTestCount=0;

// define 
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

byte data[8]={0,0,0,0,0,0,0x3F,0};

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin 7,8,9 as an output.
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  digitalWrite(LED2, HIGH);
  digitalWrite(LED3, HIGH);
  
  // initialize analog pin 0,1 as an input.
  pinMode(SW1, INPUT);
  pinMode(SW2, INPUT);

  // initialize 7-segment
  pinMode(SEVEN_DIO, OUTPUT);
  pinMode(SEVEN_CLK, OUTPUT);
  pinMode(SEVEN_STR, OUTPUT);  

  spiTransferCommand(COMMAND1);
  spiTransferCommand(COMMAND2);
  spiTransferData(data, 8) ;
  spiTransferCommand(COMMAND4);
  
  Serial.begin(115200);
  Serial.println("***** 7-Segment *****");

}

void spiTransferCommand(byte data) 
{
    //enable the line 
    digitalWrite(SEVEN_STR,LOW);
    //Now shift out the data 
    shiftOut(SEVEN_DIO,SEVEN_CLK,LSBFIRST,data);
    //latch the data onto the display
    digitalWrite(SEVEN_STR,HIGH);
}

void spiTransferData(byte data)
{
    shiftOut(SEVEN_DIO,SEVEN_CLK,LSBFIRST,data);
} 

void spiTransferData(byte* data, byte n) 
{
    byte i;
    
    //enable the line 
    digitalWrite(SEVEN_STR,LOW);
        
    spiTransferData(COMMAND3);
    for( i=0 ; i<n ; i++)
    {
      //Now shift out the data 
      spiTransferData(*data);
      data++;
    }

    //latch the data onto the display
    digitalWrite(SEVEN_STR,HIGH);
}

void printCount()
{
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

// the loop function runs over and over again forever
void loop() 
{
  static unsigned char oldKey = HIGH;

  if ( digitalRead(SW1) != oldKey)
  {
    oldKey = digitalRead(SW1);
    
    if( oldKey == HIGH )
    {
      Serial.println("Key Released");
      digitalWrite(LED1, LOW);    // turn Off LED1
      digitalWrite(LED2, HIGH);   // turn On LED2
      gTestCount ++;
      printCount();
    }else{
      Serial.println("Key Pressed");
      digitalWrite(LED1, HIGH);    // turn On LED1
      digitalWrite(LED2, LOW);     // turn Off LED2
    }             
  }
  
}
