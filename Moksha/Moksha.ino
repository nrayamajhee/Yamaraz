/*
 * This is the bleeding edge version of our robot navigation code.
 * So naturally a lot of it is not tested and sometimes coded entirely away
 * from the robot.
 *
 * Feel free to use, modify, and redistribute!
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
#include "Navigation.h"

enum Color {
  RED,
  GREEN,
  BLUE,
  YELLOW,
  PURPLE,
  SKYBLUE
};
/*
 * Initialize ports, and timers
 */
void initialize() {
  DDRL = 0xFF;          // set port L to output
  pinMode(10, OUTPUT);  // servo output
  pinMode(11, OUTPUT);  // magenet output
  delay(500);           // delay for the IR sensors
  Serial.begin(9600);
  delay(1500);
  initTimers();         // start the timers
  setTimers(ALL, motors.minSpeed);
}


void beginCourse() {
  go(FRONT, 43.5, false, false);
  delay(500);
  for(int i = 1; i <2; i++){
    if(i!=4){
      goTo(i, 48);
      delay(500);
      break;
    }
  }
}

void runPeriphery() {
  for(int i = 0; i < 10; i++) {
    go(FRONT, 60, true, false);
    delay(1000);
    go(RIGHT, 90, true, false);
  }
}

/*
 * (wikipedia)
 *
 * Beyond caste, creed, family or lineage,
 * That which is without name and form,
 * beyond merit and demerit,
 * That which is beyond space,
 * time and sense-objects,
 * You are that,
 * God himself!
 *
 * Path to Moksha -  Vivekachudamani, 8th Century AD
 *
 * ==============================================
 * All high level scripting goes below this line.
 *
 */
void setup() {
  Serial.begin(9600);
  initialize();
  beginCourse();
//  fixRobotLine();
}

void loop() {
//  pickUp();
//Serial.println(calculate_average());
}
