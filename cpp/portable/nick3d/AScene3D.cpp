
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#include <stdlib.h>
#include <iostream>

#include <portable/nickcpp/AList.h>
#include <portable/nickcpp/ANullStream.h>

#include "AScene3D.h"


////////////////////////////////////////////////////////////////////////////////
//  AScene3D Class
////////////////////////////////////////////////////////////////////////////////

AScene3D::AScene3D()
{
  DEBUG_OUT<<"AScene3D created!\n";
  init();
}


void AScene3D::init()
{
  bmin.x=0.0;  bmax.x=0.0;  bmin.y=0.0;  bmax.y=0.0;  bmin.z=0.0;  bmax.z=0.0;
  nmeshes=0;  meshes=NULL;  meshList=NULL;
  ncameras=0;  cameras=NULL;  cameraList=NULL;
  nlights=0;  lights=NULL;  lightList=NULL;
  //theScaleFactor=1.0;
  theGround=NULL;
  theSky.surf=1;  // ???
  theSky.fRed=ASCENE3D_SKY_RED;
  theSky.fGreen=ASCENE3D_SKY_GREEN;
  theSky.fBlue=ASCENE3D_SKY_BLUE;
  name=NULL;
}


AScene3D::~AScene3D()
{
  DEBUG_OUT<<"AScene3D Destroyed.\n";
}


void AScene3D::createDefaultCamera()
{
  cameras=(ACamera3D **)calloc(1,sizeof(ACamera3D *));
  if(!cameras) return;
  calcWidths();
  ncameras=1;
  cameras[0]=new ACamera3D;
  //ameras[0]->translateTo(0,ywidth/2,-zwidth);
  //cameras[0]->scale(1.0/xwidth,1.0/ywidth,1.0/zwidth);
  DEBUG_OUT<<"Created default camera.\n";
}


// This is a totally ugly hack
bool AScene3D::addMesh(AMesh3D *theMesh)
{
  bool ret=true;
  int numMeshes=1;
  if(!numMeshes) return false;
  if(!theMesh) return false;
  meshes=(AMesh3D **)calloc(numMeshes,sizeof(AMesh3D *));
  if(!meshes) return false;
  resizeToFit(theMesh);
  meshes[0]=theMesh;
  nmeshes=1;
  DEBUG_OUT<<"Created "<<nmeshes<<" meshes.\n";
  calcWidths();
  return ret;
}


// NOTE: These only grow to fit, they don't shrink
void AScene3D::resizeToFit(AMesh3D *thing)
{
  DEBUG_OUT<<"AScene3D::resizeToFit(AMesh3D *thing)...\n";
  thing->calcBoundingBox();
  AVertexF3D *tbmin=thing->getBMin();
  AVertexF3D *tbmax=thing->getBMax();
  if(bmin.x>tbmin->x) bmin.x=tbmin->x;
  if(bmin.y>tbmin->y) bmin.y=tbmin->y;
  if(bmin.z>tbmin->z) bmin.z=tbmin->z;
  if(bmax.x<tbmax->x) bmax.x=tbmax->x;
  if(bmax.y<tbmax->y) bmax.y=tbmax->y;
  if(bmax.z<tbmax->z) bmax.z=tbmax->z;
  calcWidths();
  debugDump();
}


// NOTE: These only grow to fit, they don't shrink
void AScene3D::resizeToFit(ACamera3D *thing)
{
  DEBUG_OUT<<"AScene3D::resizeToFit(ACamera3D *thing)...\n";
  AVertexF3D *tbmin=thing->getBMin();
  AVertexF3D *tbmax=thing->getBMax();
  if(bmin.x>tbmin->x) bmin.x=tbmin->x;
  if(bmin.y>tbmin->y) bmin.y=tbmin->y;
  if(bmin.z>tbmin->z) bmin.z=tbmin->z;
  if(bmax.x<tbmax->x) bmax.x=tbmax->x;
  if(bmax.y<tbmax->y) bmax.y=tbmax->y;
  if(bmax.z<tbmax->z) bmax.z=tbmax->z;
  calcWidths();
  debugDump();
}


