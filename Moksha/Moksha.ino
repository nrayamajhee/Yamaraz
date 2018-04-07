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
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE
 * .
 * ==========================================
 *
 * (from Wikipedia)
 *
 * Beyond caste, creed, family or lineage,
 * That which is without name and form, beyond merit and demerit,
 * That which is beyond space, time and sense-objects,
 * You are that, God himself; Meditate this within yourself.
 *
 * Path to Moksha -  Vivekachudamani, 8th Century AD
 */
 
#define HIGH_SPD 400
#define LOW_SPD 500
enum Direction {
  FRONT,
  BACK,
  RIGHT,
  LEFT,
  SLEFT,
  SRIGHT,
  UP,
  DOWN,
  ALL
};
enum Color {
  WHITE,
  CYAN,
  MAGENTA,
  YELLOW,
  HOME,
  BLUE,
  GREEN,
  RED,
  GRAY,
  INVALID
};
struct Debug {
  bool steps;
  bool ir;
  bool angle;
  bool servo;
  bool light;
  bool motion;
  bool nav;
};
Debug debug   = {
  0,
  0,
  0,
  0,
  0,
  0,
  0
};
unsigned long start_time;
void initialize() {
  DDRL = 0xFF;                // set port L to output
  Serial.begin(9600);
  pinMode(15, OUTPUT);
  digitalWrite(15, HIGH);
  init_servo();
  init_light();
  init_timers(); 
  init_matrix();
}
void run_course() {
  go(FRONT, 43, HIGH_SPD, false);
  go(RIGHT, 90, false);
  go_pick(MAGENTA, true);
}
void run_periphery() {
  for(int i = 0; i < 4; i++) {
    go(FRONT, 60, true);
    delay(1000);
    go(RIGHT, 90, false);
  }
}
void setup() {
  initialize();
  start_time = millis();
  run_course();
  
//  go(RIGHT, 360, false);
//go_until_spokes(FRONT, 1, true);
//align_to_coin(YELLOW, false);
//pick_up();
}
void loop() {
}
