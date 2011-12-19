
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

#include "ABspDecoder.h"


////////////////////////////////////////////////////////////////////////////////
//  ABspDecoder Class
////////////////////////////////////////////////////////////////////////////////

ABspDecoder::ABspDecoder(const char *fname) : AMapDecoder(fname)
{
  init();
  openFile(fname);
  readObjects();
}


void ABspDecoder::init()
{
  the_format='?';
}


/* STATIC */
bool ABspDecoder::recognize(uint8_t *str)
{
  DEBUG_OUT<<"ABspDecoder::recognize()...\n";
  bool ret=false;
  if(!strncmp((char *)str,"IBSP",4)) ret=true;
  if(!strncmp((char *)str,"VBSP",4)) ret=true;
  return ret;
}


void ABspDecoder::readObjects()
{
  DEBUG_OUT<<"ABspDecoder::readObjects()...\n";
  if(!fd) return;
  //if(errorFlag) return;
  the_format=readByte();
  readByte(); readByte(); readByte();  // skip
  unsigned int s=0;
  bool reading=true;
  while(reading) {
    s=readLongL();
    DEBUG_OUT<<"s="<<s<<"\n";
    if(s>64000) reading=false;
    else {
      s-=4;
      for(unsigned int t=0;t<s;t++) readByte();
    }
  }
}

