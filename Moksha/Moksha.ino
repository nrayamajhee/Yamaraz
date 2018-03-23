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
#define EMITTER_PIN   2
#define IR_THRESHHOLD 700
#define LIGHT_PIN 30
#define SERVO_PIN 10
#define MAGNET_PIN 11
#define TURN_SCALE 0.1;
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
  PURPLE,
  YELLOW,
  BLUE,
  GREEN,
  RED,
  GRAY,
  INVALID
};
struct RGB {
  int r;
  int g;
  int b;
};
RGB colors[8] = {
  {1200, 1900, 1900},
  {1000, 700, 700},
  {3500, 2500, 1600},
  {1000, 1000, 1200},
  {1500, 1700, 1000},
  {1400, 500, 400},
  {800, 800, 700},
  {700, 600, 490},
};
struct Debug {
  bool steps;
  bool ir;
  bool servo;
  bool light;
  bool motion;
};
Debug debug   = {
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
  800,  // to misec delay
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
bool spokes[6][5];
void initialize() {
  DDRL = 0xFF;                // set port L to output
  pinMode(SERVO_PIN, OUTPUT);  // servo output
  pinMode(MAGNET_PIN, OUTPUT);  // magenet output
  pinMode(LIGHT_PIN, OUTPUT);  // light
  digitalWrite(LIGHT_PIN, LOW);  // turn off the eye straining LED
  Serial.begin(9600);
  run_servo(UP);
  init_timers();         // start the timers
  set_timers(ALL, motors.minSpeed);
  for (int i =0; i < 6; i++) {
    for (int j =0; j < 5; j++) {
      spokes[i][j] = false;
    }
  }
}
void setup() {
  initialize();
  go(BACK, 43, false);
  correct_right();
  go_to(CYAN);
//  go_to_gray(4);
//digitalWrite(LIGHT_PIN, HIGH);
}
void loop() {
//pickUp();
//Serial.println(calculate_color());
//drop();
}
