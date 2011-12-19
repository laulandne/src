
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#include <iostream>

#include <guilib/ABitmapPlanes.h>
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
//  ABitmapPlanes Class
////////////////////////////////////////////////////////////////////////////////

ABitmapPlanes::ABitmapPlanes(ADisplay *someDisplay, ADisplayMode *d) : APrivateBitmap(someDisplay,d)
{
  DEBUG_OUT<<"ABitmapPlanes::ABitmapPlanes()\n";
  init();
  width=someDisplay->getWidth();  height=someDisplay->getHeight();
  depth=someDisplay->getDepth();  nPlanes=someDisplay->getNPlanes();
  x=0;  y=0;
  interleaved=false;
  bitmapType=someDisplay->getBitmapType();
  if(d) { planeSize=d->vMemSize;  bytesPerLine=d->bytesPerLine; }
  needPlaneSwitch=true;
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


ABitmapPlanes::~ABitmapPlanes()
{
  DEBUG_OUT<<"ABitmapPlanes::~ABitmapPlanes()\n";
  freeBitmap();
}


void ABitmapPlanes::draw(ABitmap *source, unsigned int dx, unsigned int dy)
{
  //DEBUG_OUT<<"Draw on ABitmapPlanes!\n";
  unsigned int sourceBytesPerLine=source->getBytesPerLine();
  unsigned int drawDepth=source->getNPlanes();
  unsigned int drawHeight=source->getHeight();
  unsigned int ssx=source->getX();
  unsigned int drawWidth=source->getWidth();
  unsigned int row,p;
  unsigned char *src;
  unsigned char *dest;
  // FIXME: Minimal clipping...take dx,dy into account!
  if(drawHeight>height) drawHeight=height;
  if(drawDepth>nPlanes) drawDepth=nPlanes;
  drawWidth=(drawWidth/source->getMinBlit())*source->getMaxBlit();
  if(drawWidth>bytesPerLine) drawWidth=bytesPerLine;
  dx=(dx/minBlit)*maxBlit;
  ssx=(ssx/source->getMinBlit())*source->getMaxBlit();
  for(p=0;p<drawDepth;p++) {
    dest=plane[p]+(dy*bytesPerLine)+dx;
    src=(source->getPlane(p))+(source->getY()*sourceBytesPerLine)+ssx;
    setVGAWrite(p);
    for(row=0;row<drawHeight;row++) {
      memcpy(dest,src,drawWidth);
      dest+=bytesPerLine; src+=sourceBytesPerLine;
    }
  }
  setVGAAll();
  return;
}


void ABitmapPlanes::writePixel(unsigned int x, unsigned int y)
{
  if(x>width) x=width;
  if(y>height) y=height;
  unsigned int tx=x/minBlit;
  unsigned int bit=x-(tx*minBlit);
  unsigned char *dest=(unsigned char *)NULL;
  unsigned char c,d,dmask,data;
  unsigned char bitMask[8]={ 1,2,4,8,16,32,64,128 };
  for(unsigned int p=0;p<nPlanes;p++) {
    setVGAWrite(p);  setVGARead(p);
    dmask=bitMask[p];
    dest=plane[p]+tx+(y*bytesPerLine);
    c=(unsigned char)(*dest&(0xff-bitMask[7-bit]));
    data=(unsigned char)((foreground&dmask)!=0);
    d=(unsigned char)((data&1)<<(7-bit));
    *dest=(unsigned char)(c|d);
  }
  setVGAAll();
}


// FIXME: Just a copy of ABitmap::readPixel for now...doesn't handle planes!
unsigned int ABitmapPlanes::readPixel(unsigned int x, unsigned int y)
{
  if(eightBits) {
    unsigned char *src=plane[0]+(y*bytesPerLine);
    unsigned int tx=(x/minBlit)*maxBlit;
    return *(src+tx);
  }
  else return (unsigned int)background;
}
