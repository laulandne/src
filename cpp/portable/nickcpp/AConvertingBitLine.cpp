
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#include <iostream>
#include <stdlib.h>
#include <string.h>

#include <portable/nickcpp/APalette.h>
#include <portable/nickcpp/ANullStream.h>

#include "AConvertingBitLine.h"


////////////////////////////////////////////////////////////////////////////////
//  Global junk
////////////////////////////////////////////////////////////////////////////////

static unsigned char bitMask8[8]={ 1,2,4,8,16,32,64,128 };

static unsigned char bitMask24[24]={
 1,2,4,8,16,32,64,128,
 1,2,4,8,16,32,64,128,
 1,2,4,8,16,32,64,128
};

static unsigned char offset24[24]={
 1,1,1,1,1,1,1,1,
 2,2,2,2,2,2,2,2,
 3,3,3,3,3,3,3,3
};

static unsigned char planeReorder24[24]={
  16,17,18,19,20,21,22,23,
  0,1,2,3,4,5,6,7,
  8,9,10,11,12,13,14,15
};


#define ABitmapComplexMin(a,b) ((a)<(b)?(a):(b))


////////////////////////////////////////////////////////////////////////////////
//  AConvertingBitLine Class
////////////////////////////////////////////////////////////////////////////////

AConvertingBitLine::AConvertingBitLine(unsigned int w, unsigned int d, unsigned int p)
{
  DEBUG_OUT<<"AConvertingBitLine::AConvertingBitLine()\n";
  init();
  width=w;  height=1;  depth=d;  nPlanes=p;  originalBits=depth;
  calc();
  for(unsigned int t=0;t<nPlanes;t++) {
    plane[t]=(unsigned char *)malloc(bytesPerLine);
    if(!plane[t]) {
      DEBUG_OUT<<"cons: Couldn't get memory for bit line plane "<<t<<"!!\n";
      exit(5);
      return;
    }
    needFree[t]=true;
  }
  if(!trueColor) colors=new APalette((unsigned int)bits2Bytes(depth));
}


AConvertingBitLine::AConvertingBitLine(ABitmap *source)
{
  DEBUG_OUT<<"AConvertingBitLine::AConvertingBitLine()\n";
  init();
  copyFields(source);
  height=1;
  originalBits=depth;
  calc();
  for(unsigned int t=0;t<nPlanes;t++) {
    plane[t]=(unsigned char *)NULL; needFree[t]=false;
  }
}


AConvertingBitLine::~AConvertingBitLine()
{
  DEBUG_OUT<<"AConvertingBitLine::~AConvertingBitLine()\n";
  if(temp24) { delete temp24; temp24=(AConvertingBitLine *)NULL; }
  freeBitmap();
}


void AConvertingBitLine::init()
{
  ABitmap::init();
  originalBits=0;
  warned=false;
  temp24=(AConvertingBitLine *)NULL;
}


// NOTE: source does NOT have to have twentyFourBits==true
// ...it can be 24 bit chunky or planar...
void AConvertingBitLine::twentyFour2Chunky8(AConvertingBitLine *source)
{
  if(!source) { DEBUG_OUT<<"no source!\n"; return; }
  if(compressed) {
    DEBUG_OUT<<"tf2e can't handle HAM modes!\n";
    return;
  }
  if(!eightBits) {
    DEBUG_OUT<<"tf2e non eightBit dest!\n";
    return;
  }
  if(!source->colors) {
    DEBUG_OUT<<"tf2e::no fake palette!\n";
    return;
  }
  if(source->depth!=24) {
    DEBUG_OUT<<"tf2e::need 24 depth!\n";
    return;
  }
  AConvertingBitLine tline(width,24,1);
  if(source->twentyFourBits) tline.duplicate(source);
  else tline.planes2Chunky24(source);
  if(!tline.twentyFourBits) {
    DEBUG_OUT<<"tf2e::need 24 tline!\n";
    return;
  }
  colors=source->colors;  ownColors=false;
  unsigned char bite;
  unsigned char rbite,gbite,bbite;
  unsigned long x,copyLen=tline.bytesPerLine/3;
  if(bytesPerLine<copyLen) copyLen=bytesPerLine;
  for(x=0;x<bytesPerLine;x++) plane[0][x]=0;
  for(x=0;x<copyLen;x++) {
    rbite=tline.plane[0][x*3];
    gbite=tline.plane[0][x*3+1];
    bbite=tline.plane[0][x*3+2];
    bite=(unsigned char)source->colors->findBestColor(rbite,gbite,bbite);
    plane[0][x]=bite;
  }
  originalBits=source->depth;
}


