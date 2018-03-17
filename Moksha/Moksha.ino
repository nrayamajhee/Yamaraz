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
  0,
  0
};
Motors motors = {
  false, 
  0, 
  0,
  400,  // to misec delay
  1500, // from misec delay
  0,
  1,     // turn ratio
  1,    // accelerate
  0
};
Servo servo = { false, 0, 0};
IR ir = {
  0,
  0
};
void initialize() {
  DDRL = 0xFF;          // set port L to output
  pinMode(10, OUTPUT);  // servo output
  pinMode(11, OUTPUT);  // magenet output
  delay(500);           // delay for the IR sensors
  Serial.begin(9600);
  init_light();
  initTimers();         // start the timers
  randomSeed(analogRead(0));
  setTimers(ALL, motors.minSpeed);
//  calibrate_IR();
}

void returnHome(Color home, int steps, int direction) {
  for(int i = 0; i < steps; i++) {
    goAccel(FRONT, 12, true);
  }
  goAccel(FRONT, 6, true);
  delay(500);
  goAccel(LEFT, direction * 45, false);
  goAccel(RIGHT, home * 45, false );
  goAccel(BACK, 60, true);
  drop();
}

void getCoin() {
  goUntilSpokes(BACK, true);
  goConst(BACK, 5.9, 2000, true);
  alignRobot();
  pickUp();
}

void beginCourse() {
  goAccel(BACK, 43, false);
  delay(500);
  correctRight();
  delay(500);
  for(int i = 5; i < 6; i++){
    if(i!=4){
      goTo(i);
      delay(500);
      break;
    }
  }
}

void goTo(int direction) {
  goAccel(RIGHT, direction * 45, false );
  goAccel(BACK, 10, true);
  int steps = 0;
  while(steps == 0) {
    getCoin();
    steps++;
  }
  returnHome(calculate_color(), steps, direction);
//  goUntilSpokes(BACK, true);
//  goUntilFound(FRONT, true);
//  pickUp();
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
//  correctRight();

  beginCourse();
//  digitalWrite(11, LOW);
}

void loop() {
//  pickUp();
}
