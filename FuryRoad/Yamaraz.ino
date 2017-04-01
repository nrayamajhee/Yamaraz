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

void setup() {
  initYamaraz();
  gridSearch();
//  routine(3,3);
//go (FRONT, 12);
//routine(1,3);
//turnItOn();
}

/*
 * Yamaraz shall test its madness
 */

void loop() {
  // make sure to uncomment code at setup
  // before testing
//pan(FRONT, 24);
//getDis(FRONT);
//getDis(REAR);
//Serial.println(getTick());
}

/*
 * The path algorithm for 5x5 grid
 */

void gridSearch() {
  
  int x = 0;
  int y = 0;

  lightOn(x, y, GREEN);
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
    go (SRIGHT, 12);
    
  }// end of lap
}


void returnHome(int x, int y) {
  go(LEFT, x-1);
  go(REAR, y-1);
  goDiag(REAR, LEFT, 19);
}

 
void routine(int x, int y) {
//  Serial.print(x);
//  Serial.print(" ");
//  Serial.println(y);
  correct(x, y);
  correctPan(x, y);
  detect(x, y);

//  handleObstacle();
}

void detect(int x, int y) {
  tickIt(); // ignore the first one
  
  if(tickIt() == WIRED){
    lightOn(x, y, RED);
  
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
    }
    else if (t1 == SOLID && t2 == SOLID) {
      lightOn(x, y, BLACK);
    }
    else {
      if( thump() == HOLLOW){
        lightOn(x, y, BLUE);
      } else {
        lightOn(x, y, BLACK);
      }
    }
  }
}

void initYamaraz() {
  initPins();
  lightUp();
  initTick();
  initThump();
  lightUp();
}
