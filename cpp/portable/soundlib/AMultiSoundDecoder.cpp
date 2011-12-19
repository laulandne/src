
//#define DEBUG_OUT std::cerr
#define DEBUG_OUT *aNullStream


#include <iostream>

#include "AMultiSoundDecoder.h"

#include <portable/ndecoder/ADecoderHelper.h>


////////////////////////////////////////////////////////////////////////////////
//  AMultiSoundDecoder Class
////////////////////////////////////////////////////////////////////////////////

AMultiSoundDecoder::AMultiSoundDecoder(const char *fname) : ASoundDecoder(fname)
{
  init();
  compressed=true;
  openFile(fname);
  header();
  //errorFlag = !(boundsValid());
}

/* STATIC */
bool AMultiSoundDecoder::recognize(uint8_t *buffer,char *fname)
{
  return false;
}
