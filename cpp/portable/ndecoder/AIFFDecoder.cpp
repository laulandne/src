
//#define DEBUG_OUT std::cerr
#define DEBUG_OUT *aNullStream


#include <iostream>
#include <stdlib.h>
#include <string.h>

#include <portable/nickcpp/APalette.h>
#include <portable/nickcpp/ANullStream.h>

#include "AIFFDecoder.h"


////////////////////////////////////////////////////////////////////////////////
//  AIFFDecoder Class
////////////////////////////////////////////////////////////////////////////////

// This is the format of a BMHD chunk in an IFF ILBM FORM file
struct aIffHeader {
  uint16_t width, height;
  uint16_t x,y;
  char depth;
  char masking;
  char compression;
  char skip1;
  uint16_t transparent;
  char aspectX,aspectY;
  uint16_t pwidth,pheight;
};


AIFFDecoder::AIFFDecoder(const char *fname) : AImageDecoder(fname)
{
  init();
  iffChunk=0;  iffViewModes=0;  hasMask=false;  isPBM=false;
  isHam=false;  isDynamic=false;  skipRead=false;
  openFile(fname);
  header();
  //errorFlag=!(boundsValid());
  /*if(!error()) if(!(*/initLineBuffer();//)) errorFlag=true;
  //if(!error()) color();
}


bool AIFFDecoder::recognize(uint8_t *buffer,char *fname)
{
  bool ret=false;
  if(!strncmp("FORM",(char *)buffer,4)) {
    if((buffer[8]=='A')&&(buffer[9]=='N')&&(buffer[10]=='I')&&(buffer[11]=='M')) return true;
    if((buffer[8]=='I')&&(buffer[9]=='L')&&(buffer[10]=='B')&&(buffer[11]=='M')) return true;
    if((buffer[8]=='P')&&(buffer[9]=='B')&&(buffer[10]=='M')&&(buffer[11]==' ')) return true;
  }
  return ret;
}


void AIFFDecoder::header()
{
  if(!fd) return;
  aIffHeader hdr;
  char cname[5];
  cname[4]=0;
  fseek(fd,8,SEEK_SET);
  fread(cname,4,1,fd);
  DEBUG_OUT<<"IFF "<<cname<<" format.\n";
  tchunkOffset=24;  // probably should be 12?
  if(!strcmp(cname,"ANIM")) {
    fseek(fd,20,SEEK_SET);
    fread(cname,4,1,fd);
    DEBUG_OUT<<"IFF ANIM ("<<cname<<"), reading only key frame.\n";
    if(!strcmp(cname,"PBM ")) isPBM=true;
  } else
  if(!strcmp(cname,"ILBM")) tchunkOffset=12; else
  if(!strcmp(cname,"PBM ")) {
    tchunkOffset=12;  isPBM=true;
    DEBUG_OUT<<"(PC-DPaint packed pixels)\n";
  }
  else {
    DEBUG_OUT<<"Don't know what that format is, giving up...\n";
    /*errorFlag=true;*/  return;
  }
  if(!iffFindChunk(tchunkOffset,(char *)"BMHD",true)) {
    DEBUG_OUT<<"Couldn't find BMHD!\n";  /*errorFlag=true;*/  return;
  }
  fread((void *)&hdr,sizeof(aIffHeader),1,fd);
  if(littleEndian) {
    byteSwap(&hdr.width);  byteSwap(&hdr.height);
  }
  depth=hdr.depth;  width=hdr.width;  height=hdr.height;
  if(!isPBM) nPlanes=depth; else nPlanes=1;
  // FIXME: See IFF39 doc about rounding...
  // FIXME: See IFF39 doc about interleaved bitplanes...
  lineBytes=calcBPL(2);
  bytesPerLine=calcBPL(4);
  DEBUG_OUT<<"IFF: lineBytes="<<lineBytes<<" BPL="<<bytesPerLine<<"\n";
  //bytesPerLine=lineBytes;
  iffChunk=0; iffViewModes=0;
  if(iffFindChunk(tchunkOffset,(char *)"CAMG",true)) {
    uint32_t camg;
    fread((void *)&camg,4,1,fd);
    if(littleEndian) byteSwap(&camg);
    iffViewModes=camg;
  }
  DEBUG_OUT<<"Picture aspect is "<<(int)hdr.aspectX<<" : "<<(int)hdr.aspectY<<"\n";
  // FIXME: don't read the bits out of CAMG if you can't help it!
  if(iffViewModes&0x800) {
    switch(depth) {
      case 6:
        DEBUG_OUT<<"Amiga HAM6 Mode...\n";  depth=12;  break;
      case 8:
        DEBUG_OUT<<"Amiga HAM8 Mode...\n";  depth=24;  break;
      default:
        //DEBUG_OUT.warning(" Ham, but neither 6 nor 8 planes!\n");
        break;
    }
    isHam=true;
  }
  switch(hdr.compression) {
    case 0: compressed=false; break;
    case 1: compressed=true; break;
    default: DEBUG_OUT<<"Unknown compression method!\n"; break;
  }
  if(hdr.masking==1) hasMask=true;
  // FIXME: handle "CMAP has all 8 bits" flag here...
  DEBUG_OUT<<"Searching for dynamic mode chunks...\n";
  if(iffFindChunk(tchunkOffset,(char *)"CTBL",true)) isDynamic=true; else // DigiView Gold dynamic mode
  if(iffFindChunk(tchunkOffset,(char *)"PCHG",true)) isDynamic=true; else // HamLab et al palette change mode
  if(iffFindChunk(tchunkOffset,(char *)"SHAM",true)) isDynamic=true;      // Sliced-HAM mode
  if(isDynamic) DEBUG_OUT<<"Image changes palette dynamically.\n";
  //DEBUG_OUT<<"Page: "<<hdr.pwidth<<"x"<<hdr.pheight<<"\n";
  needsRGBReorder=false;
  if((!isHam)&&(depth==24)) needsRGBReorder=true;
}



