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
#include <QTRSensors.h>
#define NUM_SENSORS   8
#define TIMEOUT       2500
#define EMITTER_PIN   2
#define IR_THRESHHOLD 700
/*
 * Direction types
 * ---------------
 * left and right are rotation
 * sleft and sright are strafing
 */
enum Direction {
  FRONT,
  BACK,
  RIGHT,
  LEFT,
  SLEFT,
  SRIGHT,
  ALL
};
enum Compass {
  NORTH,  // white
  NEAST,  // cyan
  EAST,   // purple
  SEAST,  // yellow
  SOUTH,  // white
  SWEST,  // blue
  WEST,   // green
  NWEST   // red
};
/*
 * QTRSensor 8 IR with EMITTER
 */
QTRSensorsRC qtrrc((unsigned char[]) {22, 23, 24, 25, 26, 27, 28, 29},
  NUM_SENSORS, TIMEOUT, EMITTER_PIN);
 /*
  * Debug flags
  */
struct Debug {
  bool steps;
  bool ir;
  bool motion;
};
/*
 * The IR structure provides RAW sensor values and values filtered as ON or OFF
 */
struct IR {
  unsigned int  sensorValues  [NUM_SENSORS];
  bool          filteredValues[NUM_SENSORS];
};
/*
 * The Motors have many states. All of these are mutable and are
 * updated by the interrupt service routines ISR on TIMER 3 and 4
 */
struct Motors {
  volatile bool running;
  volatile int steps;
  volatile int totalSteps;
  int maxSpeed;
  int minSpeed;
  volatile int speed;
  volatile float alignRatio;
};
/*
 * Instantiate these structs
 */
Debug debug   = {1, 1};
IR    ir      = {0, 0};
Motors motors = {
  false, 0 , 0,
  600,  // to mils delay
  2000, // from mils delay
  0,
  1     // turn ratio
};
/*
 * ======================================
 * All low level functionality goes below
 */
/*
 * Interrupt Service Routines
 */
void initTimers() {
  noInterrupts();
  // Timer 3
  // for left wheels
  TCCR3A = 0;
  TCCR3B = 0;
  TCNT3  = 0;
  TCCR3B |= (1 << WGM32);
  TCCR3B |= (1 << CS30);
  TIMSK3 |= (1 << OCIE3A);
  // Timer 4
  // for right wheels
  TCCR4A = 0;
  TCCR4B = 0;
  TCNT4  = 0;
  TCCR4B |= (1 << WGM42);
  TCCR4B |= (1 << CS40);
  TIMSK4 |= (1 << OCIE4A);
  interrupts();
}
/*
 * This one modifies OCR?A which controls the interrupt duration
 * 
 */
void setTimers(Direction dir, int msDelay) {
  if (dir == LEFT)
    OCR3A  = 8 * msDelay;
  else if (dir == RIGHT)
    OCR4A  = 8 * msDelay;
  else if (dir == ALL) {
    OCR3A  = 8 * msDelay;
    OCR4A  = 8 * msDelay;
  }   
}
/*
 * ISR for timer 3 LEFT MOTORS
 * responsible for accleration and step counting
 */
ISR(TIMER3_COMPA_vect) {
  // once motors.running is true this service routine takes control
  if (motors.running) {
    // this will change the speeds of both wheels because we
    // mutate the global volatile variable motors.speed
    if (motors.steps <= (int)(0.2 * motors.totalSteps) && (motors.speed > motors.maxSpeed)) {
      motors.speed -= ceil((motors.minSpeed - motors.maxSpeed) / (0.2 * motors.totalSteps));

    } else if (motors.steps >= (int)(0.8 * motors.totalSteps) && (motors.speed < motors.minSpeed)) {
      motors.speed += ceil((motors.minSpeed - motors.maxSpeed) / (0.2 * motors.totalSteps));
    }
    // Toggle the left motors
    // and change duration for them
    setTimers(LEFT, motors.speed);
    PORTL ^= 0x05;
    // This interrupt is also incharge of updating the step count
    motors.steps++;
    // If no more steps to go, stop!
    if(motors.steps >= motors.totalSteps) {
      motors.running = false;
      motors.steps = 0;
      motors.totalSteps = 0;
    }
  }
}
/*
 * ISR for timer 4 RIGHT MOTORS
 * responsible for serial printing info
 */
