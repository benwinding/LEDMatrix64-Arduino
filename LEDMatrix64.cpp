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
 *  Copyright (c) 2013 Seeed Technology Inc.
 *  @auther     Yihui Xiong
 *  @date       Nov 8, 2013
 *  @license    MIT
 */

#include "Arduino.h"
#include "LEDMatrix64.h"
#include "font5x7.h"

LEDMatrix64::LEDMatrix64(uint8_t a, uint8_t b, uint8_t c, uint8_t d, uint8_t oe, uint8_t r1, uint8_t g1, uint8_t stb, uint8_t clk)
{
  this->clk = clk;
  this->r1 = r1;
  this->g1 = g1;
  this->stb = stb;
  this->oe = oe;
  this->a = a;
  this->b = b;
  this->c = c;
  this->d = d;

  mask = 0xff;
  state = 0;
}

void LEDMatrix64::begin()
{
  pinMode(a, OUTPUT);
  pinMode(b, OUTPUT);
  pinMode(c, OUTPUT);
  pinMode(d, OUTPUT);
  pinMode(oe, OUTPUT);
  pinMode(r1, OUTPUT);
  pinMode(g1, OUTPUT);
  pinMode(clk, OUTPUT);
  pinMode(stb, OUTPUT);

  state = 1;
}

void LEDMatrix64::drawPoint(uint16_t x, uint16_t y, uint8_t pixel)
{
  uint8_t *byte = displaybuf1 + x / 8 + y * WIDTH / 8;
  uint8_t  bit = x % 8;

  if (pixel) {
    *byte |= 0x80 >> bit;
  } else {
    *byte &= ~(0x80 >> bit);
  }
}

void LEDMatrix64::drawRect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t pixel)
{
  for (uint16_t x = x1; x < x2; x++) {
    for (uint16_t y = y1; y < y2; y++) {
      drawPoint(x, y, pixel);
    }
  }
}

void LEDMatrix64::drawImage(uint16_t xoffset, uint16_t yoffset, uint16_t width, uint16_t height, const uint8_t *image)
{
  for (uint16_t y = 0; y < height; y++) {
    for (uint16_t x = 0; x < width; x++) {
      const uint8_t *byte = image + (x + y * width) / 8;
      uint8_t  bit = 7 - x % 8;
      uint8_t  pixel = (*byte >> bit) & 1;

      drawPoint(x + xoffset, y + yoffset, pixel);
    }
  }
}

void LEDMatrix64::clear()
{
  uint8_t *ptr = displaybuf1;
  for (uint16_t i = 0; i < (WIDTH * HEIGHT / 8); i++) {
    *ptr = 0x00;
    ptr++;
  }
}

void LEDMatrix64::reversePreBuf()
{
  uint8_t *ptr = prebuf;
  for (uint16_t i = 0; i < (PREWIDTH * HEIGHT / 8); i++) {
    *ptr = 0xFF;
    ptr++;
  }
}

void LEDMatrix64::reverse()
{
  mask = ~mask;
}

uint8_t LEDMatrix64::isReversed()
{
  return mask;
}

void LEDMatrix64::scan()
{
  static uint8_t row = 0;  // from 0 to 15

  if (!state) {
    return;
  }
  
  uint8_t *head = displaybuf1 + row * (WIDTH / 8);
  for (uint8_t line = 0; line < 1; line++) {
    uint8_t *ptr = head;
    uint8_t *ptr2 = head + WIDTH * HEIGHT/8/2;

    for (uint8_t byte = 0; byte < (WIDTH / 8); byte++) {
      uint8_t pixels = *ptr;
      uint8_t pixels2 = *ptr2;
      ptr++;
      ptr2++;
      pixels = pixels ^ mask;     // reverse: mask = 0xff, normal: mask =0x00
      pixels2 = pixels2 ^ mask;     // reverse: mask = 0xff, normal: mask =0x00
      for (uint8_t bit = 0; bit < 8; bit++) {
        digitalWrite(clk, LOW);
        digitalWrite(r1, pixels & (0x80 >> bit));
        digitalWrite(g1, pixels2 & (0x80 >> bit));
        digitalWrite(clk, HIGH);
      }
    }
  }

  //digitalWrite(oe, HIGH);              // disable display
  digitalWrite(a, (row & 0x01));

  // select row
  digitalWrite(a, (row & 0x01));
  digitalWrite(b, (row & 0x02));
  digitalWrite(c, (row & 0x04));
  digitalWrite(d, (row & 0x08));

  // latch data
  digitalWrite(stb, LOW);
  digitalWrite(stb, HIGH);
  digitalWrite(stb, LOW);

  digitalWrite(oe, LOW);              // enable display

  row = (row + 1) & 0x0F;
}

void LEDMatrix64::on()
{
  state = 1;
}

void LEDMatrix64::off()
{
  state = 0;
  digitalWrite(oe, HIGH);
}


// Shift matrix right
void LEDMatrix64::shiftMatrix() {
  int i,j;
  uint8_t previousByte;
  uint8_t currentByte;

  for(i=HEIGHT-1; i>=0; i--) {
    for(j=7; j>=0; j--) {
      int byteLocation = i*WIDTH/8 + j;
      if(j==0) {
        int preLocation = i*PREWIDTH/8;
        previousByte = prebuf[preLocation];
        prebuf[preLocation] = previousByte >> 1;
      }
      else {
        previousByte = displaybuf1[byteLocation-1];
      }
      currentByte = displaybuf1[byteLocation];
      displaybuf1[byteLocation] = (previousByte << 7) | (currentByte >> 1);
    }
  }
}

// Shift matrix left
void LEDMatrix64::shiftMatrixLeft() {
  int i,j;
  int previousByte;
  int currentByte;

  Serial.begin(115200);

  for(i=0; i<HEIGHT; i++) {
    for(j=0; j<8; j++) {
      int byteLocation = i*WIDTH/8 + j;
      if(j==7) {
        int preLocation = i*PREWIDTH/8;
        previousByte = prebuf[preLocation];
        prebuf[preLocation] = previousByte << 1;
      }
      else {
        previousByte = displaybuf1[byteLocation+1];
      }
      currentByte = displaybuf1[byteLocation];
      displaybuf1[byteLocation] = (previousByte >> 7) | (currentByte << 1);
    }
  }
}

byte flipByte(byte c){
  char r=0;
  for(byte i = 0; i < 8; i++){
    r <<= 1;
    r |= c & 1;
    c >>= 1;
  }
  return r;
}

void LEDMatrix64::printCharInBuffer(int x, int y, int ch) {
  int yy;
  uint8_t readByte;
  uint8_t currentByte;
  uint8_t *pDst = prebuf + y * (PREWIDTH / 8) + x / 8;
  int fontSize = 7;
  for (yy=0; yy < fontSize; yy++)
  {
    readByte = pgm_read_byte(&font5x7[ch][yy]);
    currentByte = *pDst;
    *pDst = readByte | currentByte;
    pDst += PREWIDTH / 8;
  }
}

