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
  {20000, 30000, 30000}, //C
  {11000, 7700 , 10000}, //M
  {40500, 42500, 23500}, //Y
  {6000, 11500, 16700}, //B
  {14500, 23200, 12600}, //G
  {13000, 5400, 5000}, //R
  {5700, 7600, 7000}, //G
  {2500, 2500, 2400}  //IN
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
 r = r/100;
 b = b/100;
 g = g/100;
  float min = -1;
  int indx = 0;
  for(int i = 0; i < 8; i++) {
    long Rx = (colors[i].r)/100;
    long Bx = (colors[i].b)/100;
    long Gx = (colors[i].g)/100;

    
    long red = pow ((long)r -  Rx,2);
    long green =  pow ((long)g - Gx,2);
    long blue = pow ((long)b - Bx,2);
    
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
  digitalWrite(LIGHT_PIN, LOW);
  if(indx < 3)
    return indx + 1;
  else
    return indx + 2;
}
