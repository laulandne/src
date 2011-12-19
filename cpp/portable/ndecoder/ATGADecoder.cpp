
//#define DEBUG_OUT std::cerr
#define DEBUG_OUT *aNullStream


#include <iostream>
#include <stdlib.h>
#include <string.h>

#include <portable/nickcpp/APalette.h>
#include <portable/nickcpp/ANullStream.h>

#include "ATGADecoder.h"


////////////////////////////////////////////////////////////////////////////////
//  ATGADecoder Class
////////////////////////////////////////////////////////////////////////////////

ATGADecoder::ATGADecoder(const char *fname) : AImageDecoder(fname)
{
  init();
  openFile(fname);
  header();
  //errorFlag = !(boundsValid());
  /*if(!error()) if (!(*/initLineBuffer();//)) errorFlag = true;
  /*if(!error())*/ color();
}


/* STATIC */
bool ATGADecoder::recognize(uint8_t *buf,char *fname)
{
  bool prob=false;
  if((buf[0]==0)&&(buf[1]==0)&&(buf[2]==2)&&(buf[3]==0)) prob=true;  // prob uncompressed
  if((buf[0]==0)&&(buf[1]==0)&&(buf[2]==10)&&(buf[3]==0)) prob=true; // prob compressed
  if(prob) {
    if((buf[4]==0)&&(buf[5]==0)&&(buf[6]==0)&&(buf[7]==0)) return true;
  }
  return false;
}


void ATGADecoder::header()
{
  bool comp=false;
  char l;
  fseek(fd,2,SEEK_SET);
  fread(&l,1,1,fd);
  DEBUG_OUT<<"TGA is ";
  if(l==10) { DEBUG_OUT<<"compressed.\n"; comp=true; }
  else DEBUG_OUT<<"uncompressed.\n";
  fseek(fd,12,SEEK_SET);
  width=readShortL();
  height=readShortL();
  depth=readByte();
  // if depth==24 then RGB, if 32 then RGBA
  DEBUG_OUT<<"Image is "<<width<<"x"<<height<<"x"<<depth<<"\n";
  nPlanes=1;
  calc();
  bm=NULL;
}


void ATGADecoder::color()
{
}


uint8_t *ATGADecoder::readLine(unsigned int lineNum, unsigned int numPlane)
{
  if(!lineBuffer) return (uint8_t *)NULL;
  if(lineNum>height) { DEBUG_OUT<<"line ref out of bounds!\n";  return (uint8_t *)NULL; }
  if(numPlane>depth) { DEBUG_OUT<<"plane ref out of bounds!\n";  return (uint8_t *)NULL; }
  if(!fd) return (uint8_t *)NULL;
  if(!bm) readRawBitmap(false);
  if(!bm) {
    DEBUG_OUT<<"Problem reading bitmap!\n";  return (uint8_t *)NULL;
  }
  char *src=(char *)((bm->getPlane(numPlane))+((height-lineNum-1)*bytesPerLine));
  for(uint8_t t=0;t<bytesPerLine;t++) {
    lineBuffer[t]=src[t];
  }
  return lineBuffer;
}
