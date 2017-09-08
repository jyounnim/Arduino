// Pulse Monitor Test Script
int ledPin = 13;
int sensorPin = 0;
double alpha = 0.75;
int period = 20;
double change = 0.0;

void setup ()
{
  pinMode (ledPin, OUTPUT);
  Serial.begin (115200);
}
void loop ()
{
  digitalWrite(13, HIGH);
  static double oldValue = 0;
  static double oldChange = 0;
  int rawValue = analogRead (sensorPin);
  double value = alpha * oldValue + (1 - alpha) * rawValue;

//  Serial.print ((((value*value)-100000)-942000)/10);
//  Serial.println ((((value*value)-100000)-942000)/10);

  Serial.print ((((value*value)-100000))/10);
  Serial.println ((((value*value)-100000))/10);


  oldValue = value;

  delay (period);
}
