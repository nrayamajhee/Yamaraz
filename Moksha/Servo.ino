#define SERVO_PIN1 6
#define SERVO_PIN2 10
#define MAGNET_PIN    11

void run_servo(Direction dir) {
  int speed  = 0;
  int steps = 0;
  int pin = 0;
  if ((dir == RIGHT) || (dir == LEFT)) {
    pin = 1;
    if (dir == RIGHT) {
      speed = 2400;
      steps = 150;
    } else {
      speed = 1590;
      steps = 150;
    }
  } else if ((dir == UP)  || (dir == DOWN)) {
    pin = 2;
    if(dir == UP) {
      speed = 800;
      steps = 300;
    } else  {
      speed = 2150;
      steps = 300;
    }
  }
  int highDelay = speed;
  int lowDelay = 20000 - speed;
  for (int cnt =0; cnt < steps; cnt++){
    digitalWrite(pin, LOW);
    delayMicroseconds(lowDelay);
    digitalWrite(pin, HIGH);
    delayMicroseconds(highDelay);
  }
  digitalWrite(pin, LOW);
  delayMicroseconds(lowDelay);
}

void init_servo() {
  pinMode(SERVO_PIN1, OUTPUT);
  pinMode(SERVO_PIN2, OUTPUT);
  run_servo(RIGHT);
  run_servo(UP);
}


Color pick_up() {
  pinMode(MAGNET_PIN, OUTPUT);  // magenet output
  digitalWrite(MAGNET_PIN, LOW);
  run_servo(DOWN);
  run_servo(UP);
  run_servo(LEFT);
  Color found = calculate_color();
 run_servo(RIGHT);
  return found;
}

void drop() {
  pinMode(MAGNET_PIN, INPUT);  // magenet output
}

