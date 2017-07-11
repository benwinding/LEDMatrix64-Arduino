/*
File	: myMATRIX.h
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

#ifndef myMATRIX_h
  #define myMATRIX_h
  #include "myMATRIXclass.h"

  myMATRIX myMatrix;
  #if defined(newTimer2)  
    ISR(TIMER2_COMPA_vect)
    { 
      myMatrix.Show();
    }
  #endif
  #if defined(oldTimer2)
    ISR(TIMER2_COMP_vect)
    {
      myMatrix.Show();
    }
  #endif
#endif
