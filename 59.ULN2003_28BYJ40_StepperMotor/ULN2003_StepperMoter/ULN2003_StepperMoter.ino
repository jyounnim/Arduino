#include <Stepper.h>

// 2048:한바퀴(360도), 1024:반바퀴(180도)...
const int stepsPerRevolution = 2048; 
// 모터 드라이브에 연결된 핀 IN1, IN3, IN2, IN4
Stepper myStepper(stepsPerRevolution, 8, 10, 9, 11);           
void setup() {
  myStepper.setSpeed(10); 
}
void loop() {
  // 시계 반대 방향으로 한바퀴 회전
  myStepper.step(stepsPerRevolution);
  delay(500);

  // 시계 방향으로 한바퀴 회전
  myStepper.step(-stepsPerRevolution);
  delay(500);
}

