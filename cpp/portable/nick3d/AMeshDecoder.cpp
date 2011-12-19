
//#define DEBUG_OUT std::cerr
#define DEBUG_OUT *aNullStream


#include <iostream>
#include <stdlib.h>
#include <string.h>

#include <portable/nickcpp/AList.h>
#include <portable/nickcpp/ANullStream.h>

#include <portable/nick3d/AMatrix3D.h>
#include <portable/nick3d/AVertex3D.h>
#include <portable/nick3d/AFace3D.h>
#include <portable/nick3d/AMesh3D.h>

#include "AMeshDecoder.h"


////////////////////////////////////////////////////////////////////////////////
//  AMeshDecoder Class
////////////////////////////////////////////////////////////////////////////////

AMeshDecoder::AMeshDecoder(const char *fname) : A3DDecoder(fname)
{
  init();
}


AMeshDecoder::~AMeshDecoder()
{
}


void AMeshDecoder::init()
{
  isAMesh=true;
  nverts=0; ntextcoords=0;
  nfaces=0; ntextures=0;
  nframes=1;  nseqs=0;
  numParts=0;
  vertOffset=0;  oldVertOffset=0;  faceOffset=0;
  fixTextCoords=false;
  //name=NULL;
}


AMesh3D *AMeshDecoder::createNewMeshObject()
{
  DEBUG_OUT<<"AMeshDecoder::createNewMeshObject() starting...\n";
  AMesh3D *m=new AMesh3D();
  DEBUG_OUT<<"Parts...\n";
  m->setParts((APart3D **)allocArrayBigEnoughForList(&parts));
  m->setNumParts(buildArrayFromList((void **)m->getParts(),&parts));
  DEBUG_OUT<<"Faces...\n";
  m->setFaces((AFace3D **)allocArrayBigEnoughForList(&faces));
  m->setNumFaces(buildArrayFromList((void **)m->getFaces(),&faces));
  DEBUG_OUT<<"FaceTextCoords...\n";
  m->setFaceTextCoords((AFace3D **)allocArrayBigEnoughForList(&faceTextCoords));
  buildArrayFromList((void **)m->getFaceTextCoords(),&faceTextCoords);
  //
  DEBUG_OUT<<"Frames/Verts...\n";
  m->setNumFrames(nframes);
  for(unsigned int t=0;t<nframes;t++) {
    m->createVertsPerFrame(t,getFrameVerts(t),nverts);
  }
  DEBUG_OUT<<"VertNormals...\n";
  m->createVertNormals(&vertNormals,nverts);
  m->fixTextCoords=fixTextCoords;
  if(fixTextCoords) DEBUG_OUT<<"(textCoords need to be fixed)\n";
  DEBUG_OUT<<"TextCoords...\n";
  m->createTextCoords(&textCoords,ntextcoords);
  DEBUG_OUT<<"Textures...\n";
  m->createTextures(&textures,ntextures);
  if(m->getNumTextCoords()&&(!m->getNumFaceTextCoords())) m->buildFTCsFromTCs();
  m->calcWidths();
  m->isValid();
  DEBUG_OUT<<"Done.\n";
  return m;
}


void AMeshDecoder::readVectorL(AVertexF3D *v)
{
  v->x=readFloatL();
  v->y=readFloatL();
  v->z=readFloatL();
}


void AMeshDecoder::readVectorB(AVertexF3D *v)
{
  v->x=readFloatB();
  v->y=readFloatB();
  v->z=readFloatB();
}


void AMeshDecoder::readC1VectorL(AVertexF3D *v)
{
  v->x=readByte();
  v->y=readByte();
  v->z=readByte();
  unsigned char normalIndex=readByte();
  // ???
}


void AMeshDecoder::readC2VectorL(AVertexF3D *v)
{
  v->x=readShortL();
  v->y=readShortL();
  v->z=readShortL();
  unsigned short normalIndex=readShortL();
  // ???
}


void AMeshDecoder::readMatrixL(AMatrix3D *m)
{
  AVertexF3D a,b,c;
  readVectorL(&a);
  readVectorL(&b);
  readVectorL(&c);
}


AList *AMeshDecoder::getFrameVerts(unsigned int num)
{
  if(verts) return &(verts[num]);
  else return NULL;
}

