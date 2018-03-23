void runServo(Direction dir) {
  int speed, steps;
  if (dir == UP) {
    speed = 500;
    steps = 300;
  } else if (dir == DOWN) {
    speed = 2500;
    steps = 300;
  }
  int cnt = 0;
  int highDelay = speed;
  int lowDelay = 20000 - speed;
  
  while (cnt < steps){
    digitalWrite(10, HIGH);
    delayMicroseconds(highDelay);
    digitalWrite(10, LOW);
    delayMicroseconds(lowDelay);
    cnt++;
  }
}

void pickUp() {
  digitalWrite(LIGHT_PIN, HIGH);
  runServo(DOWN);
  digitalWrite(MAGNET_PIN, LOW);
  delay(500);
  runServo(UP);
}

void drop() {
  runServo(DOWN);
  digitalWrite(MAGNET_PIN, HIGH);
  delay(500);
  runServo(UP);
}

