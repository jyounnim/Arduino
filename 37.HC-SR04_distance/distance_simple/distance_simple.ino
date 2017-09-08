 
int echo = 3; 
int trig = 2;

unsigned long duration;
float distance;

void setup() { 
  //Initialize serial and wait for port to open:
   Serial.begin(9600); 
   pinMode(echo, INPUT);
   pinMode(trig, OUTPUT);
   while (!Serial) {
   }
   
   // prints title with ending line break 
   Serial.println("------- Start -------"); 
} 

void loop() { 
  char i;

  duration = 0;
  for( i=0 ; i<10 ; i++)
  {
    // Trigger Signal output
    digitalWrite(trig, HIGH);
    delay(10);
    digitalWrite(trig, LOW);
    
    // Echo Signal Input
    duration += pulseIn(echo, HIGH);
    delay(10);
  }
  duration = duration/10;
  
  // Calcurate distance
  distance = ((float)(340*duration)/10000) / 2;
  Serial.print(distance);
  Serial.println("cm");
  delay(100);  
}
 
