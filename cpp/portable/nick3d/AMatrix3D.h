#ifndef ALIB_AMATRIX3D_H
#define ALIB_AMATRIX3D_H


#include "AVertex3D.h"
#include "AFace3D.h"

#include <math.h>


// NOTE: This is an optimized 3x4 that pretends its 4x4
class AMatrix3D {
public:
  AMatrix3D();
  void scale(float f);
  void scale(float xf, float yf, float zf);
  void translate(float x, float y, float z);
  void translateTo(float x, float y, float z);
  void xRotBy(float theta);
  void yRotBy(float theta);
  void zRotBy(float theta);
  void mult(AMatrix3D *a);  // this=this*a
  void mult(AMatrix3D *a,AMatrix3D *b);  // b=this*a
  void mult(AVertexF3D *a,AVertexF3D *b);  // b=this*a
  void unit();
  void transformI(AVertexF3D *v, AVertexI3D *tv, int nvert);
  void transformF(AVertexF3D *v, AVertexF3D *tv, int nvert);
  void transform(float *v,int *tv,int nvert);
//protected:
  float xx, xy, xz, xo;
  float yx, yy, yz, yo;
  float zx, zy, zz, zo;
};


#endif // ALIB_MATRIX3D_H


