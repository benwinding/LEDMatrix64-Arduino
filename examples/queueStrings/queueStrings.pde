#include "LEDMatrix64.h"
#include "font5x7.h"
#include "Queuer.h"

#define PREWIDTH  8
#define WIDTH   64
#define HEIGHT  32

#define RowA 2
#define RowB 3
#define RowC 4
#define RowD 5
#define STB 6
#define CLK 7
#define OE 8
#define Red 9
#define Green 10

int SCROLLDELAY = 100;

// LEDMatrix(a, b, c, d, oe, r1, stb, clk);
LEDMatrix64 matrix(RowA, RowB, RowC, RowD, OE, Red, Green, STB, CLK);

Queuer row1(0);
Queuer row2(8);
Queuer row3(16);
Queuer row4(24);

void setup()
{
  matrix.begin();
  matrix.clear();
  matrix.reverse();

  row3.push("oh ");
  row3.push("hi!");

  row4.push("how ");
  row4.push("are ");
  row4.push("you? ");
}

void loop()
{
  matrix.scan();
  AddNumbersToBuffer();
  ScrollMatrix();
}

void AddNumbersToBuffer() {
  static uint32_t lastCountTime = millis() + 1000;
  if (isInterval(lastCountTime)) {
    lastCountTime = millis();
    CallQueueMethods();
  }
}

//// Adds a number to the prebuffer, which is off screen
void CallQueueMethods() {
  if(row1.hasInQueue())
    matrix.printCharInBuffer(0, row1.rowNumber, row1.popQueueChar() - 32);
  if(row2.hasInQueue())
    matrix.printCharInBuffer(0, row2.rowNumber, row2.popQueueChar() - 32);
  if(row3.hasInQueue())
    matrix.printCharInBuffer(0, row3.rowNumber, row3.popQueueChar() - 32);
  if(row4.hasInQueue())
    matrix.printCharInBuffer(0, row4.rowNumber, row4.popQueueChar() - 32);
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
    matrix.shiftMatrixLeft();
  }
}

