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
  giveMeFive();
}

/*
 * Yamaraz shall test its dangers
 */

void loop() {
  // make sure to uncomment code at setup
  // before testing
}

/*
 * The path algorithm for 5x5 grid
 */

void giveMeFive() {
  for (int x = 1; x <= 2; x++) {
    for (int y = 1; y <= 3; y++) {
      go (FRONT, 12);
      delay(1000);
    }

    if (x % 2 == 1) {
      go (RIGHT, 90);
      delay(1000);
      go (FRONT, 12);
      delay(1000);
      go (RIGHT, 90);
      delay(1000);
    } else {
      go (LEFT, 90);
      delay(1000);
      go (FRONT, 12);
      delay(1000);
      go (LEFT, 90);
      delay(1000);
    }
  }
 }

