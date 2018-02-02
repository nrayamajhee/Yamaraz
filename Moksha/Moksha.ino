struct Motors {
  volatile bool running;
  int distance;
};

Motors motors = {false, 0};

enum Direction {
  front,
  back,
  right,
  left
};

void initTimers() {
  noInterrupts();
  // Timer 3
  TCCR3A = 0;
  TCCR3B = 0;
  TCNT3  = 0;
  TCCR3B |= (1 << WGM32);
  TCCR3B |= (1 << CS30);
  TIMSK3 |= (1 << OCIE3A);
  interrupts();
}

void setTimers(int msDelay) {
  OCR3A  = 8 * msDelay;
}

ISR(TIMER3_COMPA_vect) {
  if (motors.running) {
    PORTL ^= 0x55;
  }
}

void setDirection(Direction dir) {
  if (dir == front) {
    PORTL = 0x0A;
  } else if (dir == back) {
    PORTL = 0xA0;
  } else if (dir == left) {
    PORTL = 0x88;
  } else if (dir == right) {
    PORTL = 0x22;
  }
}

void initPorts() {
  DDRL = 0xFF;
  setDirection(front);
}

void initRobot() {
  Serial.begin(9600);
  initTimers();
  setTimers(500);
  initPorts();
}

void setup() {
  initRobot();
  setDirection(left);
  motors.running = true;
}

void loop()
{
}
