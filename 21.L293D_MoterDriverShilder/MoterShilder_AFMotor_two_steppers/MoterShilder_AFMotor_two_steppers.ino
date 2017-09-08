// MultiStepper
#include <AccelStepper.h>
#include <AFMotor.h>

// two stepper motors one on each port
AF_Stepper motor1(200, 1);
AF_Stepper motor2(200, 2);

// you can change these to DOUBLE or INTERLEAVE or MICROSTEP!
// wrappers for the first motor!
void forwardstep1() {  
  motor1.onestep(FORWARD, SINGLE);
//  motor1.onestep(FORWARD, DOUBLE);
}
void backwardstep1() {  
  motor1.onestep(BACKWARD, SINGLE);
//  motor1.onestep(BACKWARD, DOUBLE);
}
// wrappers for the second motor!
void forwardstep2() {  
  motor2.onestep(FORWARD, SINGLE);
}
void backwardstep2() {  
  motor2.onestep(BACKWARD, SINGLE);
}

// Motor shield has two motor ports, now we'll wrap them in an AccelStepper object
AccelStepper stepper1(forwardstep1, backwardstep1);
AccelStepper stepper2(forwardstep2, backwardstep2);

void setup()
{  
    Serial.begin(115200);           // set up Serial library at 115200 bps
    Serial.println("Two stepper test!");

    stepper1.setMaxSpeed(200.0);
    stepper1.setAcceleration(100.0);
    stepper1.moveTo(2000);
    
    stepper2.setMaxSpeed(300.0);
    stepper2.setAcceleration(100.0);
    stepper2.moveTo(3000);
    
}

void loop()
{
    // Change direction at the limits
    if (stepper1.distanceToGo() == 0)
    {
      Serial.println("[stepper1] Change direction");
  	  stepper1.moveTo(-stepper1.currentPosition());
    }
    
    if (stepper2.distanceToGo() == 0)
    {
      Serial.println("[stepper2] Change direction");
      stepper2.moveTo(-stepper2.currentPosition());
    }
     
    stepper1.run();
    stepper2.run();
}
