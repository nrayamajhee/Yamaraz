#include <Wire.h>
#include <math.h>
#include "Adafruit_TCS34725.h"

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_700MS, TCS34725_GAIN_1X);

void init_light() {
  pinMode(30, OUTPUT);
  digitalWrite(30, LOW);
  if (tcs.begin()) {
//    Serial.println("Found sensor");
  } else {
//    Serial.println("No TCS34725 found ... check your connections");
//    while (1);
  }
}

Color calculate_color() {
  uint16_t r, g, b, c;// colorTemp, lux;  
  tcs.getRawData(&r, &g, &b, &c);
  float hue = atan2(1.732*(g-b), 2*r-g-b) * (180 / 3.1415);
//  colorTemp = tcs.calculateColorTemperature(r, g, b);
//  lux = tcs.calculateLux(r, g, b);
  if(debug.light) {
    Serial.print(r);
    Serial.print("\t");
    Serial.print(g);
    Serial.print("\t");
    Serial.print(b);
    Serial.print(": ");
    Serial.println(hue);  
  }
  return random(7);
  digitalWrite(30, LOW);
//  if(hue < 30 || hue > 330) {
//    return RED;
//  } else if(hue < 50 && hue > 30) {
//    return YELLOW;  
//  } else if(hue < 150 && hue > 50) {
//    return GREEN;
//  } else if(hue < 210 && hue > 150) {
//    return CYAN;   
//  } else if(hue < 270 && hue > 210) {
//    return BLUE;
//  } else if(hue < 330 && hue > 270) {
//    return PURPLE;
//  }
}
