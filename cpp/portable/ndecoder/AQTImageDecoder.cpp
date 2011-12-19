
//#define DEBUG_OUT std::cerr
#define DEBUG_OUT *aNullStream


#include <iostream>
#include <stdlib.h>
#include <string.h>

#include "AQTImageDecoder.h"



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
//  AQTImageDecoder Class
////////////////////////////////////////////////////////////////////////////////

AQTImageDecoder::AQTImageDecoder(const char *fname) : AImageDecoder(fname)
{
  DEBUG_OUT<<"AQTImageDecoder constructed.\n";
  helper->initParts();
  openFile(fname);
  helper->setFile(fname,fd);
  helper->startQuickTime();
  helper->header();
  width=helper->getWidth();
  height=helper->getHeight();
  depth=helper->getDepth();
  nPlanes=helper->getNPlanes();
  calc();
  //initLineBuffer();
  DEBUG_OUT<<"(AQTImageDecoder ready for readLine()'s...)\n";
}


/* STATIC */
bool AQTImageDecoder::recognize(uint8_t *str)
{
  return AQTDecoderHelper::recognize(str);
}


uint8_t *AQTImageDecoder::readLine(unsigned int lineNum, unsigned int numPlane)
{
  return helper->readLine(lineNum,numPlane);
}

