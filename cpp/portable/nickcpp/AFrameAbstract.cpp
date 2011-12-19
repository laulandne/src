
//#define DEBUG_OUT std::cerr
#define DEBUG_OUT *aNullStream


#include <iostream>

#include "AFrameAbstract.h"


////////////////////////////////////////////////////////////////////////////////
//  AFrameAbstract Class
////////////////////////////////////////////////////////////////////////////////

AFrameAbstract::AFrameAbstract()
{
  init();
}


AFrameAbstract::~AFrameAbstract()
{
}


void AFrameAbstract::init()
{
#ifdef ASYS_MFC
  setClassName("AFrameAbstract");
#endif // ASYS_MFC
  x=0;  y=0;
  width=0;  height=0;
  depth=0;  nPlanes=0;
  bytesPerLine=0;
  minBlit=0;  maxBlit=0;
  bitmapType=A_BITMAP_NONE;
  colorOrder=A_BITMAP_RGB;
  compressed=false;
  trueColor=false;
  eightBits=false;  twentyFourBits=false;
  interleaved=false;
  modified=false;
  planeSize=0;
}


// This is still "experimental" (what here isn't)...but seems to work fine.
unsigned int AFrameAbstract::calcBPL(unsigned int unit)
{
  if((!width)||(!height)||(!depth)||(!nPlanes)) return 0;
  unsigned int nwidth=width;
  unsigned int nbytesPerLine;
  unsigned int pixel=depth;
  if(nPlanes>1) pixel=1;
  if(!pixel) pixel=1;
  unsigned int min=8/pixel;
  if(pixel>8) min=1;
  unsigned int max=pixel/8;
  if(pixel<8) max=1;
#ifdef DEBUG_VERBOSE
  //DEBUG_OUT<<"CalcBPL("<<unit<<"):\n";
  //DEBUG_OUT<<"Min/Max are "<<min<<"/"<<max<<"\n";
  //DEBUG_OUT<<"((width/min)*min) is "<<(nwidth/min)*min<<"\n";
#endif // DEBUG_VERBOSE
  int oddFix=0;
  if(((width/min)*min)!=width) {
#ifdef DEBUG_VERBOSE
    //DEBUG_OUT<<"Odd width, added a min...\n";
#endif // DEBUG_VERBOSE
    oddFix=1;
  }
  nbytesPerLine=((nwidth/min)*max)+oddFix;
#ifdef DEBUG_VERBOSE
  //DEBUG_OUT<<"Let's try "<<nbytesPerLine<<"\n";
#endif // DEBUG_VERBOSE
  if(unit) {
    //unsigned int mask=unit-1;
    if(nbytesPerLine%unit) {
#ifdef DEBUG_VERBOSE
      //DEBUG_OUT<<"(fixed an odd/unit width)\n";
#endif // DEBUG_VERBOSE
      nbytesPerLine=(nbytesPerLine-(nbytesPerLine%unit))+unit;
      nwidth=(bytesPerLine/max)*min;
    }
  }
#ifdef DEBUG_VERBOSE
  //DEBUG_OUT<<"calcbpl done: "<<nbytesPerLine<<"\n";
#endif // DEBUG_VERBOSE
  return nbytesPerLine;
}


void AFrameAbstract::calc()
{
#ifdef DEBUG_VERBOSE
  //DEBUG_OUT<<"calc: "<<width<<"x"<<height<<"x"<<depth<<" ("<<nPlanes<<")\n";
#endif // DEBUG_VERBOSE
  // Normal bitmaps
  bitmapType=A_BITMAP_CHUNKY;
  if((nPlanes>1)||(depth==1)) bitmapType=A_BITMAP_PLANES;
  // Hold-and-Modify bitmaps
  compressed=false;
  if((depth==12)&&(nPlanes==6)) { bitmapType=A_BITMAP_PLANES;  compressed=true; }
  if((depth==24)&&(nPlanes==8)) { bitmapType=A_BITMAP_PLANES;  compressed=true; }
  trueColor=false;
  if((depth>8)&&(!compressed)) trueColor=true;
  // The "standard" two formats for indexed and truecolor, 8 and 24 bits per pixel
  if((depth==8)&&(nPlanes==1)) eightBits=true; else eightBits=false;
  if((depth==24)&&(nPlanes==1)) twentyFourBits=true; else twentyFourBits=false;
  // Check if there's anything fishy going on...
  // Compute pixel per byte and byte per pixel factors
  unsigned int factor=1,max=1;
  if(bitmapType==A_BITMAP_CHUNKY) {
    switch(depth) {
      case 1: bitmapType=A_BITMAP_PLANES; factor=8; break;
      case 2: factor=4; break;
      case 4: factor=2; break;
      case 8: factor=1; break;
      case 15: case 16: max=2; break;
      case 24: max=3; break;
      case 32: max=4; break;
      default:
#ifdef DEBUG_VERBOSE
        DEBUG_OUT<<"calc: Bad depth! ("<<depth<<")\n";
#endif // DEBUG_VERBOSE
        break;
    }
  }
  if(bitmapType==A_BITMAP_PLANES) {
    if((nPlanes!=depth)&&(!compressed)) {
#ifdef DEBUG_VERBOSE
      DEBUG_OUT<<"calcBlit planes depth!=nPlanes mismatch!\n";
#endif // DEBUG_VERBOSE
    }
    // For now factor and max are always 8 and 1 for planar images
    factor=8;  max=1;
  }
  minBlit=factor;  maxBlit=max;
  // Correct for odd-width bitmaps...not finished yet...
  bytesPerLine=(width/minBlit)*maxBlit;
  planeSize=bytesPerLine*height;
#ifdef ASYS_DOS16
  if(planeSize>0xffff) {
#ifdef DEBUG_VERBOSE
    DEBUG_OUT<<"calc: Bitmap too large for tiny operating system!\n";
#endif // DEBUG_VERBOSE
    planeSize=0xffff;
  }
#endif
#ifdef ASYS_WIN16
  if(planeSize>0xffff) {
#ifdef DEBUG_VERBOSE
    DEBUG_OUT<<"calc: Bitmap too large for tiny operating system!\n";
#endif // DEBUG_VERBOSE
    planeSize=0xffff;
  }
#endif
}


bool AFrameAbstract::boundsValid()
{
  bool ret=true;
  // NOTE: don't call tooBig from here since tooBig calls us!
  if((nPlanes>32)||(width>32767)||(height>32767)) ret=false;
  if((!width)||(!height)||(!depth)||(!nPlanes)) ret=false;
  if(!bytesPerLine) ret=false;
  if(!ret) {
#ifdef DEBUG_VERBOSE
    DEBUG_OUT<<"AFrameAbstract::boundsValid: "<<width<<"x"<<height<<"x"<<depth<<" ("<<nPlanes<<") bpl:"<<
      bytesPerLine<<".\n";
#endif // DEBUG_VERBOSE
  }
  return ret;
}
