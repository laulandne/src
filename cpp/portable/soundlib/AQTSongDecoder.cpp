
//#define DEBUG_OUT std::cerr
#define DEBUG_OUT *aNullStream


#include <iostream>
#include <stdlib.h>
#include <string.h>

#include "AQTSongDecoder.h"


#include <portable/nickcpp/APalette.h>
#include <portable/nickcpp/ANullStream.h>


#ifdef ASYS_MAC
#include <guilib/alib_carbon.h>
#endif // ASYS_MAC


#ifdef USE_QUICKTIME
#ifdef USE_UMBRELLA_HEADERS
#include <QuickTime/QuickTime.h>
#else
#include <Movies.h>
#endif // USE_UMBRELLA_HEADERS
#endif // USE_QUICKTIME


////////////////////////////////////////////////////////////////////////////////
//  AQTSongDecoder Class
////////////////////////////////////////////////////////////////////////////////

AQTSongDecoder::AQTSongDecoder(const char *fname) : ASongDecoder(fname)
{
  DEBUG_OUT<<"AQTSongDecoder constructed.\n";
  helper->initParts();
  openFile(fname);
  helper->setFile(fname,fd);
  helper->startQuickTime();
  helper->header();
}


/* STATIC */
bool AQTSongDecoder::recognize(uint8_t *str)
{
  return AQTDecoderHelper::recognize(str);
}


