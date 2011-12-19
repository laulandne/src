
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

#include "AMayaDecoder.h"


////////////////////////////////////////////////////////////////////////////////
//  AMayaDecoder Class
////////////////////////////////////////////////////////////////////////////////

AMayaDecoder::AMayaDecoder(const char *fname) : ASceneDecoder(fname)
{
  init();
  openFile(fname);
  readObjects();
}


/* STATIC */
bool AMayaDecoder::recognize(uint8_t *str)
{
  bool ret=false;
  bool isText=recognizeTextFile(str);
  DEBUG_OUT<<"isText is "<<isText<<"\n";
  // TODO: finish
  // If it isn't a text file, we KNOW it isn't a Maya ASCII file...
  if(isText) ret=true;  // good enough for now...we assume ASmartDecoder checked the file extension
  return ret;
}


void AMayaDecoder::readObjects()
{
  DEBUG_OUT<<"AMayaDecoder::readObjects()...\n";
  if(!fd) return;
  //if(errorFlag) return;
  fseek(fd,0L,SEEK_SET);
  std::cerr<<"*** Warning! Not really implemented! ***\n";
  char *line=(char *)malloc(256);
  bool understood=false;
  bool reading=true;
  while(reading) {
    if(feof(fd)) {
      reading=false;
      break;
    }
    line[0]=0;
    fgets(line,256,fd);
    if(!feof(fd)) {
      stringHelp.parseString(line,NULL);
      understood=false;
      DEBUG_OUT<<stringHelp.getArgv(0)<<"\n";
      //
      if(!stringHelp.getArgc()) understood=true;
      else {
        if((stringHelp.getArgv(0)[0]=='/')&&(stringHelp.getArgv(0)[1]=='/')) understood=true;
        if(!strcmp("createNode",stringHelp.getArgv(0))) { understood=true; }
        if(!strcmp("setAttr",stringHelp.getArgv(0))) { understood=true; }
      }
      //
      if(!understood) DEBUG_OUT<<"*** Didn't understand that line!\n";
    }
  }
}

