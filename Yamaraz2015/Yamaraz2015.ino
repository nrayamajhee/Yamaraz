#include <NewPing.h>
#include <Servo.h>

int init_angle = 0;
int flag = 0;

const int FRONT      = 1;
const int REAR       = 2;
const int LEFT       = 3;
const int RIGHT      = 4;
const int CLOCK      = 5;
const int ANTI_CLOCK = 6;

const int trigFL = 50;
const int echoFL = 51;
const int trigFR = 52;
const int echoFR = 53;
const int trigBL = 26;
const int echoBL = 27;
const int trigBR = 28;
const int echoBR = 29;

const int trigWL = 38;
const int echoWL = 39;
const int trigWR = 40;
const int echoWR = 41;

const int sigVL = 24;
const int sigVR = 25;

const int MAX_DISTANCE = 200;

unsigned long a = 0;
unsigned long b = 0;

// Servo Object to control servo
Servo myservo;

// Setup pinouts for ultrasonic sensors
NewPing sonar[8] = {
  NewPing(trigFL, echoFL, MAX_DISTANCE),
  NewPing(trigFR, echoFR, MAX_DISTANCE),
  NewPing(trigBL, echoBL, MAX_DISTANCE),
  NewPing(trigBR, echoBR, MAX_DISTANCE),
  NewPing(trigWL, echoWL, MAX_DISTANCE),
  NewPing(trigWR, echoWR, MAX_DISTANCE),
  NewPing(sigVL, sigVL, MAX_DISTANCE),
  NewPing(sigVR, sigVR, MAX_DISTANCE),
};

void setup() {
  initPins();
  Serial.begin(9600);
  Serial1.begin(9600);
  delay(2000);
  exploreCity();
  exploreOffRoad();
//  approachG();
}

/*
   Path Algorithms
*/

void exploreCity() {
  goToVictim1();
  delay(3000);
  char color = approach();
  if (color == 'R') {
    goFromVictim1ToRed();
    dropVictim();
    goFromRedToYellow();
    goToVictim2();
    approach();
    goFromVictim2ToYellow();
    dropVictim();
    rotateAnticlock(180);
    correctLeft();
  } else if(color == 'Y') {
    goFromVictim1ToYellow();
    dropVictim();
    if(getReadyToTurn(true))
      rotateAnticlock(180);
    correctLeft();
    goToVictim2();
    delay(3000);
    approach();
    goFromVictim2ToRed();
    dropVictim();
    goFromRedToYellow();
  }
}

void exploreOffRoad(){
  goToVictim3();
  delay(3000);
  char color = approachG();
  correctRightOffRoad();
  if(color == 'R'){
    goFromVictim3ToRed();
    dropVictim();  
    goFromRedToYellow();
  } else if(color == 'Y'){
    Serial.println('Yo');
    goFromVictim3ToYellow();
    dropVictim();
    if(getReadyToTurn(true))
      rotateAnticlock(180);
    delay(20);
  }
}

/*
   Path Motion Victim 3
*/

void goFromVictim3ToYellow(){
  goReverse(8);
  delay(20);
  rotateAnticlockG(90);
  delay(20);
  correctLeft();
  delay(20);
  goForward(14);
  delay(20);
  rotateAnticlockG(100);
  delay(20);
  goForward(14);
  delay(20);
  rotateClock(90);
  delay(20);
  goForward(9);
  correctLeft();
  goForward(6);
  correctLeft();
  goForward(3);
  delay(50);
  if(getReadyToTurn(true)){
    delay(20);
    rotateAnticlock(90);
    delay(20);
    goForward(12);
    delay(20);
    rotateClock(90);
  }
  for (int i = 0; i < 2; i++) {
    goForward(12);
    correctRight();
  }
  delay(20);
}

void goFromVictim3ToRed(){
  goFromVictim3ToYellow();
  goFromYellowToRed();
}

