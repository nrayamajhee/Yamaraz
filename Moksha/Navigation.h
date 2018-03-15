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
void go(Direction dir, float amount, bool correct, bool constantSpeed) {
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
  if(constantSpeed) {
    motors.speed = 5000;
    motors.accelerate = false;
  } else {
    motors.speed = motors.minSpeed;
    motors.accelerate = true;
  }
  motors.running = true;

  while(motors.running == true) {

    if(correct) {
      motors.alignRatio = 1 + (calculate_average() - 4.5) * .1;        //1 is calibrated value; Dr Gray says either use .25 or .35 for better control
      //Serial.println(motors.alignRatio);
    }
  }
}

void goUntilSpokes(Direction dir, float amount, bool correct, bool constantSpeed) {
  setDirection(dir);
  if (dir == FRONT || dir == BACK){
    //motors.totalSteps = amount; // linear calibration
    motors.totalSteps = amount * 216; // linear calibration
  } else if(dir == SLEFT || dir == SRIGHT){
    motors.totalSteps = amount * 800; // strafing calibration
  } else {
    motors.totalSteps = (int)ceil(amount * 24.2); // angular calibration
  }

  // set slow speed and start the motors
  if(constantSpeed) {
    motors.speed = 5000;
    motors.accelerate = false;
  } else {
    motors.speed = motors.minSpeed;
    motors.accelerate = true;
  }
  motors.running = true;

  while(motors.running == true) {

    if(correct) {
      
    int spokes = 0;
    if(detect_spoke()) {
      motors.running = false;
      spokes++;
    }
      motors.alignRatio = 1 + (calculate_average() - 4.5) * .1;        //1 is calibrated value; Dr Gray says either use .25 or .35 for better control
      //Serial.println(motors.alignRatio);
    }
  }
}

void fixRobotLine(){
  delay(250);
  float average = calculate_average();
  Serial.println(average);
  if(average > 4.5){
    while(average > 4.5){
      go(SRIGHT, 0.1, false, true);
      average = calculate_average();
    }
  }else if(average <= 4.5){
    while(average <= 4.5){
      go(SLEFT, 0.1, false, true);
      average = calculate_average();
    }
  }
}

void goTo(int direction, int steps) {
  go(RIGHT, direction * 45, false, false);
  delay(500);
  go(FRONT, 24, true, true);
  go(FRONT, 180, false, false);
  fixRobotLine();
  goUntilSpokes(FRONT, 24, true, true);
  delay(500);
  pickUp();
  goUntilSpokes(FRONT, 24, true, true);
  delay(250);
  go(FRONT, 15, false, false);
  delay(500);
  go(LEFT, 180 - (direction * 45), false, false);
}
