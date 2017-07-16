#include "LEDMatrix64.h"
#include "font5x7.h"

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

int SCROLLDELAY = 200;

// LEDMatrix(a, b, c, d, oe, r1, stb, clk);
LEDMatrix64 matrix(RowA, RowB, RowC, RowD, OE, Red, Green, STB, CLK);

void setup()
{
  matrix.begin();
  matrix.clear();
  matrix.reverse();
}

void loop()
{
  matrix.scan();
  AddNumberToPreBuffer();
  ScrollMatrix();
}

// Adds a number to the prebuffer, which is off screen
void AddNumberToPreBuffer() {
  static uint32_t lastCountTime = 0;
  static uint8_t count = 0;
  // Run every 900 milliseconds
  if ((millis() - lastCountTime) > SCROLLDELAY * 6) {
    lastCountTime = millis();
    matrix.printCharInBuffer(0, 0, getCount(count, 0));
    matrix.printCharInBuffer(0, 8, getCount(count, 1));
    matrix.printCharInBuffer(0, 16, getCount(count, 2));
    matrix.printCharInBuffer(0, 24, getCount(count, 3));
    count++;
  }
}

// Get the counter for the numbers
int getCount(int count, int offset) {
  return (count + offset)% 96;
}

// Scroll the matrix accross the screen and pull in from the prebuffer
void ScrollMatrix() {
  static uint32_t lastCountTime = 0;
  
  // Run every 100 milliseconds
  if ((millis() - lastCountTime) > SCROLLDELAY) {
    lastCountTime = millis();
    matrix.shiftMatrix();
  }
}