void goToVictim3(){
  for (int i = 0; i < 3; i++) {
    goForward(17);
    correctRight();
  }
  if(getReadyToTurn(true)){
    delay(20);
    rotateAnticlock(90); 
    delay(20);
  }
  goForward(24);
  delay(20);
  rotateClock(90);
  delay(20);
  goForward(12);
  delay(20);
  rotateAnticlockG(90);
  delay(20);
  correctRight();
  goForward(5);
  
}

/*
   Path Motion Victim 1
*/

void goToVictim1() {
  goForward(12);
  delay(20);
  rotateAnticlock(90);
  delay(20);
  goForward(14.5);
  delay(20);
  rotateClock(90);
  delay(20);
  correctLeft();  
  for (int i = 0; i < 3; i++) {
    goForward(17);
    correctRight();
  }
  delay(20);
}

void goFromVictim1ToYellow() {
  goForward(4);
  delay(20);
  if(getReadyToTurn(true)){
    rotateAnticlock(180);
  }
  delay(20);
  for (int i = 0; i < 3; i++) {
    correctLeft();
    goForward(17);
  }
  goForward(6);
  correctRight();
  delay(20);
}

void goFromYellowToRed() {
  goReverse(12);
  correctLeft();
  if(getReadyToTurn(true)){
    delay(20);
    rotateAnticlock(90); 
    delay(20);
    goForward(12);
    delay(20);
    rotateAnticlock(90);
    delay(20);
  }
  for (int i = 0; i < 5; i++) {
    goForward(12);
    correctLeft();
  }
  delay(20);
}

void goFromVictim1ToRed() {
  goFromVictim1ToYellow();
  goFromYellowToRed();
}

void goFromRedToYellow() {
  rotateClock(180);
  delay(20);
  for (int i = 0; i < 5; i++) {
    correctRight();
    goForward(11);
  }
  if(getReadyToTurn(false)){
    delay(20);
    rotateClock(90); 
    delay(20);
    goForward(11);
    delay(20);
    rotateClock(90);
    delay(20);
    correctLeft();
  }
}

/*
   Path Motion Victim 2
*/

void goToVictim2() {
  for (int i = 0; i < 2; i++) {
    correctLeft();
    goForward(8);
  }
  if(getReadyToTurn(true)){
    delay(20);
    rotateAnticlock(90); 
    delay(20);
    goForward(12);
    delay(20);
    rotateAnticlock(90);
    delay(20);
  }
  goForward(12);
  correctLeft();
  goForward(4);
  delay(20);
}

void goFromVictim2ToYellow() {
  goReverse(28);
  correctLeft();
  if(getReadyToTurn(true)){
    delay(20);
    rotateAnticlock(90);
    delay(20);
    goForward(11);
    delay(20);
    rotateClock(90);
    delay(20);
   }
  for (int i = 0; i < 2; i++) {
    goForward(12);
    correctRight();
  }
  delay(20);
}

void goFromVictim2ToRed() {
  goFromVictim2ToYellow();
  goFromYellowToRed();
}

/*
   Motion Functions
*/

void goForward(double dis) {
  long steps = dis * 862.407585;
  setDir(FRONT);
  runMotors(steps);
}

void goForwardSlow(double dis) {
  long steps = dis * 862.407585;
  setDir(FRONT);
  runMotorsappro(steps);
}

void goReverseSlow(double dis) {
  long steps = dis * 862.407585;
  setDir(REAR);
  runMotorsappro(steps);
}

void goReverse(double dis) {
  long steps = dis * 862.407585;
  setDir(REAR);
  runMotors(steps);
}

void rotateClock(double deg) {
  long steps = (deg / 90) * 7900;
  setDir(CLOCK);
  runMotors(steps);
}

void rotateAnticlock(double deg) {
  long steps = (deg / 90) * 7900;
  setDir(ANTI_CLOCK);
  runMotors(steps);
}

void rotateClockG(double deg) {
  long steps = (deg / 90) * 7900;
  
  Serial.print("number of steps for ");
  Serial.print(deg);
  Serial.print(" deg are ");
  Serial.println(steps);
  
  setDir(CLOCK);
  runMotors(steps);
}

