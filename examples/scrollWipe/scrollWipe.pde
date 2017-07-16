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

#define RowA 2
#define RowB 3
#define RowC 4
#define RowD 5
#define OE 6
#define Red 7
#define Green 8
#define CLK 9
#define STB 10

// LEDMatrix(a, b, c, d, oe, r1, stb, clk);
LEDMatrix64 matrix(RowA, RowB, RowC, RowD, OE, Red, Green, STB, CLK);

void setup()
{
  matrix.begin();
  matrix.reversePreBuf();
}

void loop()
{
  matrix.scan();
  ScrollMatrix();
}

// Scroll the matrix accross the screen and pull in from the prebuffer
void ScrollMatrix() {
  static uint32_t lastCountTime = 0;
  
  // Run every 100 milliseconds
  if ((millis() - lastCountTime) > 100) {
    lastCountTime = millis();
    matrix.shiftMatrix();
  }
}
