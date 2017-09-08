#define IN1  8
#define IN2  9
#define IN3  10
#define IN4  11

#define STEPS_PER_ROTATION 4096

void setup()
{
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  delay(100);
}

void loop()
{
  rotate(1.5);
  delay(2000);
  rotate(-1.5);
  delay(2000);
}

void rotate(float rotations) {
  rotate_steps(rotations * STEPS_PER_ROTATION);
}

int phase = 0;
byte phases[] = { 1, 3, 2, 6, 4, 12, 8, 9 };

void rotate_steps(int steps)
{
  int dir = (steps > 0) - (steps < 0);
  steps *= dir;
  long laststep;
  for (int i = 0; i < steps;) {
    long now = micros();
    if (now - laststep < 1000) continue;
    laststep = now;
    stepper_writepins(phases[phase]);
    phase = (8 + phase + dir) % 8;
    i++;
  }
  stepper_writepins(0);
}

void stepper_writepins(int bitmap) {
    digitalWrite(IN1, bitmap & 8 ? HIGH : LOW);
    digitalWrite(IN2, bitmap & 4 ? HIGH : LOW);
    digitalWrite(IN3, bitmap & 2 ? HIGH : LOW);
    digitalWrite(IN4, bitmap & 1 ? HIGH : LOW);
}


