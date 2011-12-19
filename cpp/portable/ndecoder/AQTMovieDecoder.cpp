
//#define DEBUG_OUT std::cerr
#define DEBUG_OUT *aNullStream


#include <iostream>
#include <stdlib.h>
#include <string.h>

#include "AQTMovieDecoder.h"



//#include <guilib/ADisplay.h>

#include <portable/nickcpp/APalette.h>
#include <portable/nickcpp/ANullStream.h>

//#include <guilib/alib_carbon.h>


#ifdef USE_QUICKTIME
#ifdef USE_UMBRELLA_HEADERS
#include <QuickTime/QuickTime.h>
#else
#include <Movies.h>
#endif // USE_UMBRELLA_HEADERS
#endif // USE_QUICKTIME


////////////////////////////////////////////////////////////////////////////////
//  AQTMovieDecoder Class
////////////////////////////////////////////////////////////////////////////////

AQTMovieDecoder::AQTMovieDecoder(const char *fname) : AMovieDecoder(fname)
{
  DEBUG_OUT<<"AQTMovieDecoder constructed.\n";
  helper->initParts();
  openFile(fname);
  helper->setFile(fname,fd);
  helper->startQuickTime();
  helper->header();
}


/* STATIC */
bool AQTMovieDecoder::recognize(uint8_t *str)
{
  return AQTDecoderHelper::recognize(str);
}


