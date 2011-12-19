#ifndef ALIB_AMESH3D_H
#define ALIB_AMESH3D_H


#include "AMatrix3D.h"
#include "AVertex3D.h"
#include "APart3D.h"
#include "ATexture3D.h"
#include "ATextCoord3D.h"
#include "AFace3D.h"
#include "ACamera3D.h"


class AList;


class AMesh3D
{
public:
  AMesh3D();
  AMesh3D(unsigned int tnVerts,float *tVerts,unsigned int tnFaces,short *tFaces);
  AMesh3D(unsigned int tnVerts,AVertexF3D **tVerts,unsigned int tnFaces,AFace3D **tFaces);
  AMesh3D(AFace3D *f,AVertexF3D a,AVertexF3D b,AVertexF3D c,AVertexF3D d);
  virtual ~AMesh3D();
  //
  // An array of pointers to APart3D objects...
  APart3D **getParts() { return parts; }
  unsigned int getNumParts() { return nparts; }
  //
  // An array of arrays of AVertexF3D objects...
  AVertexF3D **getVerts() { return verts; }
  // An array of AVertexI3D objects...
  AVertexI3D *getDrawVerts() { return drawVerts; }
  unsigned int getNumVerts() { return nverts; }
  //
  // An array of AVertexF3D objects...
  AVertexF3D *getVertNormals() { return vertNormals; }
  // An array of AVertexF3D objects...
  AVertexF3D *getFaceNormals() { return faceNormals; }
  //
  // An array of ATextCoord3D objects...
  ATextCoord3D *getTextCoords() { return textCoords; }
  unsigned int getNumTextCoords() { return ntextcoords; }
  // An array of pointers to ATexture3D objects...
  ATexture3D **getTextures() { return textures; }
  unsigned int getNumTextures() { return ntextures; }
  //
  // An array of pointers to AFace3D objects...
  AFace3D **getFaces() { return faces; }
  // An array of pointers to AFace3D objects...
  AFace3D **getFaceTextCoords() { return faceTextCoords; }
  unsigned int getNumFaces() { return nfaces; }
  unsigned int getNumFaceTextCoords() { return nftcs; }
  //
  // WARNING These are only meant to be used internally when building the object from raw parts!
  void setParts(APart3D **p) { parts=p; }
  void setNumParts(unsigned int n) { nparts=n; }
  void setFaces(AFace3D **f) { faces=f; }
  void setFaceTextCoords(AFace3D **f) { faceTextCoords=f; }
  void setNumFaces(unsigned int n) { nfaces=n; }
  void setNumFrames(unsigned int n) { numFrames=n; }
  //void addFace(AFace3D *f);
  //
  // WARNING These are only meant to be used internally when building the object from raw parts!
  void createVertsPerFrame(unsigned int frame,AList *theVerts,unsigned int numVerts);
  void createVertNormals(AList *theVerts,unsigned int numVerts);
  void createTextCoords(AList *theCoords,unsigned int numtcs);
  void createTextures(AList *theTextures,unsigned int numTextures);
  void buildFTCsFromTCs();
  void allocVertParts(unsigned int tNumFrames,unsigned int whichFrame,unsigned int tNumVerts);
  //
  void setName(char *n) { name=n; }
  char *getName() { return name; }
  AMatrix3D *getModelToDrawMatrix() { return &modelToDrawMatrix; }
  AVertexF3D *getBMin() { return &bmin; }
  AVertexF3D *getBMax() { return &bmax; }
  float getXWidth() { return xwidth; }
  float getYWidth() { return ywidth; }
  float getZWidth() { return zwidth; }
  bool isStale() { return stale; }
  void setStale(bool t) { stale=t; }
  //
  void scale(float sx,float sy,float sz);
  void translate(float sx,float sy,float sz);
  void translateTo(float sx,float sy,float sz);
  void rotateBy(float sx,float sy,float sz);
  void rotateByX(float r);
  void rotateByY(float r);
  void rotateByZ(float r);
  //
  void scaleAllVerts(float sx,float sy,float sz);
  void translateAllVerts(float sx,float sy,float sz);
  void rotateXAllVerts(float r);
  void rotateYAllVerts(float r);
  void rotateZAllVerts(float r);
  //
  void transformVertsToDraw();
  void calcWidths();
  void calcBoundingBox();
  float normalizeScale();  // returns value it scaled by
  void normalizeTranslation();
  //
  unsigned int getNumAnimSeq() { return numSeq; }
  void setAnimSequence(unsigned int num) { curSeqNum=num; }
  unsigned int getNumAnimFrames() { return numFrames; }
  void setAnimFrame(unsigned int num) { curFrameNum=num; }
  bool fixTextCoords;
  //
  virtual void debugDump();
  bool isValid();
protected:
  void init();
  //
  AVertexF3D bmin,bmax;
  float xwidth,ywidth,zwidth;
  //
  unsigned int nparts;
  unsigned int curFrameNum,curSeqNum;
  unsigned int nverts;
  unsigned int nfaces,ntextures,ntextcoords;
  unsigned int numSeq,numFrames;
  unsigned int nftcs;  // should always equal nfaces
  //
  APart3D **parts;
  AList *partList;
  AFace3D **faces;
  AList *faceList;
  AFace3D **faceTextCoords;  // same as num of faces
  AList *faceTextCoordList;
  //
  AVertexF3D **verts;
  AVertexI3D *drawVerts;
  //
  AVertexF3D *vertNormals;  // same as num of verts
  AVertexF3D *faceNormals;  // same as num of faces
  ATextCoord3D *textCoords;
  ATexture3D **textures;
  //
  AMatrix3D modelToDrawMatrix;
  char *name;
  bool stale;
};


#endif // ALIB_AMESH3D_H