void AConvertingBitLine::planes2Chunky8(AConvertingBitLine *source)
{
  if(!source) { DEBUG_OUT<<"no source!\n"; return; }
  if(compressed) {
    DEBUG_OUT<<"p2e can't handle HAM modes!\n";
    return;
  }
  if((source->nPlanes==24)&&(source->colors)) {
    // Can only do this is we have a fake palette...
    twentyFour2Chunky8(source);
    return;
  }
  if(source->nPlanes>8) {
    DEBUG_OUT<<"p2e can't handle more than 8 bits per pixel!\n";
    return;
  }
  if(!eightBits) {
    DEBUG_OUT<<"p2e non eightBit dest!\n";
    return;
  }
  if(source->bitmapType!=A_BITMAP_PLANES) {
    DEBUG_OUT<<"p2e non planar source!\n";
    return;
  }
  colors=source->colors;  ownColors=false;
  unsigned char dmask,bite,data;
  unsigned long p,bit,x,copyLen=source->bytesPerLine;
  if((bytesPerLine/8)<copyLen) copyLen=bytesPerLine/8;
  for(x=0;x<bytesPerLine;x++) plane[0][x]=0;
  for(x=0;x<copyLen;x++)
    for(bit=0;bit<8;bit++) {
      for(p=0;p<source->nPlanes;p++) {
        dmask=bitMask8[7-bit];
        bite=source->plane[p][x];
        data=(unsigned char)((bite&dmask)!=0);
        if(((x<<3)+bit)<bytesPerLine)
          plane[0][(x<<3)+bit]|=(unsigned char)((data&1)<<p);
      }
    }
  originalBits=source->depth;
}


void AConvertingBitLine::remapChunky8(APalette *givenColors)
{
  if(!givenColors) {
    DEBUG_OUT<<"rme Reduce, but no palette!\n";
    return;
  }
  if(nPlanes>8) {
    DEBUG_OUT<<"rme can't handle more than 8 bits per pixel!\n";
    return;
  }
  if(!eightBits) {
    DEBUG_OUT<<"rme non eightBit!\n";
    return;
  }
  if(!givenColors->getMap()) {
    DEBUG_OUT<<"rme Reduce, but no map!\n";
    return;
  }
  unsigned char bite;
  unsigned long x,copyLen=bytesPerLine;
  for(x=0;x<copyLen;x++) {
    bite=(unsigned char)givenColors->readMap(plane[0][x]);
    plane[0][x]=bite;
  }
}


