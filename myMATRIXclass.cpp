/*
File	: myMATRIXclass.cpp
Version	: 1.1
Date	: 29.01.2017
Project	: myMatrix Arduino Library

The MIT License (MIT)

Copyright (c) 2015 Silviu - www.openhardware.ro

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#if ARDUINO < 100
#include "WProgram.h" // include core Wiring API
#endif

// include this library's description file
#include "myMATRIXclass.h"

// include description files for other libraries used (if any)
//#include "HardwareSerial.h"

// Constructor /////////////////////////////////////////////////////////////////
// Function that handles the creation and setup of instances
myMATRIX::myMATRIX()
{

}

// Public Methods //////////////////////////////////////////////////////////////

void myMATRIX::Init(uint8_t pinRed, uint8_t pinGreen, uint8_t pinClock, 
    uint8_t pinRowA, uint8_t pinRowB, uint8_t pinRowC, uint8_t pinRowD,
    uint8_t pinOE, uint8_t pinSTB)

{
  row = 0;
  pinMode(pinRed, OUTPUT); 
  pinMode(pinGreen, OUTPUT); 
  pinMode(pinClock, OUTPUT); 
  pinMode(pinRowA, OUTPUT); 
  pinMode(pinRowB, OUTPUT); 
  pinMode(pinRowC, OUTPUT); 
  pinMode(pinRowD, OUTPUT); 
  pinMode(pinOE, OUTPUT); 
  pinMode(pinSTB, OUTPUT); 
  
  bitRed = digitalPinToBitMask(pinRed);
  uint8_t portRed = digitalPinToPort(pinRed);
  outRed = portOutputRegister(portRed);

  bitGreen = digitalPinToBitMask(pinGreen);
  uint8_t portGreen = digitalPinToPort(pinGreen);
  outGreen = portOutputRegister(portGreen);

  bitClock = digitalPinToBitMask(pinClock);
  uint8_t portClock = digitalPinToPort(pinClock);
  outClock = portOutputRegister(portClock);

  bitRowA = digitalPinToBitMask(pinRowA);
  uint8_t portRowA = digitalPinToPort(pinRowA);
  outRowA = portOutputRegister(portRowA);

  bitRowB = digitalPinToBitMask(pinRowB);
  uint8_t portRowB = digitalPinToPort(pinRowB);
  outRowB = portOutputRegister(portRowB);

  bitRowC = digitalPinToBitMask(pinRowC);
  uint8_t portRowC = digitalPinToPort(pinRowC);
  outRowC = portOutputRegister(portRowC);

  bitRowD = digitalPinToBitMask(pinRowD);
  uint8_t portRowD = digitalPinToPort(pinRowD);
  outRowD = portOutputRegister(portRowD);

  bitOE = digitalPinToBitMask(pinOE);
  uint8_t portOE = digitalPinToPort(pinOE);
  outOE = portOutputRegister(portOE);

  bitSTB = digitalPinToBitMask(pinSTB);
  uint8_t portSTB = digitalPinToPort(pinSTB);
  outSTB = portOutputRegister(portSTB);

  clearScreen(); // clear buffer
  timer2Setup();
}

void myMATRIX::setPixel(uint8_t x ,uint8_t y, uint8_t color) //color 2 Bit, (R)ed (G)reen 0b000000RG
{  
  uint8_t myindex = (y*8)+x/8;
  uint8_t mybitmask = 7 -(x % 8);

  if (color & 0b00000010) // red
  {
    bitWrite(matrixBufferRed[myindex],mybitmask,1);
  }
  else
  {
    bitWrite(matrixBufferRed[myindex],mybitmask,0);
  }
  if (color & 0b00000001) //green
  {
    bitWrite(matrixBufferGreen[myindex],mybitmask,1);
  }
  else
  {
    bitWrite(matrixBufferGreen[myindex],mybitmask,0);
  }  
}

void myMATRIX::fillRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t color)
{
  for (uint16_t x = x1; x <= x2; x++) {
    for (uint16_t y = y1; y <= y2; y++) {
      setPixel(x,y,color);      
    }
  }
}

void myMATRIX::drawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t color)
{
  drawHLine(x1,x2,y1,color);
  drawHLine(x1,x2,y2,color);  
  drawVLine(x1,y1,y2,color);
  drawVLine(x2,y1,y2,color);
}

void myMATRIX::drawVLine(uint16_t x, uint16_t y1, uint16_t y2, uint8_t color)
{
  for (uint16_t y = y1; y <= y2; y++) {
    setPixel(x,y,color);      
  }
}

void myMATRIX::drawHLine(uint16_t x1, uint16_t x2, uint16_t y, uint8_t color)
{
  for (uint16_t x = x1; x <= x2; x++) {
    setPixel(x,y,color);      
  }  
}

void myMATRIX::clearScreen()
{
  for (uint8_t i=0; i<8*16; i++)
  {
    matrixBufferRed[i]=0;
    matrixBufferGreen[i]=0; 
  }
}

void myMATRIX::printChar(uint8_t x,uint8_t y, uint8_t For_color, uint8_t Bk_color, char ch)
{
  uint8_t xx,yy;
  xx=0;
  yy=0;
  ch = ch-32;
  for (yy=0; yy < 7; yy++)
  {
    for (xx=0; xx < 5; xx++)
    {
      if (bitRead(pgm_read_byte(&font5x7[ch][yy]),4-xx)) // 4 == Font witdh -1      
      {
        setPixel(x+xx,y+yy,For_color);
      }
      else
      {
        setPixel(x+xx,y+yy,Bk_color);        
      }
    }
  }  
}

void myMATRIX::printString(uint8_t x, uint8_t y, uint8_t For_color, uint8_t Bk_color,char *p)
{
  while(*p!='\0')
  {       
    printChar(x,y,For_color,Bk_color,*p);
    x+=6; // 6 = font width + 1 pixel space
    p++;
  }
}

void myMATRIX::hScroll(uint8_t y, uint8_t For_color, uint8_t Bk_color,char *mystring)
{
  int offset =0;
  
  for (offset=0; offset <((lenString(mystring)-5)*6-1); offset++)
  {
    for (byte xx=0; xx<64; xx++)
    {
      for (byte yy=0; yy<7; yy++)
      {
        byte color;
        if (getPixelHString(xx+offset,yy,mystring)) 
          color = For_color; 
        else 
          color=Bk_color;
        setPixel(xx,yy+y,color);
      }
    }
    delay(50);  
  }
}

// Private Methods /////////////////////////////////////////////////////////////

void  __attribute__((optimize("O0"))) myMATRIX::t_shiftOut(uint8_t dataRed,uint8_t dataGreen)
{
  uint8_t i;
  uint8_t val;
  uint8_t val2;
  for (i = 0; i<8; i++)  
  {
    val = (dataRed & (1 << (7 - i)));
    if (val) 
      *outRed |= bitRed;
    else 
      *outRed &= ~bitRed;            

    // val2 = (dataGreen & (1 << (7 - i)));
    // if (val2)
    //   *outGreen |= bitGreen;
    // else 
    //   *outGreen &= ~bitGreen;
    
    //Clock Pulse
    *outClock |= bitClock; //CLK, HIGH
    *outClock &= ~bitClock; //CLK, LOW               		
  }
}

void myMATRIX::rowScan(byte row)
{
  if (row & 0x01) *outRowA |= bitRowA;
    else *outRowA &= ~bitRowA;
  
  if (row & 0x02) *outRowB |= bitRowB;
    else *outRowB &= ~bitRowB;

  if (row & 0x04) *outRowC |= bitRowC;
    else *outRowC &= ~bitRowC;

  if (row & 0x08) *outRowD |= bitRowD;
    else *outRowD &= ~bitRowD;
}  

void myMATRIX::Show()
{
  byte row4=row*8;
  *outOE |= bitOE; //OE HIGH => screen OFF
  for (int i = 0; i<8; i++)  
  {
    t_shiftOut(~(matrixBufferRed[(row4)+i]),~(matrixBufferGreen[(row4)+i]));
  }
  rowScan(row);
  // *outSTB &= ~bitSTB; //STB LOW 
  *outSTB |= bitSTB; //STB HIGH ... high to copy shift register's data to output 
  *outOE &= ~bitOE; //OE LOW => screen ON

  row++;
  if (row==16) row=0;
}

byte myMATRIX::getPixelChar(uint8_t x, uint8_t y, char ch)
{
  ch = ch-32;
  if (x > 4) 
    return 0; // 4 = font Width -1
  return bitRead(pgm_read_byte(&font5x7[ch][y]),4-x); // 4 = Font witdh -1  
}

byte myMATRIX::getPixelHString(uint16_t x, uint16_t y, char *p)
{
  p=p+x/6;
  return getPixelChar(x%6,y,*p);
}


void myMATRIX::timer2Setup()
{
#if defined(newTimer2) 
  TCCR2A = 0;
  TCCR2B = 0;
  TCNT2  = 0;
  OCR2A = 254;
  TCCR2A |= (1 << WGM21);
  TCCR2B |= (1 << CS22);    
  TIMSK2 |= (1 << OCIE2A);  
#endif
#if defined(oldTimer2)
  TCCR2=0;
  TCNT2=0;
  OCR2 = 254;
  TCCR2|= (1 << WGM21);
  TCCR2|=(1 << CS22);
  TIMSK|=(1<<OCIE2);
  TCNT2=0;
#endif
}

// Other Functions //////////////////////////////////////////////////////////////
unsigned int lenString(char *p)
{
  unsigned int retVal=0;
  while(*p!='\0')
  { 
    retVal++;
    p++;
  }
  return retVal;
}
