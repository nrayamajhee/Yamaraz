#include <QTRSensors.h>
#define TIMEOUT       2500
#define EMITTER_PIN   3
#define IR_THRESHHOLD 700


struct IR {
  unsigned int  sensorValues  [NUM_SENSORS];
  bool          filteredValues[NUM_SENSORS];
};
IR ir;
IR ir2;

QTRSensorsRC qtrrc((unsigned char[]) {22, 23, 24, 25, 26, 27, 28, 29},
  NUM_SENSORS, TIMEOUT, EMITTER_PIN);

QTRSensorsRC qtrrc2((unsigned char[]) {30, 31, 32, 33, 34, 35, 36, 37},
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
void IR_filter_back() {
  qtrrc2.read(ir2.sensorValues);
  for (unsigned char i = 0; i < NUM_SENSORS; i++) {
    ir2.filteredValues[i] = (ir2.sensorValues[i] > IR_THRESHHOLD) ? 1 : 0;
  }
  if(debug.ir){
    for (unsigned char i = 0; i < NUM_SENSORS; i++) {
      Serial.print(ir2.filteredValues[i]);
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
      bool value = ir.filteredValues[i];
      if(value) {
        numActive++; 
        sum += value * (i + 1);
      }
    }
    
    float offset = 4.5 - (sum / numActive);
    if(numActive == 0 || numActive > 2 || abs(offset) == 0.5) {
      return 0; 
    }
    else {
      return offset;
    }
}
float IR_calculate_offset_back() {
    IR_filter_back();
    float sum = 0;
    float numActive = 0;
    for(int i = 0; i < NUM_SENSORS; i++){
      bool value = ir2.filteredValues[i];
      if(value) {
        numActive++; 
        sum += value * (i + 1);
      }
    }
    
    float offset = 4.5 - (sum / numActive);
    if(numActive == 0 || numActive > 2 || abs(offset) == 0.5) {
      return 0; 
    }
    else {
      return offset;
    }
}
bool IR_detect_spokes() {
  IR_filter();
  if((ir.filteredValues[0] || ir.filteredValues[1] || ir.filteredValues[2]) &&  (ir.filteredValues[5] || ir.filteredValues[6] || ir.filteredValues[7])) {
    return true;
  }
  return false;
}
