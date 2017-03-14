/*
 * This is the bleeding edge version of our robot navigation code.
 * So naturally a lot of it is not tested and coded entirely away
 * from the Yamaraz.
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
const int SFR = 1;
const int DFR = 2;
const int SFL = 3;
const int DFL = 4;
const int DRL = 5;
const int SRL = 6;
const int DRR = 7;
const int SRR = 8;

// ultrasonic pinouts
const int echoFL = 9;
const int trigFL = 10;
const int echoFR = 11;
const int trigFR = 12;
const int echoRL = 24;
const int trigRL = 25;
const int echoRR = 26;
const int trigRR = 27;

// max distance for ultrasonic
const int MAX_DISTANCE = 200;

const int SPEED = 80;

#include <NewPing.h>

// Setup pinouts for ultrasonic sensors
NewPing sonar[4] = {
  NewPing(trigFL, echoFL, MAX_DISTANCE),
  NewPing(trigFR, echoFR, MAX_DISTANCE),
  NewPing(trigRL, echoRL, MAX_DISTANCE),
  NewPing(trigRR, echoRR, MAX_DISTANCE)
};

/* 
 *  ==============
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
  pinMode(33, OUTPUT);
  pinMode(34, OUTPUT);
  pinMode(35, OUTPUT);

  // Thumper Pinout
  pinMode(23, OUTPUT);

  // micro stepping 1/16th is 010
  digitalWrite(33, HIGH);
  digitalWrite(34, LOW);
  digitalWrite(35, HIGH);

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
    steps = 107.8 * 8 * val;

    // start moving
    for (int i = 0; i < steps; i++) {

      digitalWrite(SFL, HIGH);
      digitalWrite(SFR, HIGH); 
      digitalWrite(SRL, HIGH);
      digitalWrite(SRR, HIGH);
      
      delayMicroseconds(SPEED);
      
      digitalWrite(SFL, LOW);
      digitalWrite(SFR, LOW);
      digitalWrite(SRL, LOW);
      digitalWrite(SRR, LOW);
  
      delayMicroseconds(SPEED);
    }

  // rotational motion
  
  } else if (dir == LEFT || dir == RIGHT) {

    // val is in degrees
    // 1 deg requires 11.32 steps
    steps = 11.32 * 8 * val;

    // start moving
    for (int i = 0; i < steps; i++) {

      digitalWrite(SFL, HIGH);
      digitalWrite(SFR, HIGH); 
      digitalWrite(SRL, HIGH);
      digitalWrite(SRR, HIGH);
      
      delayMicroseconds(SPEED);
      
      digitalWrite(SFL, LOW);
      digitalWrite(SFR, LOW);
      digitalWrite(SRL, LOW);
      digitalWrite(SRR, LOW);
  
      delayMicroseconds(SPEED);
    }

  // strafing motion
  
  } else if (dir == SLEFT || dir == SRIGHT) {

    // val is in inches
    // 1 inch requires 113 steps
    steps = 113 * 8 * val;

    // start moving
    for (int i = 0; i < steps; i++) {
      
      digitalWrite(SFL, HIGH);
      digitalWrite(SFR, HIGH); 
      digitalWrite(SRL, HIGH);
      digitalWrite(SRR, HIGH);
      
      delayMicroseconds(SPEED);
      
      digitalWrite(SFL, LOW);
      digitalWrite(SFR, LOW);
      digitalWrite(SRL, LOW);
      digitalWrite(SRR, LOW);
  
      delayMicroseconds(SPEED);
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
  int steps = 158 * 8 * val;

  if (dir2 == LEFT) {
    
    // start moving left
    for (int i = 0; i < steps; i++) {
    
      digitalWrite(SFR, HIGH);
      digitalWrite(SRL, HIGH);
      
      delayMicroseconds(SPEED);
      
      digitalWrite(SFR, LOW);
      digitalWrite(SRL, LOW);
  
      delayMicroseconds(SPEED);
    }
    
  } else if (dir2 == RIGHT) {
    
    // start moving right
    for (int i = 0; i < steps; i++) {

      digitalWrite(SFL, HIGH);
      digitalWrite(SRR, HIGH);
      
      delayMicroseconds(SPEED);
      
      digitalWrite(SFL, LOW);
      digitalWrite(SRR, LOW);
  
      delayMicroseconds(SPEED);
    }
  }
}

// Rotates the robot to parallel with the wall

//void alignRight() {
//  int a = sonar[1].ping_median(7);
//  int b = sonar[3].ping_median(7);
//  
//  double diff = (abs(a - b)); // No correction offset for right sensors
//  double hypo = 624.75;
//  double errorAngleRdegree = (180 / 3.14) * asin(diff / hypo);
//
//  if (a > b) go( RIGHT, (int)errorAngleRdegree);
//  if (a < b) go( LEFT, (int)errorAngleRdegree);
//}
//
//void alignLeft() {
//  int a = sonar[0].ping_median(7);
//  int b = sonar[2].ping_median(7);
//  
//  double diff = (abs(a - b)); // No correction offset for right sensors
//  double hypo = 624.75;
//  double errorAngleRdegree = (180 / 3.14) * asin(diff / hypo);
//
//  if (a > b) go( RIGHT, (int)errorAngleRdegree);
//  if (a < b) go( LEFT, (int)errorAngleRdegree);
//  
//  a = sonar[0].ping_median(7);
//  b = sonar[2].ping_median(7);
//}
