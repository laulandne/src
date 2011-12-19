
//#define DEBUG_OUT std::cerr
#define DEBUG_OUT *aNullStream


#include <iostream>
#include <stdlib.h>
#include <string.h>

#include <portable/nickcpp/APalette.h>
#include <portable/nickcpp/ANullStream.h>

#include "AJFIFDecoder.h"


////////////////////////////////////////////////////////////////////////////////
//  AJFIFDecoder Class
////////////////////////////////////////////////////////////////////////////////

AJFIFDecoder::AJFIFDecoder(const char *fname) : ADecoderHelper(fname)
{
  init();
  grayscale=false;  compressed=true;
  openFile(fname);
  header();
  //errorFlag = !(boundsValid());
  /*if(!error()) if (!(*/ initLineBuffer(); //)) errorFlag = true;
  /*if(!error())*/ color();
}


/* STATIC */
bool AJFIFDecoder::recognize(uint8_t *buffer,char *fname)
{
  return recognizeJPEG(buffer);
}


void AJFIFDecoder::header()
{
  headerJPEG();
}


void AJFIFDecoder::color()
{
  // A JPEG JFIF file never has a palette as they are always truecolor
  colors=(APalette *)NULL;
}


uint8_t *AJFIFDecoder::readLine(unsigned int lineNum, unsigned int numPlane)
{
  if(!lineBuffer) return (uint8_t *)NULL;
  if(lineNum>height) { DEBUG_OUT<<"line ref out of bounds!\n";  return (uint8_t *)NULL; }
  if(numPlane>depth) { DEBUG_OUT<<"plane ref out of bounds!\n";  return (uint8_t *)NULL; }
  if(!fd) return (uint8_t *)NULL;
  // Here's where the line will be read and decoded...
  return lineBuffer;
}



