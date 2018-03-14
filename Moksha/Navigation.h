/*
 * All the higher level navigation code that 
 * our algorith to run the course will be using
 * these features
 */
#include "Motors.h"
#include "IR.h"
#include "Servo.h"
enum Compass {
  NORTH,  // white
  NEAST,  // red
  EAST,   // green
  SEAST,  // blue
  SOUTH,  // white
  SWEST,  // yellow
  WEST,   // purple
  NWEST   // skyblue
};

/*
 * Turns the IRS flags on and sets the
 * number of steps to run
 * 
 * the amount will be in inches for
 * front, back, sleft and sright,
 * and degrees fori
 * left and right
 */
void go(Direction dir, float amount, bool correct, bool constantSpeed){
  setDirection(dir);
  if (dir == FRONT || dir == BACK){
    //motors.totalSteps = amount; // linear calibration
    motors.totalSteps = amount * 216; // linear calibration
  } else if(dir == SLEFT || dir == SRIGHT){
    motors.totalSteps = amount * 800; // strafing calibration
    
  } else {
    motors.totalSteps = (int)ceil(amount * 24.2); // angular calibration
  }
  
  if(debug.motion){
    Serial.print("Going ");
    Serial.print(dir);
    Serial.print(" by ");
    Serial.print(amount);
    Serial.print(" for ");
    Serial.println(motors.totalSteps);
  }

  // set slow speed and start the motors
  motors.speed = motors.minSpeed;
  if(constantSpeed) {
    motors.speed = 5000;
    motors.accelerate = false;
  }
  motors.running = true;

  while(motors.running == true) {
    
    if(correct) {
      motors.alignRatio = 1 + (IR_Average() - 4.5) * .1;        //1 is calibrated value; Dr Gray says either use .25 or .35 for better control
      //Serial.println(motors.alignRatio);
    }
  }
}

void fixRobotLine(){
  delay(250);
  float average = IR_Average();
  Serial.print("Average: ");
  Serial.println(average);
  if(average > 4.5){
    Serial.println(">0");
    while(average > 4.5){
      go(SRIGHT, 0.1, false, true);
      average = IR_Average();
    }
  }else if(average < 4.5){
    Serial.println("<0");
    while(average < 4.5){
      go(SLEFT, 0.1, false, true);
      average = IR_Average();
    }
  }
}

void returnHome(int direction, int steps) {
  go(RIGHT, 180, false, false);  //Turn around
  delay(500);
  go(FRONT, steps, false, false);
  delay(500);
  go(RIGHT, 180 - (direction * 45), false, false);
}

void goTo(int direction, int steps) {
  go(RIGHT, direction * 45, false, false);
  delay(500);
  go(FRONT, steps, true, false);
  delay(250);
  //pickUp();
  delay(250);
  returnHome(direction, steps);
}



