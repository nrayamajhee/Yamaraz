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
  float C_max = r;
  if(g > r){
    C_max = g;
  }else if(b > r){
    C_max = b;
  }
  float C_min = r;
    if(g < r){
    C_min = g;
  }else if(b < r){
    C_min = b;
  }
  float saturation  = (C_max - C_min)/C_max;
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
    Serial.print("Saturation: ");
    Serial.println(saturation);  
    Serial.println("Clear value");
    Serial.print(c);
    Serial.println(" ");
  }
//  return RED;
//  digitalWrite(30, LOW);
  if(hue < 35 && hue > 0.5) {
    if(saturation > 0.30){
      return RED;
    }else if(saturation < 0.35){
      return PURPLE;
    }
  }else if(hue > 55) {
    if(saturation > 0.25){
      return GREEN;
    }else if(saturation < 0.25){
      return BLUE;
    } 
  }else if(hue < 0.5) {
    return CYAN;  
  }else if(hue < 330 && hue > 270) {
    return PURPLE;
  }else if(hue > 35 && hue < 55){
    return YELLOW;
  }
}
