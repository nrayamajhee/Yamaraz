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

float getDis(const int dir){
  
  int a = 0, b = 0;
  float dis = 0;
  
  if(dir == FRONT){
    a = sonar[1].ping_median(7);
    b = sonar[3].ping_median(7);

    if (abs(a-b) > 50) {
      go(FRONT, 2);
      a = sonar[1].ping_median(7);
      b = sonar[3].ping_median(7);
    }
    
    // ((a + b) / 2 - offset) / per Inch
    dis = (float)((a + b) / 2) / 152.64;
    
  } else if(dir == REAR){
    a = sonar[5].ping_median(7);
    b = sonar[7].ping_median(7);
    
    // ((a + b) / 2 - offset) / per Inch
    dis = (float)((a + b) / 2) / 150;
    
  } else if(dir == LEFT){
    a = sonar[0].ping_median(7);
    b = sonar[4].ping_median(7);

    // ((a + b) / 2 - offset) / per Inch
    dis = (float)((a + b) / 2) / 151;
    
  } else if(dir == RIGHT){
    a = sonar[2].ping_median(7);
    b = sonar[6].ping_median(7);
    
    // ((a + b) / 2 - offset) / per Inch
    dis = (float)((a + b) / 2) / 153;
  }

  Serial.print("a = ");
  Serial.print(a);
  Serial.print(" b = ");
  Serial.println(b);
  Serial.print(" dis = ");
  Serial.println(dis);
    
  return dis;
}

void pan (const int dir, float destination){
  float dis = getDis(dir);

  if (dir == FRONT){
    
    if (destination > dis) {
      go(REAR, (destination - dis));
    
    } else {
      go(FRONT, (dis - destination));
    }
    
  } else if (dir == REAR) {
    
    if (destination > dis) {
      go(FRONT, (destination - dis));
    
    } else {
      go(REAR, (dis - destination));
    }
     
  } else if (dir == LEFT) {
    
    if (destination > dis) {
      go(SRIGHT, (destination - dis));
    
    } else {
      go(SLEFT, (dis - destination));
    }
  
  } else if (dir == RIGHT) {

    if (destination > dis) {
      go(SLEFT, (destination - dis));
    
    } else {
      go(SRIGHT, (dis - destination));
    }
  }
  
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

    // bad logic
    if(deg > 10) deg = 0;
    
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
    
    if(deg > 10) deg = 0;
    
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

void correct (int x, int y) {
  delay(200);
  int cnt = 0;
  int dir;
  
  if(x <= (ROWS / 2))
    dir = LEFT;
  else
    dir = RIGHT;
    
  while (align(dir) < 5) {
    cnt++;
    if (cnt > 3) break;
  }
  delay(200);
}

//void correctPan (int x, int y) {
//  delay(200);
//  if (x <= (ROWS / 2)) {
//    pan(LEFT, (x * 12) + 2.3);
//  
//  } else {
//    pan(RIGHT, ((ROWS + 1 - x) * 12) + 2.3);
//  }
//
//  if (y <= 4) {
//    pan(REAR, (y * 12) + 1.35);
//
//    if(getDis(REAR);
//    
//  
//  } else {
//    pan(FRONT, ((COLS + 1 - y) * 12) + 1.35);
//    
//  }
//  delay(200);
//}
//
//bool obstacle(x, y) {
//  if (x==1 && y < 5) {
//    align(LEFT);
//    align(LEFT);
//    pan(REAR, (y * 12) + 1.35);
//    
//    // yes there is obstacle
//    if(getDis(FRONT) < 6){
//      go(SRIGHT, 12);
//      go(FRONT, 24);
//      go(SLEFT, 12);
//
//      return true;
//      
//    } else {
//      return false;
//    }
//    
//  } if (x == 1 && y == 5) {
//    align(LEFT);
//    align(LEFT);
//    pan(TOP, 13.35);
//   
//    // yes there is obstacle
//    if(getDis(RIGHT) < 6){
//      go(REAR, 24);
//      go(SRIGHT, 12);
//
//      return true;
//      
//    } else {
//      return false;
//    }
//  }
//}

