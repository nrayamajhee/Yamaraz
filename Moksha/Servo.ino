#define SERVO_PIN1 6
#define SERVO_PIN2 10

void get_servo(Direction dir) {
  int speed  = 0;
  int steps = 0;
  if (dir == RIGHT) {
    speed = 2400;
    steps = 150;
  } else if (dir == LEFT) {
    speed = 1590;
    steps = 150;
  }
  int highDelay = speed;
  int lowDelay = 20000 - speed;
  for (int cnt =0; cnt < steps; cnt++){
    digitalWrite(SERVO_PIN1, LOW);
    delayMicroseconds(lowDelay);
    digitalWrite(SERVO_PIN1, HIGH);
    delayMicroseconds(highDelay);
  }
  digitalWrite(SERVO_PIN1, LOW);
  delayMicroseconds(lowDelay);
}

void run_servo(Direction dir) {
  int speed  = 0;
  int steps = 0;
  if (dir == UP) {
    speed = 800;
    steps = 300;
  } else if (dir == DOWN) {
    speed = 2150;
    steps = 300;
  }
  int highDelay = speed;
  int lowDelay = 20000 - speed;
  for (int cnt =0; cnt < steps; cnt++){
    digitalWrite(SERVO_PIN2, LOW);
    delayMicroseconds(lowDelay);
    digitalWrite(SERVO_PIN2, HIGH);
    delayMicroseconds(highDelay);
  }
  digitalWrite(SERVO_PIN1, LOW);
  delayMicroseconds(lowDelay);
}

void init_servo() {
  pinMode(SERVO_PIN1, OUTPUT);
  pinMode(SERVO_PIN2, OUTPUT);
  run_servo(UP);
  get_servo(RIGHT);
}


Color pick_up() {
  pinMode(MAGNET_PIN, OUTPUT);  // magenet output
  digitalWrite(MAGNET_PIN, LOW);
  run_servo(DOWN);
  run_servo(UP);
  get_servo(LEFT);
  Color found = calculate_color();
  get_servo(RIGHT);
  return found;
}

void drop() {
  get_servo(RIGHT);
  run_servo(DOWN);
  pinMode(MAGNET_PIN, INPUT);  // magenet output
  run_servo(UP);
}

