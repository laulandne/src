
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#include <iostream>
#include <stdlib.h>
#include <string.h>

#include <portable/nickcpp/APalette.h>
#include <portable/nickcpp/ANullStream.h>

#include "ABMPDecoder.h"


////////////////////////////////////////////////////////////////////////////////
//  ABMPDecoder Class
////////////////////////////////////////////////////////////////////////////////

// This struct is 14 bytes on disk
struct aBmpFileHeader {
  uint16_t type;       //2
  uint32_t size;        //4
  uint16_t res1,res2;  //4
  uint32_t offBits;     //4
};

// This struct is 28 bytes on disk
struct aBmpInfoHeader {
  uint32_t size;                //4
  uint32_t width,height;        //4
  uint16_t planes,bitCount;    //4
  uint32_t compressed;          //4
  uint32_t imageSize;           //4
  uint16_t xpels,ypels;         //4
  uint32_t used,important;      //4
};

// This struct is 12 bytes on disk
struct aBmpOldInfoHeader {
  uint32_t size;             //4
  uint16_t width,height;    //4
  uint16_t planes,bitCount; //4
};


ABMPDecoder::ABMPDecoder(const char *fname) : AImageDecoder(fname)
{
  DEBUG_OUT<<"ABMPDecoder::ABMPDecoder\n";
  init();
  //bm=(ABitmapSimple *)NULL;
  isArray=false;  findBest=true;
  isIcon=false;  oldBMP=false;  endRLE=false;
  openFile(fname);
  header();
  //errorFlag = !(boundsValid());
  /*if(!error()) if (!(*/ initLineBuffer(); //)) errorFlag = true;
  /*if(!error())*/ color();
  fseek(fd,bitsSeek,SEEK_SET);
}


ABMPDecoder::~ABMPDecoder()
{
  if(bm) delete bm;
  bm=(ABitmap *)NULL;
}


/* STATIC */
bool ABMPDecoder::recognize(uint8_t *buffer,char *fname)
{
  if(!strncmp("BM",(char *)buffer,2)) return true; else
  if(!strncmp("IC",(char *)buffer,2)) return true; else
  if(!strncmp("CI",(char *)buffer,2)) return true; else
  if(!strncmp("PT",(char *)buffer,2)) return true; else
  if(!strncmp("CP",(char *)buffer,2)) return true; else
  if(!strncmp("BA",(char *)buffer,2)) return true; else
  return false;
}


