struct Servo {
  volatile bool state;
  volatile int count;
  volatile int maxCount;
};

Servo servo = { false, 0, 0};

void runServo(Direction dir) {
  int speed, steps;
  if (dir == UP) {
    speed = 300;
    steps = 2000;
  } else if (dir == DOWN) {
    speed = 2500;
    steps = 200;
  }
  int cnt = 0;
  while (cnt < steps){
    digitalWrite(10, HIGH);
    delayMicroseconds(speed);
    digitalWrite(10, LOW);  
    delayMicroseconds(speed);
    cnt++;
  }
}
