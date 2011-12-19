
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

#include "AVRMLDecoder.h"


////////////////////////////////////////////////////////////////////////////////
//  AVRMLDecoder Class
////////////////////////////////////////////////////////////////////////////////

AVRMLDecoder::AVRMLDecoder(const char *fname) : ASceneDecoder(fname)
{
  init();
  openFile(fname);
  readObjects();
}


/* STATIC */
bool AVRMLDecoder::recognize(uint8_t *str)
{
  bool ret=false;
  bool isText=recognizeTextFile(str);
  DEBUG_OUT<<"isText is "<<isText<<"\n";
  // TODO: finish
  // If it isn't a text file, we KNOW it isn't a VRML file...
  if(isText) ret=true;  // good enough for now...we assume ASmartDecoder checked the file extension
  return ret;
}


void AVRMLDecoder::readObjects()
{
  DEBUG_OUT<<"AVRMLDecoder::readObjects()...\n";
  if(!fd) return;
  //if(errorFlag) return;
  fseek(fd,0L,SEEK_SET);
  std::cerr<<"*** Warning! Not really implemented! ***\n";
  char *line=(char *)malloc(256);
  bool understood=false;
  unsigned int bracketCount=0;
  unsigned int inBracket=0;
  bool inPoint=false;
  bool inCoordIndex=false;
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
        if(stringHelp.getArgv(0)[0]=='#') understood=true;
        if(!strcmp("Separator",stringHelp.getArgv(0))) {
          bracketCount++;
          understood=true;
        }
        if(!strcmp("ShapeHints",stringHelp.getArgv(0))) {
          bracketCount++;
          understood=true;
        }
        if(!strcmp("vertexOrdering",stringHelp.getArgv(0))) { understood=true; }
        if(!strcmp("}",stringHelp.getArgv(0))) {
          bracketCount--;
          DEBUG_OUT<<"bracketCount is now "<<bracketCount<<"\n";
          understood=true;
          if((bracketCount+1)==inBracket) {
            DEBUG_OUT<<"(Out of point or coordIndex)\n";
            inBracket=0;
          }
        }
        if(!strcmp("DEF",stringHelp.getArgv(0))) {
          bracketCount++;
          understood=true;
        }
        if(!strcmp("Coordinate3",stringHelp.getArgv(0))) {
          bracketCount++;
          understood=true;
        }
        if(!strcmp("point",stringHelp.getArgv(0))) {
          understood=true;
          inPoint=true;
          inBracket=bracketCount;
          DEBUG_OUT<<"(inPoint)\n";
          DEBUG_OUT<<"bracketCount is now "<<bracketCount<<"\n";
        }
        if(!strcmp("ambientColor",stringHelp.getArgv(0))) { understood=true; }
        if(!strcmp("diffuseColor",stringHelp.getArgv(0))) { understood=true; }
        if(!strcmp("specularColor",stringHelp.getArgv(0))) { understood=true; }
        if(!strcmp("transparency",stringHelp.getArgv(0))) { understood=true; }
        if(!strcmp("IndexedFaceSet",stringHelp.getArgv(0))) {
          bracketCount++;
          understood=true;
        }
        if(!strcmp("coordIndex",stringHelp.getArgv(0))) {
          understood=true;
          inCoordIndex=true;
          inBracket=bracketCount;
          DEBUG_OUT<<"(inCoordIndex)\n";
          DEBUG_OUT<<"bracketCount is now "<<bracketCount<<"\n";
        }
     }
      //
      if(!understood) DEBUG_OUT<<"*** Didn't understand that line!\n";
    }
  }
}

