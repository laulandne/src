
//#define DEBUG_OUT std::cerr
#define DEBUG_OUT *aNullStream


#include <iostream>
#include <stdlib.h>
#include <string.h>

#include <portable/nickcpp/AStringHelper.h>
#include <portable/nickcpp/ANullStream.h>

#include "ASmartSoundDecoder.h"

// Sounds
#include "A8SVXDecoder.h"
#include "AAUDecoder.h"
#include "AMP3Decoder.h"
#include "AWAVDecoder.h"
#include "AQTSoundDecoder.h"
#include "AMultiSoundDecoder.h"


//static char tempStringBuf[256];


////////////////////////////////////////////////////////////////////////////////
//  ASmartSoundDecoder Class
////////////////////////////////////////////////////////////////////////////////

/* STATIC */
ASoundDecoder *ASmartSoundDecoder::pickSoundDecoder(const char *theFilename)
{
  uint8_t buffer[128];
  DEBUG_OUT<<"ASmartSoundDecoder::pickSoundDecoder...\n";
  if(!getBuffer(theFilename,buffer)) { std::cerr<<"pickSoundDecoder didn't getBuffer!\n"; return NULL; }
#ifdef USE_QUICKTIME
  if(AQTSoundDecoder::recognize(buffer))  return (new AQTSoundDecoder(theFilename));
#endif // USE_QUICKTIME
  if(AAIFFDecoder::recognize(buffer))  return (new AAIFFDecoder(theFilename));
  if(A8SVXDecoder::recognize(buffer))  return (new A8SVXDecoder(theFilename));
  if(AWAVDecoder::recognize(buffer))  return (new AWAVDecoder(theFilename));
  if(AAUDecoder::recognize(buffer))  return (new AAUDecoder(theFilename));
  if(AMultiSoundDecoder::recognize(buffer))  return (new AMultiSoundDecoder(theFilename));
  std::cerr<<"pickSoundDecoder couldn't recognize that file.\n";
  return NULL;
}


