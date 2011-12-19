
//#define DEBUG_OUT std::cerr
#define DEBUG_OUT *aNullStream


#include <iostream>
#include <stdlib.h>
#include <string.h>

#include <portable/nickcpp/APalette.h>
#include <portable/nickcpp/ANullStream.h>

#include "APCXDecoder.h"


////////////////////////////////////////////////////////////////////////////////
//  APCXDecoder Class
////////////////////////////////////////////////////////////////////////////////

// NOTE: Be sure int types are correct when using this
struct aPcxHeader {
  uint8_t  pcx_id;
  uint8_t  version;
  uint8_t  encoding;
  uint8_t  bpp;
  uint16_t upleftx,uplefty;
  uint16_t lorightx,lorighty;
  uint16_t display_xres,display_yres;
  uint8_t  palette[48];
  uint8_t  reserved;
  uint8_t  nPlanes;
  uint16_t bytesperline;
  uint16_t paletteinfo;
  uint8_t  filler[58];
};


enum { PCX_MASK=0x3f, PCX_COMP=0xc0 };


APCXDecoder::APCXDecoder(const char *fname) : AImageDecoder(fname)
{
  init();
  readPlanes=false;  lineBytes=0;
  openFile(fname);
  header();
  //errorFlag = !(boundsValid());
  unsigned int temp=bytesPerLine;
  if(lineBytes<bytesPerLine) bytesPerLine=lineBytes;
  /*if(!error()) if (!(*/ initLineBuffer(); //)) errorFlag = true;
  bytesPerLine=temp;
  /*if(!error())*/ color();
}


/* STATIC */
bool APCXDecoder::recognize(uint8_t *buffer,char *fname)
{
  bool ret=false;
  if((buffer[0]==0x0a)&&(buffer[2]==0x01)) ret=true;
  return ret;
}


void APCXDecoder::header()
{
  aPcxHeader hdr;
  fseek(fd,0,SEEK_SET);
  fread((void *)&hdr,127,1,fd);
  if(!littleEndian) {
    byteSwap((uint16_t *)&hdr.lorightx);
    byteSwap((uint16_t *)&hdr.lorighty);
    byteSwap((uint16_t *)&hdr.upleftx);
    byteSwap((uint16_t *)&hdr.uplefty);
    byteSwap((uint16_t *)&hdr.bytesperline);
    byteSwap((uint16_t *)&hdr.paletteinfo);
  }
  depth=hdr.bpp;
  nPlanes=hdr.nPlanes;
  width=hdr.lorightx-hdr.upleftx+1;
  height=hdr.lorighty-hdr.uplefty+1;
  // Here is where one detects stacked nybbles...
  if((nPlanes==4)&&(depth==1)) depth=4;
  // 24 bit pixels show up as "3 planes of 8 bits-per-pixel",
  // we just use 1 plane of 24
  if((nPlanes==3)&&(depth==8)) { depth=24; nPlanes=1; }
  bytesPerLine=hdr.bytesperline;
  if(depth==24) bytesPerLine*=3;
  lineBytes=bytesPerLine;
  if(depth==4) { readPlanes=true; lineBytes*=4; }
  if(hdr.encoding!=1) {
    DEBUG_OUT<<"Unknown PCX encoding method!\n";
    //errorFlag=true;
  }
  unsigned int hBPL=bytesPerLine;
  bytesPerLine=calcBPL(4);
  if(hBPL!=bytesPerLine) {
    DEBUG_OUT<<"Header says BPL is "<<bytesPerLine<<"\n";
    DEBUG_OUT<<"But we think BPL is "<<bytesPerLine<<"\n";
  }
}