// NOTE: These only grow to fit, they don't shrink
void AScene3D::resizeToFit(ALight3D *thing)
{
  DEBUG_OUT<<"AScene3D::resizeToFit(ALight3D *thing)...\n";
  AVertexF3D *tbmin=thing->getBMin();
  AVertexF3D *tbmax=thing->getBMax();
  if(bmin.x>tbmin->x) bmin.x=tbmin->x;
  if(bmin.y>tbmin->y) bmin.y=tbmin->y;
  if(bmin.z>tbmin->z) bmin.z=tbmin->z;
  if(bmax.x<tbmax->x) bmax.x=tbmax->x;
  if(bmax.y<tbmax->y) bmax.y=tbmax->y;
  if(bmax.z<tbmax->z) bmax.z=tbmax->z;
  calcWidths();
  debugDump();
}


void AScene3D::debugDump()
{
  DEBUG_OUT<<"AScene3D: bbox x=("<<bmin.x<<","<<bmax.x<<") y=("<<bmin.y<<","<<bmax.y<<") z=("<<bmin.z<<","<<bmax.z<<")\n";
  DEBUG_OUT<<"There are "<<getNumLights()<<" lights...\n";
  unsigned int t=0;
  for(t=0;t<getNumLights();t++) lights[t]->debugDump();
  DEBUG_OUT<<"There are "<<getNumCameras()<<" cameras...\n";
  for(t=0;t<getNumCameras();t++) cameras[t]->debugDump();
  DEBUG_OUT<<"There are "<<getNumMeshes()<<" meshes...\n";
  for(t=0;t<getNumMeshes();t++) meshes[t]->debugDump();
}


/*
void AScene3D::scaleAllMeshesDown()
{
  DEBUG_OUT<<"AScene3D::scaleAllMeshesDown()...\n";
  for(unsigned int i=0;i<nmeshes;i++) {
    if(meshes[i]) meshes[i]->scaleAllVerts(theScaleFactor,theScaleFactor,theScaleFactor);
  }
  bmax.x=bmax.x*theScaleFactor;
  bmax.y=bmax.y*theScaleFactor;
  bmax.z=bmax.z*theScaleFactor;
  bmin.x=bmin.x*theScaleFactor;
  bmin.y=bmin.y*theScaleFactor;
  bmin.z=bmin.z*theScaleFactor;
  calcWidths();
}
*/


/*
void AScene3D::scaleAllCamerasDown()
{
  DEBUG_OUT<<"AScene3D::scaleAllCamerasDown()...\n";
  for(unsigned int i=0;i<ncameras;i++) {
    //cameras[i]
  }
}
*/


/*
void AScene3D::scaleAllLightsDown()
{
  DEBUG_OUT<<"AScene3D::scaleAllLightsDown()...\n";
  for(unsigned int i=0;i<nlights;i++) {
    //lights[i]
  }
}
*/


void AScene3D::calcWidths()
{
  calcBoundingBox();
  // Not really right, neg values are screwed, or are they?
  xwidth=bmax.x-bmin.x;
  ywidth=bmax.y-bmin.y;
  zwidth=bmax.z-bmin.z;
  DEBUG_OUT<<"calcWidths: "<<xwidth<<","<<ywidth<<","<<zwidth<<"\n";
}


void AScene3D::calcBoundingBox()
{
  // need to implement...look at all objects, etc.
}


/*
void AScene3D::createGroundMesh(AFace3D *gf)
{
  if(!gf) return;
  AVertex3D a,b,c,d;
  calcWidths();
  a.x=-xwidth; a.y=0.0; a.z=-zwidth;
  b.x=-xwidth; b.y=0.0; b.z=zwidth;
  c.x=xwidth; c.y=0.0; c.z=zwidth;
  d.x=xwidth; d.y=0.0; d.z=-zwidth;
  theGround=new AMesh3D(gf,a,b,c,d);
}
*/

