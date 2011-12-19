
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream



#include <iostream>
#include <stdlib.h>
#include <string.h>

#include <portable/nickcpp/AList.h>
#include <portable/nickcpp/ANullStream.h>

#include <portable/nick3d/AMatrix3D.h>
#include <portable/nick3d/AVertex3D.h>
#include <portable/nick3d/AFace3D.h>
#include <portable/nick3d/ATextCoord3D.h>
#include <portable/nick3d/AMesh3D.h>
#include <portable/nick3d/ATexture3D.h>

#include "ALightwaveDecoder.h"


////////////////////////////////////////////////////////////////////////////////
//  ALightwaveDecoder Class
////////////////////////////////////////////////////////////////////////////////

ALightwaveDecoder::ALightwaveDecoder(const char *fname) : ASceneDecoder(fname)
{
  init();
  openFile(fname);
  readObjects();
}


/* STATIC */
bool ALightwaveDecoder::recognize(uint8_t *str)
{
  if(!strncmp((char *)str,"FORM",4)) {
    char type[5];
    memcpy(type,&str[8],4);
    type[4]=0;
    //DEBUG_OUT<<"IFF file type: "<<type<<"\n";
    if(!strncmp((char *)(str+8),"LWO2",4)) {
      // good enough
      return true;
    }
  }
  return false;
}


// NOTE: Pretty much all chunks may be repeated!
void ALightwaveDecoder::readObjects()
{
  DEBUG_OUT<<"ALightwaveDecoder::readObjects()...\n";
  if(!fd) return;
  //if(errorFlag) return;
  fseek(fd,0L,SEEK_SET);
  unsigned long chunkOffset=12;
  if(iffFindChunk(chunkOffset,(char *)"PNTS",true)) readPnts();
  if(iffFindChunk(chunkOffset,(char *)"POLS",true)) readPols();
  if(iffFindChunk(chunkOffset,(char *)"PTAG",true)) readPtag();
  if(iffFindChunk(chunkOffset,(char *)"SURF",true)) readSurf();
  if(iffFindChunk(chunkOffset,(char *)"TAGS",true)) readTags();
  if(iffFindChunk(chunkOffset,(char *)"LAYR",true)) readLayr();
  if(iffFindChunk(chunkOffset,(char *)"BBOX",true)) readBbox();
  if(iffFindChunk(chunkOffset,(char *)"CLIP",true)) readClip();
}


void ALightwaveDecoder::readSurf()
{
  DEBUG_OUT<<"ALightwaveDecoder::readSurf()...\n";
  if(!fd) return;
  //if(errorFlag) return;
  DEBUG_OUT<<"*** Not implemented! ***\n";
}


void ALightwaveDecoder::readPnts()
{
  DEBUG_OUT<<"ALightwaveDecoder::readPnts()...\n";
  if(!fd) return;
  //if(errorFlag) return;
  DEBUG_OUT<<"*** Not implemented! ***\n";
}


void ALightwaveDecoder::readPols()
{
  DEBUG_OUT<<"ALightwaveDecoder::readPols()...\n";
  if(!fd) return;
  //if(errorFlag) return;
  DEBUG_OUT<<"*** Not implemented! ***\n";
}


void ALightwaveDecoder::readPtag()
{
  DEBUG_OUT<<"ALightwaveDecoder::readPtag()...\n";
  if(!fd) return;
  //if(errorFlag) return;
  DEBUG_OUT<<"*** Not implemented! ***\n";
}


void ALightwaveDecoder::readTags()
{
  DEBUG_OUT<<"ALightwaveDecoder::readTags()...\n";
  if(!fd) return;
  ////if(errorFlag) return;
  DEBUG_OUT<<"*** Not implemented! ***\n";
}


void ALightwaveDecoder::readLayr()
{
  DEBUG_OUT<<"ALightwaveDecoder::readLayr()...\n";
  if(!fd) return;
  //if(errorFlag) return;
  DEBUG_OUT<<"*** Not implemented! ***\n";
}


void ALightwaveDecoder::readBbox()
{
  DEBUG_OUT<<"ALightwaveDecoder::readBbox()...\n";
  if(!fd) return;
  //if(errorFlag) return;
  DEBUG_OUT<<"*** Not implemented! ***\n";
}


void ALightwaveDecoder::readClip()
{
  DEBUG_OUT<<"ALightwaveDecoder::readClip()...\n";
  if(!fd) return;
  //if(errorFlag) return;
  DEBUG_OUT<<"*** Not implemented! ***\n";
}
