/*
 * This is the bleeding edge version of our robot navigation code.
 * So naturally a lot of it is not tested and sometimes coded entirely away
 * from our robot, Yamaraz.
 * 
 * This file is free to copy, modify, and redistribute under the
 * terms specified in the COPYING file (its MIT).
 * 
 * Use at your own discretion.
 * Don't come to me if your robot is on fire!
 * 
 * Sincerely,
 * Nishan Rayamajhee
 */
  
/*
 * Yamaraz shall rule the Fury Road
 */


// 6.8 front front grabber;

int dispMatrix[7][7];

int end1[] = {-1, -1};
int end2[] = {-1, -1};
bool doneEnd = false;

void setup() {
  initYamaraz();
  go(FRONT, 12);
  gridSearch();
//  thump();
//openLid();
//returnHome();
}

/*
 * Yamaraz shall test its madness
 */

void loop() {
  // make sure to uncomment code at setup
  // before testing
//  openLid();
}

void initYamaraz() {
  initPins();
  lightUp();
  initTick();
  initThump();
  resetM();
}

/*
 * The path algorithm for 5x5 grid
 */

void gridSearch() {
  
  int x = 0;
  int y = 0;

  lightOn(0, 0, YELLOW);
  // turn the tick tracer on
  turnItOn();
  
  goDiag(FRONT, RIGHT, 19);
  correct(x, y);
  
  for (x = 1; x <= ROWS; x++) {
    
    // odd lap
    if (x % 2 == 1) {
      
      // go front
      for (y = 1; y < COLS; y++) {
        routine(x, y);
        go (FRONT, 12);
      }

    // even lap
    } else {
    
      // go reverse
      for (y = COLS; y > 1; y--) {
        routine(x, y);
        go (REAR, 12);
      }
    }


    routine(x, y);
    if(x < ROWS){
      go (SRIGHT, 12); 
    } else {
//      gotoEnd();
      returnHome();
    }
    
  }// end of lap
}

void gotoEnd() {
  routine(end1[0] + 1, end1[1] + 1);
  correct(end1[0] + 1, end1[1] + 1);
  routine(end1[0], end1[1]);
  go(REAR, 4);
  openLid();
  
  pinMode(33, OUTPUT);
  pinMode(34, INPUT);
  digitalWrite(33, HIGH);
  delay(5000);
  digitalWrite(33, LOW);
  
  if(digitalRead(34) == HIGH){
    routine(1, 1);
    goDiag(REAR, LEFT, 19);
  }
  else
  { 
    routine(end2[0] + 1, end2[1] + 1);
    correct(end2[0] + 1, end2[1] + 1);
    routine(end2[0], end2[1]);
    openLid();
    
    digitalWrite(33, HIGH);
    delay(5000);
    returnHome();
  }
}

void returnHome() {
  
//  correctPan(1, 1);
//  correct(1,1);
//  correctPan(1, 1);
//  correctPan(1, 1);
//  correct(1,1);
  go(LEFT, 90);
  go(FRONT, 48);
  align(RIGHT);
  go(LEFT, 90);
  go(FRONT, 48);
}

 
void routine(int x, int y) {

  Serial.print("(");
  Serial.print(x);
  Serial.print(" ");
  Serial.println(y);
  Serial.println(")");

  if( x != 3 && (y != 5 || y != 4))
    correct(x, y);
   
//  correctPan(x, y);
  detect(x, y);
  
  for(int x = 0; x < 7; x++){
    for(int y = 0; y < 7; y++){
      Serial.print(dispMatrix[x][y]);
      Serial.print("  ");
    }
    Serial.println();
  }
}

void detect(int x, int y) {
  tickIt(); // ignore the first one
  
  if(tickIt() == WIRED){
    lightOn(x, y, RED);
    dispMatrix[x][y] = WIRED;
    
    if (x == 1){
      go(SLEFT, 11);
      if(tickIt() == WIRED)
        lightOn(0, y, RED);
      go(SRIGHT, 11);
      
      correct(x,y);
//      correctPan(x,y);

    } else if (x == 5){
      go(SRIGHT, 11);
      if(tickIt() == WIRED)
        lightOn(6, y, RED);
      go(SLEFT, 11);
      
      correct(x,y);
//      correctPan(x,y);
    }

    if (y == 1){
      go(REAR, 11);
      if(tickIt() == WIRED)
        lightOn(x, 0, RED);
      go(FRONT, 11);
      
      correct(x,y);
//      correctPan(x,y);
      
    } else if (y == 5) {
      go(FRONT, 11);
      if(tickIt() == WIRED)
        lightOn(x, 6, RED);
      go(REAR, 11);
      
      correct(x,y);
//      correctPan(x,y);
    }
  
  } else {
    if(x == 2){
      go(SLEFT, 2);
      delay(250);
      
    } else if (x == 3) {
      go(SRIGHT, 2);
      delay(250);
    }
    
    int t1 = thump();
    int t2 = thump();

    if (t1 == HOLLOW && t2 == HOLLOW){
      lightOn(x, y, BLUE);
       dispMatrix[x][y] = 8;
    }
    else if (t1 == SOLID && t2 == SOLID) {
      lightOn(x, y, BLACK);
    }
    else {
        lightOn(x, y, BLACK);
    }
  }
}

void resetM() {
  for(int x = 0; x < 7; x++){
    for(int y = 0; y < 7; y++){
      dispMatrix[x][y] = SOLID;
    }
  }
}


//  int neighbours = 0;
//  for(int x = 0; x < 7; x++){
//    for(int y = 0; y < 7; y++){
//      if(dispMatrix[x][y] == WIRED){
//        
//        if(dispMatrix[x][y+1]){
//          neighbours++;
//        }
//        if(dispMatrix[x+1][y]){
//          neighbours++;
//        }
//        if(dispMatrix[x][y-1]){
//          neighbours++;
//        }
//        if(dispMatrix[x-1][y]){
//          neighbours++;
//        }
//      }
//    }
//
//    Serial.println();
//  }

