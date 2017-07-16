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
#define OE 6
#define Red 7
#define Green 8
#define CLK 9
#define STB 10

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
  randomSeed(1);
}

void loop()
{
  matrix.scan();
  AddRandomlyToQueue();
  AddNumbersToBuffer();
  ScrollMatrix();
}

void AddRandomlyToQueue() {
  static uint32_t lastCountTime = millis();
  static int nextWait = 0;
  if((millis() - lastCountTime) > nextWait) {
    lastCountTime = millis();
    nextWait = random(1000,3000);
    pushRandomRow(getRandomPhrase());
  }
}

string getRandomPhrase() {
  int row = random(1,6);
  switch(row) {
    case  1: return "wow!";
    case  2: return "epic!";
    case  3: return "rad!";
    case  4: return "LOL";
    default: return "haha";
  }
}

void pushRandomRow(string msg) {
  int row = random(1,5);
  switch(row) {
    case  1: row1.push(msg); break;
    case  2: row2.push(msg); break;
    case  3: row3.push(msg); break;
    default: row4.push(msg); break;
  }
}

void AddNumbersToBuffer() {
  static uint32_t lastCountTime = millis() + 1000;
  if (isInterval(lastCountTime)) {
    lastCountTime = millis();
    CallQueueMethods();
  }
}

bool isInterval(uint32_t lastCountTime) {
  return (millis() - lastCountTime) > SCROLLDELAY * 6;
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

// Scroll the matrix accross the screen and pull in from the prebuffer
void ScrollMatrix() {
  static uint32_t lastCountTime = 0;
  
  // Run every 100 milliseconds
  if ((millis() - lastCountTime) > SCROLLDELAY) {
    lastCountTime = millis();
    matrix.shiftMatrixLeft();
  }
}
