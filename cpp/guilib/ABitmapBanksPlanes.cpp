
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#include <iostream>

#include <guilib/ABitmapBanksPlanes.h>
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
//  ABitmapBanksPlanes Class
////////////////////////////////////////////////////////////////////////////////

ABitmapBanksPlanes::ABitmapBanksPlanes(ADisplay *someDisplay, ADisplayMode *d) : APrivateBitmap(someDisplay,d)
{
  DEBUG_OUT<<"ABitmapBanksPlanes::ABitmapBanksPlanes()\n";
  init();
  // Specific to a banked bitmap...
  wsize=0;  wgran=0;
  width=someDisplay->getWidth();  height=someDisplay->getHeight();
  depth=someDisplay->getDepth();  nPlanes=someDisplay->getNPlanes();
  x=0;  y=0;
  interleaved=false;
  bitmapType=someDisplay->getBitmapType();
  if(d) { planeSize=d->vMemSize;  bytesPerLine=d->bytesPerLine; }
  needPlaneSwitch=true;
#ifdef ASYS_DOS
  wsize=someDisplay->getWSize()*1024L;  wgran=someDisplay->getWGran()*1024L;
#endif // ASYS_DOS
  DEBUG_OUT<<"Banked bitmap with planes and a window of "<<wsize<<" at "<<wgran<<".\n";
#ifdef ASYS_DOS16
  pageFunc=someDisplay->getFunc();
#endif // ASYS_DOS16
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


ABitmapBanksPlanes::~ABitmapBanksPlanes()
{
  DEBUG_OUT<<"ABitmapBanksPlanes::~ABitmapBanksPlanes()\n";
  freeBitmap();
}


void ABitmapBanksPlanes::draw(ABitmap *source, unsigned int dx, unsigned int dy)
{
  //DEBUG_OUT<<"Draw on ABitmapBanksPlanes!\n";
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
  //DEBUG_OUT<<"dx is "<<dx<<"  dy is "<<dy<<"  bpl is "<<bytesPerLine<<"\n";
  unsigned long off=((unsigned long)dy*(unsigned long)bytesPerLine)+(unsigned long)dx;
  unsigned int b;
  for(p=0;p<drawDepth;p++) {
    dest=plane[0];
    off=(dy*bytesPerLine)+dx;
    b=0;
    //DEBUG_OUT<<b<<"\n";
    while(off>wgran) {
      off-=wgran;  b++;
      //DEBUG_OUT<<b<<"\n";
    }
    setVGABank(b);
    //DEBUG_OUT<<"(set)\n";
    src=(source->getPlane(p));
    unsigned long soff=(source->getY()*sourceBytesPerLine)+ssx;
    setVGAWrite(p);
    for(row=0;row<drawHeight;row++) {
      memcpy((unsigned char *)(dest+off),
        (unsigned char *)(src+soff),drawWidth);
      off+=bytesPerLine;
      if(off>wgran) {
        off-=wgran;  b++;
        //DEBUG_OUT<<b<<"\n";
      }
      setVGABank(b);
      soff+=sourceBytesPerLine;
      //DEBUG_OUT<<(long)soff<<" "<<((long)src+soff)<<"\n";
    }
  }
  setVGAAll();
  return;
}


// NOTE: This is just a copy of the ABitmapPlanes version...doesn't handle banks yet
void ABitmapBanksPlanes::writePixel(unsigned int x, unsigned int y)
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


// FIXME: Just a copy of ABitmap::readPixel for now...doesn't handle banks or planes!
unsigned int ABitmapBanksPlanes::readPixel(unsigned int x, unsigned int y)
{
  if(eightBits) {
    unsigned char *src=plane[0]+(y*bytesPerLine);
    unsigned int tx=(x/minBlit)*maxBlit;
    return *(src+tx);
  }
  else return (unsigned int)background;
}
