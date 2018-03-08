/*
 * All the higher level navigation code that 
 * our algorith to run the course will be using
 * these features
 */
#include "Motion.h"
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

  while(motors.running == true) {
    IR_filter();
    if(correct) {
      float avg = 4.5;
      float sum = ir.filteredValues[0] + 2*ir.filteredValues[1] + 3*ir.filteredValues[2] + 4*ir.filteredValues[3]+5*ir.filteredValues[4] + 6*ir.filteredValues[5] + 7*ir.filteredValues[6] + 8*ir.filteredValues[7];

      float onCount = 0;
      
      for(int i = 0; i < 8; i++){
        if(ir.filteredValues[i] == 1){
          onCount++;
        }
      }

      if(onCount == 0){
        onCount = 4.5;
      }

      float newAverage = sum / onCount;

      if(onCount > 2){
        newAverage = 4.5;
      }
      motors.alignRatio = 1 + (newAverage - 4.5) * .1;        //1 is calibrated value; Dr Gray says either use .25 or .35 for better control
      //Serial.println(motors.alignRatio);
    }
  }
}

void returnHome(int direction, int steps) {
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
  go(FRONT, steps, true);
  delay(250);
  //pickUp();
  delay(250);
  returnHome(direction, steps);
  //Color colors[6] = {RED, GREEN, SKYBLUE, BLUE, RED, PURPLE};
  //p = p+1;
  //coinColor = colors[p];
}

