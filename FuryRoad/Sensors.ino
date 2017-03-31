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
 
// New Ping Library
#include <NewPing.h>

// max distance for ultrasonic
const int MAX_DISTANCE = 200;

// Setup pinouts for ultrasonic sensors
NewPing sonar[8] = {
  NewPing(trigFL, echoFL, MAX_DISTANCE),
  NewPing(trigFFL, echoFFL, MAX_DISTANCE),
  NewPing(trigFR, echoFR, MAX_DISTANCE),
  NewPing(trigFFR, echoFFR, MAX_DISTANCE),
  NewPing(trigRL, echoRL, MAX_DISTANCE),
  NewPing(trigRRL, echoRRL, MAX_DISTANCE),
  NewPing(trigRR, echoRR, MAX_DISTANCE),
  NewPing(trigRRR, echoRRR, MAX_DISTANCE)
};

/*
 * Ultraonic method for alignment
 */

void pan (const int dir, int destination){
  int a = 0, b = 0;
  int dis = 0;

  if (dir == FRONT){

    a = sonar[1].ping_median(7);
    b = sonar[3].ping_median(7);

    // ((a + b) / 2 - offset) / per Inch
    dis = ((a + b) / ( 2 * 142)) - 1;

    if (destination > dis) {
      go(REAR, (destination - dis));
    
    } else {
      go(FRONT, (dis - destination));
    }
    
  } else if (dir == LEFT) {
    a = sonar[0].ping_median(7);
    b = sonar[4].ping_median(7);

    // ((a + b) / 2 - offset) / per Inch
    dis = ((a + b) / ( 2 * 142)) - 1;

    if (destination > dis) {
      go(SRIGHT, (destination - dis));
    
    } else {
      go(SLEFT, (dis - destination));
    }
  
  } else if (dir == RIGHT) {

    a = sonar[2].ping_median(7);
    b = sonar[6].ping_median(7);

    dis = (a + b) / (2 * 154) - 1;

    if (destination > dis) {
      go(SLEFT, (destination - dis));
    
    } else {
      go(SRIGHT, (dis - destination));
    }
  }

  Serial.print("a = ");
  Serial.print(a);
  Serial.print(" b = ");
  Serial.println(b);
  Serial.print("dest = ");
  Serial.print(destination);
  Serial.print(" dis = ");
  Serial.println(dis);
}

int align(const int dir) {
  int a = 0;
  int b = 0;
  int deg = 0;

  if (dir == FRONT){

    a = sonar[1].ping_median(7);
    b = sonar[3].ping_median(7);

    int diff = abs(b-a) - 30; // 30 is the difference in reading between the right sensors

    deg = (180 / 3.14) * asin((double)diff / 1050);

    if (a > b) {
      go(RIGHT, deg); 
    } else if (b > a) {
      go (LEFT, deg);
    }
    
  } else if (dir == RIGHT) {
    a = sonar[2].ping_median(7);
    b = sonar[6].ping_median(7);

    int diff = abs(b-a) - 30; // 30 is the difference in reading between the right sensors

    deg = (180 / 3.14) * asin((double)diff / 1050);

    if (a > b) {
      go(RIGHT, deg); 
    } else if (b > a) {
      go (LEFT, deg);
    }
  } else if (dir == LEFT) {
    a = sonar[0].ping_median(7);
    b = sonar[4].ping_median(7);

    int diff = abs(b-a) - 4; // 4 is the difference in reading between the left sensors

    deg = (180 / 3.14) * asin((double)diff / 1050);

    if (a > b) {
      go(LEFT, deg); 
    } else if (b > a) {
      go (RIGHT, deg);
    }
  } else if (dir == REAR) {
    a = sonar[5].ping_median(7);
    b = sonar[7].ping_median(7);

    int diff = abs(b-a) - 4; // 4 is the difference in reading between the left sensors

    deg = (180 / 3.14) * asin((double)diff / 1050);

    if (a > b) {
      go(LEFT, deg); 
    } else if (b > a) {
      go (RIGHT, deg);
    }
  }

  Serial.println(a);
  Serial.println(b);
  Serial.println(deg);
  Serial.println();
  return deg;
 
}

// correct the placement within the grid with this method

void correct (int x, int y) {
  int cnt = 0;
  int dir;
  
  if(x < (ROWS / 2))
    dir = LEFT;
  else
    dir = RIGHT;
    
  while (align(dir) < 20) {
    cnt++;
    if (cnt > 3) break;
  }
}

void correctPan (int x, int y) {
  if (x <= (ROWS / 2)){
    pan(LEFT, (x * 12) + 2);
  } else {
    pan(RIGHT, ((ROWS - x) * 12) + 2);
  }
}

