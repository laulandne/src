
//#define DEBUG_OUT std::cerr
#define DEBUG_OUT *aNullStream


#include <iostream>
#include <stdlib.h>
#include <string.h>

#include <portable/nickcpp/ANullStream.h>

#include "A8SVXDecoder.h"

#include <portable/ndecoder/ADecoderHelper.h>


////////////////////////////////////////////////////////////////////////////////
//  A8SVXDecoder Class
////////////////////////////////////////////////////////////////////////////////

// This is the format of a VHDR chunk in an IFF 8SVX FORM file
struct aVHDRHeader  {
  unsigned long oneShots;
  unsigned long repeats;
  unsigned long samplesPerCycle;
  unsigned short samplesPerSec;
  unsigned char octaves;
  unsigned char compressed;
  unsigned long volume;
};


/* STATIC */
bool A8SVXDecoder::recognize(uint8_t *str)
{
  if(!strncmp((char *)str,"FORM",4)) {
    char type[5];
    memcpy(type,&str[8],4);
    type[4]=0;
    DEBUG_OUT<<"IFF file type: "<<type<<"\n";
    if(!strncmp((char *)(str+8),"8SVX",4)) {
      // good enough
      return true;
    }
  }
  return false;
}


A8SVXDecoder::A8SVXDecoder(const char *fname) : ASoundDecoder(fname)
{
  init();
  openFile(fname);
  header();
  body();
}


void A8SVXDecoder::header()
{
  if(!fd) return;
  //if(errorFlag) return;
  //DEBUG_OUT.warning("A8SVXDecoder::header not implemented yet...\n");
  unsigned long chunkOffset=12;
  iffFindChunk(chunkOffset,(char *)"????",true);
}


void A8SVXDecoder::body()
{
  if(!fd) return;
  //if(errorFlag) return;
  //DEBUG_OUT.warning("A8SVXDecoder::body not implemented yet...\n");
  unsigned long chunkOffset=12;
  iffFindChunk(chunkOffset,(char *)"????",true);
}


////////////////////////////////////////////////////////////////////////////////
//  AAIFFDecoder Class
////////////////////////////////////////////////////////////////////////////////


/* STATIC */
bool AAIFFDecoder::recognize(uint8_t *buffer)
{
  if(ADecoderHelper::recognizeAIFF(buffer)) return true;
  return false;
}


AAIFFDecoder::AAIFFDecoder(const char *fname) : ASoundDecoder(fname)
{
  init();
  openFile(filename);
  header();
  body();
}


void AAIFFDecoder::header()
{
  if(!fd) return;
  //if(errorFlag) return;
  //DEBUG_OUT.warning("AAIFFDecoder::header not implemented yet...\n");
  unsigned long chunkOffset=12;
  iffFindChunk(chunkOffset,(char *)"????",true);
}


void AAIFFDecoder::body()
{
  if(!fd) return;
  //if(errorFlag) return;
  //DEBUG_OUT.warning("AAIFFDecoder::body not implemented yet...\n");
  unsigned long chunkOffset=12;
  iffFindChunk(chunkOffset,(char *)"????",true);
}

