
//#define DEBUG_OUT std::cerr
#define DEBUG_OUT *aNullStream


#include <iostream>
#include <stdlib.h>
#include <string.h>

#include <portable/nickcpp/AStringHelper.h>
#include <portable/nickcpp/ANullStream.h>

#include "ASmartImageDecoder.h"


// Images
#include "APCXDecoder.h"
#include "ABMPDecoder.h"
#include "AGIFDecoder.h"
#include "AJFIFDecoder.h"
#include "ATGADecoder.h"

// Multi or special
#include "AMultiImageDecoder.h"
#include "AQTImageDecoder.h"
#include "AIFFDecoder.h"


//static char tempStringBuf[256];


////////////////////////////////////////////////////////////////////////////////
//  ASmartImageDecoder Class
////////////////////////////////////////////////////////////////////////////////

/* STATIC */
AImageDecoder *ASmartImageDecoder::pickImageDecoder(const char *theFilename)
{
  uint8_t buffer[128];
  DEBUG_OUT<<"ASmartImageDecoder::pickImageDecoder...\n";
  if(!getBuffer(theFilename,buffer)) { std::cerr<<"pickImageDecoder didn't getBuffer!\n"; return NULL; }
#ifdef USE_QUICKTIME
  if(AQTImageDecoder::recognize(buffer))  return (new AQTImageDecoder(theFilename));
#endif // USE_QUICKTIME
  if(APCXDecoder::recognize(buffer))  return (new APCXDecoder(theFilename));
  if(AIFFDecoder::recognize(buffer))  return (new AIFFDecoder(theFilename));
  if(AJFIFDecoder::recognize(buffer)) return (new AJFIFDecoder(theFilename));
  if(AGIFDecoder::recognize(buffer))  return (new AGIFDecoder(theFilename));
  if(ATGADecoder::recognize(buffer))  return (new ATGADecoder(theFilename));
  if(ABMPDecoder::recognize(buffer))  return (new ABMPDecoder(theFilename));
  if(AMultiImageDecoder::recognize(buffer))  return (new AMultiImageDecoder(theFilename));
  std::cerr<<"pickImageDecoder couldn't recognize that file.\n";
  return NULL;
}


