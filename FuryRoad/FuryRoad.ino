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
const int SLEFT  = 4;
const int SRIGHT = 5;

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
 * Yamaraz shall rule the Fury Road
 */

void setup() {
  initPins();
  go (FRONT, 36);
  go (RIGHT, 90);
  go (FRONT, 12);
  go (RIGHT, 90);
  go (FRONT, 12);
  go (SRIGHT, 12);
  goDiag (FRONT, LEFT,16);
  go (SRIGHT, 12);
  go (LEFT, 90);
  go (FRONT, 12);
}

/*
 * Yamaraz shall test its dangers
 */

void loop() {
  // make sure to uncomment code at setup
  // before testing
}

/* 
 *  ==============
 * --------------
 * Initialization
 * --------------
 * ==============
 */
 
void initPins() {
  
  // set all the motor pins to OUTPUT
  DDRD = 11111111;

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
 *             go (RIGHT, 90);  // 90 degrees
 *             go (SFLEFT, 12); // strafe left 12 inches
 ***********************/

void go (const int dir, int val) {
  
  // reset the port
  PORTD = 00000000;

  // set the direction
  setDirection (dir);

  // steps will be calculated later
  int steps = 0;

  // linear motion
  
  if (dir == FRONT || dir == REAR) {

    // val is in inches
    // 1 inch requires 107.8 steps x 2 because we're toggling the ports
    steps = 107.8 * val * 2;

    // start moving
    for (int i = 0; i < steps; i++) {

      // toggle port
      PORTD ^= 10100101;
      
      // control the speed
      delayMicroseconds(500);
    }

  // rotational motion
  
  } else if (dir == LEFT || dir == RIGHT) {

    // val is in degrees
    // 1 deg requires 11.32 steps x 2 because we're toggling the ports
    steps = 11.32 * val * 2;

    // start moving
    for (int i = 0; i < steps; i++) {
      
      // toggle port
      PORTD ^= 10100101;
      
      // control the speed
      delayMicroseconds(500);
    }

  // strafing motion
  
  } else if (dir == SLEFT || dir == SRIGHT) {

    // val is in inches
    // 1 inch requires 113 steps x 2 because we're toggling the ports
    steps = 113 * val * 2;

    // start moving
    for (int i = 0; i < steps; i++) {
      
      // toggle port
      PORTD ^= 10100101;
      
      // control the speed
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
  PORTD = 00000000;

  // set the direction
  setDirection (dir);
  
  // val is in inches
  // 1 inch requires 158 steps x 2 because we're toggling the ports
  int steps = 158 * val * 2;

  if (dir2 == LEFT) {
    
    // start moving left
    for (int i = 0; i < steps; i++) {
    
      // toggle only 2 stepping pins
      // SFR and SRL
      PORTD ^= 10000100;
      
      // control the speed
      delayMicroseconds(500);
    }
    
  } else if (dir2 == RIGHT) {
    
    // start moving right
    for (int i = 0; i < steps; i++) {
    
      // toggle only 2 stepping pins
      // SFL and SRR
      PORTD ^= 00100001;
      
      // control the speed
      delayMicroseconds(500);
    }
  }
}
