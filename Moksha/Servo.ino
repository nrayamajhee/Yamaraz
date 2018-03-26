void run_servo(Direction dir) {
  int speed  = 0;
  int steps = 0;
  if (dir == UP) {
    speed = 500;
    steps = 300;
  } else if (dir == DOWN) {
    speed = 2500;
    steps = 300;
  }
  int highDelay = speed;
  int lowDelay = 20000 - speed;
  for (int cnt =0; cnt < steps; cnt++){
    digitalWrite(10, HIGH);
    delayMicroseconds(highDelay);
    digitalWrite(10, LOW);
    delayMicroseconds(lowDelay);
  }
}

void pick_up() {
  pinMode(MAGNET_PIN, OUTPUT);  // magenet output
  digitalWrite(LIGHT_PIN, HIGH);
  digitalWrite(MAGNET_PIN, LOW);
  run_servo(DOWN);
  run_servo(UP);
}

void drop() {
  run_servo(DOWN);
  pinMode(MAGNET_PIN, INPUT);  // magenet output
  run_servo(UP);
}

