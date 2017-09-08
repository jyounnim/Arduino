void setup()
{
  Serial.begin(115200);
}

void loop()
{
  for(int i=0;i<=360;i++)
   {
    Serial.println(sin(i*(PI/180)));
   }
}

