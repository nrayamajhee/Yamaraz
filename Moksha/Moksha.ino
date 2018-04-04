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
#define NUM_SENSORS   8
#define TIMEOUT       2500
#define EMITTER_PIN   3
#define IR_THRESHHOLD 700
#define MAGNET_PIN    11
#define TURN_SCALE    0.35
#define ROUND         1
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
struct RGB {
  long r;
  long g;
  long b;
};
RGB colors[8] = {
  {41000, 65000, 65000}, //C
  {45000, 31000, 39000}, //M
  {65535, 65535, 65535}, //Y
  {26000, 50000, 65000}, //B
  {62000, 65000, 53000}, //G
  {55000, 22000, 20000}, //R
  {24000, 32000, 29000}, //G
  {10000, 12000, 10000}  //IN
};
struct Debug {
  bool steps;
  bool ir;
  bool angle;
  bool servo;
  bool light;
  bool motion;
};
Debug debug   = {
  0,
  0,
  0,
  0,
  0,
  0
};
struct Motors {
  volatile bool running;
  volatile int steps;
  volatile long totalSteps;
  int maxSpeed;
  int minSpeed;
  volatile int speed;
  volatile float alignRatio;
  volatile bool accelerate;
};
Motors motors = {
  false,
  0,
  0,
  600,  // to misec delay
  2000, // from misec delay
  0,
  1,     // turn ratio
  1,    // accelerate
};
struct IR {
  unsigned int  sensorValues  [NUM_SENSORS];
  bool          filteredValues[NUM_SENSORS];
  
};

IR ir;
IR ir2;

bool matrix[8][4];

void initialize() {
  DDRL = 0xFF;                // set port L to output
  Serial.begin(9600);
  init_servo();
  init_light();
  init_timers();         // start the timers
  set_timers(ALL, motors.minSpeed);
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 4; j++) {
      if((ROUND == 1) && ((j % 2) == 1))
        matrix[i][j] = true;
      else
        matrix[i][j] = false;
    }
  }
}
void run_course() {
  go(FRONT, 43, false);
  correct_right();
  go_to(CYAN);
}
void run_periphery() {
  for(int i = 0; i < 4; i++) {
    go(BACK, 12, true);
    delay(1000);
    go(RIGHT, 90, false);
  }
}
void setup() {
  initialize();
//  go(FRONT, 24, true);
  run_course();
}
void loop() {
//Serial.println(pick_up());
//drop();
}
