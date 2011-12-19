
//#define DEBUG_OUT std::cerr
#define DEBUG_OUT *aNullStream


#include <iostream>
#include <stdlib.h>
#include <string.h>

#include "ASoundDecoder.h"


////////////////////////////////////////////////////////////////////////////////
//  ASoundDecoder Class
////////////////////////////////////////////////////////////////////////////////

ASoundDecoder::ASoundDecoder(const char *fname) : ADecoder(fname)
{
  init();
}


void ASoundDecoder::init()
{
  ADecoder::init();
  numSamples=0;
  samples=(uint8_t *)NULL;
  isAnImage=false;
  isASound=true;
  isASong=false;
  stereo=false;
  compressed=false;
  numBits=0;
  sampleRate=0;
}


ASoundDecoder::~ASoundDecoder()
{
  if(samples) free(samples);
  samples=(uint8_t *)NULL;
}


ASound *ASoundDecoder::createNewSoundObject()
{
  // TODO: Obviously
  return NULL;
}
