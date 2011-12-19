#ifndef ALIB_AMESHDECODER_H
#define ALIB_AMESHDECODER_H


// I'm an idiot.  I don't know where the circular references are that make me need this
class AMatrix3D;

#include <portable/nickcpp/AList.h>

#include <portable/nick3d/AMatrix3D.h>
#include <portable/nick3d/AVertex3D.h>
#include <portable/nick3d/AMesh3D.h>

#include "A3DDecoder.h"


class AMeshDecoder : public A3DDecoder
{
public:
  AMeshDecoder(const char *fname);
  virtual ~AMeshDecoder();
  //
  AMesh3D *createNewMeshObject();
  //
  unsigned int getNumParts() { return numParts; }
  AList *getParts() { return &parts; }
  AList *getFrameVerts(unsigned int num);
  unsigned int getNumSeqs() { return nseqs; }
  unsigned int getNumFrames() { return nframes; }
  unsigned int getNumVerts() { return nverts; }
  //
  AList *getFaces() { return &faces; }
  unsigned int getNumFaces() { return nfaces; }
  AList *getVertNormals() { return &vertNormals; }
  AList *getFaceNormals() { return &faceNormals; }
  AList *getFaceTextCoords() { return &faceTextCoords; }
  //
  AList *getTextures() { return &textures; }
  unsigned int getNumTextures() { return ntextures; }
  AList *getTextCoords() { return &textCoords; }
  unsigned int getNumTextCoords() { return ntextcoords; }
  bool getFixTextCoords() { return fixTextCoords; }
  //char *getName() { return name; }
protected:
  //
  virtual void init();
  void readVectorL(AVertexF3D *v);
  void readVectorB(AVertexF3D *v);
  void readC1VectorL(AVertexF3D *v);  // one byte compressed quake vector with normal index
  void readC2VectorL(AVertexF3D *v);  // two byte compressed quake vector with normal index
  void readMatrixL(AMatrix3D *m);
  //
  unsigned int nverts;
  unsigned int ntextures,ntextcoords;
  unsigned int nfaces,nframes,nseqs;
  unsigned int numParts;
  unsigned int vertOffset,oldVertOffset,faceOffset;
  bool fixTextCoords;
  //
  AList parts;
  AList *verts;  // array of list of verts, one per anim frame
  AList faces;
  // all the rest are optional
  AList seqs;
  AList vertNormals;  // same as num of verts
  AList faceNormals;  // same as num of faces
  AList faceTextCoords;  // same as num of faces
  AList textures;
  AList textCoords;
  //char *name;
};


#endif // ALIB_AMESHDECODER_H

