/*
 * Direction types
 * ---------------
 * left and right are rotation
 * sleft and sright are strafing
 */
enum Direction {
  FRONT,
  BACK,
  RIGHT,
  LEFT,
  SLEFT,
  SRIGHT,
  UP,
  DOWN,
  ALL
};
/*
* Debug flags
*/
struct Debug {
  bool steps;
  bool ir;
  bool servo;
  bool motion;
};
Debug debug   = {0, 0, 0, 0};
/*
 * The Motors have many states. All of these are mutable and are
 * updated by the interrupt service routines ISR on TIMER 3 and 4
 */
struct Motors {
  volatile bool running;
  volatile int steps;
  volatile long totalSteps;
  int maxSpeed;
  int minSpeed;
  volatile int speed;
  volatile float alignRatio;
};
Motors motors = {
  false, 
  0, 
  0,
  600,  // to mils delay
  1000, // from mils delay
  0,
  1     // turn ratio
};
/*
 * Sets the direction PINS form PORTL
 * left and right are rotation
 * sleft and sright are strafing
 */
void setDirection(Direction dir) {
  if (dir == FRONT) {
    PORTL = 0xA0;
  } else if (dir == BACK) {
    PORTL = 0x0A;
  } else if (dir == SLEFT) {
    PORTL = 0x88;
  } else if (dir == SRIGHT) {
    PORTL = 0x22;
  } else if (dir == LEFT) {
    PORTL = 0x00;
  } else if (dir == RIGHT) {
    PORTL = 0xAA;
  }
}
/*
 * Interrupt Service Routines
 */
void initTimers() {
  noInterrupts();
  // Timer 3
  // for left wheels
  TCCR3A = 0;
  TCCR3B = 0;
  TCNT3  = 0;
  TCCR3B |= (1 << WGM32);
  TCCR3B |= (1 << CS30);
  TIMSK3 |= (1 << OCIE3A);
  // Timer 4
  // for right wheels
  TCCR4A = 0;
  TCCR4B = 0;
  TCNT4  = 0;
  TCCR4B |= (1 << WGM42);
  TCCR4B |= (1 << CS40);
  TIMSK4 |= (1 << OCIE4A);
  // Timer 5
//  TCCR5A = 0;
//  TCCR5B = 0;
//  TCNT5  = 0;
//  TCCR5B |= (1 << WGM52);
//  TCCR5B |= (1 << CS50);
//  TIMSK5 |= (1 << OCIE5A);
  interrupts();
}
/*
 * This one modifies OCR?A which controls the interrupt duration
 * 
 */
void setTimers(Direction dir, int msDelay) {
  if (dir == LEFT)
    OCR3A  = 8 * msDelay;
  else if (dir == RIGHT)
    OCR4A  = 8 * msDelay;
  else if (dir == ALL) {
    OCR3A  = 8 * msDelay;
    OCR4A  = 8 * msDelay;
  }
}
/*
 * ISR for timer 3 LEFT MOTORS
 * responsible for accleration and step counting
 */
ISR(TIMER3_COMPA_vect) {
  // once motors.running is true this service routine takes control
  if (motors.running) {
    // this will change the speeds of both wheels because we
    // mutate the global volatile variable motors.speed
    
    if (motors.steps <= (int)(0.2 * motors.totalSteps) && (motors.speed > motors.maxSpeed)) {
      motors.speed -= ceil((motors.minSpeed - motors.maxSpeed) / (0.2 * motors.totalSteps));

    } else if (motors.steps >= (int)(0.8 * motors.totalSteps) && (motors.speed < motors.minSpeed)) {
      motors.speed += ceil((motors.minSpeed - motors.maxSpeed) / (0.2 * motors.totalSteps));
    }
//    
    // Toggle the left motors
    // and change duration for them
    setTimers(LEFT, motors.speed);
    PORTL ^= 0x05;
    // This interrupt is also incharge of updating the step count
    motors.steps++;
    // If no more steps to go, stop!
    if(motors.steps >= motors.totalSteps) {
      motors.running = false;
      motors.steps = 0;
      motors.totalSteps = 0;
    }
  }
}
/*
 * ISR for timer 4 RIGHT MOTORS
 * responsible for serial printing info
 */
ISR(TIMER4_COMPA_vect) {
  if (motors.running) {
    // Toggle the left motors
    // and change duration for them with the given ratio
    PORTL ^= 0x50;
    setTimers(RIGHT, motors.alignRatio * motors.speed);
    if(debug.steps) {
      Serial.print(" | ");
      Serial.print(motors.totalSteps);
      Serial.print(" # ");
      Serial.print(motors.steps);
      Serial.print(" : ");
      Serial.println(motors.speed);
    }
  }
}
