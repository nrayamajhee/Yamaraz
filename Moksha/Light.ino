#include <Wire.h>
#include <math.h>
#include "Adafruit_TCS34725.h"

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_700MS, TCS34725_GAIN_1X);
int col = CYAN;
Color calculate_color() {
  uint16_t r, g, b, c;
  for(int i = 0; i < 2; i++)
    tcs.getRawData(&r, &g, &b, &c);

  float min = -1;
  int indx = 0;
  for(int i = 0; i < 9; i++) {
    long red = pow ((long)r - colors[i].r,2);
    long green =  pow ((long)g - colors[i].g,2);
    long blue = pow ((long)b - colors[i].b,2);
    long calc = red + green + blue;
    if(debug.light){
      Serial.println();
      Serial.print("i = ");
      Serial.print(i);
      Serial.print(" = ");
      Serial.print(calc);
    }
    if (min == -1) min = calc;
    if (calc < min) {
      min = calc;
      indx = i;
    }
  }
  if(debug.light) {
    Serial.println();
    Serial.print(r);
    Serial.print("\t");
    Serial.print(g);
    Serial.print("\t");
    Serial.print(b);
    Serial.print("\t");
    Serial.print(c);
    Serial.print(": ");
    Serial.println(min);
  }
//  return indx + 1;
  col++;
  if(col == HOME) col++;
  return col;
}

/*
Color calculate_color() {
  uint16_t r, g, b, c;// colorTemp, lux;
  for(int i = 0; i< 3; i++)
    tcs.getRawData(&r, &g, &b, &c);
  float hue = atan2(1.732*(g-b), 2*r-g-b) * (180 / 3.1415);
  float C_max = r;

  if((g > r) && (g > b))
    C_max = g;
  else if((b > g) && (b > r))
    C_max = b;
  float C_min = r;
  if((g < r) && (g < b))
    C_min = g;
  else if((b < g) && (b < r))
    C_min = b;

  float min = -1;
  int indx = 0;
  for(int i = 0; i < 9; i++) {
    float calc = sqrt(pow((r - colors[i].r), 2) + pow((g - colors[i].g), 2) + pow((b - colors[i].b), 2));
    if (min == -1) min = calc;
    if (calc < min) {
      min = calc;
      indx = i;
    }
  }
  Serial.println(min);
  Serial.println(indx);
  return indx;
//  float saturation  = (C_max - C_min)/C_max;
//  colorTemp = tcs.calculateColorTemperature(r, g, b);
//  int lux = tcs.calculateLux(r, g, b);
  if(debug.light) {
    Serial.print(r);
    Serial.print("\t");
    Serial.print(g);
    Serial.print("\t");
    Serial.print(b);
    Serial.print("\t");
    Serial.println(c);
//    Serial.print(": ");
//    Serial.print(hue);
//    Serial.print("\t");
//    Serial.print(saturation);
//    Serial.print("\t");
//    Serial.println(lux);
//    Serial.println(" ");
  }
//  digitalWrite(LIGHT_PIN, LOW);
  return(RED);
//  if(hue < 35 && hue > 0.5) {
//    if(saturation > 0.30){
//      return RED;
//    }else if(saturation < 0.35){
//      return MAGENTA;
//    }
//  }else if(hue > 35) {
//    if(saturation > 0.25){
//      return GREEN;
//    }else if(saturation < 0.25){
//      return BLUE;
//    }
//  }else if(hue < 0.5) {
//    return CYAN;
//  }else if(hue < 330 && hue > 270) {
//    return MAGENTA;
//  }else if(hue > 35 && hue < 55){
//    return YELLOW;
//  }
}*/
