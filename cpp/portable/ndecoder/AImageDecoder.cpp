
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#include <iostream>
#include <stdlib.h>
#include <string.h>

#include <portable/nickcpp/ABitmap.h>
#include <portable/nickcpp/APalette.h>
#include <portable/nickcpp/ANullStream.h>

#include "AImageDecoder.h"


////////////////////////////////////////////////////////////////////////////////
//  AImageDecoder Class
////////////////////////////////////////////////////////////////////////////////

AImageDecoder::AImageDecoder(const char *fname) : ADecoder(fname)
{
  DEBUG_OUT<<"AImageDecoder::AImageDecoder()\n";
  init();
}


void AImageDecoder::init()
{
  ADecoder::init();
  //errorFlag=false;
  lineBuffer=(uint8_t *)NULL;
  colors=(APalette *)NULL;
  needsRGBReorder=false;
  compressed=false;
  isAnImage=true;
  isASound=false;  isASong=false;
  bm=NULL;
  //
  width=0; height=0; depth=0;
  bytesPerLine=0; nPlanes=0;
  hasColorMap=false;
  for(unsigned int t=0;t<1024;t++) tBuffer[t]=0;
}


ABitmap *AImageDecoder::createNewBitmapObject()
{
  DEBUG_OUT<<"createNewBitmapObject "<<width<<"x"<<height<<"x"<<depth<<"("<<nPlanes<<")...\n";
  ABitmap *i=new ABitmap(width,height,depth,nPlanes);
  APalette *c=i->getColors();
  if(c) { if(colors) { c->copy(colors); } }
  unsigned int theBPL=bytesPerLine; // FIXME: May need to check i's bpl...
  DEBUG_OUT<<"theBPL is "<<theBPL<<"\n";
  for(unsigned int p=0;p<nPlanes;p++) {
    for(unsigned int t=0;t<height;t++) {
      //DEBUG_OUT<<"plane "<<p<<" line "<<t<<"\n";
      uint8_t *src=readLine(t,p);
      uint8_t *dest=i->getPlane(p);
      memcpy(dest+(theBPL*t),src,theBPL);
    }
  }
  i->touch();
  return i;
}


void AImageDecoder::color()
{
  DEBUG_OUT<<"Parent AImageDecoder::color called!\n";
}


uint8_t *AImageDecoder::readLine(unsigned int lineNum, unsigned numPlane)
{
  DEBUG_OUT<<"readLine("<<lineNum<<","<<numPlane<<") not implemented for that format!\n";
  return (uint8_t *)NULL;
}


bool AImageDecoder::initLineBuffer()
{
  if(lineBuffer) { DEBUG_OUT<<"Can't initBuffer, already there!\n"; return false; }
  lineBufferSize=bytesPerLine;
  lineBuffer=(uint8_t *)malloc(bytesPerLine);
  if (!lineBuffer) {
     lineBufferSize=0;
     //errorFlag = true;
     DEBUG_OUT << "No memory available for line buffer!\n";
     return false;
  }
  return true;
}


void AImageDecoder::readRawBitmap(bool needReorder)
{
  DEBUG_OUT<<"AImageDecoder::readRawBitmap: "<<width<<"x"<<height<<"x"<<depth<<" ("<<nPlanes<<")\n";
  if(!bm) bm=new ABitmap(width,height,depth,nPlanes);
  for(unsigned int t=0;t<height;t++) {
    uint8_t *dest=(uint8_t *)((bm->getPlane(0))+(t*bytesPerLine));
    fread(dest,1,bytesPerLine,fd);
  }
  DEBUG_OUT<<"(readRawBitmap read the bitmap)\n";
  if(needReorder) reorderBitmapRGB();
}


void AImageDecoder::reorderBitmapRGB()
{
  DEBUG_OUT<<"AImageDecoder::reorderBitmapRGB()...\n";
  if(depth!=24) { DEBUG_OUT<<"depth is not 24!\n"; return; }
  if(!bm) { DEBUG_OUT<<"no bm!\n"; return; }
  for(unsigned int y=0;y<height;y++) {
    for(unsigned int x=0;x<width;x++) {
      uint8_t *src=(uint8_t *)((bm->getPlane(0))+(y*bytesPerLine)+(x*3));
      uint8_t r=src[0];  uint8_t g=src[1];  uint8_t b=src[2];
      src[2]=r;  src[1]=g;  src[0]=b;
    }
  }
  DEBUG_OUT<<"(readorderBitmapRGB done)\n";
}


// This is still "experimental" (what here isn't)...but seems to work fine.
unsigned int AImageDecoder::calcBPL(unsigned int unit)
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