void AConvertingBitLine::eightBits2Chunky(AConvertingBitLine *source)
{
  if(!source) { DEBUG_OUT<<"no source!\n"; return; }
  if(!source->eightBits) {
    DEBUG_OUT<<"e2c non eightBit source!\n";
    return;
  }
  if(depth==1) { eightBits2Planes(source); return; }
  if(bitmapType!=A_BITMAP_CHUNKY) {
    DEBUG_OUT<<"e2c non chunky dest!\n";
    return;
  }
  bool reduce=false;
  if((depth<8)&&(source->originalBits>depth)) {
    if(!source->colors) {
      DEBUG_OUT<<"e2c Reduce, but no palette!\n";
    } else
    if(!source->colors->getMap()) {
      DEBUG_OUT<<"e2c Reduce, but no map!\n";
    }
    else reduce=true;
  }
  unsigned long x;
  switch(depth) {
    case 2:
      if(reduce) {
        unsigned int p1,p2,p3,p4;
        for(x=0;x<bytesPerLine;x++) {
          p1=source->colors->readMap(source->plane[0][(x<<2)]);
          p2=source->colors->readMap(source->plane[0][(x<<2)+1]);
          p3=source->colors->readMap(source->plane[0][(x<<2)+2]);
          p4=source->colors->readMap(source->plane[0][(x<<2)+3]);
          plane[0][x]=(unsigned char)((p1<<6)|(p2<<4)|(p3<<2)|(p4));
        }
      }
      else {
        for(x=0;x<bytesPerLine;x++) {
          plane[0][x]=(unsigned char)
            ((source->plane[0][(x<<2)]<<6)|(source->plane[0][(x<<2)+1]<<4)|
              (source->plane[0][(x<<2)+2]<<2)|(source->plane[0][(x<<2)+3]));
        }
      }
      break;
    case 4:
      if(reduce) {
        unsigned int p1,p2;
        for(x=0;x<bytesPerLine;x++) {
          p1=source->colors->readMap(source->plane[0][(x<<1)]);
          p2=source->colors->readMap(source->plane[0][(x<<1)+1]);
          plane[0][x]=(unsigned char)((p1<<4)|(p2));
        }
      }
      else {
        for(x=0;x<bytesPerLine;x++) {
          plane[0][x]=(unsigned char)((source->plane[0][(x<<1)]<<4)|
            (source->plane[0][(x<<1)+1]));
        }
      }
      break;
    case 8:
      // At this depth there is no reducing needed...
      memcpy(plane[0],source->plane[0],bytesPerLine);
      break;
    default:
      DEBUG_OUT<<"e2c: Bad depth=="<<depth<<"!\n";
      break;
  }
}


void AConvertingBitLine::chunky2Chunky8(AConvertingBitLine *source)
{
  if(!source) { DEBUG_OUT<<"no source!\n"; return; }
  if(!eightBits) {
    DEBUG_OUT<<"c2e non eightBit dest!\n";
    return;
  }
  if(source->bitmapType!=A_BITMAP_CHUNKY) {
    DEBUG_OUT<<"c2e non chunky source!\n";
    return;
  }
  unsigned long x,copyLen;
  switch(source->depth) {
    case 2:
      // FIXME: Check that source+dest line lengths are compatible
      for(x=0;x<source->bytesPerLine;x++) {
        unsigned char bite=source->plane[0][x];
        plane[0][(x<<2)]=(unsigned char)((bite>>6)&0x03);
        plane[0][(x<<2)+1]=(unsigned char)((bite>>4)&0x03);
        plane[0][(x<<2)+2]=(unsigned char)((bite>>2)&0x03);
        plane[0][(x<<2)+3]=(unsigned char)(bite&0x03);
      }
      break;
    case 4:
      // FIXME: Check that source+dest line lengths are compatible
      for(x=0;x<source->bytesPerLine;x++) {
        unsigned char bite=source->plane[0][x];
        plane[0][(x<<1)]=(unsigned char)((bite>>4)&0x0f);
        plane[0][(x<<1)+1]=(unsigned char)(bite&0x0f);
      }
      break;
    case 8:
      // FIXME: Check that source+dest line lengths are compatible
      copyLen=ABitmapComplexMin(bytesPerLine,source->bytesPerLine);
      memcpy(plane[0],source->plane[0],copyLen);
      break;
    case 24:
      // Can only do this is we have a fake palette...
      if(source->colors) twentyFour2Chunky8(source);
      else {
        DEBUG_OUT<<"c2e: 24 bit and no fake palette...\n";
      }
      break;
    default:
      DEBUG_OUT<<"depth=="<<source->depth<<" in c2e!\n";
      break;
  }
  originalBits=source->depth;
}


