/*
  MC2003 7-Segment
*/

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


// the setup function runs once when you press reset or power the board
void setup() 
{
  // initialize serial port
  Serial.begin(115200); 

  pinMode(LED_DIO, OUTPUT);
  pinMode(LED_CLK, OUTPUT);
  pinMode(LED_STR, OUTPUT);  

  spiTransferCommand(COMMAND1);
  spiTransferCommand(COMMAND2);
  spiTransferCommand(COMMAND3);
  spiTransferCommand(COMMAND4);
  
  Serial.println("------- MC2003 7-Segment Blink -------"); 
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

void loop()
{ 
  byte i = 1;
  byte data[8]={0,0,0,0,0,0,0,0};
  
  while(1)
  {
    for( i=0 ; i<7 ; i++ )
    { 
#if (1)       
      data[0] = digit[i];
      data[2] = digit[i];
      data[4] = digit[i];
      data[6] = digit[i];
      spiTransferData(data, 8);
#else     
      digitalWrite(LED_STR,LOW);    
      spiTransferData(COMMAND3);
      spiTransferData(digit[i]);   //4th digit
      spiTransferData(0x00);   
      spiTransferData(digit[i]);   //3rd digit
      spiTransferData(0x00);
      spiTransferData(digit[i]);   //2nd digit
      spiTransferData(0x00);
      spiTransferData(digit[i]);   //1st digit
      spiTransferData(0x00);
      digitalWrite(LED_STR,HIGH);    
#endif      
      delay(1000);
    }

    for( i=0 ; i<11; i++ )
    { 
#if (1)       
      data[0] = value[i];
      data[2] = value[i];
      data[4] = value[i];
      data[6] = value[i];
      spiTransferData(data, 8);
#else        
      digitalWrite(LED_STR,LOW);    
      spiTransferData(COMMAND3);
      spiTransferData(value[i]);   //4th digit
      spiTransferData(0x00);   
      spiTransferData(value[i]);   //3rd digit
      spiTransferData(0x00);
      spiTransferData(value[i]);   //2nd digit
      spiTransferData(0x00);
      spiTransferData(value[i]);   //1st digit
      spiTransferData(0x00);
      digitalWrite(LED_STR,HIGH);    
#endif      
      delay(1000);
    }
  }
}
