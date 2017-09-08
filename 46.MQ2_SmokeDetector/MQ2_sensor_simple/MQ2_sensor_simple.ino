#define SENSOR      A0
#define TIME_DELAY  500

void setup ()
{
  Serial.begin(115200);
  Serial.println("----- MQ-2 Smoke and Gas sensor test -----" );
}

void loop ()
{
    Serial.print("rawData = ");
    Serial.println(analogRead(SENSOR));

    delay(TIME_DELAY);
}
