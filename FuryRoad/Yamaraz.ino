/*
 * This is the bleeding edge version of our robot navigation code.
 * So naturally a lot of it is not tested and coded entirely away
 * from the physical FuryRoad (officially called Yamaraz).
 * 
 * This file is free to copy, modify, and redistribute under the
 * terms specified in the LICENSE file (its MIT).
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
  initPins();
//  Serial.begin(9600);
  gridSearch();
//  digitalWrite(23, HIGH);
}

/*
 * Yamaraz shall test its madness
 */

void loop() {
  // make sure to uncomment code at setup
  // before testing

// Testing Ultrasonics

//  Serial.println(sonar[0].ping_median());
//  Serial.println(sonar[1].ping_median());
//  Serial.println(sonar[2].ping_median());
//  Serial.println(sonar[3].ping_median());

}

/*
 * The path algorithm for 5x5 grid
 */

void gridSearch() {
  goDiag(FRONT, RIGHT, 16);
  
  for (int x = 1; x <= 5; x++) {
    for (int y = 1; y <= 4; y++) {
      thump();
      go (FRONT, 12);
    }

    if (x % 2 == 1) {
      uTurn(RIGHT);
    
    } else {
      uTurn(LEFT);
    
    }
  }
 }

 /*
  * Take a L-shaped turn
  */

 void lTurn(const int dir) {
  go (FRONT, 12);
  delay(500);
  go (dir, 90);
  delay(500);
  go (FRONT, 12);
  delay(500);
}

/*
 * Take a full U turn
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

 void thump() {
  delay(500);
  digitalWrite(23, HIGH);
  delay(500);
  digitalWrite(23, LOW);
  delay(500);
 }


