
//#define DEBUG_OUT std::cerr
#define DEBUG_OUT *aNullStream


#include <iostream>
#include <string.h>
#include <stdlib.h>

#include <portable/nickcpp/ANullStream.h>

#include "AAUDecoder.h"


////////////////////////////////////////////////////////////////////////////////
//  AAUDecoder Class
////////////////////////////////////////////////////////////////////////////////

/* STATIC */
bool AAUDecoder::recognize(uint8_t *str)
{
  if(!strncmp((char *)str,".snd",4)) return true; else return false;
}


AAUDecoder::AAUDecoder(const char *fname) : ASoundDecoder(fname)
{
  init();
  openFile(fname);
  header();
  body();
}


void AAUDecoder::header()
{
  if(!fd) return;
  //if(errorFlag) return;
  fseek(fd,0,SEEK_SET);
  uint32_t foo;
  fread(&foo,4,1,fd);  // skip magic word...
  uint32_t loc;
  fread(&loc,4,1,fd);
  if(littleEndian) byteSwap(&loc);
  uint32_t size;
  fread(&size,4,1,fd);
  if(littleEndian) byteSwap(&size);
  uint32_t format;
  fread(&format,4,1,fd);
  if(littleEndian) byteSwap(&format);
  uint32_t rate;
  fread(&rate,4,1,fd);
  if(littleEndian) byteSwap(&rate);
  uint32_t channels;
  fread(&channels,4,1,fd);
  if(littleEndian) byteSwap(&channels);
  DEBUG_OUT<<"Sun/NeXT .au file.\n";
  DEBUG_OUT<<"loc: "<<loc<<"\n";
  DEBUG_OUT<<"size: "<<size<<"\n";
  DEBUG_OUT<<"format: "<<format<<"\n";
  DEBUG_OUT<<"rate: "<<rate<<"\n";
  DEBUG_OUT<<"channels: "<<channels<<"\n";
  sampleRate=rate;
  if(channels==2) stereo=true; else stereo=false;
  numBits=8;
  switch(format) {
    case 1: numBits=8; break;  //mu-law
    case 2: numBits=8; break;  //linear
    case 3: numBits=16; break;  //linear
    case 4: numBits=24; break;  //linear
    case 5: numBits=32; break;  //linear
    case 27: numBits=8; break;  //alaw
    default: DEBUG_OUT<<"Some other format...\n"; break;
  }
  dataLoc=loc;
  dataSize=size;
}


void AAUDecoder::body()
{
  if(!fd) return;
  //if(errorFlag) return;
  fseek(fd,dataLoc,SEEK_END);
  numSamples=dataSize;
  DEBUG_OUT<<"Sound is "<<numSamples<<" bytes long.\n";
  samples=(uint8_t *)malloc((int)numSamples);
  if(!samples) { DEBUG_OUT<<"No mem for samples.\n"; /*errorFlag=true;*/  return; }
  //unsigned long howMany=fread(samples,(dataSize/1024)+1,1024,fd);
  unsigned long howMany=fread(samples,(int)dataSize,1,fd);
}


