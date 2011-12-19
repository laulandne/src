
//#define DEBUG_OUT std::cerr
#define DEBUG_OUT *aNullStream


#include <iostream>

#include <guilib/ABitmapSimple.h>
#include <portable/nickcpp/ANullStream.h>


////////////////////////////////////////////////////////////////////////////////
//  ABitmapSimple Class
////////////////////////////////////////////////////////////////////////////////

ABitmapSimple::ABitmapSimple()
{
  DEBUG_OUT<<"ABitmapSimple::ABitmapSimple()\n";
  init();
}


ABitmapSimple::ABitmapSimple(unsigned int w, unsigned int h, unsigned int d, unsigned int np)
{
  DEBUG_OUT<<"ABitmapSimple::ABitmapSimple("<<w<<","<<h<<","<<d<<","<<np<<")\n";
  init();
}


ABitmapSimple::~ABitmapSimple()
{
  DEBUG_OUT<<"ABitmapSimple::~ABitmapSimple()\n";
}



void ABitmapSimple::init()
{
  DEBUG_OUT<<"ABitmapSimple::init()\n";
  AFrameAbstract::init();
}