void ABMPDecoder::header()
{
  if(!fd) return;
  aBmpFileHeader bfh;
  aBmpInfoHeader bih;
  // Need to clear these structs here...
  bih.compressed=0;
  char ftype[4];
  ftype[2]=0;
  fseek(fd,0,SEEK_SET);
  fread((void *)ftype,2,1,fd);
  unsigned int headerSeek=0;
  readLongL();  readShortL();  readShortL();
  bitsSeek=readLongL();
  DEBUG_OUT<<"(bitsSeek is "<<bitsSeek<<")\n";
  if(!strncmp("IC",ftype,2)) isIcon=true;
  if(!strncmp("CI",ftype,2)) isIcon=true;
  if(!strncmp("PT",ftype,2)) isIcon=true;
  if(!strncmp("CP",ftype,2)) isIcon=true;
  if(!strncmp("BA",ftype,2)) isArray=true;
  if(isArray) {
    headerSeek=14; DEBUG_OUT<<"OS/2 style array bitmap...\n";
  }
  if(isIcon) DEBUG_OUT<<"OS/2 style icon bitmap...\n";
  fseek(fd,14+headerSeek,SEEK_SET);
  unsigned short bihsize,rdsize;
  bihsize=readShortL();
  rdsize=bihsize;
  if(rdsize>28) rdsize=28;
  DEBUG_OUT<<"seek is "<<(int)headerSeek<<"  bihsize is "<<(int)bihsize<<"  rdsize is "<<(int)rdsize<<".\n";
  if(!bihsize) {
    DEBUG_OUT<<"No header!!\n";  width=0;  height=0;  nPlanes=0;  depth=0;  return;
  }
  // Maybe this should be 14...
  if(rdsize==12) {
    aBmpOldInfoHeader bioh;
    fseek(fd,headerSeek,SEEK_SET);
    fread((void *)&bfh,14,1,fd);
    fread((void *)&bioh,rdsize,1,fd);
    if(!littleEndian) {
      byteSwap(&bioh.width);  byteSwap(&bioh.height);
      byteSwap(&bioh.planes); byteSwap(&bioh.bitCount);
      byteSwap(&bih.compressed);
    }
    depth=bioh.bitCount;  width=bioh.width;  height=bioh.height;
    nPlanes=bioh.planes;
    oldBMP=true;
  }
  else {
    fseek(fd,headerSeek,SEEK_SET);
    // Read file header
    bfh.type=readShortL();
    bfh.size=readLongL();
    bfh.res1=readShortL();
    bfh.res2=readShortL();
    bfh.offBits=readLongL();
    // Read Info Header
    unsigned int off=4;
    if(rdsize>off) { bih.size=(unsigned short)readLongL(); off+=4; }
    if(rdsize>off) { bih.width=(unsigned short)readLongL(); off+=4; }
    if(rdsize>off) { bih.height=(unsigned short)readLongL(); off+=4; }
    if(rdsize>off) { bih.planes=(unsigned short)readShortL(); off+=2; }
    if(rdsize>off) { bih.bitCount=(unsigned short)readShortL(); off+=2; }
    if(rdsize>off) { bih.compressed=readLongL(); off+=4; }
    if(rdsize>off) { bih.imageSize=readLongL(); off+=4; }
    if(rdsize>off) { bih.xpels=readLongL(); off+=4; }
    if(rdsize>off) { bih.ypels=readLongL(); off+=4; }
    if(rdsize>off) { bih.used=readLongL(); off+=4; }
    if(rdsize>off) { bih.important=readLongL(); off+=4; }
    depth=bih.bitCount;  width=(int)bih.width;  height=(int)bih.height;
    nPlanes=bih.planes;
    DEBUG_OUT<<"BMP: "<<width<<"x"<<height<<"x"<<depth<<" ("<<nPlanes<<")\n";
    if(bih.compressed) {
      DEBUG_OUT<<"BMP is compressed: "<<(bih.compressed&0xf)<<" ("<<bih.compressed<<") ";
      switch(bih.compressed) {
        case 1L: DEBUG_OUT<<"RLE8\n";  break;
        case 2L: DEBUG_OUT<<"RLE4\n";  break;
        case 3L: if(depth==1) DEBUG_OUT<<"HUFF1\n"; else DEBUG_OUT<<"BF\n";  break;
        case 4L: DEBUG_OUT<<"RLE24\n";  break;
        default:
          //DEBUG_OUT.warning("Unknown compression method!\n");
          width=0;  height=0;  nPlanes=0;  depth=0;  return;
          //break;
      }
      compField=bih.compressed;  compressed=true;
    }
  }
  bytesPerLine=calcBPL(4);
  fseek(fd,14+bihsize+headerSeek,SEEK_SET);
}


// FIXME: This doesn't seem to work too well with 1 bit bmp's
void ABMPDecoder::color()
{
  unsigned int howMany;
  switch(depth) {
    case 1: howMany=2;  break;
    case 2: howMany=4;  break;
    case 4: howMany=16;  break;
    case 8: howMany=256;  break;
    case 24: howMany=0; colors=(APalette *)NULL; return; //break;
    default:
      //DEBUG_OUT.warning("Unimplemented depth!\n");
      howMany=2;
      break;
  }
  // Only create the palette if it is needed
  colors=new APalette(howMany);
  char skip,r,g,b;
  for(unsigned int t=0;t<colors->getNumColors();t++) {
    fread(&b,1,1,fd);  fread(&g,1,1,fd);  fread(&r,1,1,fd);
    if(!oldBMP) fread(&skip,1,1,fd);
    colors->setColor(t,r,g,b);
  }
}