// FIXME: Obviously not finished
void AConvertingBitLine::twentyFourBits2Chunky(AConvertingBitLine *source)
{
  if(!source) { DEBUG_OUT<<"no source!\n"; return; }
  if(!source->twentyFourBits) {
    DEBUG_OUT<<"tf2c non twentyFourBit source!\n";
    return;
  }
  if(depth==1) { twentyFourBits2Planes(source); return; }
  if(bitmapType!=A_BITMAP_CHUNKY) {
    DEBUG_OUT<<"tf2c non chunky dest!\n";
    return;
  }
  bool reduce=false;
  if(depth<9) {
    if(!colors) {
      DEBUG_OUT<<"tf2c Reduce, but no palette!\n";
    } else
    if(!source->colors->getMap()) {
      DEBUG_OUT<<"tf2c Reduce, but no map!\n";
    }
    else reduce=true;
    if(!reduce) return;
  }
  unsigned long x,copyLen,i;
  unsigned char r,g,b;
  switch(depth) {
    case 15:
      // FIXME: This is actually the same as 16 bit and most probably wrong for real 15 bit
      for(x=0;x<width;x++) {
        r=source->plane[0][x*3];
        g=source->plane[0][x*3+1];
        b=source->plane[0][x*3+2];
        i=((r>>3)<<10)+((g>>3)<<5)+(b>>3);
        plane[0][x*2+0]=(unsigned char)(i&0xff);
        plane[0][x*2+1]=(unsigned char)((i>>8)&0xff);
      }
      break;
    case 16:
      for(x=0;x<width;x++) {
        r=source->plane[0][x*3];
        g=source->plane[0][x*3+1];
        b=source->plane[0][x*3+2];
        i=((r>>3)<<10)+((g>>3)<<5)+(b>>3);
        //plane[0][x*2+0]=(unsigned char)(i&0xff);
        //plane[0][x*2+1]=(unsigned char)((i>>8)&0xff);
        plane[0][x*2+1]=(unsigned char)(i&0xff);
        plane[0][x*2+0]=(unsigned char)((i>>8)&0xff);
      }
      break;
    case 24:
      copyLen=ABitmapComplexMin(bytesPerLine,source->bytesPerLine);
      memcpy(plane[0],source->plane[0],copyLen);
      break;
    case 32:
      for(x=0;x<width;x++) {
        plane[0][x*4+0]=0;  // unused
        plane[0][x*4+1]=source->plane[0][x*3+0];  // red
        plane[0][x*4+2]=source->plane[0][x*3+1];  // green
        plane[0][x*4+3]=source->plane[0][x*3+2];  // blue
        //plane[0][x*4+3]=0;  // unused
        //plane[0][x*4+2]=source->plane[0][x*3+0];  // red
        //plane[0][x*4+1]=source->plane[0][x*3+1];  // green
        //plane[0][x*4+0]=source->plane[0][x*3+2];  // blue
      }
      break;
    default:
      DEBUG_OUT<<"tf2c: Bad depth=="<<depth<<"!\n";
      break;
  }
}


void AConvertingBitLine::chunky2Chunky24(AConvertingBitLine *source)
{
  if(!source) { DEBUG_OUT<<"no source!\n"; return; }
  if(!twentyFourBits) {
    DEBUG_OUT<<"c2tf non twentyFourBit dest!\n";
    return;
  }
  if(source->bitmapType!=A_BITMAP_CHUNKY) {
    DEBUG_OUT<<"c2tf non chunky source!\n";
    return;
  }
  unsigned long x,copyLen;
  unsigned char r,g,b,left,right;
  switch(source->depth) {
    case 24:
      copyLen=ABitmapComplexMin(bytesPerLine,source->bytesPerLine);
      memcpy(plane[0],source->plane[0],copyLen);
      break;
    case 32:
      copyLen=ABitmapComplexMin(width,source->width);
      for(x=0;x<copyLen;x++) {
        // (drop the alpha)
        plane[0][x*3]=source->plane[0][x*4+1];  // red
        plane[0][x*3+1]=source->plane[0][x*4+2];  // green
        plane[0][x*3+2]=source->plane[0][x*4+3];  // blue
      }
      break;
    case 15:
      copyLen=ABitmapComplexMin(width,source->width);
      if(!warned) {
        warned=true;
        DEBUG_OUT<<"Converting 15 bits to 24!  Untested!\n";
      }
      for(x=0;x<copyLen;x++) {
        r=(unsigned char)(source->plane[0][x*2+1]&0xf8);  // red 5
        g=(unsigned char)
         ((((source->plane[0][x*2+1]&0x7)<<5)+((source->plane[0][x*2+2]&0xe0)>>5))
            &0xfc);  // green  5
        b=(unsigned char)((source->plane[0][x*2+2]<<3)&0xf8);
          // blue 5
        plane[0][x*3]=r;  plane[0][x*3+1]=g;  plane[0][x*3+2]=b;
      }
      break;
    case 16:
      copyLen=ABitmapComplexMin(width,source->width);
      for(x=0;x<copyLen;x++) {
        left=source->plane[0][x*2+1];  right=source->plane[0][x*2+0];
        r=(unsigned char)(((left>>3)&0x1f)<<3);
        g=(unsigned char)(((left&0x7)<<3)+((right&0xe0)>>5));
        b=(unsigned char)((right&0x1f)<<3);
        plane[0][x*3]=r;  plane[0][x*3+1]=g;  plane[0][x*3+2]=b;
      }
      break;
    default:
      //DEBUG_OUT<<"depth=="<<source->depth<<" in c2tf!\n";
      if(source->eightBits) eightBits2Chunky24(source);
      else {
        AConvertingBitLine tsource(width,8);
        tsource.chunky2Chunky8(source);
        eightBits2Chunky24(&tsource);
      }
      break;
  }
  originalBits=source->depth;
}