ISR(TIMER4_COMPA_vect) {
  if (motors.running) {
    // Toggle the left motors
    // and change duration for them with the given ratio
    PORTL ^= 0x50;
    setTimers(RIGHT, motors.alignRatio * motors.speed);
    if(debug.steps) {
      Serial.print(" | ");
      Serial.print(motors.totalSteps);
      Serial.print(" # ");
      Serial.print(motors.steps);
      Serial.print(" : ");
      Serial.println(motors.speed);
    }
  }
}
/*
 * Read and filter out sensor values
 */
void IR_filter() {
  qtrrc.read(ir.sensorValues);
  for (unsigned char i = 0; i < NUM_SENSORS; i++) {
    ir.filteredValues[i] = (ir.sensorValues[i] > IR_THRESHHOLD) ? 1 : 0;
  }
  if(debug.ir){
    for (unsigned char i = 0; i < NUM_SENSORS; i++) {
      Serial.print(ir.filteredValues[i]);
      Serial.print('\t');
    }
  }
  Serial.println();
  delay(250);
}
/*
 * Sets the direction PINS form PORTL
 * left and right are rotation
 * sleft and sright are strafing
 */
void setDirection(Direction dir) {
  if (dir == FRONT) {
    PORTL = 0xA0;
  } else if (dir == BACK) {
    PORTL = 0x0A;
  } else if (dir == LEFT) {
    PORTL = 0x88;
  } else if (dir == RIGHT) {
    PORTL = 0x22;
  } else if (dir == SLEFT) {
    PORTL = 0x22;
  } else if (dir == SRIGHT) {
    PORTL = 0x22;
  }
}
/*
 * Turns the IRS flags on and sets the
 * number of steps to run
 * 
 * the amount will be in inches for
 * front, back, sleft and sright,
 * and degrees for
 * left and right
 */
void go(Direction dir, int amount){
  setDirection(dir);
  if(dir == LEFT || dir == RIGHT){
    motors.totalSteps = amount * 870; // linear calibration
    
  } else {
    motors.totalSteps = amount * 870; // angular calibration
  }
  
  if(debug.motion){
    Serial.print("Going ");
    Serial.print(dir);
    Serial.print(" by ");
    Serial.print(amount);
    Serial.print(" for ");
    Serial.println(motors.totalSteps);
  }

  // set slow speed and start the motors
  motors.speed = motors.minSpeed;
  motors.running = true;

  //while(motors.running == true) {
  while(true){
    IR_filter();
    
    if(ir.filteredValues[3] || ir.filteredValues[4]){
      motors.alignRatio = 1;
      
    } else if (ir.filteredValues[0] || ir.filteredValues[1]) {
      motors.alignRatio = 0.7; 
      
    } else if (ir.filteredValues[1] || ir.filteredValues[2]) {
      motors.alignRatio = 0.9;
      
    } else if (ir.filteredValues[5] || ir.filteredValues[6]) {
      motors.alignRatio = 1.1;
      
    } else if (ir.filteredValues[6] || ir.filteredValues[7]) {
      motors.alignRatio = 1.3;
    }
    
    Serial.println(motors.alignRatio);
  }
}
/*
 * Initialize ports, and timers
 */
void initialize() {
  // set port L to output
  DDRL = 0xFF;
  // delay for the IR sensors
  delay(500);
  Serial.begin(9600);
  delay(1500);
  // start the timers
  initTimers();
  setTimers(ALL, motors.minSpeed);
}
/*
 * All low level functionalities end here
 * ======================================  
 * 
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
void beginCourse() {
} 
void goTo(Compass direction, int steps) {
}
void returnHome() {
}
void setup() {
  initialize();
  go(FRONT, 15);
}
// loops seems to break with IRS,
// might behave in undefined ways
void loop() {}
