
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#include <iostream>
#include <stdlib.h>
#include <string.h>

#include <portable/nickcpp/ANullStream.h>

#include "ASceneDecoder.h"


////////////////////////////////////////////////////////////////////////////////
//  ASceneDecoder Class
////////////////////////////////////////////////////////////////////////////////

ASceneDecoder::ASceneDecoder(const char *fname) : A3DDecoder(fname)
{
  DEBUG_OUT<<"ASceneDecoder::ASceneDecoder("<<fname<<")\n";
  init();
}


ASceneDecoder::~ASceneDecoder()
{
}


void ASceneDecoder::init()
{
  DEBUG_OUT<<"ASceneDecoder::init()\n";
  isAScene=true;
  nmeshes=0;  ncameras=0;  nlights=0;
  // default sky is light blue
  theSky.surf=1; /// ???
  theSky.fRed=ASCENE3D_SKY_RED;
  theSky.fGreen=ASCENE3D_SKY_GREEN;
  theSky.fBlue=ASCENE3D_SKY_BLUE;
  // default ground is green
  theGround.surf=2; // ???
  theGround.fRed=ASCENE3D_GROUND_RED;
  theGround.fGreen=ASCENE3D_GROUND_GREEN;
  theGround.fBlue=ASCENE3D_GROUND_BLUE;
  //name=NULL;
}


AScene3D *ASceneDecoder::createNewSceneObject()
{
  DEBUG_OUT<<"ASceneDecoder::createNewSceneObject() starting...\n";
  AScene3D *s=new AScene3D();
  DEBUG_OUT<<"Meshes...\n";
  s->setMeshes((AMesh3D **)allocArrayBigEnoughForList(getMeshes()));
  s->setNumMeshes(buildArrayFromList((void **)s->getMeshes(),getMeshes()));
  DEBUG_OUT<<"Lights...\n";
  s->setLights((ALight3D **)allocArrayBigEnoughForList(getLights()));
  s->setNumLights(buildArrayFromList((void **)s->getLights(),getLights()));
  DEBUG_OUT<<"Cameras...\n";
  s->setCameras((ACamera3D **)allocArrayBigEnoughForList(getCameras()));
  s->setNumCameras(buildArrayFromList((void **)s->getCameras(),getCameras()));
  //
  //DEBUG_OUT<<"Sky...\n";
  //theSky.surf=readFile->getTheSky()->surf;
  //theSky.fRed=readFile->getTheSky()->fRed;
  //theSky.fBlue=readFile->getTheSky()->fBlue;
  //theSky.fGreen=readFile->getTheSky()->fGreen;
  //DEBUG_OUT<<"Ground...\n";
  //createGroundMesh(readFile->getTheGround());
  DEBUG_OUT<<"Done.\n";
  return s;
}


