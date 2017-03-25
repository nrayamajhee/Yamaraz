#include <Adafruit_NeoPixel.h>

// LED
const int LEDpin = 6;

// Adafruit LED
Adafruit_NeoPixel strip;

// init the light
void initLight() {
  strip = Adafruit_NeoPixel (60, LEDpin, NEO_GRB + NEO_KHZ800);
  strip.begin();
  strip.show();
}

// turn the light on
void lightOn() {
  for(int i = 0; i < 56; i++) {
    if(i % 8 != 0)
      strip.setPixelColor(i,5,0,0);
  }
  strip.show();
}

void lightOff() {
  for(int i = 0; i < 56; i++) {
    if(i % 8 != 0)
      strip.setPixelColor(i,0,0,0);
  }
  strip.show();
}

