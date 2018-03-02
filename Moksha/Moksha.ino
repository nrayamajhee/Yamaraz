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
 * ON ANY THEORY OF LIABILI
    }TY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include <QTRSensors.h>
#define NUM_SENSORS   8
#define TIMEOUT       2500
#define EMITTER_PIN   2
#define IR_THRESHHOLD 70
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
  NEAST,  // red
  EAST,   // green
  SEAST,  // blue
  SOUTH,  // white
  SWEST,  // yellow
  WEST,   // purple
  NWEST   // skyblue
};
enum Color {
  RED,
  GREEN,
  BLUE,
  YELLOW,
  PURPLE,
  SKYBLUE
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
Debug debug   = {0, 1, 0};
/*
 * The IR structure provides RAW sensor values and values filtered as ON or OFF
 */
struct IR {
  unsigned int  sensorValues  [NUM_SENSORS];
  bool          filteredValues[NUM_SENSORS];
};
IR ir = {0, 0};
/*
 * The Motors have many states. All of these are mutable and are
 * updated by the interrupt service routines ISR on TIMER 3 and 4
 */
struct Motors {
  volatile bool running;
  volatile int steps;
  volatile long totalSteps;
  int maxSpeed;
  int minSpeed;
  volatile int speed;
  volatile float alignRatio;
};
Motors motors = {
  false, 
  0, 
  0,
  600,  // to mils delay
  1000, // from mils delay
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
//    
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
//  delay(250);
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
  } else if (dir == SLEFT) {
    PORTL = 0x88;
  } else if (dir == SRIGHT) {
    PORTL = 0x22;
  } else if (dir == LEFT) {
    PORTL = 0x00;
  } else if (dir == RIGHT) {
    PORTL = 0xAA;
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
void go(Direction dir, int amount, bool correct){
  setDirection(dir);
  if (dir == FRONT || dir == BACK){
    //motors.totalSteps = amount; // linear calibration
    motors.totalSteps = amount * 216; // linear calibration
  } else if(dir == SLEFT || dir == SRIGHT){
    motors.totalSteps = amount * 800; // strafing calibration
    
  } else {
    motors.totalSteps = (int)ceil(amount * 24.2); // angular calibration
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

  while(motors.running == true) {
    IR_filter();
    if(correct) {
      float avg = 4.5;
      float sum = ir.filteredValues[0] + 2*ir.filteredValues[1] + 3*ir.filteredValues[2] + 4*ir.filteredValues[3]+5*ir.filteredValues[4] + 6*ir.filteredValues[5] + 7*ir.filteredValues[6] + 8*ir.filteredValues[7];

      float onCount = 0;
      
      for(int i = 0; i < 8; i++){
        if(ir.filteredValues[i] == 1){
          onCount++;
        }
      }

      if(onCount == 0){
        onCount = 4.5;
      }

      float newAverage = sum / onCount;

      if(onCount > 2){
        newAverage = 4.5;
      }
      motors.alignRatio = 1 + ( newAverage - 4.5) * .08;        //1 is calibrated value
      Serial.println(motors.alignRatio);
    }
//      int left = ir.filteredValues[0] + ir.filteredValues[1] + ir.filteredValues[2] + ir.filteredValues[3];
//      int right = ir.filteredValues[0] + ir.filteredValues[1] + ir.filteredValues[2] + ir.filteredValues[3];
//      Serial.println(left);
//      Serial.println(right);
//      if(ir.filteredValues[3] || ir.filteredValues[4]){
//        motors.alignRatio = 1;
//        
//      } else if (ir.filteredValues[0] || ir.filteredValues[1]) {
//        motors.alignRatio = 0.7; 
//        
//      } else if (ir.filteredValues[1] || ir.filteredValues[2]) {
//        motors.alignRatio = 0.9;
//        
//      } else if (ir.filteredValues[5] || ir.filteredValues[6]) {
//        motors.alignRatio = 1.1;
//        
//      } else if (ir.filteredValues[6] || ir.filteredValues[7]) {
//        motors.alignRatio = 1.3;
//      }
//    }
    //Serial.println(motors.alignRatio);
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
Color coinColor;

//for demo
int p = -1;
 
void returnHome(int direction, int steps) {
  Serial.println("Reached returnHome");
  go(RIGHT, 180, false);  //Turn around
  delay(500);
  go(FRONT, steps, false);
  delay(500);
  go(RIGHT, 180 - (direction * 45), false);
}

void getCoin(){
    //Write code that will use line sensors to detect coin
    //Use buffer to avoid errors
    //picUpCoin();
    //returnHome should be here
}
 
void goTo(int direction, int steps) {
  go(RIGHT, direction * 45, false);
  delay(500);
  go(FRONT, steps, true);
  delay(500);
  returnHome(direction, steps);
  Color colors[6] = {RED, GREEN, SKYBLUE, BLUE, RED, PURPLE};
  p = p+1;
  coinColor = colors[p];
}

void goToRed(){
  go(RIGHT, 3 * 45, false);
  delay(500);
  go(FRONT, 52, true);
  delay(500);
  returnHome(3, 52);
}

void goToGreen(){
  go(RIGHT, 2 * 45, false);
  delay(500);
  go(FRONT, 41, true);
  delay(500);
  returnHome(2, 41);
}

void goToBlue(){
  go(RIGHT, 1 * 45, false);
  delay(500);
  go(FRONT, 52, true);
  delay(500);
  returnHome(1, 52);
}

void goToYellow(){
  go(RIGHT, 7 * 45, false);
  delay(500);
  go(FRONT, 52, true);
  delay(500);
  returnHome(7, 52);
}

void goToPurple(){
  go(RIGHT, 6 * 45, false);
  delay(500);
  go(FRONT, 41, true); 
  delay(500);
  returnHome(6, 41);
}

void goToSkyBlue(){
  go(RIGHT, 5 * 45, false);
  delay(500);
  go(FRONT, 52, true);   
  delay(500);
  returnHome(5, 52);
}

void robotAutonimiouso(){
  if(coinColor == RED){
    goToRed();  
  }else if(coinColor == GREEN){
    goToGreen();
  }else if(coinColor == BLUE){
    goToBlue();
  }else if(coinColor == YELLOW){
    goToYellow();
  }else if(coinColor == PURPLE){
    goToPurple();
  }else if(coinColor == SKYBLUE){
    goToSkyBlue();
  }
}

void beginCourse() {
  go(FRONT, 96, true);
//  delay(500);
//  for(int i = 1; i <=8; i++){
//    if((i!=4)&&(i!=8)){
//      goTo(i, 11.7);
//      delay(500);
//      robotAutonimiouso();
//    }
//  }
}

void setup() {
  Serial.begin(9600);
  int i =0;
  for (i =0 ; i < 8; i++){
  Serial.println(qtrrc.calibratedMinimumOn[i]);
  
  qtrrc.calibratedMinimumOn[i] = 272;
  qtrrc.calibratedMaximumOn[i] = 2500;
  
  }
  initialize();
  beginCourse();
  //go(FRONT, 2000, true);
}
// loops seems to break with ISR,
// might behave in undefined ways
void loop() {}
