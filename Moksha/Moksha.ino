#include "Definitions.h"
extern Motors motors;
extern int cnt;
Debug debug   = {
  0,
  0, // IR
  0,
  0
};
Motors motors = {
  false, 
  0, 
  0,
  400,  // to misec delay
  1000, // from misec delay
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
  initTimers();         // start the timers
  setTimers(ALL, motors.minSpeed);
  calibrate_IR();
}

void setup() {
  initialize();
  beginCourse();
//runPeriphery();
}

void loop() {
//  correct_exit();
}
