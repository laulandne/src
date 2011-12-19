
//#define DEBUG_OUT std::cerr
#define DEBUG_OUT *aNullStream


#include <iostream>
#include <stdlib.h>
#include <string.h>

#include <portable/nickcpp/APalette.h>
#include <portable/nickcpp/ANullStream.h>

#include "ADecoder.h"


////////////////////////////////////////////////////////////////////////////////
//  ADecoder Class
////////////////////////////////////////////////////////////////////////////////

ADecoder::ADecoder(const char *fname)
{
#ifdef ASYS_MFC
  setClassName("ADecoder");
#endif // ASYS_MFC
  init();
}


ADecoder::~ADecoder()
{
}


void ADecoder::init()
{
  fd=(FILE *)NULL;
  filename=(char *)NULL;
  compressed=false;
  isAnImage=false;  isAMovie=false;  isAScene=false; isAMesh=false;  isASound=false;  isASong=false;
  littleEndian=true;
  // FIXME: This tries to work around shorts that are larger than 16 bits
  uint8_t endianTestC[]={ 0,1,2,3,4,5,6,7 };
  uint16_t *endianTestS=(uint16_t *)endianTestC;
  DEBUG_OUT<<"*endianTestS is "<<*endianTestS<<"\n";
  switch(*endianTestS) {
    case 256:
      littleEndian=true;
      break;
    case 1:
      littleEndian=false;
      break;
    default:
      DEBUG_OUT<<"endian test failed, we got something we didn't expect, check your types!\n";
      exit(5);
      break;
  }
  // Be pedantic about it and refuse to run if our assumptions aren't correct
  DEBUG_OUT<<"sizeof(uint8_t)=="<<sizeof(uint8_t)<<" (should be 1)\n";
  if(sizeof(uint8_t)!=1) exit(5);
  DEBUG_OUT<<"sizeof(uint16_t)=="<<sizeof(uint16_t)<<" (should be 2)\n";
  if(sizeof(uint16_t)!=2) exit(5);
  DEBUG_OUT<<"sizeof(uint32_t)=="<<sizeof(uint32_t)<<" (should be 4)\n";
  if(sizeof(uint32_t)!=4) exit(5);
  DEBUG_OUT<<"sizeof(float)=="<<sizeof(float)<<" (should be 4)\n";
  if(sizeof(float)!=4) exit(5);
}


void ADecoder::header()
{
  DEBUG_OUT<<"Parent ADecoder::header called!\n";
}


void ADecoder::body()
{
  DEBUG_OUT<<"Parent ADecoder::body called!\n";
}


void ADecoder::openFile(const char *imgFile)
{
  // open the file
  fd=fopen(imgFile,"rb");
  if(!fd) fd=fopen(stringHelp.removeQuotes(imgFile),"rb");
  if(!fd) fd=fopen(stringHelp.flipSlashes(imgFile),"rb");
  if(fd) {
    filename=strdup(imgFile);
  }
  else {
    std::cerr<<"Unable to open file: '"<<imgFile<<"'\n";
    //errorFlag=true;
  }
  //errorFlag=false;
}


// This is very nasty and foolish...but it works
void ADecoder::byteSwap(uint16_t *val)
{
  //DEBUG_OUT<<"(byteSwap16 "<<aThisApp.myHex4(*val)<<")";
  char *m;  char t[3];
  m=(char *)val;
  t[0]=m[1];  t[1]=m[0];
  m[0]=t[0];  m[1]=t[1];
}


// This is very nasty and foolish...but it works
void ADecoder::byteSwap(uint32_t *val)
{
  //DEBUG_OUT<<"(byteSwap32 "<<aThisApp.myHex8(*val)<<")";
  char *m;  char t[5];
  m=(char *)val;
  t[0]=m[3];  t[1]=m[2];  t[2]=m[1];  t[3]=m[0];
  m[0]=t[0];  m[1]=t[1];  m[2]=t[2];  m[3]=t[3];
}


unsigned long ADecoder::iffFindChunk(unsigned long chunkOffset,char *findName,bool bigEndian)
{
  unsigned long *foo=(unsigned long *)findName;
  return findChunk(chunkOffset,4,*foo,bigEndian,false);
}


unsigned long ADecoder::tagFindChunk(unsigned long chunkOffset,uint16_t tag,bool bigEndian)
{
  return findChunk(chunkOffset,2,tag,bigEndian,false);
}