void AIFFDecoder::color()
{
  colors=(APalette *)NULL;
  unsigned int size=3;
  if(iffChunk!=1) {
    iffChunk=1;
    size=iffFindChunk(tchunkOffset,(char *)"CMAP",true);
    if(!size) { DEBUG_OUT<<"CMAP Chunk not found...\n"; return; }
  }
  unsigned int num=(int)size/3;
  colors=new APalette(num);
  /*
  if(colors->error()) {
    DEBUG_OUT<<"IFF palette creation error!\n";
    colors=(APalette *)NULL; return;
  }
  */
  unsigned char r,g,b;
  // FIXME: Check for 0's in bottom 4 bits and scale if so...
  for(unsigned int t=0;t<num;t++) {
    fread(&r,1,1,fd);  fread(&g,1,1,fd);  fread(&b,1,1,fd);
    colors->setColor(t,r,g,b);
    /*
    if(isPBM) {
      DEBUG_OUT<<t<<":"<<(int)r<<","<<(int)g<<","<<(int)b<<"\n";
    }
    */
  }
  unsigned int howMany=num;
  // NOTE: depth and not nPlanes now...
  switch(depth) {
    case 1:  howMany=2;  break;
    case 2:  howMany=4;  break;
    case 3:  howMany=8;  break;
    case 4:  howMany=16;  break;
    case 5:  howMany=32;  break;
    case 6:  if(isHam) howMany=16; else howMany=64;  break;
    case 7:  howMany=128;  break;
    case 8:  if(isHam) howMany=64; else howMany=256;  break;
    case 12: if(isHam) howMany=16; else howMany=0;  break;
    case 24: if(isHam) howMany=64; else howMany=0;  break;
    default:
      //DEBUG_OUT.warning("Unimplemented depth!\n");
      break;
  }
  if(num!=howMany) {
    DEBUG_OUT<<"(decoder has to resize the palette from "
      <<num<<" to "<<howMany<<")\n";
    colors->resize(howMany);
  }
  // FIXME: Don't read bits out of CAMG if you can help it...
  if(iffViewModes&0x80) {
    // This is a halfbrite image
    DEBUG_OUT<<"Amiga HalfBrite 64 mode...\n";
    if(colors->getNumColors()<64) {
      DEBUG_OUT<<"ERROR!  HalfBrite IFF image with less than 64 colors!\n";  return;
    }
    for(unsigned int t=32;t<64;t++) {
      colors->setColor(t,colors->red(t-32)/2,colors->green(t-32)/2,colors->blue(t-32)/2);
    }
  }
}


uint8_t *AIFFDecoder::readLine(unsigned int lineNum, unsigned int numPlane)
{
  if(!lineBuffer) return (uint8_t *)NULL;
  if(lineNum>height) { DEBUG_OUT<<"line ref out of bounds!\n";  return (uint8_t *)NULL; }
  if(numPlane>nPlanes) { DEBUG_OUT<<"plane ref out of bounds!\n";  return (uint8_t *)NULL; }
  if(!fd) return (uint8_t *)NULL;
  if(iffChunk!=2) {
    iffChunk=2;
    if(!iffFindChunk(tchunkOffset,(char *)"BODY",true)) {
      DEBUG_OUT<<"Chunk not found error.\n";  //errorFlag=true;
      return (uint8_t *)NULL;
    }
  }
  volatile uint8_t *temp=(uint8_t *)NULL;
  // NOTE: Must be signed
  signed char bite=0,bite2=0,tRead;
  unsigned int b=0;
  if(compressed) {
    while(b<lineBytes) {
      fread((unsigned char *)&bite,1,1,fd);
      if(bite>=0) {
        temp=(uint8_t *)(lineBuffer+b);
        for(unsigned long cnt=0;cnt<(uint8_t)(bite+1);cnt++) {
          if(skipRead) fread(&tRead,1,1,fd); else fread((void *)temp,1,1,fd);
          temp++;
        }
        b+=bite+1;
      } /* if count<=127 */
      if(bite<0) {
        fread((uint8_t *)&bite2,1,1,fd);
        temp=(uint8_t *)(lineBuffer+b);
        /* inner count loop */
        for(unsigned long count=0;count<(uint8_t)((-bite)+1);count++) {
          if(!skipRead) *temp=bite2;
          temp++;
        }
        b+=(-bite)+1;
      } /* if bite<0 */
    }  /* while b */
  }
  else {
    // Uncompressed lines
    if(skipRead) {
      for(unsigned int t=0;t<lineBytes;t++) fread(&tRead,1,1,fd);
    }
    else fread(lineBuffer,1,lineBytes,fd);
  }
  if((hasMask)&&(numPlane==(nPlanes-1))) {
    // Skip mask plane if present
    skipRead=true; readLine(0,0); skipRead=false;
  }
  return lineBuffer;
}


