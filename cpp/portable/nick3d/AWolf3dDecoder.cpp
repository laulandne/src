
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

#include "AWolf3dDecoder.h"


////////////////////////////////////////////////////////////////////////////////
//  AWolf3dDecoder Class
////////////////////////////////////////////////////////////////////////////////

AWolf3dDecoder::AWolf3dDecoder(const char *fname) : AMapDecoder(fname)
{
  init();
  openFile(fname);
  readObjects();
}


/* STATIC */
bool AWolf3dDecoder::recognize(uint8_t *str)
{
  bool ret=false;
  //bool isText=ADecoderHelper::recognizeTextFile(str);
  //DEBUG_OUT<<"isText is "<<isText<<"\n";
  // TODO: finish
  // If it isn't a text file, we KNOW it isn't a Wolf3d file...
  //if(isText) ret=true;  // good enough for now...we assume ASmartDecoder checked the file extension
  return ret;
}


void AWolf3dDecoder::readObjects()
{
  DEBUG_OUT<<"AWolf3dDecoder::readObjects()...\n";
  if(!fd) return;
  //if(errorFlag) return;
  fseek(fd,0L,SEEK_SET);
  std::cerr<<"*** Warning! Not really implemented! ***\n";
}

