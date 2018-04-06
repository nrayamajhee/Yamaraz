#define SERVO_PIN1 6
#define SERVO_PIN2 10
#define MAGNET_PIN    14

void run_servo(Direction dir) {
  int speed  = 0;
  int steps = 0;
  int pin = 0;
 if ((dir == RIGHT) || (dir == LEFT)) {
    pin = SERVO_PIN1;
    if (dir == RIGHT) {
      speed = 2400;
      steps = 150;
    } else {
      speed = 1645;
      steps = 150;
    }
  } else if ((dir == UP)  || (dir == DOWN)) {
    pin = SERVO_PIN2;
    if(dir == UP) {
      speed = 960;
      steps = 300;
    } else  {
      speed = 2250;
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
  run_servo(DOWN);
  digitalWrite(MAGNET_PIN, HIGH);
  pinMode(MAGNET_PIN, INPUT);  // magenet output
  run_servo(UP);
  delay(200);
}

