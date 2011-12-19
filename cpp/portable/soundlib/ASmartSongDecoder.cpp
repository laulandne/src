
//#define DEBUG_OUT std::cerr
#define DEBUG_OUT *aNullStream


#include <iostream>
#include <stdlib.h>
#include <string.h>

#include <portable/nickcpp/AStringHelper.h>
#include <portable/nickcpp/ANullStream.h>

#include "ASmartSongDecoder.h"

// Songs
#include "AMidiDecoder.h"
#include "AMultiSongDecoder.h"
#include "AQTSongDecoder.h"


//static char tempStringBuf[256];


////////////////////////////////////////////////////////////////////////////////
//  ASmartSongDecoder Class
////////////////////////////////////////////////////////////////////////////////

/* STATIC */
ASongDecoder *ASmartSongDecoder::pickSongDecoder(const char *theFilename)
{
  uint8_t buffer[128];
  DEBUG_OUT<<"ASmartSongDecoder::pickSongDecoder...\n";
  if(!getBuffer(theFilename,buffer)) { std::cerr<<"pickSongDecoder didn't getBuffer!\n"; return NULL; }
#ifdef USE_QUICKTIME
  if(AQTSongDecoder::recognize(buffer))  return (new AQTSongDecoder(theFilename));
#endif // USE_QUICKTIME
  if(AMidiDecoder::recognize(buffer))  return (new AMidiDecoder(theFilename));
  if(AMultiSongDecoder::recognize(buffer))  return (new AMultiSongDecoder(theFilename));
  std::cerr<<"pickSongDecoder couldn't recognize that file.\n";
  return NULL;
}