void rotateAnticlockG(double deg) {
  long steps = (deg / 90) * 9100;
  
  Serial.print("number of steps for ");
  Serial.print(deg);
  Serial.print(" deg are ");
  Serial.println(steps);
  
  setDir(ANTI_CLOCK);
  runMotors(steps);
}

bool getReadyToTurn(bool isLeft){
  bool isReady = false;
  while(!isReady){
    if(isLeft){
      a = sonar[0].ping_median(7);
      b = sonar[2].ping_median(7);
    } else {
      a = sonar[1].ping_median(7);
      b = sonar[3].ping_median(7);
    }

    Serial.println(a);
    Serial.println(b);
    
    if(a > 600 && b < 600) {
      goForwardSlow(1);
    } else if (a < 600 && b > 600) {
      goReverseSlow(1);
    } else if (a < 600 && b < 600) {
      goForwardSlow(1);
    } else {
      goForward(1.5);
      isReady = true;
    }
  }
  return isReady;
}

/*
   Grabbing
*/

void openJaw() {
  for (int pos = 250; pos >= 80; pos--) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(5);
  }
}
void closeJaw() {
  for (int pos = 80; pos <= 250; pos++) { // goes from 0 degrees to 180 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(5);
  }
}

void initJaw() {
  myservo.write(80);
  delay(5);
}

/*
 * Alignment with Ultrasonics
*/

void approachBySensors(bool grab){
  int unit = 145;
  
  a = sonar[4].ping_median(7);
  b = sonar[5].ping_median(7);

  double distance = 1047;
  int avg = (a + b) /2;
  
  int dif = (avg - distance);
  long diff = dif / unit;
  
  goForward(diff);
  if(grab){
    correctRight();
    closeJaw();
  }
  else
    openJaw();
  delay(500);
  goReverse(diff);
  delay(20);
}

void get_serial(char pdata[]){
  byte index = 0;
  char inChar=-1;
  while(true) {
    if (Serial1.available()>0 && index<=6) {
      inChar = Serial1.read();
      if(inChar == '\0') {
       continue; 
      }
      else {
        pdata[index] = inChar;   
        index++;
      }
    }
    else if(Serial1.available()==0 && index<=6) {
      break;
    }
    else{break;}
  }
}

