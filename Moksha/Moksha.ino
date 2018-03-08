#include <QTRSensors.h>
#include "Motion.h"
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

struct Servo {
  volatile bool state;
  volatile int count;
  volatile int maxCount;
};

Servo servo = { false, 0, 0};
/*
 * The IR structure provides RAW sensor values and values filtered as ON or OFF
 */
struct IR {
  unsigned int  sensorValues  [NUM_SENSORS];
  bool          filteredValues[NUM_SENSORS];
};
IR ir = {0, 0};

void runServo(Direction dir) {
  int speed, steps;
  if (dir == UP) {
    speed = 300;
    steps = 2000;
  } else if (dir == DOWN) {
    speed = 2500;
    steps = 200;
  }
  int cnt = 0;
  while (cnt < steps){
    digitalWrite(10, HIGH);
    delayMicroseconds(speed);
    digitalWrite(10, LOW);  
    delayMicroseconds(speed);
    cnt++;
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
 * Turns the IRS flags on and sets the
 * number of steps to run
 * 
 * the amount will be in inches for
 * front, back, sleft and sright,
 * and degrees fori
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
      motors.alignRatio = 1 + (newAverage - 4.5) * .1;        //1 is calibrated value; Dr Gray says either use .25 or .35 for better control
      //Serial.println(motors.alignRatio);
    }
  }
}
/*
 * Initialize ports, and timers
 */
void initialize() {
  DDRL = 0xFF;          // set port L to output
  pinMode(10, OUTPUT);  // servo output
  delay(500);           // delay for the IR sensors
  Serial.begin(9600);
  delay(1500);
  initTimers();         // start the timers
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
  delay(250);
  //pickUp();
  delay(250);
  returnHome(direction, steps);
  Color colors[6] = {RED, GREEN, SKYBLUE, BLUE, RED, PURPLE};
  p = p+1;
  coinColor = colors[p];
}

void goToRed(){
  go(RIGHT, 3 * 45, false);
  delay(500);
  go(FRONT, 52, true);
  delay(250);
  //drop();
  delay(250);
  returnHome(3, 52);
}

void goToGreen(){
  go(RIGHT, 2 * 45, false);
  delay(500);
  go(FRONT, 41, true);
  delay(250);
  //drop();
  delay(250);
  returnHome(2, 41);
}

void goToBlue(){
  go(RIGHT, 1 * 45, false);
  delay(500);
  go(FRONT, 52, true);
  delay(250);
  //drop();
  delay(250);
  returnHome(1, 52);
}

void goToYellow(){
  go(RIGHT, 7 * 45, false);
  delay(500);
  go(FRONT, 52, true);
  delay(250);
  //drop();
  delay(250);
  returnHome(7, 52);
}

void goToPurple(){
  go(RIGHT, 6 * 45, false);
  delay(500);
  go(FRONT, 41, true); 
  delay(250);
  //drop();
  delay(250);
  returnHome(6, 41);
}

void goToSkyBlue(){
  go(RIGHT, 5 * 45, false);
  delay(500);
  go(FRONT, 52, true);   
  delay(250);
  //drop();
  delay(250);
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
  go(FRONT, 60, true);
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
  initialize();
  beginCourse();
}

void loop() {
//  runServo(UP);
//  delay(500);
//  runServo(DOWN);
//  delay(500);
}
