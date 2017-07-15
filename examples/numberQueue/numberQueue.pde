#include "LEDMatrix64.h"
#include "font5x7.h"
#include "NumberQueue.h"

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

int SCROLLDELAY = 100;

// LEDMatrix(a, b, c, d, oe, r1, stb, clk);
LEDMatrix64 matrix(RowA, RowB, RowC, RowD, OE, Red, Green, STB, CLK);

NumberQueue row1(0);
NumberQueue row2(8);
NumberQueue row3(16);
NumberQueue row4(24);

void setup()
{
  matrix.begin();
  matrix.clear();
  matrix.reverse();

  row1.push('h');
  row2.push('e');
  row3.push('y');
  row4.push('!');
}

void loop()
{
  matrix.scan();
  AddNumbersToBuffer();
  ScrollMatrix();
}

void AddNumbersToBuffer() {
  static uint32_t lastCountTime = 0;
  if (isInterval(lastCountTime)) {
    lastCountTime = millis();
    CallQueueMethods();
  }
}

//// Adds a number to the prebuffer, which is off screen
void CallQueueMethods() {
  if(row1.hasInQueue())
    matrix.printCharInBuffer(0, row1.rowNumber, row1.popQueue() - 32);
  if(row2.hasInQueue())
    matrix.printCharInBuffer(0, row2.rowNumber, row2.popQueue() - 32);
  if(row3.hasInQueue())
    matrix.printCharInBuffer(0, row3.rowNumber, row3.popQueue() - 32);
  if(row4.hasInQueue())
    matrix.printCharInBuffer(0, row4.rowNumber, row4.popQueue() - 32);
}

bool isInterval(uint32_t lastCountTime) {
  return (millis() - lastCountTime) > SCROLLDELAY * 6;
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
