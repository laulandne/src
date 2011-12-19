
//#define DEBUG_OUT std::cerr
#define DEBUG_OUT *aNullStream


#include <iostream>
#include <stdlib.h>
#include <string.h>

#include "ASongDecoder.h"


////////////////////////////////////////////////////////////////////////////////
//  ASongDecoder Class
////////////////////////////////////////////////////////////////////////////////

ASongDecoder::ASongDecoder(const char *fname) : ADecoder(fname)
{
  init();
}


void ASongDecoder::init()
{
  ADecoder::init();
  playingTime=0;
  dataRate=0;
  stereo=true;
  numTracks=0;
  isAnImage=false;
  isASound=false;
  isASong=true;
}


ASongDecoder::~ASongDecoder()
{
  //
}


ASong *ASongDecoder::createNewSongObject()
{
  // TODO: Obviously
  return NULL;
}
