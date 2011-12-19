
//#define DEBUG_OUT std::cerr
#define DEBUG_OUT *aNullStream


#include <iostream>

#include "AMultiMovieDecoder.h"
#include "ADecoderHelper.h"


////////////////////////////////////////////////////////////////////////////////
//  AMultiMovieDecoder Class
////////////////////////////////////////////////////////////////////////////////

AMultiMovieDecoder::AMultiMovieDecoder(const char *fname) : AMovieDecoder(fname)
{
  init();
  compressed=true;
  openFile(fname);
  header();
  //errorFlag = !(boundsValid());
}


/* STATIC */
bool AMultiMovieDecoder::recognize(uint8_t *buffer,char *fname)
{
  return false;
}
