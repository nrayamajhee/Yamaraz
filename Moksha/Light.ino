#include <Wire.h>
#include <math.h>
#include "Adafruit_TCS34725.h"

#define LIGHT_PIN 2

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_700MS, TCS34725_GAIN_4X);
int col = CYAN;

void init_light() {
  pinMode(LIGHT_PIN, OUTPUT);
  digitalWrite(LIGHT_PIN, LOW);
}

Color calculate_color() {
  digitalWrite(LIGHT_PIN, HIGH);
  delay(1000);
  uint16_t r, g, b, c;
  for(int i = 0; i < 2; i++)
    tcs.getRawData(&r, &g, &b, &c);

  float min = -1;
  int indx = 0;
  for(int i = 0; i < 8; i++) {
    float red = pow ((float)r - colors[i].r,2);
    float green =  pow ((float)g - colors[i].g,2);
    float blue = pow ((float)b - colors[i].b,2);
    float calc = red + green + blue;
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
  digitalWrite(LIGHT_PIN, LOW);
  if(indx < 3)
    return indx + 1;
  else
    return indx + 2;
//  col++;
//  if(col == HOME) col++;
//  return col;
}
