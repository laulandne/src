
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#include <iostream>

#include <guilib/ABitmapInterFour.h>
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
//  ABitmapInterFour Class
////////////////////////////////////////////////////////////////////////////////

ABitmapInterFour::ABitmapInterFour(ADisplay *someDisplay, ADisplayMode *d) : APrivateBitmap(someDisplay,d)
{
  DEBUG_OUT<<"ABitmapInterFour::ABitmapInterFour()\n";
  init();
  width=someDisplay->getWidth();  height=someDisplay->getHeight();
  depth=someDisplay->getDepth();  nPlanes=someDisplay->getNPlanes();
  x=0;  y=0;
  interleaved=true;
  bitmapType=someDisplay->getBitmapType();
  if(d) { planeSize=d->vMemSize;  bytesPerLine=d->bytesPerLine; }
  calc();
  foreground=1;  background=0;
  //NOTE: all planes will point to the same area of memory
  for(unsigned int t=0;t<nPlanes;t++) {
    if(d) plane[t]=mapVideoMem((unsigned char *)d->vMemPtr,planeSize);
    if(!plane[t]) plane[t]=(unsigned char *)malloc(planeSize);
    needFree[t]=false;
  }
  if(someDisplay->getColors()) colors=new APalette(0,depth);
}


ABitmapInterFour::~ABitmapInterFour()
{
  DEBUG_OUT<<"ABitmapInterFour::~ABitmapInterFour()\n";
  freeBitmap();
}


void ABitmapInterFour::draw(ABitmap *source, unsigned int dx, unsigned int dy)
{
  //DEBUG_OUT<<"Draw on ABitmapInterFour!\n";
  unsigned int sourceBytesPerLine=source->getBytesPerLine();
  unsigned int drawDepth=source->getNPlanes();
  unsigned int drawHeight=source->getHeight();
  unsigned int ssx=source->getX();
  unsigned int drawWidth=source->getWidth();
  unsigned int row;
  unsigned char *src;
  unsigned char *dest;
  // FIXME: Minimal clipping...take dx,dy into account!
  if(drawHeight>height) drawHeight=height;
  if(drawDepth>nPlanes) drawDepth=nPlanes;
  drawWidth=(drawWidth/source->getMinBlit())*source->getMaxBlit();
  if(drawWidth>bytesPerLine) drawWidth=bytesPerLine;
  dx=(dx/minBlit)*maxBlit;
  ssx=(ssx/source->getMinBlit())*source->getMaxBlit();
  unsigned int ssy=source->getY()/4;  dy=dy/4;
  drawHeight=source->getHeight()/4;
  sourceBytesPerLine*=4;
  unsigned int add=sourceBytesPerLine/4;
  unsigned int fix=0;
  // lines mod=0
  dest=plane[0]+(dy*bytesPerLine)+dx-fix;
  src=(source->getPlane(0))+(ssy*sourceBytesPerLine)+ssx;
  for(row=0;row<drawHeight;row++) {
    memcpy(dest,src,drawWidth);
    dest+=bytesPerLine; src+=sourceBytesPerLine;
  }
  // lines mod=1
  dest=plane[0]+(dy*bytesPerLine)+dx+8192-fix;
  src=(source->getPlane(0))+(ssy*sourceBytesPerLine)+ssx+add;
  for(row=0;row<drawHeight;row++) {
    memcpy(dest,src,drawWidth);
    dest+=bytesPerLine; src+=sourceBytesPerLine;
  }
  // lines mod=2
  dest=plane[0]+(dy*bytesPerLine)+dx+16384-fix;
  src=(source->getPlane(0))+(ssy*sourceBytesPerLine)+ssx+add+add;
  for(row=0;row<drawHeight;row++) {
    memcpy(dest,src,drawWidth);
    dest+=bytesPerLine; src+=sourceBytesPerLine;
  }
  // lines mod=3
  dest=plane[0]+(dy*bytesPerLine)+dx+24576-fix;
  src=(source->getPlane(0))+(ssy*sourceBytesPerLine)+ssx+add+add+add;
  for(row=0;row<drawHeight;row++) {
    memcpy(dest,src,drawWidth);
    dest+=bytesPerLine; src+=sourceBytesPerLine;
  }
  return;
}


// NOTE: Just a copy of ABitmap's version...doesn't handle interleaving
void ABitmapInterFour::writePixel(unsigned int x, unsigned int y)
{
  if(x>width) x=width;
  if(y>height) y=height;
  unsigned int tx=(x/minBlit)*maxBlit;
  unsigned char *dest=plane[0]+tx+(y*bytesPerLine);
  // FIXME: If maxBlit>1 this will only draw a portion of the pixel
  // FIXME: If minBlit>1 this will draw multiple pixels
  *dest=(unsigned char)foreground;
}


// FIXME: Just a copy of ABitmap::readPixel for now...doesn't handle InterFour!
unsigned int ABitmapInterFour::readPixel(unsigned int x, unsigned int y)
{
  if(eightBits) {
    unsigned char *src=plane[0]+(y*bytesPerLine);
    unsigned int tx=(x/minBlit)*maxBlit;
    return *(src+tx);
  }
  else return (unsigned int)background;
}
