struct Motors {
  volatile bool running;
  volatile int distance;
  volatile int speed;
};

Motors motors = {false, 0, 0};

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
    motors.distance++;
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
  setTimers(motors.speed);
  initPorts();
}

//acceleration function
void go(int inches){
  int steps = inches * 870;     //870 is calibrated value
  motors.speed = 1200;
  motors.running = true;
  while(motors.distance <= steps){

    
    if(motors.distance <= ((int)(0.2 * steps)) && (motors.speed > 300)){
      Serial.println("Reached if");
      motors.speed -= 6;    //exponential banaune
      setTimers(motors.speed);
    }
    if(motors.distance >= ((int)(0.8 * steps))){
      Serial.println("Reached deacce");
      setTimers(motors.speed += 4);   //exponential banaune
    }
    //do nothing
    if(motors.distance >= steps){
      motors.running = false;
      motors.distance = 0;
    }
  }
}

void setup() {
  initRobot();
  setDirection(back);
  delay(500);
  go(15);
}

void loop()
{
}
