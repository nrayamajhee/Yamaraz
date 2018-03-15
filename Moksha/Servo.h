struct Servo {
  volatile bool state;
  volatile int count;
  volatile int maxCount;
};

Servo servo = { false, 0, 0};

void runServo(Direction dir) {
  int speed, steps;
  if (dir == UP) {
    speed = 500;
  } else if (dir == DOWN) {
    speed = 2500;
  }
  int cnt = 0;
  int highDelay = speed;
  int lowDelay = 20000 - speed;
  
  while (cnt < 300){
    digitalWrite(10, HIGH);
    delayMicroseconds(highDelay);
    digitalWrite(10, LOW);
    delayMicroseconds(lowDelay);
    cnt++;
  }
}

void pickUp() {
  runServo(DOWN);
  digitalWrite(11, LOW);
  delay(500);
  runServo(UP);
  delay(500);
  digitalWrite(11, HIGH);
  delay(500);
}