void AConvertingBitLine::planes2Chunky24(AConvertingBitLine *source)
{
  if(!source) { DEBUG_OUT<<"no source!\n"; return; }
  if(source->compressed) { ham2Chunky24(source); return; }
  if(source->depth<9) {
    AConvertingBitLine tempLine(width,8,1);
    tempLine.planes2Chunky8(source);
    eightBits2Chunky24(&tempLine);
    return;
  }
  if(source->depth>24) {
    DEBUG_OUT<<"p2tf can't handle more than 24 bits per pixel!\n";
    return;
  }
  if(!twentyFourBits) {
    DEBUG_OUT<<"p2tf non twentyFourBit dest!\n";
    return;
  }
  if(source->bitmapType!=A_BITMAP_PLANES) {
    DEBUG_OUT<<"p2tf non planar source!\n";
    return;
  }
  unsigned char dmask,bite,data;
  unsigned long p,bit,x,copyLen=source->bytesPerLine;
  if((bytesPerLine/24)<copyLen) copyLen=bytesPerLine/24;
  for(x=0;x<bytesPerLine;x++) plane[0][x]=0;
  for(x=0;x<copyLen;x++)
    for(bit=0;bit<24;bit++) {
      for(p=0;p<source->nPlanes;p++) {
        dmask=bitMask8[7-(bit&0x7)];
        bite=source->plane[p][x];
        data=(unsigned char)((bite&dmask)!=0);
        plane[0][((x<<3)+(bit&0x7))*3+offset24[p]]|=
          (unsigned char)((data&1)<<(p&0x7));
      }
    }
  originalBits=source->depth;
}


void AConvertingBitLine::twentyFourBits2Planes(AConvertingBitLine *source)
{
  if(!source) { DEBUG_OUT<<"no source!\n"; return; }
  if(!source->twentyFourBits) {
    DEBUG_OUT<<"tf2p non twentyFourBit source!\n";
    return;
  }
  if(bitmapType!=A_BITMAP_PLANES) {
    DEBUG_OUT<<"tf2p non planes dest!\n";
    return;
  }
  bool reduce=false;
  if(depth<9) {
    reduce=true;
    if(!source->colors) {
      DEBUG_OUT<<"tf2p Reduce, but no palette!\n";
      return;
    }
    if(!source->colors->getMap()) {
      DEBUG_OUT<<"tf2p Reduce, but no map!\n";
      return;
    }
  }
  if(nPlanes==24) { twentyFourBitsChunky2Planes(source); return; }
  // NOTE: Continue here only if dest is not 24 bits...
  if(!temp24) temp24=new AConvertingBitLine(width,24,24);
  if(temp24) {
    temp24->twentyFourBitsChunky2Planes(source);
    // Now we have 24 planes decoded correctly in temp24
    unsigned int t,planePick[NUM_PLANES],doPlane[NUM_PLANES];
    for(t=0;t<NUM_PLANES;t++) { planePick[t]=0; doPlane[t]=1; }
    switch(nPlanes) {
      case 15:
        for(t=0;t<5;t++) planePick[t]=t+3; // green
        for(t=0;t<5;t++) planePick[t+5]=t+3+8; // blue
        for(t=0;t<5;t++) planePick[t+10]=t+3+16; // red
        break;
      case 16:
        for(t=0;t<5;t++) planePick[t]=t+3; // green
        for(t=0;t<6;t++) planePick[t+5]=t+2+8; // blue
        for(t=0;t<5;t++) planePick[t+11]=t+3+16; // red
        break;
      case 32:
        // FIXME: This may be totally wrong (off by 8 planes)
        // (where do those extra 8 bits go?!?)
        for(t=0;t<8;t++) planePick[t]=t; // green
        for(t=0;t<8;t++) planePick[t+8]=t+8; // blue
        for(t=0;t<8;t++) planePick[t+16]=t+16; // red
        break;
      default:
        DEBUG_OUT<<"tf2p not implemented for that nPlanes!\n";
        break;
    }
    unsigned int copyLen=bytesPerLine;
    if(temp24->bytesPerLine<copyLen) copyLen=temp24->bytesPerLine;
    for(t=0;t<nPlanes;t++)
      if(doPlane[t]) memcpyl(plane[t],temp24->plane[planePick[t]],copyLen);
  }
}


