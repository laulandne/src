
//#define DEBUG_OUT std::cerr
#define DEBUG_OUT *aNullStream


#include <iostream>
#include <stdlib.h>
#include <string.h>

#include <portable/nickcpp/ANullStream.h>

#include "AMovieDecoder.h"


////////////////////////////////////////////////////////////////////////////////
//  AMovieDecoder Class
////////////////////////////////////////////////////////////////////////////////

AMovieDecoder::AMovieDecoder(const char *fname) : ADecoder(fname)
{
  DEBUG_OUT<<"AMovieDecoder constructed.\n";
  init();
}


AMovieDecoder::~AMovieDecoder()
{
}


void AMovieDecoder::init()
{
  playingTime=0;
  numTracks=0;
  dataRate=0;
  stereo=false;
  compressed=false;
  isAMovie=true;
  isAnImage=false;
  isASound=false;
  isASong=false;
}


AMovie *AMovieDecoder::createNewMovieObject()
{
  // TODO: Obviously
  return NULL;
}
