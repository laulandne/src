
//#define DEBUG_OUT std::cerr
#define DEBUG_OUT *aNullStream


#include <iostream>
#include <stdlib.h>
#include <string.h>

#include <portable/nickcpp/ANullStream.h>

#include "AMidiDecoder.h"

#include <portable/ndecoder/ADecoderHelper.h>


////////////////////////////////////////////////////////////////////////////////
//  AMidiDecoder Class
////////////////////////////////////////////////////////////////////////////////

/* STATIC */
bool AMidiDecoder::recognize(uint8_t *buffer)
{
  if(ADecoderHelper::recognizeMidi(buffer)) return true;
  return false;
}


AMidiDecoder::AMidiDecoder(const char *fname) : ASongDecoder(fname)
{
  init();
  openFile(fname);
  header();
  body();
}


void AMidiDecoder::header()
{
  if(!fd) return;
  //if(errorFlag) return;
  //DEBUG_OUT.warning("AMidiDecoder::header() not implemented\n");
  unsigned long chunkOffset=0;
  if(!iffFindChunk(chunkOffset,(char *)"MThd",true)) {
    DEBUG_OUT<<"Chunk not found error.\n";  //errorFlag=true;
    return;
  }
}


void AMidiDecoder::body()
{
  if(!fd) return;
  //if(errorFlag) return;
  //DEBUG_OUT.warning("AMidiDecoder::body() not implemented\n");
  unsigned long chunkOffset=14;
  // TODO: Look and read all MTrk chunks...
  unsigned long cSize=0;
  cSize=iffFindChunk(chunkOffset,(char *)"MTrk",true);
  chunkOffset+=cSize+8;
  cSize=iffFindChunk(chunkOffset,(char *)"MTrk",true);
  chunkOffset+=cSize+8;
  cSize=iffFindChunk(chunkOffset,(char *)"MTrk",true);
  if(!cSize) {
    DEBUG_OUT<<"Chunk not found error.\n";  //errorFlag=true;
    return;
  }
}