void AConvertingBitLine::twentyFourBitsChunky2Planes(AConvertingBitLine *source)
{
  if(!source) { DEBUG_OUT<<"no source!\n"; return; }
  if(!source->twentyFourBits) {
    DEBUG_OUT<<"tfc2p non twentyFourBit source!\n";
    return;
  }
  if(source->bitmapType!=A_BITMAP_CHUNKY) {
    DEBUG_OUT<<"tfc2p non chunky source!\n";
    return;
  }
  if(source->depth!=24) {
    DEBUG_OUT<<"tfc2p non 24-bit source!\n";
    return;
  }
  if(bitmapType!=A_BITMAP_PLANES) {
     DEBUG_OUT<<"tfc2p non planes dest!\n";
     return;
  }
  if(nPlanes!=24) {
    DEBUG_OUT<<"tfc2p non 24 plane dest!\n";
    return;
  }
  unsigned char dmask,bite,data;
  unsigned long p,bit,x,copyLen=bytesPerLine,soffset,doffset;
  if((source->bytesPerLine/8)<copyLen) copyLen=source->bytesPerLine/8;
  for(x=0;x<bytesPerLine;x++) { for(p=0;p<nPlanes;p++) plane[p][x]=0; }
  for(x=0;x<width;x++) {
    //DEBUG_OUT<<"x="<<x<<" ";
    for(p=0;p<nPlanes;p++) {
      //DEBUG_OUT<<"p="<<p<<" ";
      soffset=offset24[p]+(x*3)-1;
      doffset=x/8;
      bite=source->plane[0][soffset];
      bit=bite&bitMask24[p];
      //DEBUG_OUT<<"bit="<<bit<<" ";
      if(bit) {
        data=plane[planeReorder24[p]][doffset];
        data|=1<<(7-(x%8));
        plane[planeReorder24[p]][doffset]=data;
      }
    }
    //DEBUG_OUT<<"\n";
  }
}


void AConvertingBitLine::eightBits2Planes(AConvertingBitLine *source)
{
  if(!source) { DEBUG_OUT<<"no source!\n"; return; }
  if(nPlanes>8) {
    DEBUG_OUT<<"e2p can't handle more than 8 bits per pixel!\n";
    return;
  }
  if(!source->eightBits) {
    DEBUG_OUT<<"e2p non eightBit source!\n";
    return;
  }
  if(bitmapType!=A_BITMAP_PLANES) {
    DEBUG_OUT<<"e2p non planes dest!\n";
    return;
  }
  bool reduce=false;
  if((depth<8)&&(source->originalBits>depth)) {
    if(!source->colors) {
      DEBUG_OUT<<"e2p Reduce, but no palette!\n";
    } else
    if(!source->colors->getMap()) {
      DEBUG_OUT<<"e2p Reduce, but no map!\n";
    }
    else reduce=true;
  }
  unsigned char bitMask[8]={ 1,2,4,8,16,32,64,128 },dmask,bite,data;
  unsigned long p,bit,x,copyLen=bytesPerLine;
  if((source->bytesPerLine/8)<copyLen) copyLen=source->bytesPerLine/8;
  for(x=0;x<bytesPerLine;x++) for(p=0;p<nPlanes;p++) plane[p][x]=0;
  for(x=0;x<copyLen;x++) {
    for(p=0;p<nPlanes;p++) {
      for(bit=0;bit<8;bit++) {
        dmask=bitMask[p];
        if(reduce) {
          bite=(unsigned char)
            source->colors->readMap(source->plane[0][(x<<3)+(8-bit)]);
        }
        else bite=source->plane[0][(x<<3)+(8-bit)];
        data=(unsigned char)((bite&dmask)!=0);
        plane[p][x]|=(unsigned char)((data&1)<<bit);
      }
    }
  }
}


