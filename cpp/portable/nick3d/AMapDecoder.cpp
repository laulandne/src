
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

#include "AMapDecoder.h"


////////////////////////////////////////////////////////////////////////////////
//  ABrush2D Class
////////////////////////////////////////////////////////////////////////////////

ABrush2D::ABrush2D()
{
  for(unsigned int t=0;t<4;t++) { v[t].x=0.0;  v[t].y=0.0;  v[t].z=0.0; }
  fRed=.5;  fGreen=.5;  fBlue=.5;
  textID=0;
  solid=true;
}


////////////////////////////////////////////////////////////////////////////////
//  AMapDecoder Class
////////////////////////////////////////////////////////////////////////////////

AMapDecoder::AMapDecoder(const char *fname) : ASceneDecoder(fname)
{
  init();
}


void AMapDecoder::init()
{
  DEBUG_OUT<<"AMapDecoder::init()\n";
  numMaps=0;
  curMapNum=0;
}


unsigned int AMapDecoder::getNumMaps()
{
  return numMaps;
}


void AMapDecoder::setMapNumber(unsigned int n)
{
  if(n<(numMaps+1)) curMapNum=n;
}


bool AMapDecoder::readTheCurrentMap()
{
  std::cerr<<"AMapDecoder::readTheCurrentMap()...NOT IMPLEMENTED!\n";
  return false;
}


void AMapDecoder::brushes2Mesh()
{
  std::cerr<<"AMapDecoder::brushes2mesh()...\n";
  AFace3D **tFaces=(AFace3D **)calloc(sizeof(AFace3D *),numBrushes*2);
  AVertexF3D **tVertices=(AVertexF3D **)calloc(sizeof(AVertexF3D *),numBrushes*4);
  brushes.reset();
  ABrush2D *b=NULL;
  AVertexF3D *v0=NULL;
  AVertexF3D *v1=NULL;
  AVertexF3D *v2=NULL;
  AVertexF3D *v3=NULL;
  AFace3D *f1=NULL;
  AFace3D *f2=NULL;
  bool reading=true;
  unsigned int iFace=0,iVertex=0;
  while(reading) {
    b=(ABrush2D *)brushes.get();
    if(b) {
      //DEBUG_OUT<<"iVertex="<<iVertex<<" iFace="<<iFace<<"\n";
      // First thing, we get the verts from the brush...
      v0=new AVertexF3D();  if(!v0) { std::cerr<<"Couldn't alloc v0!\n"; return; }
      v1=new AVertexF3D();  if(!v1) { std::cerr<<"Couldn't alloc v1!\n"; return; }
      v2=new AVertexF3D();  if(!v2) { std::cerr<<"Couldn't alloc v2!\n"; return; }
      v3=new AVertexF3D();  if(!v3) { std::cerr<<"Couldn't alloc v3!\n"; return; }
      v0->x=b->v[0].x;  v0->y=b->v[0].y;  v0->z=b->v[0].z;
      v1->x=b->v[1].x;  v1->y=b->v[1].y;  v1->z=b->v[1].z;
      v2->x=b->v[2].x;  v2->y=b->v[2].y;  v2->z=b->v[2].z;
      v3->x=b->v[3].x;  v3->y=b->v[3].y;  v3->z=b->v[3].z;
      tVertices[iVertex]=v0;  tVertices[iVertex+1]=v1;  tVertices[iVertex+2]=v2;  tVertices[iVertex+3]=v3;
      // Then the first face...
      f1=new AFace3D();  if(!f1) { std::cerr<<"Couldn't alloc f1!\n"; return; }
      f1->a=iVertex;  f1->b=iVertex+1;  f1->c=iVertex+3;  // 0, 1 and 3...
      f1->textID=b->textID;  f1->fRed=b->fRed;  f1->fGreen=b->fGreen;  f1->fBlue=b->fBlue;
      //DEBUG_OUT<<"face1 "<<f1->fRed<<" "<<f1->fGreen<<" "<<f1->fBlue<<"\n";
      // Then second face...
      f2=new AFace3D();  if(!f2) { std::cerr<<"Couldn't alloc f2!\n"; return; }
      f2->a=iVertex+1;  f2->b=iVertex+2;  f2->c=iVertex+3;  // 1, 2 and 3...
      f2->textID=b->textID;  f2->fRed=b->fRed;  f2->fGreen=b->fGreen;  f2->fBlue=b->fBlue;
      //DEBUG_OUT<<"face2 "<<f2->fRed<<" "<<f2->fGreen<<" "<<f2->fBlue<<"\n";
      //
      tFaces[iFace]=f1;
      tFaces[iFace+1]=f2;
      iVertex+=4;
      iFace+=2;
    }
    brushes.next();
    if(brushes.atEnd()) reading=false;
  }
  AMesh3D *mapMesh=new AMesh3D(numBrushes*4,tVertices,numBrushes*2,tFaces);
  nmeshes++;
  meshes.append(mapMesh);
}

