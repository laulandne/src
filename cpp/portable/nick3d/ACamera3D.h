#ifndef ALIB_ACAMERA3D_H
#define ALIB_ACAMERA3D_H


#include "AMatrix3D.h"
#include "AVertex3D.h"


class ACamera3D
{
public:
  ACamera3D();
  ACamera3D(float sx,float sy,float sz,float sa, float sb, float sc);
  //
  void setViewSize(unsigned int w);
  void scale(float sx, float sy, float sz);
  void translate(float sx, float sy, float sz);
  void translateTo(float sx, float sy, float sz);
  void xRotTo(float theta);
  void yRotTo(float theta);
  void zRotTo(float theta);
  void xRotBy(float theta);
  void yRotBy(float theta);
  void zRotBy(float theta);
  //
  AVertexF3D *getBMin() { return &bmin; }
  AVertexF3D *getBMax() { return &bmax; }
  AMatrix3D *getTrans() { recalcTrans(); return &trans; }
  //
  void calcBoundingBox();
  bool isDirty() { return transDirty; }
  void recalcTrans();
  void debugDump();
  //
protected:
  void init();
  //
  float x,y,z;    // location
  float a,b,c;    // rotation
  float sx,sy,sz; // scale
  bool transDirty;
  AMatrix3D trans;
  AVertexF3D bmin,bmax;
  unsigned int vwidth;
};


#endif // ALIB_ACAMERA3D_H