void AConvertingBitLine::ham2Chunky24(AConvertingBitLine *source)
{
  if(!source) { DEBUG_OUT<<"no source!\n"; return; }
  if(!source->compressed) {
    DEBUG_OUT<<"h2tf can only handle HAM mode lines!\n";
    return;
  }
  if(!twentyFourBits) {
    DEBUG_OUT<<"h2tf non twentyFourBit dest!\n"; return;
  }
  if(source->bitmapType!=A_BITMAP_PLANES) {
    DEBUG_OUT<<"h2tf non planar source!\n"; return;
  }
  if(!source->colors) {
    DEBUG_OUT<<"h2tf HAM bitline doesn't have palette!!!\n"; return;
  }
  AConvertingBitLine eLine(source->width,8,1);
  eLine.planes2Chunky8(source);
  unsigned long copyLen=width;
  if(source->width<width) copyLen=source->width;
  unsigned char r,g,b;
  unsigned char mode,data;
  unsigned int shift,mask,lmask;
  if(source->nPlanes==6) { shift=4;  mask=0xf; lmask=4; } else
  if(source->nPlanes==8){ shift=6;  mask=0x3f; lmask=2; } else
  {
    DEBUG_OUT<<"Bad depth for h2tf!\n";
    return;
  }
  r=(unsigned char)source->colors->red(0);
  g=(unsigned char)source->colors->green(0);
  b=(unsigned char)source->colors->blue(0);
  for(unsigned long t=0;t<copyLen;t++) {
    data=eLine.plane[0][t];
    mode=(unsigned char)((data>>shift)&0x3);
    data&=(unsigned char)mask;
    switch(mode) {
      case 0x0:
        //if(data<source->colors->getNumColors()) {
          r=(unsigned char)source->colors->red(data);
          g=(unsigned char)source->colors->green(data);
          b=(unsigned char)source->colors->blue(data);
        //}
        break;
      case 0x2:  r=(unsigned char)(data<<lmask);  break;  //red
      case 0x3:  g=(unsigned char)(data<<lmask);  break;  //green
      case 0x1:  b=(unsigned char)(data<<lmask);  break;  //blue
    }
    plane[0][t*3]=r;  plane[0][t*3+1]=g;  plane[0][t*3+2]=b;
  }
  originalBits=source->depth;
}


void AConvertingBitLine::eightBits2Chunky24(AConvertingBitLine *source)
{
  if(!source) { DEBUG_OUT<<"no source!\n"; return; }
  if(!twentyFourBits) {
    DEBUG_OUT<<"e2tf non twentyFourBit dest!\n";
    return;
  }
  if(!source->eightBits) {
    DEBUG_OUT<<"e2tf non eightBit source!\n";
    return;
  }
  if(!source->colors) {
    DEBUG_OUT<<"e2tf source doesn't have a palette!\n";
    return;
  }
  unsigned long x,copyLen=width;
  if(width>source->width) copyLen=source->width;
  unsigned char r,g,b,c;
  for(x=0;x<copyLen;x++) {
    c=source->plane[0][x];
    r=(unsigned char)source->colors->red(c);
    g=(unsigned char)source->colors->green(c);
    b=(unsigned char)source->colors->blue(c);
    //plane[0][x*3]=r;  plane[0][x*3+1]=g;  plane[0][x*3+2]=b;
    plane[0][x*3]=b;  plane[0][x*3+1]=g;  plane[0][x*3+2]=r;
  }
  originalBits=source->depth;
  calc();
}


void AConvertingBitLine::resetHistogram()
{
  if(depth<9) {
    if(!colors) {
      DEBUG_OUT<<"Line doesn't have a palette!!!\n";
      return;
    }
  }
  if(colors) colors->resetHistogram();
}


