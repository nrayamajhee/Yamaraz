/*
 * This is the bleeding edge version of our robot navigation code.
 * So naturally a lot of it is not tested and sometimes coded entirely away
 * from the robot.
 *
 * Feel free to use, modify, and redistribute according to the following terms!
 *
 * ==========================================
 *
 * Copyright (c) 2018, Nishan Rayamajhee
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include "Definitions.h"
extern Motors motors;
Debug debug   = {
  0,
  0, // IR
  0,
  0,  //Light
  0
};
Motors motors = {
  false, 
  0, 
  0,
  800,  // to misec delay
  2000, // from misec delay
  0,
  1,     // turn ratio
  1,    // accelerate
};
Servo servo = {
  false,
  0,
  0
};
IR ir = {
  0,
  0
};
bool spokes[6][5];
void initialize() {
  DDRL = 0xFF;          // set port L to output
  pinMode(SERVO_PIN, OUTPUT);  // servo output
  pinMode(MAGNET_PIN, OUTPUT);  // magenet output
  pinMode(LIGHT_PIN, OUTPUT);  // light
  digitalWrite(30, LOW);
  delay(500);           // delay for the IR sensors
  Serial.begin(9600);
  runServo(UP);
  initTimers();         // start the timers
  randomSeed(analogRead(0));
  setTimers(ALL, motors.minSpeed);
  for (int i =0; i < 6; i++) {
    for (int j =0; j < 5; j++) {
      spokes[i][j] = false;
    }
  }
//  calibrate_IR();
}
void beginCourse() {
  goAccel(BACK, 43, false);
  correctRight();
  for(int i = 5; i < 6; i++){
    if(i!=4){
      goTo(i);
      delay(500);
      break;
    }
  }
}
bool isDiag(int turn) {
  if(turn == 2 || turn == 6)
    return false;
   else
    return true;
}
void goToGray(int direction) {
  int spokes = 0;
  for(int i = 0; i < 4; i++) {
    goUntilSpokes(BACK, true, 1);
    if(isDiag(direction)) {
      goConst(BACK, 5, 2000, true);
      alignRobot();
      goConst(FRONT, 2, 2000, true);
    } else {
      goConst(BACK, 7, 2000, true);
      alignRobot();
      goConst(FRONT, 2, 2000, true);
    }
    spokes++;
    pickUp();
    Color found = calculate_color();
    if(found != INVALID) {
      goConst(FRONT, 3, 2000, true);
      returnTo(found, (4 - spokes), direction);
      break;
    }
    goConst(FRONT, 3, 2000, true);
  }
}
void returnTo(Color color, int steps, Direction direction) {
  goUntilSpokes(BACK, true, 1);
  if(isDiag(direction))
    goAccel(BACK, steps * 8.5, true);
  else
    goAccel(BACK, steps * 6, true);
  goAccel(BACK, 6, true);
  int dir = 180 - (direction * 45);
  if (dir < 0)
    goAccel(LEFT, abs(dir), false);
  else
    goAccel(RIGHT, abs(dir), false);
  correctFront();
  goAccel(RIGHT, color * 45, false );
  if(isDiag(color))
    goAccel(BACK, 56, true);
  else
    goAccel(BACK, 40, true);
  goAccel(RIGHT, 180, false);
  drop();
  if(isDiag(color))
    goAccel(BACK, 7, false);
  else
    goAccel(BACK, 3, false);
  alignRobot();
  goAccel(BACK, 5, false);
  goToGray(color);
}
void goTo(int direction) {
  int spokes = 0;
  for(int i = 0; i < 4; i++) {
    goUntilSpokes(BACK, true, 1);
    goConst(BACK, 8, 2000, true);
    alignRobot();
    if(isDiag(direction))
      goConst(FRONT, 1.5, 2000, true);
    else
      goConst(BACK, 0.5, 2000, true);
    spokes++;
    pickUp();
    Color found = calculate_color();
    if(found != INVALID) {
      goAccel(BACK, 4, false);
      goAccel(RIGHT, 180, false);
      returnTo(found, spokes, direction);
      break;
    }
    goConst(FRONT, 3, 2000, true);
  }
}
void runPeriphery() {
  for(int i = 0; i < 4; i++) {
    goAccel(BACK, 60, true);
    delay(1000);
    goAccel(LEFT, 90, false);
  }
}
/*
 * (from Wikipedia)
 *
 * Beyond caste, creed, family or lineage,
 * That which is without name and form, beyond merit and demerit,
 * That which is beyond space, time and sense-objects,
 * You are that, God himself; Meditate this within yourself.
 *
 * Path to Moksha -  Vivekachudamani, 8th Century AD
 */
void setup() {
  initialize();
//  beginCourse();
//goAccel(RIGHT, 180, false);
goToGray(5);
  //digitalWrite(LIGHT_PIN, HIGH);
}
void loop() {
//pickUp();
//Serial.println(calculate_color());
//drop();
//   evaluate_coin();
//  drop();
//  goConst(SRIGHT, 1, 2000, false);
}
