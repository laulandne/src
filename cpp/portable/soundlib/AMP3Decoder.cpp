
//#define DEBUG_OUT std::cerr
#define DEBUG_OUT *aNullStream


#include <iostream>
#include <stdlib.h>
#include <string.h>

#include "AMP3Decoder.h"

#include <portable/ndecoder/ADecoderHelper.h>


////////////////////////////////////////////////////////////////////////////////
//  AMP3Decoder Class
////////////////////////////////////////////////////////////////////////////////

/* STATIC */
bool AMP3Decoder::recognize(uint8_t *buffer)
{
  if(ADecoderHelper::recognizeMPEG(buffer)) return true;
  return false;
}


AMP3Decoder::AMP3Decoder(const char *fname) : ASoundDecoder(fname)
{
  init();
  openFile(fname);
  header();
  body();
}


void AMP3Decoder::header()
{
  if(!fd) return;
  //if(errorFlag) return;
  //cerr.warning("AMP3Decoder::header not implemented!\n");
  //ASimpleMsgWindow("AMP3Decoder not implemented yet!\n");
}


void AMP3Decoder::body()
{
  if(!fd) return;
  //if(errorFlag) return;
  //cerr.warning("AMP3Decoder::body not implemented!\n");
}
