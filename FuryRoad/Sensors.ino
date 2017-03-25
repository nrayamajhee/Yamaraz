const int SOLID = 0;
const int WIRED = 1;
const int HOLLOW = 2;

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


// tick tracer pinout
const int tickPin    = 23;
const int tickButton = 7;


// thumper pinout
const int thumper = 29;
const int micro = 8;

// thumper threashold
const int posThresh = 550;
const int negThresh = 400;

// LED
const int LEDpin = 6;
Adafruit_NeoPixel strip;


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

void correct (const int dir) {
  int cnt = 0;
  while (align (RIGHT) < 5) {
    cnt++;
    if (cnt > 5) break;
  }
}

int thump() {
  int counter = 0;
  int arrayIndex = 20000;
  int getData[arrayIndex];

  digitalWrite(thumper, HIGH);        //hit
  delay(25);                          //wait for the thumper to be close to the ground
  for (int i = 0; i < arrayIndex; i++) {
    getData[i] = analogRead(8);       //sample data
    if (getData[i] > posThresh|| getData[i] < negThresh)         //if the sampled data is greater than threshold increase counter
    counter++;
  }
  delay(100);
  
  digitalWrite(thumper,LOW);     //take it up
  delay(500);
  Serial.print("Counter = ");
  Serial.println(counter);          //Display counter. It is different for hollow and foam

  if (counter < 2000) {
    
    Serial.println("Its solid babe!");
    return SOLID;
    
  } else if (counter < 2500) {
    
     Serial.println("Don't kill me!");
     return WIRED;
     
  } else {
    
     Serial.println("It smells fishy in here!");
     return HOLLOW;
  }
}

void initLight() {
  strip = Adafruit_NeoPixel (60, LEDpin, NEO_GRB + NEO_KHZ800);
  strip.begin();
  strip.show();
}

void lightOn() {
  for(int i = 0; i < 56; i++) {
    if(i % 8 != 0)
      strip.setPixelColor(i,5,0,0);
  }
  strip.show();
}

void lightOff() {
  for(int i = 0; i < 56; i++) {
    if(i % 8 != 0)
      strip.setPixelColor(i,0,0,0);
  }
  strip.show();
}

