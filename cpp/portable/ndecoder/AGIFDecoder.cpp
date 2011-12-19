
//#define DEBUG_OUT std::cerr
#define DEBUG_OUT *aNullStream


#include <iostream>
#include <stdlib.h>
#include <string.h>

#include <portable/nickcpp/APalette.h>
#include <portable/nickcpp/ANullStream.h>

#include "AGIFDecoder.h"


////////////////////////////////////////////////////////////////////////////////
//  AGIFDecoder Class
////////////////////////////////////////////////////////////////////////////////

AGIFDecoder::AGIFDecoder(const char *fname) : ADecoderHelper(fname)
{
  init();
  ADecoderHelper::init();
  compressed=true;
  openFile(fname);
  header();
  //errorFlag = !(boundsValid());
  /*if(!error()) if (!(*/ initLineBuffer(); //)) errorFlag = true;
  /*if(!error())*/ color();
}


/* STATIC */
bool AGIFDecoder::recognize(uint8_t *buffer,char *fname)
{
  return recognizeGIF(buffer);
}


void AGIFDecoder::header()
{
  headerGIF();
}


void AGIFDecoder::color()
{
  // Should've already been handled by ADecoderHelper::headerGIF()
}


uint8_t *AGIFDecoder::readLine(unsigned int lineNum, unsigned int numPlane)
{
  if(!lineBuffer) return (uint8_t *)NULL;
  if(lineNum>height) { DEBUG_OUT<<"line ref out of bounds!\n";  return (uint8_t *)NULL; }
  if(numPlane>depth) { DEBUG_OUT<<"plane ref out of bounds!\n";  return (uint8_t *)NULL; }
  if(!fd) return (uint8_t *)NULL;
  // Here's where the line will be read and decoded...
  return lineBuffer;
}


