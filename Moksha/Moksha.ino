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
  delay(500);           // delay for the IR sensors
  Serial.begin(9600);
  delay(1500);
  initTimers();         // start the timers
  setTimers(ALL, motors.minSpeed);
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
}

void loop() {
//  runServo(UP);
//  delay(500);
//  runServo(DOWN);
//  delay(500);
}

//Color coinColor;
//
////for demo
//int p = -1;
 


//void getCoin(){
//    //Write code that will use line sensors to detect coin
//    //Use buffer to avoid errors
//    //picUpCoin();
//    //returnHome should be here
//}
// 
//
//
//void goToRed(){
//  go(RIGHT, 3 * 45, false);
//  delay(500);
//  go(FRONT, 52, true);
//  delay(250);
//  //drop();
//  delay(250);
//  returnHome(3, 52);
//}
//
//void goToGreen(){
//  go(RIGHT, 2 * 45, false);
//  delay(500);
//  go(FRONT, 41, true);
//  delay(250);
//  //drop();
//  delay(250);
//  returnHome(2, 41);
//}
//
//void goToBlue(){
//  go(RIGHT, 1 * 45, false);
//  delay(500);
//  go(FRONT, 52, true);
//  delay(250);
//  //drop();
//  delay(250);
//  returnHome(1, 52);
//}
//
//void goToYellow(){
//  go(RIGHT, 7 * 45, false);
//  delay(500);
//  go(FRONT, 52, true);
//  delay(250);
//  //drop();
//  delay(250);
//  returnHome(7, 52);
//}
//
//void goToPurple(){
//  go(RIGHT, 6 * 45, false);
//  delay(500);
//  go(FRONT, 41, true); 
//  delay(250);
//  //drop();
//  delay(250);
//  returnHome(6, 41);
//}
//
//void goToSkyBlue(){
//  go(RIGHT, 5 * 45, false);
//  delay(500);
//  go(FRONT, 52, true);   
//  delay(250);
//  //drop();
//  delay(250);
//  returnHome(5, 52);
//}
//
//void robotAutonimiouso(){
//  if(coinColor == RED){
//    goToRed();  
//  }else if(coinColor == GREEN){
//    goToGreen();
//  }else if(coinColor == BLUE){
//    goToBlue();
//  }else if(coinColor == YELLOW){
//    goToYellow();
//  }else if(coinColor == PURPLE){
//    goToPurple();
//  }else if(coinColor == SKYBLUE){
//    goToSkyBlue();
//  }
//}