void AImageDecoder::calc()
{
#ifdef DEBUG_VERBOSE
  //DEBUG_OUT<<"calc: "<<width<<"x"<<height<<"x"<<depth<<" ("<<nPlanes<<")\n";
#endif // DEBUG_VERBOSE
  // Normal bitmaps
  //bitmapType=A_BITMAP_CHUNKY;
  //if((nPlanes>1)||(depth==1)) bitmapType=A_BITMAP_PLANES;
  // Hold-and-Modify bitmaps
  compressed=false;
  //if((depth==12)&&(nPlanes==6)) { bitmapType=A_BITMAP_PLANES;  compressed=true; }
  //if((depth==24)&&(nPlanes==8)) { bitmapType=A_BITMAP_PLANES;  compressed=true; }
  //trueColor=false;
  //if((depth>8)&&(!compressed)) trueColor=true;
  // The "standard" two formats for indexed and truecolor, 8 and 24 bits per pixel
  //if((depth==8)&&(nPlanes==1)) eightBits=true; else eightBits=false;
  //if((depth==24)&&(nPlanes==1)) twentyFourBits=true; else twentyFourBits=false;
  // Check if there's anything fishy going on...
  //if((!depth)||(!nPlanes)||(!width)||(!height)) errorFlag=true;
  // Compute pixel per byte and byte per pixel factors
  unsigned int factor=1,max=1;
  /*
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
        //DEBUG_OUT<<"calc: Bad depth! ("<<depth<<")\n";
#endif // DEBUG_VERBOSE
        break;
    }
  }
  */
  /*
  if(bitmapType==A_BITMAP_PLANES) {
    if((nPlanes!=depth)&&(!compressed)) {
#ifdef DEBUG_VERBOSE
      //DEBUG_OUT<<"calcBlit planes depth!=nPlanes mismatch!\n";
#endif // DEBUG_VERBOSE
    }
    // For now factor and max are always 8 and 1 for planar images
    factor=8;  max=1;
  }
  */
  //minBlit=factor;  maxBlit=max;
  // Correct for odd-width bitmaps...not finished yet...
  bytesPerLine=(width/factor)*max;
  /*
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
*/
}


bool AImageDecoder::boundsValid()
{
  bool ret=true;
  // NOTE: don't call tooBig from here since tooBig calls us!
  if((nPlanes>32)||(width>32767)||(height>32767)) ret=false;
  if((!width)||(!height)||(!depth)||(!nPlanes)) ret=false;
  if(!bytesPerLine) ret=false;
  return ret;
}


/* STATIC */
bool AImageDecoder::recognizePNG(uint8_t *buf)
{
  if(!strncmp("PNG",(char *)(buf+1),3)) return true;
  return false;
}


/* STATIC */
bool AImageDecoder::recognizeJPEG(uint8_t *buf)
{
  if(!strncmp("JFIF",(char *)(buf+6),4)) return true;
  return false;
}


/* STATIC */
bool AImageDecoder::recognizeGIF(uint8_t *buf)
{
  if(!strncmp("GIF8",(char *)buf,4)) return true;
  return false;
}


/* STATIC */
bool AImageDecoder::recognizeTGA(uint8_t *buf)
{
  //
  return false;
}


/* STATIC */
bool AImageDecoder::recognizeTIFF(uint8_t *buf)
{
  //
  return false;
}


void AImageDecoder::headerGIF()
{
  DEBUG_OUT<<"ADecoderHelper::headerGIF...\n";
  width=0;  height=0;  depth=0;  nPlanes=0;
  fseek(fd,10,SEEK_SET);
  unsigned char gflags,lflags;
  fread(&gflags,1,1,fd);
  if(gflags&0x80) hasColorMap=true;
  depth=(gflags&0x7)+1;  nPlanes=depth;
  DEBUG_OUT<<"Depth is "<<depth<<"...\n";
  fseek(fd,13,SEEK_SET);
  unsigned char red,green,blue;
  if(hasColorMap) {
    DEBUG_OUT<<"GIF has a global color map...\n";
    colors=new APalette(bits2Num(depth));
    for(unsigned int t=0;t<colors->getNumColors();t++) {
      fread(&red,1,1,fd);  fread(&green,1,1,fd);  fread(&blue,1,1,fd);
      colors->setColor(t,red,green,blue);
    }
  }
  // FIXME: This fails on files that are perfectly legit...skip until we DO find an image desc!
  if(readByte()!=0x2c) {
    DEBUG_OUT<<"GIF: Something's wrong, didn't find an image desc!\n";
    return;
  }
  fread(tBuffer,4,1,fd);  // skip
  width=readShortL();
  height=readShortL();
  DEBUG_OUT<<"GIF is "<<width<<"x"<<height<<"...\n";
  calc();
  hasColorMap=false;  // now we're looking for LOCAL color maps...
  fread(&lflags,1,1,fd);
  if(lflags&0x80) hasColorMap=true;
  if(hasColorMap) {
    DEBUG_OUT<<"GIF has a local color map...\n";
    depth=(lflags&0x7)+1;  // it may need to be corrected...
    if(!colors) colors=new APalette(bits2Num(depth));
    for(unsigned int t=0;t<colors->getNumColors();t++) {
      fread(&red,1,1,fd);  fread(&green,1,1,fd);  fread(&blue,1,1,fd);
      colors->setColor(t,red,green,blue);
    }
  }
}


void AImageDecoder::headerPNG()
{
  // FIXME: ADecoderHelper::headerPNG needs to be implemented
  ////DEBUG_OUT.warning("ADecoderHelper::headerPNG not implemented!\n";
  width=320;  height=200;
  depth=24;  nPlanes=1;
  calc();
}


void AImageDecoder::headerJPEG()
{
  // FIXME: ADecoderHelper::headerJPEG needs to be implemented
  ////DEBUG_OUT.warning("ADecoderHelper::headerJPEG not implemented!\n";
  width=320;  height=200;
  depth=24;  nPlanes=1;
  calc();
}


void AImageDecoder::headerTGA()
{
  // FIXME: ADecoderHelper::headerTGA needs to be implemented
  //DEBUG_OUT.warning("ADecoderHelper::headerTGA not implemented!\n";
  width=320;  height=200;
  depth=24;  nPlanes=1;
  calc();
}


void AImageDecoder::headerTIFF()
{
  // FIXME: ADecoderHelper::headerTIFF needs to be implemented
  //DEBUG_OUT.warning("ADecoderHelper::headerTIFF not implemented!\n";
  width=320;  height=200;
  depth=24;  nPlanes=1;
  calc();
}

