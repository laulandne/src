
//#define DEBUG_OUT std::cerr
#define DEBUG_OUT *aNullStream


#include <iostream>

#include <portable/nickcpp/ANullStream.h>

#include "ABitmapAbstract.h"


////////////////////////////////////////////////////////////////////////////////
//  ABitmapAbstract Class
////////////////////////////////////////////////////////////////////////////////

ABitmapAbstract::ABitmapAbstract()
{
  DEBUG_OUT<<"ABitmapAbstract::ABitmapAbstract()\n";
  init();
}


ABitmapAbstract::ABitmapAbstract(unsigned int w, unsigned int h, unsigned int d, unsigned int np)
{
  DEBUG_OUT<<"ABitmapAbstract::ABitmapAbstract("<<w<<","<<h<<","<<d<<","<<np<<")\n";
  init();
}


ABitmapAbstract::~ABitmapAbstract()
{
  DEBUG_OUT<<"ABitmapAbstract::~ABitmapAbstract()\n";
}



void ABitmapAbstract::init()
{
  DEBUG_OUT<<"ABitmapAbstract::init()\n";
  AFrameAbstract::init();
}
