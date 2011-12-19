
//#define DEBUG_OUT std::cerr
#define DEBUG_OUT *aNullStream


#include <iostream>
#include <stdlib.h>
#include <string.h>

#include <portable/nickcpp/ANullStream.h>

#include "AWAVDecoder.h"

#include <portable/ndecoder/ADecoderHelper.h>


////////////////////////////////////////////////////////////////////////////////
//  AWAVDecoder Class
////////////////////////////////////////////////////////////////////////////////

/* STATIC */
bool AWAVDecoder::recognize(uint8_t *buffer)
{
  if(ADecoderHelper::recognizeRIFF(buffer)) return true;
  return false;
}


AWAVDecoder::AWAVDecoder(const char *fname) : ASoundDecoder(fname)
{
  init();
  openFile(fname);
  header();
  body();
}


void AWAVDecoder::header()
{
  if(!fd) return;
  //if(errorFlag) return;
  //DEBUG_OUT.warning("AWAVDecoder::header() not implemented\n");
  //ASimpleMsgWindow("AWAVDecoder not implemented yet!\n");
  unsigned long chunkOffset=12;
  if(!iffFindChunk(chunkOffset,(char *)"fmt ",false)) {
    DEBUG_OUT<<"Chunk not found error.\n";  //errorFlag=true;
    return;
  }
  // TODO: These are all faked, since we don't know the header format yet
  stereo=false;
  compressed=false;
  numBits=8;
  sampleRate=8000;
}


void AWAVDecoder::body()
{
  if(!fd) return;
  //if(errorFlag) return;
  //DEBUG_OUT.warning("AWAVDecoder::body() not implemented\n");
  unsigned long chunkOffset=12;
  unsigned long cSize=iffFindChunk(chunkOffset,(char *)"data",false);
  if(!cSize) {
    DEBUG_OUT<<"Chunk not found error.\n";  //errorFlag=true;
    return;
  }
  samples=(uint8_t *)malloc(cSize);
  if(!samples) { DEBUG_OUT<<"Couldn't get data buffer\n"; return; }
  int e1=fread(samples,cSize,1,fd);
  numSamples=cSize;
}


