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
//  correctPan(1, 1);
//go(FRONT, 12);/
// / turnItOn();
}

/*
 * Yamaraz shall test its madness
 */

void loop() {
  // make sure to uncomment code at setup
  // before testing
///Serial.println(getTick());
//align(REAR);/
}

/*
 * The path algorithm for 5x5 grid
 */

void gridSearch() {
  
  goDiag(FRONT, RIGHT, 19);

  int x = 1;
  int y = 1;

  // turn the tick tracer on
  turnItOn();
//  correct(x, y);
  
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

//    correct(x, y);
    routine(x, y);
    go (SRIGHT, 12);
    
  }// end of lap

//  returnHome(7,3);/
}


void returnHome(int x, int y) {
  go(LEFT, x-1);
  go(REAR, y-1);
  goDiag(REAR, LEFT, 19);
}


void initYamaraz() {
  initPins();
  lightUp();
  initTick();
  lightUp();
  pinMode(thumper, OUTPUT);
}


/*
 * Take a U turn
 */
 
void routine(int x, int y) {
  
  Serial.print(x);
  Serial.print(" ");
  Serial.println(y);
  
//  correctPan(x, y);
//  detect(x, y);
}

bool detect(int x, int y) {
  tickIt(); // ignore the first one
  
  if(tickIt() == WIRED){
    lightOn(x, y, RED);
  
  } else {
    if(thump() == HOLLOW){
      lightOn(x, y, BLUE);
    
    } else {
      
      lightOn(x, y, GREEN);
    }
  }
}