void APCXDecoder::color()
{
  unsigned int howMany;
  switch(depth) {
    case 1: howMany=2;  break;
    case 2: howMany=4;  break;
    case 4: howMany=16;  break;
    case 8: howMany=256;  break;
    case 24: colors=(APalette *)NULL; return; //break;
    default:
      //DEBUG_OUT.warning("Unimplemented PCX depth!\n");
      DEBUG_OUT<<"Bad depth is "<<depth<<"\n";
      howMany=2;
      break;
  }
  // TODO: Only create the palette if it is needed
  // TODO: Only use builtin palette if needed
  bool needPalette=true;
  bool builtInPalette=true;
  if(needPalette) {
    colors=new APalette(howMany);
    if(!colors) { DEBUG_OUT<<"Something went wrong with the palette!\n"; return; }
    //if(colors->error()) DEBUG_OUT<<"Something went wrong with the palette!\n";
    unsigned long pos=ftell(fd);
    char check,r,g,b;
    unsigned int t;
    DEBUG_OUT<<"PCX: depth=="<<depth<<"  howMany=="<<howMany<<"\n";
    switch(depth) {
      case 8:
        // 256 colors at end of file...
        DEBUG_OUT<<"PCX: 256 color palette at end of file...\n";
        fseek(fd,-769L,SEEK_END);
        fread(&check,1,1,fd);
        if(check!=12) DEBUG_OUT<<"WARNING!  PCX 256 colormap reading problem!\n";
        else {
          for(t=0;t<colors->getNumColors();t++) {
            fread(&r,1,1,fd);  fread(&g,1,1,fd);  fread(&b,1,1,fd);
            colors->setColor(t,r,g,b);
          }
        }
        break;
      case 4:
        // 16 colors in header
        DEBUG_OUT<<"PCX: 16 color palette in header...\n";
        fseek(fd,79,SEEK_SET);
        for(t=0;t<colors->getNumColors();t++) {
          fread(&r,1,1,fd);  fread(&g,1,1,fd);  fread(&b,1,1,fd);
          colors->setColor(t,r,g,b);
        }
        break;
      case 2:
        DEBUG_OUT<<"PCX: 4 color palette in header...\n";
        fseek(fd,79,SEEK_SET);
        for(t=0;t<colors->getNumColors();t++) {
          fread(&r,1,1,fd);  fread(&g,1,1,fd);  fread(&b,1,1,fd);
          colors->setColor(t,r,g,b);
        }
        if(builtInPalette) {
          // 4 colors built-in (except background in header)
          DEBUG_OUT<<"PCX: 4 color palette built in, except bkgnd...\n";
          colors->setColor(0,0x00,0x00,0x00);
          colors->setColor(1,0x50,0xff,0xff);
          colors->setColor(2,0xff,0x50,0xff);
          colors->setColor(3,0xff,0xff,0xff);
        }
        break;
      case 1:
        DEBUG_OUT<<"PCX: 2 color palette in header...\n";
        fseek(fd,79,SEEK_SET);
        for(t=0;t<colors->getNumColors();t++) {
          fread(&r,1,1,fd);  fread(&g,1,1,fd);  fread(&b,1,1,fd);
          colors->setColor(t,r,g,b);
        }
        if(builtInPalette) {
          // 2 colors built-in
          DEBUG_OUT<<"PCX: 2 color palette built in...\n";
          colors->setColor(0,0x00,0x00,0x00);
          colors->setColor(1,0xff,0xff,0xff);
        }
        break;
      default:
        break;
    }
    fseek(fd,pos+1,SEEK_SET);
    //if(colors->error()) DEBUG_OUT<<"Something went wrong with the palette!\n";
  }
  //DEBUG_OUT.stop("APCDXDecode::color\n");
}


uint8_t *APCXDecoder::readLine(unsigned int lineNum, unsigned int numPlane)
{
  if(!lineBuffer) return (uint8_t *)NULL;
  unsigned int n=0,i;
  uint8_t c;
  if(!fd) return (uint8_t *)NULL;
  if(lineNum>height) {
    DEBUG_OUT<<"line ref out of bounds!\n";  return (uint8_t *)NULL;
  }
  if(numPlane>depth) {
    DEBUG_OUT<<"plane ref out of bounds!\n";  return (uint8_t *)NULL;
  }
  if((numPlane)&&(readPlanes)) return (lineBuffer+(numPlane*bytesPerLine));
  // Obviously not originally my code here...
  do {
    c=(uint8_t)(fgetc(fd)&0x0ff);
    if((c&PCX_COMP)==PCX_COMP) {
      i=c&PCX_MASK;
      c=(uint8_t)fgetc(fd);
      while(i--) lineBuffer[n++]=c;
    }
    else lineBuffer[n++]=c;
  } while(n<lineBytes);
  if(depth==24) {
    uint8_t *tBuf=(uint8_t *)malloc(bytesPerLine);
    if(!tBuf) return (uint8_t *)NULL;
    memcpy(tBuf,lineBuffer,bytesPerLine);
    for(unsigned int x=0;x<width;x++) {
      lineBuffer[x*3]=tBuf[x];
      lineBuffer[x*3+1]=tBuf[x+width];
      lineBuffer[x*3+2]=tBuf[x+(width*2)];
    }
    free(tBuf);
  }
  return lineBuffer;
}
