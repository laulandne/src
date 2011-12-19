#ifndef ALIB_ASCENE3D_H
#define ALIB_ASCENE3D_H


#include "AVertex3D.h"
#include "AFace3D.h"
#include "AVertex3D.h"
#include "AMesh3D.h"
#include "ACamera3D.h"
#include "ALight3D.h"


#define ASCENE3D_SKY_RED (0.0F/255.0F)
#define ASCENE3D_SKY_GREEN (166.0F/255.0F)
#define ASCENE3D_SKY_BLUE (227.0F/255.0F)

#define ASCENE3D_GROUND_RED (0.0F/255.0F)
#define ASCENE3D_GROUND_GREEN (166.0F/255.0F)
#define ASCENE3D_GROUND_BLUE (0.0F/255.0F)


class AScene3D
{
public:
  AScene3D();
  virtual ~AScene3D();
  //
  bool addMesh(AMesh3D *theMesh);
  //
  AMesh3D **getMeshes() { return meshes; }
  unsigned int getNumMeshes() { return nmeshes; }
  ALight3D **getLights() { return lights; }
  unsigned int getNumLights() { return nlights; }
  ACamera3D *getCamera(unsigned int n) { if(!cameras) createDefaultCamera(); return cameras[0]; }
  ACamera3D **getCameras() { return cameras; }
  unsigned int getNumCameras() { return ncameras; }
  AMesh3D *getTheGround() { return theGround; }
  AFace3D *getTheSky() { return &theSky; }
  AVertexF3D *getBMin() { return &bmin; }
  AVertexF3D *getBMax() { return &bmax; }
  float getXWidth() { return xwidth; }
  float getYWidth() { return ywidth; }
  float getZWidth() { return zwidth; }
  char *getName() { return name; }
  //
  //  **** WARNING These functions are temporary and can break the obj state!
  void setMeshes(AMesh3D **m) { meshes=m; }
  void setNumMeshes(unsigned int n) { nmeshes=n; }
  void setLights(ALight3D **l) { lights=l; }
  void setNumLights(unsigned int n) { nlights=n; }
  void setCameras(ACamera3D **c) { cameras=c; }
  void setNumCameras(unsigned int n) { ncameras=n; }
  //
  virtual void resizeToFit(AMesh3D *thing);
  virtual void resizeToFit(ACamera3D *thing);
  virtual void resizeToFit(ALight3D *thing);
  //
  virtual void debugDump();
protected:
  void init();
  //void createGroundMesh(AFace3D *gf);
  void createDefaultCamera();
  //void scaleAllMeshesDown();
  //void scaleAllCamerasDown();
  //void scaleAllLightsDown();
  void calcWidths();
  void calcBoundingBox();
  //
  unsigned int nmeshes,ncameras,nlights;
  AMesh3D **meshes;
  AList *meshList;
  ACamera3D **cameras;
  AList *cameraList;
  ALight3D **lights;
  AList *lightList;
  //
  AVertexF3D bmin,bmax;
  float xwidth,ywidth,zwidth;
  //float theScaleFactor;
  AMesh3D *theGround;  // watch out, may be null
  AFace3D theSky;  // for the color only
  char *name;
};


#endif // ALIB_ASCENE3D_H

