
//#define DEBUG_OUT std::cerr
#define DEBUG_OUT *aNullStream


#include <iostream>
#include <stdlib.h>
#include <string.h>

#include <portable/nickcpp/AStringHelper.h>
#include <portable/nickcpp/ANullStream.h>

#include "ASmartMovieDecoder.h"


// Muliple formats
#include "AMultiMovieDecoder.h"
#include "AQTMovieDecoder.h"


//static char tempStringBuf[256];


////////////////////////////////////////////////////////////////////////////////
//  ASmartMovieDecoder Class
////////////////////////////////////////////////////////////////////////////////

/* STATIC */
AMovieDecoder *ASmartMovieDecoder::pickMovieDecoder(const char *theFilename)
{
  uint8_t buffer[128];
  DEBUG_OUT<<"ASmartMovieDecoder::pickMovieDecoder...\n";
  if(!getBuffer(theFilename,buffer)) { std::cerr<<"pickMovieDecoder didn't getBuffer!\n"; return NULL; }
#ifdef USE_QUICKTIME
  if(AQTMovieDecoder::recognize(buffer))  return (new AQTMovieDecoder(theFilename));
#endif // USE_QUICKTIME
  if(AMultiMovieDecoder::recognize(buffer))  return (new AMultiMovieDecoder(theFilename));
  std::cerr<<"pickMovieDecoder couldn't recognize that file.\n";
  return NULL;
}


