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
  
//  Serial.begin(9600);
//getTick();

//go(FRONT, 12);
  gridSearch();
//calDiag(FRONT, LEFT);
}

/*
 * Yamaraz shall test its madness
 */

void loop() {
  // make sure to uncomment code at setup
  // before testing

//  Serial.println(sonar[0].ping_median());
//  Serial.println(sonar[1].ping_median());
//  Serial.println(sonar[2].ping_median());
//  Serial.println(sonar[3].ping_median());

//  Serial.println(getTick());
}

/*
 * The path algorithm for 5x5 grid
 */

void gridSearch() {
  
  goDiag(FRONT, RIGHT, 16.9);
  initTick();
  delay(1000);
  
  for (int x = 1; x <= 5; x++) {
    
    // if it is odd lap
    //-----------------
    if (x % 2 == 1) {
      
      // go straight
      for (int y = 1; y <= 4; y++) { deduce(); go (FRONT, 12); }

      // take a turn
//      uTurn(RIGHT);
      go (SRIGHT, 12);
      
    // if it is even lap
    //-----------------
    } else {
      
      // go straight
      for (int y = 1; y <= 4; y++) { deduce(); go (REAR, 12); }
      
      // take a turn
//      uTurn(LEFT);
      go (SRIGHT, 12);
    }
  
  }// end of lap
}

/*
 * Take a U turn
 */
 
void uTurn(const int dir) {
  thump();
  go(dir, 90);
  delay(500);
  go(FRONT, 12);
  delay(500);
  go(dir, 90);
  delay(500);
 }

void initYamaraz() {
  initPins();
  initTick();
  
  // Thumper Pinout
  pinMode(0, OUTPUT);
}

void deduce() {
  if(getTick() > 1700){
    thump();
  }
}

 void thump() {
  delay(500);
  digitalWrite(0, HIGH);
  delay(500);
  digitalWrite(0, LOW);
  delay(500);
 }

 void calDiag(const int dir1, const int dir2){
  thump();
  delay(1000);
  goDiag (dir1, dir2, 16.9);
  thump();

}

