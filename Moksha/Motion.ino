#define TURN_SCALE    0.2

struct Motors {
  volatile bool running;
  volatile int steps;
  volatile long totalSteps;
  int maxSpeed;
  int minSpeed;
  volatile int speed;
  volatile float alignRatio;
  volatile bool accelerate;
};
Motors motors = {
  false,
  0,
  0,
  500,  // to misec delay
  2000, // from misec delay
  0,
  1,     // turn ratio
  1,    // accelerate
};

void set_direction(Direction dir) {
  if (dir == FRONT) {
    PORTL = 0x0A;
  } else if (dir == BACK) {
    PORTL = 0xA0;
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

void init_timers() {
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
  interrupts();
}

void set_timers(Direction dir, int msDelay) {
  if (dir == LEFT)
    OCR3A  = 8 * msDelay;
  else if (dir == RIGHT)
    OCR4A  = 8 * msDelay;
  else if (dir == ALL) {
    OCR3A  = 8 * msDelay;
    OCR4A  = 8 * msDelay;
  }
}
ISR(TIMER3_COMPA_vect) {
  // once motors.running is true this service routine takes control
  if (motors.running) {
    // this will change the speeds of both wheels because we
    // mutate the global volatile variable motors.speed
    if (motors.accelerate) {
      if (motors.steps <= (int)(0.2 * motors.totalSteps) && (motors.speed > motors.maxSpeed)) {
        motors.speed -= ceil((motors.minSpeed - motors.maxSpeed) / (0.2 * motors.totalSteps));

      } else if (motors.steps >= (int)(0.8 * motors.totalSteps) && (motors.speed < motors.minSpeed)) {
        motors.speed += ceil((motors.minSpeed - motors.maxSpeed) / (0.2 * motors.totalSteps));
      }
    }
    // Toggle the left motors
    // and change duration for them
    set_timers(LEFT, motors.speed);
    PORTL ^= 0x05;
    // This interrupt is also incharge of updating the step count
    motors.steps++;
    // If no more steps to go, stop!
    if (motors.steps >= motors.totalSteps) {
      motors.running = false;
      motors.steps = 0;
      motors.totalSteps = 0;
    }
  }
}
/*
   ISR for timer 4 RIGHT MOTORS
   responsible for serial printing info
*/
ISR(TIMER4_COMPA_vect) {
  if (motors.running) {
    set_timers(RIGHT, motors.alignRatio * motors.speed);
    PORTL ^= 0x50;
  }
  if (debug.steps) {
    Serial.print(" | ");
    Serial.print(motors.totalSteps);
    Serial.print(" # ");
    Serial.print(motors.steps);
    Serial.print(" : ");
    Serial.println(motors.speed);
  }
}
void set_steps(Direction dir, float amount) {
  // if negative angles
  if (amount < 0) {
    if (dir == RIGHT) dir = LEFT;
    else dir = RIGHT;
    amount = abs(amount);
  }
  set_direction(dir);
  if (dir == FRONT || dir == BACK) {
    motors.totalSteps = amount * 216; // linear calibration

  } else if (dir == SLEFT || dir == SRIGHT) {
    motors.totalSteps = amount * 228; // strafing calibration

  } else {
    motors.totalSteps = (int)ceil(amount * 23.1); // angular calibration
  }
  if (debug.motion) {
    Serial.print("Going ");
    Serial.print(dir);
    Serial.print(" by ");
    Serial.print(amount);
    Serial.print(" for ");
    Serial.print(motors.totalSteps);
    Serial.print(" with ");
    Serial.print(motors.totalSteps);
    Serial.print(" acceleartion ");
  }
}
void update_motors(bool correct, Direction dir) {
  while (motors.running == true) {
    if (correct) {
      if (dir == FRONT)
        motors.alignRatio = 1 + IR_calculate_offset() * TURN_SCALE;
      if (dir == BACK)
        motors.alignRatio = 1 + IR_calculate_offset_back() * TURN_SCALE;
    }
  }
}
void go(Direction dir, float amount, bool correct) {
  set_steps(dir, amount);
  // set slow speed and start the motors
  motors.accelerate = true;
  motors.speed = motors.minSpeed;
  motors.running = true;
  update_motors(correct, dir);
}
void go(Direction dir, float amount, int speed, bool correct) {
  set_steps(dir, amount);
  // set slow speed and start the motors
  motors.accelerate = true;
  motors.speed = motors.minSpeed;
  motors.maxSpeed = speed;
  motors.running = true;
  update_motors(correct, dir);
  motors.maxSpeed = 500;
}
void go_const(Direction dir, float amount, int speed, bool correct) {
  set_steps(dir, amount);
  motors.accelerate = false;
  motors.speed = speed;
  motors.running = true;
  update_motors(correct, dir);
}
void go_until_spokes(Direction dir, int steps, bool correct) {
  set_direction(dir);
  // will only run a maximum of 5000 steps as a safety precaus
  motors.totalSteps = 5000;
  motors.steps = 0;
  motors.accelerate = true;
  motors.speed = motors.minSpeed;
  motors.running = true;
  int counter = 0;
  bool countLock = false;
  while (motors.running == true) {
    if (IR_detect_spokes()) {
      if (!countLock) {
        counter++;
        countLock = true;
        motors.totalSteps = 5000;
        motors.steps = 0;
      }
      // deaccelerate for 3 in * 216 steps
      if (counter >= steps) {
        motors.speed = motors.maxSpeed;
        motors.totalSteps = 3240;
        motors.steps = 2592;
      }
    } else {
      countLock = false;
    }
    if (correct) {
      motors.alignRatio = 1 + IR_calculate_offset() * TURN_SCALE;
    }
  }
}
void go_until_spokes(Direction dir, int steps, int speed, bool correct) {
  set_direction(dir);
  // will only run a maximum of 5000 steps as a safety precaus
  motors.totalSteps = 5000;
  motors.steps = 0;
  motors.accelerate = false;
  motors.speed = speed;
  motors.running = true;
  int counter = 0;
  bool countLock = false;
  while (motors.running == true) {
    if (IR_detect_spokes()) {
      if (!countLock) {
        counter++;
        countLock = true;
        motors.totalSteps = 5000;
        motors.steps = 0;
      }
      // deaccelerate for 3 in * 216 steps
      if (counter >= steps) {
        motors.steps = 5000;
      }
    } else {
      countLock = false;
    }
    if (correct) {
      motors.alignRatio = 1 + IR_calculate_offset() * TURN_SCALE;
    }
  }
}
void strafe_align() {
  int average = 0;
  do {
    average = IR_calculate_offset();
    if (average > 0) {
      go_const(SRIGHT, 0.1, 5000, false);
    } else if (average < 0) {
      go_const(SLEFT, 0.1, 5000, false);
    }
  } while (average != 0);
}
void correct_right() {
  int count = 0;
  while (count < 5) {
    IR_filter();
    if (!ir.filteredValues[7])
      count++;
    go_const(SLEFT, 0.1, 2000, false);
  }
  go_const(SRIGHT, 5, 2000, false);
}

void correct_front() {
  int count = 0;
  while (count < 5) {
    IR_filter();
    if (!ir.filteredValues[3] || !ir.filteredValues[4])
      count++;
    go_const(FRONT, 0.1, 2000, false);
  }
  go_const(BACK, 3, 2000, false);
}

void correct_angle() {
  float sum = 0;
  float front = 0;
  float back = 0;
  do {
    front = IR_calculate_offset();
    back = IR_calculate_offset_back();
    sum = front - back;
    if (debug.angle) {
      Serial.print(front);
      Serial.print("\t");
      Serial.print(back);
      Serial.print("\t");
      Serial.print(" = ");
      Serial.println(sum);
    }
    if (sum < 0) {
      go_const(LEFT, 1, 2000, false);
    } else if (sum > 0) {
      go_const(RIGHT, 1, 2000, false);
    }
  } while (sum != 0);
}