void AConvertingBitLine::cumulativeHistogram(AConvertingBitLine *source)
{
  if(!source) { DEBUG_OUT<<"cumulativeHistogram no source!\n"; return; }
  if(eightBits) {
    if(!source->colors) {
      DEBUG_OUT<<"Src doesn't have a palette!!!\n";
      return;
    }
    if(!source->colors->getScore()) {
      DEBUG_OUT<<"Src doesn't have a histogram!!!\n";
      return;
    }
    long sc;
    for(unsigned x=0;x<width;x++) {
      sc=source->colors->readScore(plane[0][x]);
      source->colors->writeScore(plane[0][x],sc+1);
    }
    return;
  }
  if(twentyFourBits) {
    // FIXME: This doesn't do anything...it weighs all colors equally!
    return;
  }
  DEBUG_OUT<<"Can't do histogram for this type of pixels!\n";
}


// NOTE: Assumes that source and dest are compatible and probably chunky 8 or 24 or something like that.
void AConvertingBitLine::scaleLine(AConvertingBitLine *source,int xFactor)
{
  if(!source) { DEBUG_OUT<<"no source!\n"; return; }
  bool err=false;
  if(source->depth!=depth) err=true;
  if(source->nPlanes!=nPlanes) err=true;
  if(source->bitmapType!=bitmapType) err=true;
  if(err) { DEBUG_OUT<<"mismatched lines in scale\n"; return; }
  unsigned int len=source->bytesPerLine;
  if(len>(bytesPerLine)) len=bytesPerLine;
  for(unsigned int t=0;t<source->bytesPerLine;t++) {
    for(unsigned int p=0;p<nPlanes;p++) {
      char bite=source->plane[p][t];
      for(int s=0;s<xFactor;s++) {
        plane[p][t*xFactor+s]=bite;
        //DEBUG_OUT<<bite<<"*";
      }
    }
  }
  //DEBUG_OUT<<"\n";
  modified=true;
}


void AConvertingBitLine::flipLine()
{
  //if(depth<9) { convertChunky8(); }
  //else { convertChunky24(); }
  for(unsigned int t=0;t<width/2;t++) {
    for(unsigned int p=0;p<nPlanes;p++) {
      char v=plane[p][t];
      plane[p][t]=plane[p][width-t];
      plane[p][width-t]=v;
    }
  }
  modified=true;
}


// A useful shorthand function...
void AConvertingBitLine::convertChunky8()
{
  if((depth==8)&&(bitmapType==A_BITMAP_CHUNKY)) return;
  AConvertingBitLine tLine(width,8,1);
  if(bitmapType==A_BITMAP_CHUNKY) tLine.chunky2Chunky8(this);
  if(bitmapType==A_BITMAP_PLANES) tLine.planes2Chunky8(this);
  duplicate(&tLine);
  // Steal the planes from the temp bitline
  for(unsigned int p=0;p<nPlanes;p++) {
    needFree[p]=true;
    tLine.needFree[p]=false;
  }
  // Let tLine be automatically deleted...
  modified=true;
}


// A useful shorthand function...
void AConvertingBitLine::convertChunky24()
{
  if((depth==24)&&(bitmapType==A_BITMAP_CHUNKY)) return;
  AConvertingBitLine *tLine=new AConvertingBitLine(width,24,1);
  if(bitmapType==A_BITMAP_CHUNKY) { tLine->chunky2Chunky24(this); return; }
  if(bitmapType==A_BITMAP_PLANES) { tLine->planes2Chunky24(this); return; }
  duplicate(tLine);
  // Steal the planes from the temp bitline
  for(unsigned int p=0;p<nPlanes;p++) {
    needFree[p]=true;
    tLine->needFree[p]=false;
  }
  //delete tLine;
  tLine=NULL;
  modified=true;
}


void AConvertingBitLine::debugDump()
{
  for(unsigned int t=0;t<nPlanes;t++) {
    DEBUG_OUT<<"plane "<<t<<":";
    for(unsigned int q=0;q<bytesPerLine;q++) DEBUG_OUT<<" "<<(int)plane[t][q];
    DEBUG_OUT<<"\n";
  }
}

