/*
 * This is the bleeding edge version of our robot navigation code.
 * So naturally a lot of it is not tested and coded entirely away
 * from the physical FuryRoad (officially called Yamaraz).
 * 
 * This file is free to copy, modify, and redistribute under the
 * terms specified in the LICENSE file (its MIT).
 * 
 * Use at your own discretion.
 * Don't come to me if your robot is on fire!
 * 
 * Sincerely,
 * Nishan Rayamajhee
 */

// direction flags
const int FRONT    = 0;
const int REAR     = 1;
const int LEFT     = 2;
const int RIGHT    = 3;
const int SLEFT    = 4;
const int SRIGHT   = 5;

// teensy pinouts
const int SFR = 0;
const int DFR = 1;
const int SFL = 2;
const int DFL = 3;
const int DRL = 4;
const int SRL = 5;
const int DRR = 6;
const int SRR = 7;

/* 
 *  ==============
 * --------------
 * Initialization
 * --------------
 * ==============
 */
 
void initPins() {
  
  // set all the motor pins to OUTPUT
  pinMode(0, OUTPUT);
  pinMode(1, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);

  // Set MicroStepping Pins to OUTPUT
  pinMode(21, OUTPUT);
  pinMode(22, OUTPUT);
  pinMode(23, OUTPUT);

  // micro stepping 1/4th is 010
  digitalWrite(21, LOW);
  digitalWrite(22, HIGH);
  digitalWrite(23, LOW);

}

/* 
 * ===========
 * -----------
 * MOTION CODE
 * -----------
 * ===========
 */

/***********************************
 * Sets the direction for all motors
 * 
 * Parameters: DIRECTION flags
 * 
 * Example:    setDirection(FRONT);
 ***********************************/

void setDirection (const int dir) {
  
  if (dir == FRONT) {
    
    // forward direction 
    digitalWrite(DFL, HIGH);
    digitalWrite(DFR, LOW);
    digitalWrite(DRL, HIGH);
    digitalWrite(DRR, LOW);
      
  } else if (dir == REAR) {
    
    // rear direction 
    digitalWrite(DFL, LOW);
    digitalWrite(DFR, HIGH);
    digitalWrite(DRL, LOW);
    digitalWrite(DRR, HIGH);
  
  } else if (dir == LEFT) {
    
    // left direction for rotation
    digitalWrite(DFL, LOW);
    digitalWrite(DFR, LOW);
    digitalWrite(DRL, LOW);
    digitalWrite(DRR, LOW);
  
  } else if (dir == RIGHT) {
    
    // right direction for rotation
    digitalWrite(DFL, HIGH);
    digitalWrite(DFR, HIGH);
    digitalWrite(DRL, HIGH);
    digitalWrite(DRR, HIGH);
  
  } else if (dir == SLEFT) {
    
    // left direction for strafing
    digitalWrite(DFL, LOW);
    digitalWrite(DFR, LOW);
    digitalWrite(DRL, HIGH);
    digitalWrite(DRR, HIGH);
  
  } else if (dir == SRIGHT) {
    
    // right direction for strafing
    digitalWrite(DFL, HIGH);
    digitalWrite(DFR, HIGH);
    digitalWrite(DRL, LOW);
    digitalWrite(DRR, LOW);
  }
}

/***********************
 * Drives all the motors
 * 
 * Paramaters: DIRECTION flags,
 *             VALUE in inches or degrees
 *             
 * Example:    go (FRONT, 12);  // 12 inches
 *             go (RIGHT, 90);  // rotate 90 degrees
 *             go (SFLEFT, 12); // strafe left 12 inches
 ***********************/

void go (const int dir, int val) {
  
  // reset the port
  PORTD = B00000000;

  // set the direction
  setDirection (dir);

  // steps will be calculated later
  int steps = 0;

  // linear motion
  
  if (dir == FRONT || dir == REAR) {

    // val is in inches
    // 1 inch requires 107.8 steps
    steps = 107.8 * val;

    // start moving
    for (int i = 0; i < steps; i++) {

      digitalWrite(SFL, HIGH);
      digitalWrite(SFR, HIGH); 
      digitalWrite(SRL, HIGH);
      digitalWrite(SRR, HIGH);
      
      delayMicroseconds(500);
      
      digitalWrite(SFL, LOW);
      digitalWrite(SFR, LOW);
      digitalWrite(SRL, LOW);
      digitalWrite(SRR, LOW);
  
      delayMicroseconds(500);
    }

  // rotational motion
  
  } else if (dir == LEFT || dir == RIGHT) {

    // val is in degrees
    // 1 deg requires 11.32 steps
    steps = 11.32 * val;

    // start moving
    for (int i = 0; i < steps; i++) {

      digitalWrite(SFL, HIGH);
      digitalWrite(SFR, HIGH); 
      digitalWrite(SRL, HIGH);
      digitalWrite(SRR, HIGH);
      
      delayMicroseconds(500);
      
      digitalWrite(SFL, LOW);
      digitalWrite(SFR, LOW);
      digitalWrite(SRL, LOW);
      digitalWrite(SRR, LOW);
  
      delayMicroseconds(500);
    }

  // strafing motion
  
  } else if (dir == SLEFT || dir == SRIGHT) {

    // val is in inches
    // 1 inch requires 113 steps
    steps = 113 * val * 2;

    // start moving
    for (int i = 0; i < steps; i++) {
      
      digitalWrite(SFL, HIGH);
      digitalWrite(SFR, HIGH); 
      digitalWrite(SRL, HIGH);
      digitalWrite(SRR, HIGH);
      
      delayMicroseconds(500);
      
      digitalWrite(SFL, LOW);
      digitalWrite(SFR, LOW);
      digitalWrite(SRL, LOW);
      digitalWrite(SRR, LOW);
  
      delayMicroseconds(500);
    }
  }
}

/**************************************
 * Drives two motors for diagonal magic
 * 
 * Paramaters: DIRECTION flag (forward/reverse),
 *             DIRECTION flag (left/right),
 *             VALUE in inches
 *
 * Example:    goDiag (FRONT, RIGHT, 16); // 16 inches to NorthEast             
 **************************************/

void goDiag (const int dir1, const int dir2, int val) {
  
  // reset the port
  PORTD = B00000000;

  // set the direction
  setDirection (dir1);
  
  // val is in inches
  // 1 inch requires 158 steps
  int steps = 158 * val * 2;

  if (dir2 == LEFT) {
    
    // start moving left
    for (int i = 0; i < steps; i++) {
    
      digitalWrite(SFR, HIGH);
      digitalWrite(SRL, HIGH);
      
      delayMicroseconds(300);
      
      digitalWrite(SFR, LOW);
      digitalWrite(SRL, LOW);
  
      delayMicroseconds(300);
    }
    
  } else if (dir2 == RIGHT) {
    
    // start moving right
    for (int i = 0; i < steps; i++) {

      digitalWrite(SFL, HIGH);
      digitalWrite(SRR, HIGH);
      
      delayMicroseconds(300);
      
      digitalWrite(SFL, LOW);
      digitalWrite(SRR, LOW);
  
      delayMicroseconds(300);
    }
  }
}
