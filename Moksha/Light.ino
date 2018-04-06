#include <Wire.h>
#include <math.h>
#include "Adafruit_TCS34725.h"

#define LIGHT_PIN 2

struct RGB {
  long r;
  long g;
  long b;
};

RGB colors[8] = {
  {43, 105, 114}, //C
  {104, 73, 91}, //M
  {125, 122, 69}, //Y
  {49, 87, 127}, //B
  {77, 114, 65}, //G
  {156, 60, 58}, //R
  {75, 96, 91}, //G
  {93, 93, 86}  //IN
};

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_700MS, TCS34725_GAIN_1X);

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
  long red =(long) r *255 / c;
  long green =(long) g * 255 / c;
  long blue = (long)b * 255 / c;
  float min = -1;
  int indx = 0;
  for(int i = 0; i < 8; i++) {
    float calc = pow(red - colors[i].r, 2) + pow(green - colors[i].g, 2) + pow(blue - colors[i].b, 2);
    Serial.println();
    Serial.print("i = ");
    Serial.print(i);
    Serial.print(" = ");
    Serial.print(calc);
    if (min == -1) min = calc;
    if (calc < min) {
      min = calc;
      indx = i;
    }
  }
    Serial.println();
    Serial.print(red);
    Serial.print("\t");
    Serial.print(green);
    Serial.print("\t");
    Serial.print(blue);
    Serial.print("\t");
    Serial.print(c);
    Serial.print(": ");
    Serial.println(min);
  digitalWrite(LIGHT_PIN, LOW);
  if(indx < 3)
    return indx + 1;
  else
    return indx + 2;
} 
