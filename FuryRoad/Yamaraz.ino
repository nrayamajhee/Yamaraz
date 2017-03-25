/*
 * This is the bleeding edge version of our robot navigation code.
 * So naturally a lot of it is not tested and sometimes coded entirely away
 * from our roboto, Yamaraz.
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
//  gridSearch();


}

/*
 * Yamaraz shall test its madness
 */

void loop() {
  // make sure to uncomment code at setup
  // before testing
  getGrid();
}

/*
 * The path algorithm for 5x5 grid
 */

void gridSearch() {
  
  goDiag(FRONT, RIGHT, 16.9);
  delay(1000);

  int direction = FRONT;
  
  for (int x = 1; x <= 2; x++) {
    
    if (x % 2 == 1) {
      direction = FRONT;
    } else {
      direction = FRONT;
      
    }
    
    // go straight
    for (int y = 1; y <= 2; y++) {
      getGrid();
      go (direction, 12);
    }
    
    go (SRIGHT, 12);
    
  }// end of lap
}


void initYamaraz() {
  initPins();
  initTick();
  initLight();
  pinMode(thumper, OUTPUT);
}


/*
 * Take a U turn
 */
 
void uTurn(const int dir) {
  go(dir, 90);
  delay(500);
  go(FRONT, 12);
  delay(500);
  go(dir, 90);
  delay(500);
}

void correct() {
//  correct(LEFT);
//  if(getTick() > 1700){
//      thump();
//  }
}

bool getGrid() {
//  if (thump() == WIRED){/
    lightOn();
    delay(200);
    lightOff();
///  }
//  bool fromTick = false;
//  bool fromThumper = false;
//  
//  fromTick = tickIt();
//  fromThumper = (thump() == WIRED);
//
//  if(fromTick == fromThumper){
//    return true;
//  } else {
//    return fromThumper;
//  }
}