uint8_t *ABMPDecoder::readLine(unsigned int lineNum, unsigned int numPlane)
{
  if(!lineBuffer) { DEBUG_OUT<<"no lineBuffer!\n";  return NULL; }
  if(lineNum>height) { DEBUG_OUT<<"line ref out of bounds!\n";  return NULL; }
  if(numPlane>depth) { DEBUG_OUT<<"plane ref out of bounds!\n";  return NULL; }
  if(!fd) { DEBUG_OUT<<"no fd!\n";  return NULL; }
  bool ro=false;
  if(depth==24) ro=true;
  if(!bm) {
    DEBUG_OUT<<"ABMPDecoder::readLine going to readRawBitmap...\n";
    readRawBitmap(ro);
    DEBUG_OUT<<"(readRawBitmap done)\n";
  }
  if(!bm) {
    DEBUG_OUT<<"Problem reading bitmap!\n";  return NULL;
  }
  uint8_t *thePlane=bm->getPlane(numPlane);
  if(!thePlane)  { DEBUG_OUT<<"no thePlane!\n";  return NULL; }
  uint8_t *src=(uint8_t *)(thePlane+((height-lineNum)*bytesPerLine));
  for(unsigned int t=0;t<bytesPerLine;t++) {
    lineBuffer[t]=src[t];
  }
  return lineBuffer;
}


/*
// TODO: The compression handling probably doesn't work at all!
unsigned char *ABMPDecoder::readLine(unsigned int lineNum, unsigned int numPlane)
{
  if(!lineBuffer) return (unsigned char *)NULL;
  if(lineNum>height) { DEBUG_OUT<<"line ref out of bounds!\n";  return (unsigned char *)NULL; }
  if(numPlane>depth) { DEBUG_OUT<<"plane ref out of bounds!\n";  return (unsigned char *)NULL; }
  if(!fd) return (unsigned char *)NULL;
  if(compressed) {
    unsigned int c=0;
    if(endRLE) c=bytesPerLine;
    // FIXME: Here I just fill in with color 0...is that right?!?
    while(c<bytesPerLine) {
      // Handle outstanding lineSkips
      if(lineSkip) {
        for(unsigned int t=0;t<bytesPerLine;t++) lineBuffer[t]=0;
        lineSkip--;  c=bytesPerLine;
      }
      // Handle outstanding byteSkips
      while((byteSkip)&&(c<bytesPerLine)) {
        lineBuffer[c]=0; c++;  byteSkip--;
      }
      // Handle normal case
      if(c<bytesPerLine) {
        unsigned char type;
        fread(&type,1,1,fd);
        if(type) {
          // rle record
          unsigned char val;
          fread(&val,1,1,fd);
          // FIXME: This doesn't handle RLE4 right!  (odd lengths)
          if(compField==2) type/=(unsigned char)2; //rle4
          if(compField==4) type/=(unsigned char)3; //rle24
          for(unsigned int t=0;t<(unsigned int)type;t++) {
            if(c<bytesPerLine) lineBuffer[c]=val;
            c++;
          }
        }
        else {
          // unecoded/misc run
          unsigned char val;
          fread(&val,1,1,fd);
          switch(val) {
            case 0:  // end of row
              c=bytesPerLine;  break;
            case 1:  // end of rle
              c=bytesPerLine;  endRLE=true; break;
            case 2:  // delta
              unsigned char x;
              fread(&x,1,1,fd);  byteSkip=x;
              fread(&x,1,1,fd);  lineSkip=x;
              break;
            default: // unencoded
              if(compField==2) type/=(unsigned char)2;
              for(unsigned int t=0;t<(unsigned int)type;t++) {
                if(c<bytesPerLine) lineBuffer[c]=val;
                c++;
              }
              break;
          } // switch(val)
        } // type
      } // if(c...)
    } // while(c...)
  } // compressed
  else fread(lineBuffer,1,bytesPerLine,fd);
  if(depth==24) {
    // rearrange the rgb values
    char s;
    for(unsigned int t=0;t<bytesPerLine;t+=3) {
      s=lineBuffer[t];  lineBuffer[t]=lineBuffer[t+2];  lineBuffer[t+2]=s;
    }
  }
  return lineBuffer;
}
*/


