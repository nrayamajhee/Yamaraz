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
    // and change duration  for them with the given ratio
//    if(motors.alignCounter < 1000) {
      setTimers(RIGHT, motors.alignRatio * motors.speed); 
//      motors.alignCounter++;
//    } else {
//      setTimers(RIGHT, motors.speed); 
//      motors.alignCounter = 0;
//    }
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
int spokes = 0;
void go(Direction dir, float amount, bool correct, bool accelerate, bool stopIfSpokes, bool stopIfCenter) {
  setDirection(dir);
  if (dir == FRONT || dir == BACK){
    motors.totalSteps = amount * 216; // linear calibration
    
  } else if(dir == SLEFT || dir == SRIGHT){
    motors.totalSteps = amount * 800; // strafing calibration

  } else {
    motors.totalSteps = (int)ceil(amount * 24.2); // angular calibration
  }

  if (stopIfSpokes || stopIfCenter) {
    motors.totalSteps = 5000;
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
  if(accelerate) {
    motors.accelerate = true;
    motors.speed = motors.minSpeed;
  } else {
    motors.speed = 5000;
    motors.accelerate = false;
  }
  
  motors.running = true;

  while(motors.running == true) {
    if(stopIfSpokes) {
      if (detect_spokes()) {
        spokes++;
        if(spokes >= 2) {
          motors.totalSteps = 3240;
          motors.steps = 2592;
        }
      }
    }
    if(stopIfCenter) {
      if(detect_center()) {
        motors.running = false;
      }
    }
    if(correct) {
      motors.alignRatio = 1 + (calculate_average() - 4.5) * .1;        //1 is calibrated value; Dr Gray says either use .25 or .35 for better control
//      Serial.println(motors.alignRatio);
//      motors.alignCounter = 0;
      //Serial.println(motors.alignRatio);
    }
  }
}

void goAccel(Direction dir, float amount, bool correct) {
  go(dir, amount, correct, true, false, false);
}

void goUntilSpokes(Direction dir, bool correct) {
  go(dir, 0, correct, true, true, false);
}

void goUntilFound(Direction dir, bool correct) {
  go(dir, 0, correct, false, false, true);
}

void goUntilReady(Direction dir, bool correct) {
  go(dir, 0, correct, false, false, true);
}

void goConst(Direction dir, float amount, bool correct) {
  go(dir, amount, correct, false, false, false);
}

void alignRobot(){
//  delay(250);
  float average = calculate_average();
//  Serial.println(average);
  if(average > 4.5){
    while(average > 4.5){
      goConst(SLEFT, 0.1, false);
      average = calculate_average();
    }
  }else if(average < 4.5){
    while(average < 4.5){
      goConst(SRIGHT, 0.1, false);
      average = calculate_average();
    }
  }
}

void goUntilReady() {
  motors.steps = 0;
  motors.totalSteps = 2000;
  motors.accelerate = false;
  motors.speed = 1000;
  motors.running = true;
  setDirection(BACK);

  while(motors.running) {
    
//      Serial.println(motors.alignRatio);
    if(correct_exit()) {
      motors.running = false;
    }
  }
}

void goTo(int direction, int steps) {
  goAccel(RIGHT, direction * 45, false );
    goUntilReady();
  goAccel(BACK, 40, true);
  goAccel(RIGHT, 180, true);
  alignRobot();
  goUntilSpokes(BACK, true);
  goUntilFound(FRONT, true);
  pickUp();
}

void beginCourse() {
  goAccel(BACK, 43, false);
  goConst(SRIGHT, 0.5, false);
  delay(500);
  for(int i = 5; i < 6; i++){
    if(i!=4){
      goTo(i, 48);
      delay(500);
      break;
    }
  }
}

void runPeriphery() {
  for(int i = 0; i < 4; i++) {
    goAccel(BACK, 60, true);
    delay(1000);
    goAccel(LEFT, 90, false);
  }
}