char approach(){
    
  float distanceToObject = 0;
  char objectColor = 0;
  char angleDirection = 0;
  long angleDegree = 0;
  
  Serial1.println('B');
  
  while(distanceToObject <15 || objectColor =='N'){
    
    char inData[7] = {0,0,0,0,0,0,0};
    get_serial(inData);
    
    String string_DTO = "";
    String string_A = "";
    
    string_DTO += (char)inData[0];
    string_DTO += (char)inData[1];
    string_DTO += (char)inData[2];
    
    distanceToObject = string_DTO.toInt() ;
    angleDirection = inData[3];
    string_A += (char)inData[4];
    string_A += (char)inData[5];
    angleDegree = string_A.toInt();
    objectColor = (char)inData[6];
    
    Serial.println(inData[0]);
    Serial.println(inData[1]);
    Serial.println(inData[2]);
    Serial.println(inData[3]);
    Serial.println(inData[4]);
    Serial.println(inData[5]);
    Serial.println(inData[6]);
    delay(500);
    Serial.println("Distance to object");
    Serial.println(distanceToObject);
    Serial.println("Angle direction");
    Serial.println(angleDirection);
    Serial.println("Angle from center in degrees");
    Serial.println(angleDegree);
    Serial.println("color");
    Serial.println(objectColor);
    delay(500);
  }
  long diff = ((distanceToObject/10)-7.5);
  goForward(diff/2);
  correctRight();
  goForwardSlow(diff/2);
  closeJaw();
  delay(500);
  goReverse(diff);    
  delay(1000);
  return objectColor;
}
void get_serialG(char pdata[]){
  byte index = 0;
  char inChar=-1;
  
  while(1){
  if (Serial1.available()>0 && index<=6) {
    inChar = Serial1.read();
    if(inChar == '\0'){
     continue; 
    }
    else{
      pdata[index] = inChar;   
      index++;}
  }
  else if(Serial1.available()==0 && index<=6) {
    break;
  }
  else{break;}
  }
}
char approachG(){
  
  float distanceToObject = 0;
  char angleDirection = 0;
  long angleDegree = 0;
  char objectColor = 0;
  
  Serial1.println('A');
    
  while(1){    
    char inData[7] = {0,0,0,0,0,0,0};
    get_serialG(inData);
    
    String string_DTO = "";
    String string_A = "";
    
    string_DTO += (char)inData[0];
    string_DTO += (char)inData[1];
    string_DTO += (char)inData[2];
    distanceToObject = string_DTO.toInt() ;
    
    angleDirection = inData[3];
    
    string_A += (char)inData[4];
    string_A += (char)inData[5];
    angleDegree = string_A.toInt();
    
    objectColor = (char)inData[6];
    
    Serial.println(inData[0]);
    Serial.println(inData[1]);
    Serial.println(inData[2]);
    Serial.println(inData[3]);
    Serial.println(inData[4]);
    Serial.println(inData[5]);
    Serial.println(inData[6]);
    delay(500);
    Serial.println("Distance to object");
    Serial.println(distanceToObject);
    Serial.println("Angle direction");
    Serial.println(angleDirection);
    Serial.println("Angle from center in degrees");
    Serial.println(angleDegree);
    Serial.println("color");
    Serial.println(objectColor);
    delay(500);

    if(distanceToObject > 12 && objectColor != 'X'){
     break; 
    }
    else if(distanceToObject <12 && objectColor != 'X'){
     continue;
    }
    else if(objectColor == 'X'){
      flag = 1;
      break;
    }
  }
  if(flag==1){ 
        correctRightOffRoad();
        delay(50);
        goForward(16);
        delay(50);
        correctRightOffRoad();
        delay(50);
        rotateAnticlock(43);
        delay (2000);
        
        //taking picture 
         Serial1.println('A');
         while(1){
            char inData[7] = {0,0,0,0,0,0,0};
            get_serialG(inData);
            
            String string_DTO = "";
            String string_A = "";
            
            string_DTO += (char)inData[0];
            string_DTO += (char)inData[1];
            string_DTO += (char)inData[2];
            distanceToObject = string_DTO.toInt() ;
            
            angleDirection = inData[3];
            
            string_A += (char)inData[4];
            string_A += (char)inData[5];
            angleDegree = string_A.toInt();
            
            objectColor = (char)inData[6];
            
            Serial.println(inData[0]);
            Serial.println(inData[1]);
            Serial.println(inData[2]);
            Serial.println(inData[3]);
            Serial.println(inData[4]);
            Serial.println(inData[5]);
            Serial.println(inData[6]);
            delay(500);
            Serial.println("Distance to object");
            Serial.println(distanceToObject);
            Serial.println("Angle direction");
            Serial.println(angleDirection);
            Serial.println("Angle from center in degrees");
            Serial.println(angleDegree);
            Serial.println("color");
            Serial.println(objectColor);
            delay(500);
        
            if(distanceToObject > 12 && objectColor != 'X'){
           
             break; 
            }
            else if(distanceToObject <12 && objectColor != 'X'){
             continue;
            }
           }
            // end of taking pic 
         
          long diff = ((distanceToObject/10)-5);
          delay(50);
          if(angleDirection == 'L') {
            rotateAnticlockG(angleDegree);
            Serial.println("Rotating left");
          } else if (angleDirection == 'R') {
            rotateClockG(angleDegree);
            Serial.println("Rotating right");
          }

          init_angle = angleDegree;
          delay(2000);
          
          approachGConfirm();
          
          goForward(diff/2);
          goForwardSlow(diff/2);
          closeJaw();
          delay(500);
          goReverse(diff - 3);
          rotateClockG(45);
          correctRightOffRoad();
          goReverse(12);
          correctRightOffRoad();
          goReverse(12);
  } 
  else if (flag==0) {
    
      long diff = ((distanceToObject/10)-5);
      init_angle = angleDegree;
      
      delay(50);
      if(angleDirection == 'L') {
        rotateAnticlockG(angleDegree);
        Serial.println("Rotating left");
      } else if (angleDirection == 'R') {
        rotateClockG(angleDegree);
        Serial.println("Rotating right");
      }
      delay(2000);
      
      approachGConfirm();
      
      goForward(diff/2);
      goForwardSlow(diff/2);
      closeJaw();
      delay(500);
      goReverse(diff);     
  }
  return objectColor;
}

