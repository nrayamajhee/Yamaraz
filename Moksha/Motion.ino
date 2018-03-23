void setDirection(Direction dir) {
  if (dir == FRONT) {
    PORTL = 0xA0;
  } else if (dir == BACK) {
    PORTL = 0x0A;
  } else if (dir == SLEFT) {
    PORTL = 0x22;
  } else if (dir == SRIGHT) {
    PORTL = 0x88;
  } else if (dir == LEFT) {
    PORTL = 0x00;
  } else if (dir == RIGHT) {
    PORTL = 0xAA;
  }
}

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
  interrupts();
}

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
ISR(TIMER3_COMPA_vect) {
  // once motors.running is true this service routine takes control
  if (motors.running) {
    // this will change the speeds of both wheels because we
    // mutate the global volatile variable motors.speed

    if(motors.accelerate) {
      if (motors.steps <= (int)(0.2 * motors.totalSteps) && (motors.speed > motors.maxSpeed)) {
        motors.speed -= ceil((motors.minSpeed - motors.maxSpeed) / (0.2 * motors.totalSteps));
  
      } else if (motors.steps >= (int)(0.8 * motors.totalSteps) && (motors.speed < motors.minSpeed)) {
        motors.speed += ceil((motors.minSpeed - motors.maxSpeed) / (0.2 * motors.totalSteps));
      }
    }
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
    setTimers(RIGHT, motors.alignRatio * motors.speed); 
    PORTL ^= 0x50;
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
void goConst(Direction dir, float amount, int speed, bool correct) {
  setDirection(dir);
  
  if (dir == FRONT || dir == BACK){
    motors.totalSteps = amount * 216; // linear calibration
    
  } else if(dir == SLEFT || dir == SRIGHT){
    motors.totalSteps = amount * 228; // strafing calibration

  } else {
    motors.totalSteps = (int)ceil(amount * 24); // angular calibration
  }
  
  motors.speed = speed;
  motors.accelerate = false;
  
  motors.running = true;

  while(motors.running == true) {
    if(correct) {
      motors.alignRatio = 1 + (calculate_average() - 4.5) * .1;
    }
  }
}
void goUntilSpokes(Direction dir, bool correct, int steps) {
  setDirection(dir);
  motors.totalSteps = 2000;
  motors.accelerate = true;
  motors.speed = motors.minSpeed;
  motors.running = true;
  int counter = 0;
  bool counted = false;
  while(motors.running == true) {
    if (detect_spokes()) {
      if(!counted) {
        counter++;
        counted = true;
        motors.totalSteps = 2000;
        motors.steps = 0;
      }
      // deaccelerate for 3 in * 216 steps
      if(counter >= steps){
        motors.speed = motors.maxSpeed;
        motors.totalSteps = 3240;
        motors.steps = 2592;  
      }
    } else {
      counted = false;
    }
    if(correct) {
      motors.alignRatio = 1 + (calculate_average() - 4.5) * .1;
    }
  }
}
void goAccel(Direction dir, float amount, bool correct) {
  setDirection (dir);
  if (dir == FRONT || dir == BACK){
    motors.totalSteps = amount * 216; // linear calibration
    
  } else if(dir == SLEFT || dir == SRIGHT){
    motors.totalSteps = amount * 228; // strafing calibration

  } else {
    motors.totalSteps = (int)ceil(amount * 24.2); // angular calibration
  }

  if(debug.motion){
    Serial.print("Going ");
    Serial.print(dir);
    Serial.print(" by ");
    Serial.print(amount);
    Serial.print(" for ");
    Serial.println(motors.totalSteps);
  }

  // set slow speed and start the motors
  motors.accelerate = true;
  motors.speed = motors.minSpeed;
  motors.running = true;

  while(motors.running == true) {
    if(correct) {
      motors.alignRatio = 1 + (calculate_average() - 4.5) * .1;        //1 is calibrated value; Dr Gray says either use .25 or .35 for better control
    }
  }
}

void alignRobot(){
  float average = calculate_average();
  if(average > 4.5){
    while(average > 4.5){
      goConst(SLEFT, 0.1, 5000, false);
      average = calculate_average();
    }
  }else if(average < 4.5){
    while(average < 4.5){
      goConst(SRIGHT, 0.1, 5000, false);
      average = calculate_average();
    }
  }
}
void correctRight() {
  int count = 0;
  while(count < 5) {
    IR_filter();
    if(!ir.filteredValues[7])
      count++;
    goConst(SLEFT, 0.1, 2000, false);
  }
  goConst(SRIGHT, 6.5, 2000, false);
}

void correctFront() {
  int count = 0;
  while(count < 5) {
    IR_filter();
    if(!ir.filteredValues[3] || !ir.filteredValues[4])
      count++;
    goConst(BACK, 0.1, 2000, false);
  }
  goConst(FRONT, 3, 2000, false);
}