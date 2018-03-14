#include <QTRSensors.h>
#define NUM_SENSORS   8
#define TIMEOUT       2500
#define EMITTER_PIN   2
#define IR_THRESHHOLD 700
/*
 * QTRSensor 8 IR with EMITTER
 */
QTRSensorsRC qtrrc((unsigned char[]) {22, 23, 24, 25, 26, 27, 28, 29},
  NUM_SENSORS, TIMEOUT, EMITTER_PIN);
/*
 * The IR structure provides RAW sensor values and values filtered as ON or OFF
 */
struct IR {
  unsigned int  sensorValues  [NUM_SENSORS];
  bool          filteredValues[NUM_SENSORS];
};
IR ir = {0, 0};
/*
 * Read and filter out sensor values
 */
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
//  delay(250);
}

float IR_Average(){
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
}

