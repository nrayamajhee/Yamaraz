// direction flags
const int FRONT    = 0;
const int REAR     = 1;
const int LEFT     = 2;
const int RIGHT    = 3;
const int SLEFT  = 4;
const int SRIGHT = 5;

// teensy pinouts
const int SFR = 0;
const int DFR = 1;
const int SFL = 2;
const int DFL = 3;
const int DRL = 4;
const int SRL = 5;
const int DRR = 6;
const int SRR = 7;

void initPins() {
  
  // set all the pins to OUTPUT

//  DDRD = 11111111;/
  
  pinMode(0, OUTPUT);
  pinMode(1, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  
  
  pinMode(21, OUTPUT);
  pinMode(22, OUTPUT);
  pinMode(23, OUTPUT);

  // micro stepping 1/4th is 010
  digitalWrite(21, LOW);
  digitalWrite(22, HIGH);
  digitalWrite(23, LOW);

}

// this function sets the directions
void setDirection (const int dir) {
  
  if (dir == FRONT) {
    
    // forward direction 
    digitalWrite(DFL, HIGH);
    digitalWrite(DFR, LOW);
    digitalWrite(DRL, HIGH);
    digitalWrite(DRR, LOW);
      
  } else if (dir == REAR) {
    
    // rear direction 
    digitalWrite(DFL, LOW);
    digitalWrite(DFR, HIGH);
    digitalWrite(DRL, LOW);
    digitalWrite(DRR, HIGH);
  
  } else if (dir == LEFT) {
    
    // left direction 
    digitalWrite(DFL, LOW);
    digitalWrite(DFR, LOW);
    digitalWrite(DRL, LOW);
    digitalWrite(DRR, LOW);
  
  } else if (dir == RIGHT) {
    
    // right direction 
    digitalWrite(DFL, HIGH);
    digitalWrite(DFR, HIGH);
    digitalWrite(DRL, HIGH);
    digitalWrite(DRR, HIGH);
  
  } else if (dir == SLEFT) {
    
    // left direction 
    digitalWrite(DFL, LOW);
    digitalWrite(DFR, LOW);
    digitalWrite(DRL, HIGH);
    digitalWrite(DRR, HIGH);
  
  } else if (dir == SRIGHT) {
    
    // right direction 
    digitalWrite(DFL, HIGH);
    digitalWrite(DFR, HIGH);
    digitalWrite(DRL, LOW);
    digitalWrite(DRR, LOW);
  }
}

// this function drives the motors forward/back
void go (const int dir, int val) {
  
  setDirection (dir);
  
  int steps = 0;

  // steps per inch
  if (dir == FRONT || dir == REAR) {

    // the val is in inches
    steps = 107.8 * val;

    for (int i = 0; i < steps; i++) {
      
      digitalWrite(SFL, HIGH);
      digitalWrite(SFR, HIGH); 
      digitalWrite(SRL, HIGH);
      digitalWrite(SRR, HIGH);
      
      delayMicroseconds(500);
      
      digitalWrite(SFL, LOW);
      digitalWrite(SFR, LOW);
      digitalWrite(SRL, LOW);
      digitalWrite(SRR, LOW);
  
      delayMicroseconds(500);
    }

  // steps per degree 
  } else if (dir == LEFT || dir == RIGHT) {

    // val is in degrees
    steps = 11.32 * val;

     for (int i = 0; i < steps; i++) {
      
      digitalWrite(SFL, HIGH);
      digitalWrite(SFR, HIGH); 
      digitalWrite(SRL, HIGH);
      digitalWrite(SRR, HIGH);
      
      delayMicroseconds(300);
      
      digitalWrite(SFL, LOW);
      digitalWrite(SFR, LOW);
      digitalWrite(SRL, LOW);
      digitalWrite(SRR, LOW);
  
      delayMicroseconds(300);
    }
  } else if (dir == SLEFT || dir == SRIGHT) {

    // val is in degrees
    steps = 113 * val;

     for (int i = 0; i < steps; i++) {
      
      digitalWrite(SFL, HIGH);
      digitalWrite(SFR, HIGH); 
      digitalWrite(SRL, HIGH);
      digitalWrite(SRR, HIGH);
      
      delayMicroseconds(300);
      
      digitalWrite(SFL, LOW);
      digitalWrite(SFR, LOW);
      digitalWrite(SRL, LOW);
      digitalWrite(SRR, LOW);
  
      delayMicroseconds(300);
    }
  }
}

void goDiag (const int dir1, const int dir2, int val) {
  
  setDirection(dir1);

  // val is in degrees
  int steps = 158 * val;

  if (dir2 == LEFT) {

    for (int i = 0; i < steps; i++) {
  
      digitalWrite(SFR, HIGH);
      digitalWrite(SRL, HIGH);
      
      delayMicroseconds(300);
      
      digitalWrite(SFR, LOW);
      digitalWrite(SRL, LOW);
  
      delayMicroseconds(300);
    }
  } else if (dir2 == RIGHT) {
    
    for (int i = 0; i < steps; i++) {
  
      digitalWrite(SFL, HIGH);
      digitalWrite(SRR, HIGH);
      
      delayMicroseconds(300);
      
      digitalWrite(SFL, LOW);
      digitalWrite(SRR, LOW);
  
      delayMicroseconds(300);
    }
  }
}

void setup() {
  initPins();
  goDiag(FRONT, RIGHT,16);
  delay(1000);
}

void loop() {
}
