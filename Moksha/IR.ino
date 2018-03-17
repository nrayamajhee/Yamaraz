#include <QTRSensors.h>

QTRSensorsRC qtrrc((unsigned char[]) {22, 23, 24, 25, 26, 27, 28, 29},
  NUM_SENSORS, TIMEOUT, EMITTER_PIN);

void calibrate_IR() {
  int calArray[] = {240, 180, 124, 180, 184, 248, 240, 300};
  qtrrc.calibrate();
  for(int i = 0; i < NUM_SENSORS; i++) {
    qtrrc.calibratedMinimumOn[i] = calArray[i];
    qtrrc.calibratedMaximumOn[i] = TIMEOUT;
  }
}

void debug_IR() {
  qtrrc.read(ir.sensorValues);
  for (unsigned char i = 0; i < NUM_SENSORS; i++) {
    Serial.print(ir.sensorValues[i]);
    Serial.print('\t');
  }
  Serial.println();
}

//int cnt = 0;

void IR_filter() {
  qtrrc.read(ir.sensorValues);
  for (unsigned char i = 0; i < NUM_SENSORS; i++) {
    ir.filteredValues[i] = (ir.sensorValues[i] > IR_THRESHHOLD) ? 1 : 0;
  }
  if(debug.ir){
    for (unsigned char i = 0; i < NUM_SENSORS; i++) {
      Serial.print(ir.filteredValues[i]);
      Serial.print('\t');
    }
  }
  Serial.println();
}
//
//float calculate_average() {
//  IR_filter();
//  float right = ir.filteredValues[0] * 4 + ir.filteredValues[1] * 3 + ir.filteredValues[2] * 2;
//  float left =  ir.filteredValues[7] * 4 + ir.filteredValues[6] * 3 + ir.filteredValues[5] * 2;
//  return left - right;
//}

float calculate_average() {
  IR_filter();
    float avg = 4.5;
    float sum = ir.filteredValues[0] + 2*ir.filteredValues[1] + 3*ir.filteredValues[2] + 4*ir.filteredValues[3]+5*ir.filteredValues[4] + 6*ir.filteredValues[5] + 7*ir.filteredValues[6] + 8*ir.filteredValues[7];

    float onCount = 0;

    for(int i = 0; i < 8; i++){
      if(ir.filteredValues[i] == 1){
        onCount++;
      }
    }

    if(onCount == 0){
      onCount = 4.5;
    }

    float newAverage = sum / onCount;

    if(onCount > 2){
      newAverage = 4.5;
    }
    return newAverage;
    //Serial.println(motors.alignRatio);

}

bool detect_spokes() {
  if((ir.filteredValues[3] || ir.filteredValues[4]) &&  (ir.filteredValues[0] || ir.filteredValues[7])) {
    return true;
  }
  return false;
}

bool detect_center() {
  if(ir.filteredValues[2] && ir.filteredValues[5]){
    return true;
  }
  return false;
}

bool correct_exit() {
  IR_filter();
  if((ir.filteredValues[3] || ir.filteredValues[4]) && (!ir.filteredValues[0] && !ir.filteredValues[1] && !ir.filteredValues[2] && !ir.filteredValues[5] && !ir.filteredValues[6] && !ir.filteredValues[7])){
    return true;
  }
  if(!ir.filteredValues[7] && !ir.filteredValues[6] && !ir.filteredValues[5] && (ir.filteredValues[4] && ir.filteredValues[3] && ir.filteredValues[2] && ir.filteredValues[1] && ir.filteredValues[1])){
    motors.alignRatio= 1.9;
  } else if(!ir.filteredValues[0] && !ir.filteredValues[1] && !ir.filteredValues[2] && (ir.filteredValues[3] && ir.filteredValues[4] && ir.filteredValues[5] && ir.filteredValues[6] && ir.filteredValues[7])){
    motors.alignRatio= 0.1;
  } else {
    motors.alignRatio = 1;
  }
  return false;
}

