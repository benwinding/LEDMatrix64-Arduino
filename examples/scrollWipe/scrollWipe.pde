/**
 * LED Matrix library for http://www.seeedstudio.com/depot/ultrathin-16x32-red-led-matrix-panel-p-1582.html
 * The LED Matrix panel has 32x16 pixels. Several panel can be combined together as a large screen.
 *
 * Coordinate & Connection (Arduino -> panel 0 -> panel 1 -> ...)
 *   (0, 0)                                     (0, 0)
 *     +--------+--------+--------+               +--------+--------+
 *     |   5    |    4   |    3   |               |    1   |    0   |
 *     |        |        |        |               |        |        |<----- Arduino
 *     +--------+--------+--------+               +--------+--------+
 *     |   2    |    1   |    0   |                              (64, 16)
 *     |        |        |        |<----- Arduino
 *     +--------+--------+--------+
 *                             (96, 32)
 *
 */

#include "LEDMatrix64.h"

#define PREWIDTH  8
#define WIDTH   64
#define HEIGHT  32

#define RowA_Pin 2
#define RowB_Pin 3
#define RowC_Pin 4
#define RowD_Pin 5
#define OE_Pin 6
#define Red_Pin 7
#define Green_Pin 8
#define CLK_Pin 9
#define STB_Pin 10

// LEDMatrix(a, b, c, d, oe, r1, stb, clk);
LEDMatrix64 matrix(RowA_Pin, RowB_Pin, RowC_Pin, RowD_Pin, OE_Pin, Red_Pin, Green_Pin, STB_Pin, CLK_Pin);

uint8_t displaybuf[WIDTH * HEIGHT / 8] = {}; // Display Buffer 128 = 64 * 16 / 8
uint8_t prebuf[PREWIDTH * HEIGHT / 8] = {
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
};  // Pre buffer (off screen)

void setup()
{
  matrix.begin(displaybuf, WIDTH, HEIGHT);
  matrix.clear();
}

void loop()
{
  matrix.scan();
  ScrollMatrix();
}

// Get the counter for the numbers
int getCount() {
  static uint8_t count = 0;
  if(count > 9)
    count = 0;
  return count++;
}

// Scroll the matrix accross the screen and pull in from the prebuffer
void ScrollMatrix() {
  static uint32_t lastCountTime = 0;
  
  // Run every 100 milliseconds
  if ((millis() - lastCountTime) > 100) {
    lastCountTime = millis();
    shiftMatrix(displaybuf, prebuf);
  }
}

// Shift matrix
void shiftMatrix(uint8_t *displaybuf, uint8_t *prebuf) {
  int i,j;
  uint8_t previousByte;
  uint8_t currentByte;

  for(i=HEIGHT-1; i>=0; i--) {
    for(j=7; j>=0; j--) {
      int byteLocation = i*WIDTH/8 + j;
      if(j==0) {
        previousByte = prebuf[i*PREWIDTH/8];
        prebuf[i*PREWIDTH/8] = previousByte >> 1;
      }
      else
        previousByte = displaybuf[byteLocation-1];
      currentByte = displaybuf[byteLocation];
      displaybuf[byteLocation] = (previousByte << 7) | (currentByte >> 1);
    }
  }
}