unsigned long ADecoder::findChunk(unsigned long chunkOffset,int nameSize,uint32_t tagName,bool bigEndian,bool fixOdd)
{
  char *t=NULL;
  DEBUG_OUT<<"Looking for '";
  if(nameSize==4) {
    t=(char *)&tagName;
    DEBUG_OUT<<t[0]<<t[1]<<t[2]<<t[3];
  }
  //else dBug<<aThisApp.myHex4(tagName);
  DEBUG_OUT<<"'...chunkOffset is "<<chunkOffset<<"...\n";
  DEBUG_OUT<<"(nameSize="<<nameSize<<" bigEndian="<<(int)bigEndian<<" fixOdd="<<(int)fixOdd<<")\n";
  uint32_t cSize=0,ret=0;
  uint32_t cName=0;
  bool w=true;
  int val=fseek(fd,chunkOffset,SEEK_SET);
  if(val) w=false;
  while(w) {
    int e1=0;
    uint16_t tt;
    if(nameSize==2) { e1=fread((char *)&tt,nameSize,1,fd); }
    else e1=fread((char *)&cName,nameSize,1,fd);
    if(nameSize==2) {
      if(bigEndian) { if(littleEndian) byteSwap((uint16_t *)&tt); }
      else  { if(!littleEndian) byteSwap((uint16_t *)&tt); }
      cName=tt;
    }
    //if(e1!=1) { dBug<<"READ ERROR, e1="<<e1<<"!\n"; }
    int e2=fread((char *)&cSize,4,1,fd);
    //if(e2!=1) { dBug<<"READ ERROR, e2="<<e2<<"!\n"; }
    if(bigEndian) { if(littleEndian) byteSwap((uint32_t *)&cSize); }
    else  { if(!littleEndian) byteSwap((uint32_t *)&cSize); }
    if(nameSize==2) cSize-=6; // this should be a flag
    if(feof(fd)) w=false;
    else {
      DEBUG_OUT<<"...found '";
      if(nameSize==4) {
        t=(char *)&cName;
        DEBUG_OUT<<t[0]<<t[1]<<t[2]<<t[3];
      }
      //else dBug<<aThisApp.myHex4(cName);
      DEBUG_OUT<<"' sized "<<cSize<<" at "<<(ftell(fd)-6)<<"...\n"; // change the 6
      if(fixOdd) { if(cSize&1) cSize++; }
    }
    if((!e1)||(!e2)||(feof(fd))) {
      DEBUG_OUT<<" EOF reached, not found.";
      w=false;  ret=0;
    }
    if(tagName==cName) {
      DEBUG_OUT<<"...got what we were looking for!";
      w=false;  ret=cSize;
    }
    else {
      if(w) {
        DEBUG_OUT<<"...(going to seek "<<cSize<<")...\n";
        val=fseek(fd,cSize,SEEK_CUR);
        if(val) w=false;
        if(feof(fd)) w=false;
      }
    }
    //if(ret||w) dBug<<"Found chunk '"<<cName<<"' "<<(unsigned long)cSize<<" bytes...\n";
  }
  DEBUG_OUT<<"\n";
  return ret;
}


unsigned long ADecoder::iffCountChunks(unsigned long chunkOffset,char *findName,bool bigEndian)
{
  unsigned long *foo=(unsigned long *)findName;
  return countChunks(chunkOffset,4,*foo,bigEndian,false);
}


unsigned long ADecoder::tagCountChunks(unsigned long chunkOffset,uint16_t tag,bool bigEndian)
{
  return countChunks(chunkOffset,2,tag,bigEndian,false);
}


unsigned long ADecoder::countChunks(unsigned long chunkOffset,int nameSize,uint32_t tagName,bool bigEndian,bool fixOdd)
{
  char *t=NULL;
  DEBUG_OUT<<"nameSize="<<nameSize<<" bigEndian="<<(int)bigEndian<<" fixOdd="<<(int)fixOdd<<"\n";
  DEBUG_OUT<<"Counting '";
  if(nameSize==4) {
    t=(char *)&tagName;
    DEBUG_OUT<<t[0]<<t[1]<<t[2]<<t[3];
  }
  //else dBug<<aThisApp.myHex4(tagName);
  DEBUG_OUT<<"'...chunkOffset is "<<chunkOffset;
  uint32_t cSize,ret=0;
  uint32_t cName=0;
  bool w=true;
  int val=fseek(fd,chunkOffset,SEEK_SET);
  if(val) w=false;
  while(w) {
    int e1=0;
    uint16_t tt;
    if(nameSize==2) {e1=fread((char *)&tt,nameSize,1,fd); }
    else e1=fread((char *)&cName,nameSize,1,fd);
    if(nameSize==2) {
      if(bigEndian) { if(littleEndian) byteSwap((uint16_t *)&tt); }
      else  { if(!littleEndian) byteSwap((uint16_t *)&tt); }
      cName=tt;
    }
    //if(e1!=1) { dBug<<"READ ERROR, e1="<<e1<<"!\n"; }
    int e2=fread((char *)&cSize,4,1,fd);
    //if(e2!=1) { dBug<<"READ ERROR, e2="<<e2<<"!\n"; }
    if(bigEndian) { if(littleEndian) byteSwap((uint32_t *)&cSize); }
    else  { if(!littleEndian) byteSwap((uint32_t *)&cSize); }
    if(nameSize==2) cSize-=6; // This should be a flag
    DEBUG_OUT<<"...found '";
    if(nameSize==4) {
      t=(char *)&cName;
      DEBUG_OUT<<t[0]<<t[1]<<t[2]<<t[3];
    }
    //else dBug<<aThisApp.myHex4(cName);
    DEBUG_OUT<<"' sized "<<cSize<<" at "<<(ftell(fd)-6); // change the 6
    if(fixOdd) { if(cSize&1) cSize++; }
    if((!e1)||(!e2)||(feof(fd))) {
      DEBUG_OUT<<" EOF reached.";
      w=false;
    }
    if(tagName==cName) {
      DEBUG_OUT<<" (Got it!)";
      DEBUG_OUT<<" (going to seek "<<cSize<<") ";
      fseek(fd,cSize,SEEK_CUR);
      ret++;
    } else {
      DEBUG_OUT<<" (going to seek "<<cSize<<") ";
      val=fseek(fd,cSize,SEEK_CUR);
      if(val) w=false;
      if(feof(fd)) w=false;
    }
    //if(ret||w) dBug<<"Found chunk '"<<aThisApp.myHex4(cName)<<"' "<<(unsigned long)cSize<<" bytes...\n";
  }
  DEBUG_OUT<<"\n";
  return ret;
}


