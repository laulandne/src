#ifndef ALIB_ASCENEDECODER_H
#define ALIB_ASCENEDECODER_H


#include <portable/nickcpp/AList.h>

#include <portable/nick3d/AFace3D.h>
#include <portable/nick3d/AScene3D.h>

#include "A3DDecoder.h"


class ASceneDecoder : public A3DDecoder
{
public:
  ASceneDecoder(const char *fname);
  virtual ~ASceneDecoder();
  //
  AScene3D *createNewSceneObject();
  //
  AList *getMeshes() { return &meshes; }
  unsigned int getNumMeshes() { return nmeshes; }
  AList *getCameras() { return &cameras; }
  unsigned int getNumCameras() { return ncameras; }
  AList *getLights() { return &cameras; }
  unsigned int getNumLights() { return nlights; }
  AFace3D *getTheGround() { return &theGround; }
  AFace3D *getTheSky() { return &theSky; }
  //char *getName() { return name; }
protected:
  //
  virtual void init();
  //
  AList lights;
  AList cameras;
  AList meshes;
  unsigned int nmeshes,ncameras,nlights;
  AFace3D theGround;  // for the color only
  AFace3D theSky;  // for the color only
  //char *name;
  //
};


#endif // ALIB_ASCENEDECODER_H

