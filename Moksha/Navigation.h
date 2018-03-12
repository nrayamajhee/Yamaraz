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
void go(Direction dir, int amount, bool correct){
  Serial.println(correct);
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
  motors.running = true;

  int spokes = 0;

  while(motors.running == true) {
    if(correct) {
      motors.alignRatio = 1 + (calculate_average() - 4.5) * .1;        //1 is calibrated value; Dr Gray says either use .25 or .35 for better control
      if(detect_spoke()){
        spokes++;
      }
      if(spokes > 2) {
        motors.running = false;
        return;
      }
     }
  }
}

void returnHome(int direction, int amount) {
  Serial.println("Reached returnHome");
  go(RIGHT, 180, false);  //Turn around
  delay(500);
  go(FRONT, steps, false);
  delay(500);
  go(RIGHT, 180 - (direction * 45), false);
}

void goTo(int direction, int steps) {
  go(RIGHT, direction * 45, false);
  delay(500);
  go(FRONT, 5, false);
  delay(500);
  go(FRONT, 48, true);
  delay(250);
  
//  go(FRONT, steps, true);
//  delay(250);
//  //pickUp();
//  delay(250);
  returnHome(direction, 48);
}