void approachGConfirm(){
  Serial1.println('A');

  float distanceToObject = 0;
  char objectColor = 0;
  long angleDegree = 0;
  char angleDirection = 0;
  
  while(distanceToObject <15 || objectColor =='N'){    
    char inData[7] = {0,0,0,0,0,0,0};
    get_serial(inData);
    
    String string_DTO = "";
    String string_A = "";
    
    string_DTO += (char)inData[0];
    string_DTO += (char)inData[1];
    string_DTO += (char)inData[2];
    
    distanceToObject = string_DTO.toInt() ;
    angleDirection = inData[3];
    string_A += (char)inData[4];
    string_A += (char)inData[5];
    angleDegree = string_A.toInt();
    objectColor = (char)inData[6];
    
    Serial.println(inData[0]);
    Serial.println(inData[1]);
    Serial.println(inData[2]);
    Serial.println(inData[3]);
    Serial.println(inData[4]);
    Serial.println(inData[5]);
    Serial.println(inData[6]);
    delay(500);
    Serial.println("Distance to object");
    Serial.println(distanceToObject);
    Serial.println("Angle direction");
    Serial.println(angleDirection);
    Serial.println("Angle from center in degrees");
    Serial.println(angleDegree);
    Serial.println("color");
    Serial.println(objectColor);
    delay(500);
  }
  
  long actualAngle = abs(init_angle - angleDegree);
  double error = init_angle > 0 ? (double)angleDegree / init_angle : 0;
  double correctedAngle = angleDegree + ( error * angleDegree);
  
  Serial.println("Rotated only");
  Serial.println(actualAngle);
  Serial.println("Should now rotate");
  Serial.println(correctedAngle);
  
  delay(50);
  if(angleDirection == 'L'){
    Serial.println("Rotating left");
    
    // !This is a calibrated Value for our board
    correctedAngle += 4; 
    
    rotateAnticlockG(correctedAngle);
  } else if (angleDirection == 'R') {
    Serial.println("Rotating right");
    
     // !This is a calibrated Value for our board
    correctedAngle -= 3;
    
    rotateClockG(correctedAngle);
  }
  delay(50);
}

void dropVictim(){
  goForward(8);
  delay(20);
  openJaw();
  delay(20);
  goReverse(8);
}

/*
   Robot's alignment corrections
*/

// Rotates the robot and strafs it 2.0 inches away from the wall

void correctRightOffRoad(){
  alignRight();
  gobyRightS(3.5, false);
}

void correctLeftOffRoad(){
  alignLeft();
  gobyLeftS(3.5, false);
}

void correctRight() {
  alignRight();
  gobyRightS(2.5, true);
}

void correctLeft() {
  alignLeft();
  gobyLeftS(2.5, true);
}

// Rotates the robot to parallel with the wall

void alignRight() {
  a = sonar[1].ping_median(7);
  b = sonar[3].ping_median(7);

  int difR = a - b;
  double difRR = (abs(difR)); // No correction offset for right sensors
  double hypo = 624.75;
  double errorAngleRdegree = (180 / 3.14) * asin(difRR / hypo);

  if (a > b) rotateClock((int)errorAngleRdegree);
  if (a < b) rotateAnticlock((int)errorAngleRdegree);
}

void alignLeft() {
  a = sonar[0].ping_median(7);
  b = sonar[2].ping_median(7);

  int difL = a - b;
  double difLL = (abs(difL) - 24); // 24 is correction offset for left sensors
  double hypo = 624.75;
  double errorAngleLdegree = (180 / 3.14) * asin(difLL / hypo);

  if (a < b) rotateClock((int)errorAngleLdegree);
  if (a > b) rotateAnticlock((int)errorAngleLdegree);
}

