#include <QTRSensors.h>

QTRSensorsRC qtrrc((unsigned char[]) {22, 23, 24, 25, 26, 27, 28, 29},
  NUM_SENSORS, TIMEOUT, EMITTER_PIN);

void IR_calibrate() {
  int calArray[] = {240, 180, 124, 180, 184, 248, 240, 300};
  qtrrc.calibrate();
  for(int i = 0; i < NUM_SENSORS; i++) {
    qtrrc.calibratedMinimumOn[i] = calArray[i];
    qtrrc.calibratedMaximumOn[i] = TIMEOUT;
  }
}

void IR_display_raw() {
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
    Serial.println();
  }
}
float IR_calculate_offset() {
    IR_filter();
    float sum = 0;
    float numActive = 0;
    for(int i = 0; i < NUM_SENSORS; i++){
      sum += ir.filteredValues[i] * (i + 1);
      numActive++;
    }
    if(numActive == 0)
      return 0;
    else
      return (sum / numActive) - 4.5;
}
bool IR_detect_spokes() {
  IR_filter();
  if((ir.filteredValues[3] || ir.filteredValues[4]) &&  (ir.filteredValues[0] || ir.filteredValues[7])) {
    return true;
  }
  return false;
}

//bool correct_exit() {
//  IR_filter();
//  if((ir.filteredValues[3] || ir.filteredValues[4]) && (!ir.filteredValues[0] && !ir.filteredValues[1] && !ir.filteredValues[2] && !ir.filteredValues[5] && !ir.filteredValues[6] && !ir.filteredValues[7])){
//    return true;
//  }
//  if(!ir.filteredValues[7] && !ir.filteredValues[6] && !ir.filteredValues[5] && (ir.filteredValues[4] && ir.filteredValues[3] && ir.filteredValues[2] && ir.filteredValues[1] && ir.filteredValues[1])){
//    motors.alignRatio= 1.9;
//  } else if(!ir.filteredValues[0] && !ir.filteredValues[1] && !ir.filteredValues[2] && (ir.filteredValues[3] && ir.filteredValues[4] && ir.filteredValues[5] && ir.filteredValues[6] && ir.filteredValues[7])){
//    motors.alignRatio= 0.1;
//  } else {
//    motors.alignRatio = 1;
//  }
//  return false;
//}