uint32_t ADecoder::readLongL()
{
  uint32_t l=0;
  uint8_t b=0;
  fread(&b,1,1,fd);  l+=b;
  fread(&b,1,1,fd);  l+=b*0x100;
  fread(&b,1,1,fd);  l+=b*0x10000L;
  fread(&b,1,1,fd);  l+=b*0x1000000L;
  return l;
}


uint32_t ADecoder::readLongB()
{
  uint32_t l=0;
  uint8_t b=0;
  fread(&b,1,1,fd);  l+=b*0x1000000L;
  fread(&b,1,1,fd);  l+=b*0x10000L;
  fread(&b,1,1,fd);  l+=b*0x100;
  fread(&b,1,1,fd);  l+=b;
  return l;
}


uint16_t ADecoder::readShortB()
{
  uint16_t l=0;
  uint8_t b=0;
  fread(&b,1,1,fd);  l+=b*0x100;
  fread(&b,1,1,fd);  l+=b;
  return l;
}


int16_t ADecoder::readSShortL()
{
  int16_t l=0;
  uint8_t b=0;
  fread(&b,1,1,fd);  l+=b;
  fread(&b,1,1,fd);  l+=b*0x100;
  return l;
}


uint16_t ADecoder::readShortL()
{
  uint16_t l=0;
  uint8_t b=0;
  fread(&b,1,1,fd);  l+=b;
  fread(&b,1,1,fd);  l+=b*0x100;
  return l;
}


uint8_t ADecoder::readByte()
{
  uint8_t b=0;
  fread(&b,1,1,fd);
  return b;
}


int8_t ADecoder::readSByte()
{
  int8_t b=0;
  fread(&b,1,1,fd);
  return b;
}


// NOTE: This makes gross assumptions about the format of a float!
float ADecoder::readFloatL()
{
  float l=0.0;
  fread(&l,4,1,fd);
  //if(!littleEndian) byteSwap((uint32_t *)&l);
  return (float)l;
}


// NOTE: This makes gross assumptions about the format of a float!
float ADecoder::readFloatB()
{
  float l=0.0;
  fread(&l,4,1,fd);
  if(littleEndian) byteSwap((uint32_t *)&l);
  return (float)l;
}


void ADecoder::readString(char *s,unsigned int len)
{
  char c;
  for(unsigned int t=0;t<len;t++) {
    fread(&c,1,1,fd);
    s[t]=c;
  }
}


void ADecoder::readNullString(char *s,unsigned int limit)
{
  if(!s) return;
  unsigned int i=0;
  bool reading=true;
  char c;
  while(reading) {
    fread(&c,1,1,fd);
    s[i]=c;
    i++;
    if(!c) reading=false;
    if(i==limit) reading=false;
  }
}


unsigned int ADecoder::bits2Num(unsigned int bits)
{
  unsigned int ret=1<<bits;
  if(sizeof(int)==2) {
    if(bits>31) ret=0xffffffffL; // FIXME: doesn't work for 16bit ints obviously...
  }
  return ret;
}


/* STATIC */
bool ADecoder::recognizeTextFile(uint8_t *buf)
{
  if(!buf) return false;
  bool ret=true;
  unsigned int i=0;
  for(i=0;i<128;i++) {
    uint8_t c=buf[i];
    //DEBUG_OUT<<(int)i<<" "<<(int)c<<"\n";
    if(c<30) {
      switch(c) {
        case 0: case 9: case 10: case 13: case 14: break;
        default:  ret=false; break;
      }
    }
    /*
    if(c>128) {
      switch(c) {
        default:  ret=false; break;
      }
    }
    */
  }
  return ret;
}

