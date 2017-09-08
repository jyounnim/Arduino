#define LEVEL0 0    // 0%
#define LEVEL1 2    // 2%
#define LEVEL2 20   // 20%
#define LEVEL3 100  // 100%

#define _USE_LEDS_  0

#if (_USE_LEDS_)
#define RED_LED     12
#define BLUE_LED    11
#define YELLOW_LED  10
#endif

int s0=3, s1=4, s2=5, s3=6;
int out=2;
int flag=0;
unsigned int counter=0;
unsigned int countR=0, countG=0, countB=0;

void setup()
{
  Serial.begin(115200);
  pinMode(s0,OUTPUT);
  pinMode(s1,OUTPUT); 
  pinMode(s2,OUTPUT);
  pinMode(s3,OUTPUT);

#if (_USE_LEDS_)
  // initialize LED pins as an output.
  pinMode(RED_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);  
#endif
}

void ScanColor(char freq)
{
  flag = 0;  
  
  if(freq == LEVEL0)
  {
    digitalWrite(s0,LOW);
    digitalWrite(s1,LOW);    
  }
  else if(freq == LEVEL1)
  {
    digitalWrite(s0,LOW);
    digitalWrite(s1,HIGH);    
  }
  else if(freq == LEVEL3)
  {
    digitalWrite(s0,HIGH);
    digitalWrite(s1,HIGH);    
  }
  else // Default is LEVEL2 (20)
  {
    digitalWrite(s0,HIGH);
    digitalWrite(s1,LOW);    
  }
  digitalWrite(s2,HIGH);
  digitalWrite(s3,LOW);

  attachInterrupt(0, ISR_INTO, RISING);

  timer0_start();
  delay(100);
}

void ColorSensorStop()
{
  digitalWrite(s2,HIGH);
  digitalWrite(s3,LOW);
    
  detachInterrupt(0);
  timer0_stop();
}

void ISR_INTO()
{ 
  counter++;
}
 
void timer0_start(void)
{
  TCCR2A=0x00;
  TCCR2B=0x07;   //the clock frequency source 1024 points  
  TCNT2= 100;    //10 ms overflow again
  TIMSK2 = 0x01; //enable interrupt  
}

void timer0_stop(void)
{
  TCNT2= 0xFF;
  TIMSK2 = 0x00; //disable interrupt
}

ISR(TIMER2_OVF_vect)//the timer 2, 10ms interrupt overflow again. Internal overflow interrupt executive function
{
  flag++;
  
  if(flag==1)
  {
    digitalWrite(s2,LOW);
    digitalWrite(s3,LOW); 
  }
  else if(flag==2)
  {
    countR=counter;
    digitalWrite(s2,HIGH);
    digitalWrite(s3,HIGH);
  }
  else if(flag==3)
  {
    countG=counter;
    digitalWrite(s2,LOW);
    digitalWrite(s3,HIGH);
  }
  else if(flag==4)
  {
    countB=counter*1.25;
    ColorSensorStop();  
  }
  else
  {
    ColorSensorStop();
  }
  
  counter=0;
  delay(2);
}

void loop()
{
  ScanColor(LEVEL2); 
  
  Serial.print("red=");
  Serial.print(countR,DEC);    
  Serial.print("   green=");
  Serial.print(countG,DEC);    
  Serial.print("   blue=");
  Serial.print(countB,DEC);
 
  if((countR>countG)&&(countR>countB))
  {
    Serial.println("   (Red)");
#if (_USE_LEDS_)   
    digitalWrite(RED_LED, HIGH);   // turn the LED on (HIGH is the voltage level)
    digitalWrite(BLUE_LED, LOW);
    digitalWrite(YELLOW_LED, LOW);
#endif    
  }
  else if((countG>=countR)&&(countG>countB))
  {
    Serial.println("   (Green)");
#if (_USE_LEDS_)   
    digitalWrite(RED_LED, LOW);   // turn the LED on (HIGH is the voltage level)
    digitalWrite(BLUE_LED, LOW);
    digitalWrite(YELLOW_LED, HIGH);
#endif    
  }
  else if((countB>countG)&&(countB>countR))
  {
    Serial.println("   (Blue)");
#if (_USE_LEDS_)    
    digitalWrite(RED_LED, LOW);   // turn the LED on (HIGH is the voltage level)
    digitalWrite(BLUE_LED, HIGH);
    digitalWrite(YELLOW_LED, LOW);
#endif    
  }
  else
  {
    Serial.println("   (None)");
#if (_USE_LEDS_)  
    digitalWrite(RED_LED, LOW);
    digitalWrite(BLUE_LED, LOW);
    digitalWrite(YELLOW_LED, LOW);    
#endif    
  }
    
  delay(100);
}
