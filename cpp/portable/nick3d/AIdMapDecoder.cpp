
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

#include "AIdMapDecoder.h"


////////////////////////////////////////////////////////////////////////////////
//  AIdMapDecoder Class
////////////////////////////////////////////////////////////////////////////////

AIdMapDecoder::AIdMapDecoder(const char *fname) : AMapDecoder(fname)
{
  init();
  openFile(fname);
  readObjects();
}


void AIdMapDecoder::init()
{
  isVMF=false;
}


/* STATIC */
bool AIdMapDecoder::recognize(uint8_t *str)
{
  bool ret=false;
  bool isText=recognizeTextFile(str);
  DEBUG_OUT<<"isText is "<<isText<<"\n";
  // TODO: finish
  // If it isn't a text file, we KNOW it isn't a map file...
  if(isText) ret=true;  // good enough for now...we assume ASmartDecoder checked the file extension
  return ret;
}


void AIdMapDecoder::readObjects()
{
  DEBUG_OUT<<"AIdMapDecoder::readObjects()...\n";
  if(!fd) return;
  //if(errorFlag) return;
  fseek(fd,0L,SEEK_SET);
  uint8_t *line=(uint8_t *)malloc(256);
  unsigned int bracketCount=0;
  bool understood=false;
  bool reading=true;
  while(reading) {
    if(feof(fd)) {
      reading=false;
      break;
    }
    line[0]=0;
    fgets((char *)line,256,fd);
    if(!feof(fd)) {
      stringHelp.parseString((char *)line,NULL);
      understood=false;
      //DEBUG_OUT<<stringHelp.getArgv(0)<<"\n";
      //
      if(!stringHelp.getArgc()) understood=true;
      else {
        if(!strcmp("Version",stringHelp.getArgv(0))) {
          DEBUG_OUT<<"Version: "<<stringHelp.getArgv(1)<<"\n";
          understood=true;  // a version number
        }
        if(!strcmp("classname",stringHelp.getArgv(0))) {
        //if(!strcmp("\"classname\"",stringHelp.getArgv(0))) {
          DEBUG_OUT<<"classname: "<<stringHelp.getArgv(1)<<"\n";
          if(!strcmp("worldspawn",stringHelp.getArgv(0))) {
            DEBUG_OUT<<"Found wordspawn...\n";
          }
          understood=true;
        }
        if(!strcmp("//",stringHelp.getArgv(0))) understood=true;  // a comment
        if(!strcmp("{",stringHelp.getArgv(0))) { bracketCount++; understood=true; }
        if(!strcmp("}",stringHelp.getArgv(0))) {
          bracketCount--; understood=true;
        }
        if(!strcmp("brushDef3",stringHelp.getArgv(0))) {
          understood=true;
          readBrushdef(line);
        }
       }
      //
      if(!understood) DEBUG_OUT<<"*** Command \""<<stringHelp.getArgv(0)<<"\" not understood!\n";
    }
  }
}


void AIdMapDecoder::readBrushdef(unsigned char *line)
{
  DEBUG_OUT<<"AIdMapDecoder::readBrushdef()...\n";
  bool reading=true;
  bool understood=false;
  unsigned int bracketCount=0;
  while(reading) {
    fgets((char *)line,256,fd);
    stringHelp.parseString((char *)line,NULL);
    if(!strcmp("{",stringHelp.getArgv(0))) { understood=true; bracketCount++; }
    if(!strcmp("(",stringHelp.getArgv(0))) {
      understood=true;
      for(unsigned int t=0;t<stringHelp.getArgc();t++) DEBUG_OUT<<"\""<<stringHelp.getArgv(t)<<"\" ";
      DEBUG_OUT<<"\n";
    }
    if(!strcmp("}",stringHelp.getArgv(0))) {
      understood=true;
      bracketCount--;
      reading=false;
    }
    if(!understood) DEBUG_OUT<<"*** Command \""<<stringHelp.getArgv(0)<<"\" not understood!\n";
  }
  DEBUG_OUT<<"AIdMapDecoder::readBrushdef() done.\n";
}
