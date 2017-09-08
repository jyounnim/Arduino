#define RED_LED 13
#define GREEN_LED 12
#define BUZZER 11
#define SENSOR A0
// Your threshold value
#define SENSOR_THRES 400
#define TIME_DELAY 500

void setup() {
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(SENSOR, INPUT);
  Serial.begin(115200);
}

void loop() {
  int analogSensor = analogRead(SENSOR);

  Serial.print("Pin A0: ");
  Serial.println(analogSensor);
  
  // Checks if it has reached the threshold value
  if (analogSensor > SENSOR_THRES)
  {
    digitalWrite(RED_LED, HIGH);
    digitalWrite(GREEN_LED, LOW);
    tone(BUZZER, 1000, 200);
  }
  else
  {
    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, HIGH);
    noTone(BUZZER);
  }
  delay(TIME_DELAY);
}
