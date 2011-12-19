
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#include <iostream>

#include <guilib/ABitmapText.h>
#include <guilib/ADisplay.h>

#include <portable/nickcpp/APalette.h>
#include <portable/nickcpp/ANullStream.h>


#ifdef ASYS_X11_BASED
#include <X11/Xutil.h>
#endif // ASYS_X11_BASED


#ifdef ASYS_DOS
#include <guilib/alib_dos.h>
#define ASYS_NEED_MAP_VIDEO 1
#endif // ASYS_DOS

#ifdef ASYS_AMIGA
#include <graphics/gfx.h>
#include <exec/memory.h>
#include <clib/graphics_protos.h>
#include <clib/intuition_protos.h>
#include <clib/exec_protos.h>
#endif // ASYS_AMIGA


#ifndef ASYS_DOS
#define mapVideoMem(a,b) (a)
#endif // ASYS_DOS


////////////////////////////////////////////////////////////////////////////////
//  ABitmapText Class
////////////////////////////////////////////////////////////////////////////////

ABitmapText::ABitmapText(ADisplay *someDisplay, ADisplayMode *d) : APrivateBitmap(someDisplay,d)
{
  DEBUG_OUT<<"ABitmapText::ABitmapText()\n";
  init();
  width=someDisplay->getWidth();  height=someDisplay->getHeight();
  depth=someDisplay->getDepth();  nPlanes=someDisplay->getNPlanes();
  x=0;  y=0;
  interleaved=false;
  minBlit=1;  maxBlit=2;
  if(d) { planeSize=d->vMemSize;  bytesPerLine=d->bytesPerLine; }
  calc();
  foreground=7;  background=32;
  //NOTE: all planes will point to the same area of memory
  for(unsigned int t=0;t<nPlanes;t++) {
    if(d) plane[t]=mapVideoMem((unsigned char *)d->vMemPtr,planeSize);
    if(!plane[t]) plane[t]=(unsigned char *)malloc(planeSize);
    needFree[t]=false;
  }
  if(someDisplay->getColors()) colors=new APalette(0,depth);
}


ABitmapText::~ABitmapText()
{
  DEBUG_OUT<<"ABitmapText::~ABitmapText()\n";
  freeBitmap();
}


void ABitmapText::draw(ABitmap *source, unsigned int dx, unsigned int dy)
{
  //DEBUG_OUT<<"Draw on ABitmapText-t!\n";
  return;
}


void ABitmapText::writePixel(unsigned int x, unsigned int y)
{
  //DEBUG_OUT<<"writePixel on ABitmapText-t!\n";
  return;
}


unsigned int ABitmapText::readPixel(unsigned int x, unsigned int y)
{
  //DEBUG_OUT<<"readPixel on ABitmapText-t!\n";
  return (unsigned int)background;
}
