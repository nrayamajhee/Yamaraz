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
