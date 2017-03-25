// New Ping Library
#include <NewPing.h>

// max distance for ultrasonic
const int MAX_DISTANCE = 200;

// ultrasonic pinouts
const int echoFL  = 18;
const int trigFL  = 19;
const int echoFFL = 20;
const int trigFFL = 21;

const int echoFR  = 16;
const int trigFR  = 17;
const int echoFFR = 8;
const int trigFFR = 9;

const int trigRL = 36;
const int echoRL = 35;
const int trigRR = 34;
const int echoRR = 33;


const int echoRRL = 14;
const int trigRRL = 15;
const int echoRRR = 37;
const int trigRRR = 38;

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
int align (const int dir){
  int a = 0, b = 0;
  double aIn = 0.0, bIn = 0.0;
  double diff = 0.0, errorDeg = 0.0;
  
  if (dir == LEFT) {
    
    a = sonar[0].ping_median(7) - 248;
    b = sonar[4].ping_median(7) - 182;
    
    aIn = (double) a / 141.25;
    bIn = (double) b / 146.66;
    
    diff = abs(aIn - bIn);
    errorDeg = (180 / 3.14) * asin(diff / 7); // the sensors are 7 inch apart
    
    if (errorDeg < 45) {
      if (a < b) go (RIGHT, (int)errorDeg);
      if (a > b) go (LEFT, (int)errorDeg);
    }

  } else if (dir == RIGHT) {
    
    a = sonar[2].ping_median(7) - 178;
    b = sonar[6].ping_median(7) - 183;
    
    aIn = (double) a / 145.33;
    bIn = (double) b / 147.66;
    
    diff = abs(aIn - bIn);
    errorDeg = (180 / 3.14) * asin(diff / 7); // the sensors are 7 inch apart

    if (errorDeg < 45) {
      if (a > b) go (RIGHT, (int)errorDeg);
      if (a < b) go (LEFT, (int)errorDeg);
    }
  }

  Serial.print("a = ");
  Serial.print(a);
  Serial.print(" b = ");
  Serial.println(b);
  Serial.print("aIn = ");
  Serial.print(aIn);
  Serial.print(" bIn = ");
  Serial.println(bIn);
  Serial.print("diff = ");
  Serial.print(diff);
  Serial.print(" errorDeg = ");
  Serial.println(errorDeg);
  
  return (int) errorDeg;
}

// correct the placement within the grid with this method

void correct (const int dir) {
  int cnt = 0;
  while (align (RIGHT) < 5) {
    cnt++;
    if (cnt > 5) break;
  }
}

