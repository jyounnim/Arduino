unsigned long time;
unsigned long time0=0, time1=0, t1_t0;
int OptPin = 2;

void setup() {
  Serial.begin(9600);
  pinMode(OptPin, INPUT);    
  attachInterrupt(0, getRPM, RISING);  
}
 
void loop() {
  int rpm;
  
  t1_t0 = time1-time0;
  time0 = time1;

  if( t1_t0 == 0 )
    rpm = 0;
  else
    rpm = 1000/t1_t0;
  
  Serial.print("time0 = ");
  Serial.print(time0);
  Serial.print("   time1 = ");
  Serial.println(time1);
  
  Serial.print("t1-t0 = ");
  Serial.print(t1_t0);

  Serial.print("   rpm = ");
  Serial.println(rpm);
  Serial.println();

  // wait a second so as not to send massive amounts of data
  delay(1000);
}

void getRPM()
{
  time1 = millis();
}
