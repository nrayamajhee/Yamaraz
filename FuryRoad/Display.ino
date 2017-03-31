/*
 * This is the bleeding edge version of our robot navigation code.
 * So naturally a lot of it is not tested and sometimes coded entirely away
 * from our robot, Yamaraz.
 * 
 * This file is free to copy, modify, and redistribute under the
 * terms specified in the COPYING file (its MIT).
 * 
 * Use at your own discretion.
 * Don't come to me if your robot is on fire!
 * 
 * Sincerely,
 * Nishan Rayamajhee
 */

#include <Adafruit_NeoPixel.h>
#include <Adafruit_NeoMatrix.h>
#include <gamma.h>
#include <Adafruit_GFX.h>

//color definitions
#define BLACK    0x0000
#define BLUE     0x001F
#define RED      0xF800
#define GREEN    0x07E0
#define CYAN     0x07FF
#define MAGENTA  0xF81F
#define YELLOW   0xFFE0 
#define WHITE    0xFFFF

//Adafruit LED matrix
/*
  first two args are dimensions of array, 8x8
  third arg is arduino pin the array is connected to 
  last arg is for orientation of array - 
  BOTTOM can be changed to TOP, RIGHT to LEFT, COLUMNS to ROWS
*/
Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(8, 8, LEDpin,
  NEO_MATRIX_TOP     + NEO_MATRIX_LEFT +
  NEO_MATRIX_COLUMNS + NEO_MATRIX_PROGRESSIVE);

void lightUp(){
  matrix.begin();
  matrix.setBrightness(10);
  for(int x = 0; x < 8; x++) {
    for(int y = 0; y < 8; y++){
      matrix.drawPixel(x,y,CYAN); 
    }
  }
  matrix.show();
  delay(1000);
  for(int x = 0; x < 8; x++) {
    for(int y = 0; y < 8; y++){
      matrix.drawPixel(x,y,BLACK); 
    }
  }
  matrix.show();
}

void lightOn(int x, int y, int color) {
  //change brightness of array, values 0-255 (off - retina death)
  matrix.setBrightness(10);

//write to a pixel with predefined colors - (x,y,color)
  matrix.drawPixel(x,y,color);

//show
  matrix.show();
}