// Moves the robot some distance from the wall

void gobyLeftS(double dis, boolean isCity) { // dis is the distance from wheel to wall
  a = sonar[0].ping_median(7);
  b = sonar[2].ping_median(7);

  int sensor1in = 112; // 1 in of ultrasonic = 142 value

  int calc = (dis * sensor1in + 180); // 180 is the sensor reading when the wheels are attached to the wall

  int avg = (int)((a + b) / 2);

  if (avg < calc) {
    int steps;
    int correction = calc - avg;
    double correctionStep =  correction / sensor1in;
    if(isCity)
      steps = correctionStep * 1000; // 1 inch = 1000 steps of our robot
    else
      steps = correctionStep * 1500;
    setDir(RIGHT);
    runMotorsStrafe(steps);
  }
  else if (avg >= calc) {
    int steps;
    int correction = avg - calc;
    double correctionStep =  correction / sensor1in;
    if(isCity)
      steps = correctionStep * 1000; // 1 inch = 1000 steps of our robot
    else
      steps = correctionStep * 1500;
    setDir(LEFT);
    runMotorsStrafe(steps);
  }
}

void gobyRightS(double dis, boolean isCity) { // dis is the distance from wheel to wall
  a = sonar[1].ping_median(7);
  b = sonar[3].ping_median(7);

  int sensor1in = 112; // 1 in of ultrasonic = 112 value

  int calc = (dis * sensor1in + 159); // 159 is the sensor reading when the wheels are attached to the wall

  int avg = (int)((a + b) / 2);

  if (avg < calc) {
    int steps;
    int correction = calc - avg;
    double correctionStep =  correction / sensor1in;
    if(isCity)
      steps = correctionStep * 1000; // 1 inch = 1000 steps of our robot
    else
      steps = correctionStep * 1500;
    setDir(LEFT);
    runMotorsStrafe(steps);
  }
  else if (avg >= calc) {
    int steps;
    int correction = avg - calc;
    double correctionStep =  correction / sensor1in;
    if(isCity)
      steps = correctionStep * 1000; // 1 inch = 1000 steps of our robot
    else
      steps = correctionStep * 1500;
    setDir(RIGHT);
    runMotorsStrafe(steps);
  }
}

/*
   Initialize the pins
*/

void initPins() {
  myservo.attach(22);  // attaches the servo on pin 9 to the servo object
  initJaw();

  // Set direction pins to output
  // PORTC is 30 to 37
  DDRC = B10101010;

  // Set the stepper pins to output
  // PORTL is 42 to 48
  DDRL = B10101010;

  // Set the direction values to front
  // and Stepping to zero
  PORTC = B00100010;
  PORTL = B00000000;
}

/*
   Set direction of the motors
*/

void setDir(const int dir) {
  if (dir == FRONT) {
    PORTC = B10001000;
  } else if (dir == REAR) {
    PORTC = B00100010;
  } else if (dir == LEFT) {
    PORTC = B00001010;
  } else if (dir == RIGHT) {
    PORTC = B10100000;
  } else if (dir == CLOCK) {
    PORTC = B10101010;
  } else if (dir == ANTI_CLOCK) {
    PORTC = B00000000;
  }
}

/*
   run motor with 50 microseconds delay for forward/reverse
   and with 150 microseconds delay for strafing left/right
*/

void runMotorsWithSpeed(long steps, const int del){
  for (long i = 0; i < steps * 2; i++) {
    // toggle port
    PORTL ^= B10101010;
    delayMicroseconds(del);
  }
}

void runMotors(long steps) {
  runMotorsWithSpeed(steps, 50);
}

void runMotorsappro(long steps) {
  runMotorsWithSpeed(steps, 300);
}

void runMotorsStrafe(long steps) {
  runMotorsWithSpeed(steps, 150);
}

void loop() {
  // put your main code here, to run repeatedly:
}

