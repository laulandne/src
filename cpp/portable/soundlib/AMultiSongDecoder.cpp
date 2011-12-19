
//#define DEBUG_OUT std::cerr
#define DEBUG_OUT *aNullStream


#include <iostream>

#include "AMultiSongDecoder.h"

#include <portable/ndecoder/ADecoderHelper.h>


////////////////////////////////////////////////////////////////////////////////
//  AMultiSongDecoder Class
////////////////////////////////////////////////////////////////////////////////

AMultiSongDecoder::AMultiSongDecoder(const char *fname) : ASongDecoder(fname)
{
  init();
  compressed=true;
  openFile(fname);
  header();
  //errorFlag = !(boundsValid());
}


/* STATIC */
bool AMultiSongDecoder::recognize(uint8_t *buffer,char *fname)
{
  return false;
}
