/*
 * This is the bleeding edge version of our robot navigation code.
 * So naturally a lot of it is not tested and sometimes coded entirely away
 * from our robot, Yamaraz.
 * 
 * This file is free to copy, modify, and redistribute under the
 * terms specified in the COPYING file (its MIT).
 * 
 * Use at your own discretion.
 * Don't come to me if your robot is on fire!
 * 
 * Sincerely,
 * Nishan Rayamajhee
 */

// grid property
const int ROWS = 5;
const int COLS = 5;

// the flags representing solid, wired and hollow state.
const int SOLID   = 0; // represented by green
const int WIRED   = 1;                // red
const int HOLLOW  = 2;               // blue


// direction flags
const int FRONT    = 0;
const int REAR     = 1;
const int LEFT     = 2;
const int RIGHT    = 3;
const int SLEFT    = 4;
const int SRIGHT   = 5;

/* teensy pinouts LEFT */

// GND
const int tickButton = 0;
const int thumper    = 1;
const int LEDpin     = 2;

const int echoFL = 3;
const int trigFL = 4;
const int echoFR = 5;
const int trigFR = 6;
const int echoRL = 7;
const int trigRL = 8;
const int echoRR = 9;
const int trigRR = 10;

const int SRR = 11;
const int DRR = 12;
// tic tracer 3.3 volts
const int SRL = 24;
const int DRL = 25;
const int DFL = 26;
const int SFL = 27;
const int DFR = 28;
const int SFR = 29;
const int MS0 = 30;
const int MS1 = 31;
const int MS2 = 32;

/* teensy pinouts RIGHT */
// VIN
// Microphone GND
// Microphone 3.3V
const int mic     = 23;
const int tickPin = 22;
const int echoFFL = 21;
const int trigFFL = 20;
const int echoFFR = 19;
const int trigFFR = 18;
const int echoRRL = 17;
const int trigRRL = 16;
const int echoRRR = 15;
const int trigRRR = 14;

// These are the calibartion value for motors
int linear     = 870;     // per inch
int lateral    = 454 * 2; // per inch
int diagonal   = 646 * 2; // per inch
int rotational = 23 * 2;  // per degree

// speed and acceleartion
const int TOP_SPEED = 60;
int spd = TOP_SPEED * 9;
const int ACCEL_RANGE = TOP_SPEED * 8;

/* 
 * ==============
 * --------------
 * Initialization
 * --------------
 * ==============
 */
 
void initPins() {
  
  // set all the motor pins to OUTPUT
  pinMode(SFR, OUTPUT);
  pinMode(DFR, OUTPUT);
  pinMode(SFL, OUTPUT);
  pinMode(DFL, OUTPUT);
  pinMode(DRL, OUTPUT);
  pinMode(SRL, OUTPUT);
  pinMode(DRR, OUTPUT);
  pinMode(SRR, OUTPUT);

  // Set MicroStepping Pins to OUTPUT
  pinMode(MS0, OUTPUT);
  pinMode(MS1, OUTPUT);
  pinMode(MS2, OUTPUT);
  
  // micro stepping 1/16th is 010
  digitalWrite(MS0, HIGH);
  digitalWrite(MS1, LOW);
  digitalWrite(MS2, HIGH);

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

void go (const int dir, float val) {
  // reset the speed
  spd = TOP_SPEED * 9;

  // set the direction
  setDirection (dir);

  // steps will be calculated later
  int steps = 0;

  // linear motion
  // -------------
  if (dir == FRONT || dir == REAR) {

    // val is in inches
    // 1 inch requires 435 steps
    steps = (float)linear * val;

    // start moving
    for (int i = 0; i < steps; i++) {
      digitalWrite(SFL, HIGH);
      digitalWrite(SFR, HIGH); 
      digitalWrite(SRL, HIGH);
      digitalWrite(SRR, HIGH);

      delayMicroseconds(spd);
      
      digitalWrite(SFL, LOW);
      digitalWrite(SFR, LOW);
      digitalWrite(SRL, LOW);
      digitalWrite(SRR, LOW);
  
      delayMicroseconds(spd);

      // acceleartion
      if (i < (ACCEL_RANGE)) {
        spd--;
      } else if (i > (steps - (ACCEL_RANGE))){
        spd++;
      }
    }

  // rotational motion
  // -----------------
  } else if (dir == LEFT || dir == RIGHT) {

    // val is in degrees
    // 1 deg requires 23 steps
    steps = (float)rotational * val;

    // start moving
    for (int i = 0; i < steps; i++) {

      digitalWrite(SFL, HIGH);
      digitalWrite(SFR, HIGH); 
      digitalWrite(SRL, HIGH);
      digitalWrite(SRR, HIGH);
      
      delayMicroseconds(spd);
      
      digitalWrite(SFL, LOW);
      digitalWrite(SFR, LOW);
      digitalWrite(SRL, LOW);
      digitalWrite(SRR, LOW);
  
      delayMicroseconds(spd);

      // acceleartion
      if (i < (ACCEL_RANGE)) {
        spd--;
      } else if (i > (steps - (ACCEL_RANGE))){
        spd++;
      }
    }

  // strafing motion
  // ---------------
  } else if (dir == SLEFT || dir == SRIGHT) {

    // val is in inches
    // 1 inch requires 454 steps
    steps = (float)lateral * val;

    // start moving
    for (int i = 0; i < steps; i++) {
      
      digitalWrite(SFL, HIGH);
      digitalWrite(SFR, HIGH); 
      digitalWrite(SRL, HIGH);
      digitalWrite(SRR, HIGH);
      
      delayMicroseconds(spd);
      
      digitalWrite(SFL, LOW);
      digitalWrite(SFR, LOW);
      digitalWrite(SRL, LOW);
      digitalWrite(SRR, LOW);
  
      delayMicroseconds(spd);

      // acceleartion
      if (i < (ACCEL_RANGE)) {
        spd--;
      } else if (i > (steps - (ACCEL_RANGE))){
        spd++;
      }
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

void goDiag (const int dir1, const int dir2, float val) {
  // reset the speed
  spd = TOP_SPEED * 9;
  
  // set the direction
  setDirection (dir1);
  
  // val is in inches
  // 1 inch requires 646 steps
  int steps = (float)diagonal * val;

  if (dir2 == LEFT) {
    
    // start moving left
    for (int i = 0; i < steps; i++) {
    
      digitalWrite(SFR, HIGH);
      digitalWrite(SRL, HIGH);
      
      delayMicroseconds(spd);
      
      digitalWrite(SFR, LOW);
      digitalWrite(SRL, LOW);
  
      delayMicroseconds(spd);

      // acceleartion
      if (i < (ACCEL_RANGE)) {
        spd--;
      } else if (i > (steps - (ACCEL_RANGE))){
        spd++;
      }
    }
    
  } else if (dir2 == RIGHT) {
    
    // start moving right
    for (int i = 0; i < steps; i++) {

      digitalWrite(SFL, HIGH);
      digitalWrite(SRR, HIGH);
      
      delayMicroseconds(spd);
      
      digitalWrite(SFL, LOW);
      digitalWrite(SRR, LOW);
  
      delayMicroseconds(spd);

      // acceleartion
      if (i < (ACCEL_RANGE)) {
        spd--;
      } else if (i > (steps - (ACCEL_RANGE))){
        spd++;
      }
    }
  }
}

