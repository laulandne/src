
//#define DEBUG_OUT std::cerr
#define DEBUG_OUT *aNullStream


#include "AMatrix3D.h"

#include <iostream>

#include <portable/nickcpp/ANullStream.h>


#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif // M_PI


////////////////////////////////////////////////////////////////////////////////
// AMatrix3D  Class
////////////////////////////////////////////////////////////////////////////////

/** Create a new unit matrix */
AMatrix3D::AMatrix3D()
{
  //DEBUG_OUT<<"AMatrix3D::AMatrix3D\n";
  unit();
}


/** Scale by f in all dimensions */
void AMatrix3D::scale(float f)
{
  DEBUG_OUT<<"AMatrix3D::scale("<<f<<")\n";
  xx *= f;
  xy *= f;
  xz *= f;
  xo *= f;
  yx *= f;
  yy *= f;
  yz *= f;
  yo *= f;
  zx *= f;
  zy *= f;
  zz *= f;
  zo *= f;
}


/** Scale along each axis independently */
void AMatrix3D::scale(float xf, float yf, float zf)
{
  DEBUG_OUT<<"AMatrix3D::scale("<<xf<<","<<yf<<","<<zf<<")\n";
  xx *= xf;
  xy *= xf;
  xz *= xf;
  xo *= xf;
  yx *= yf;
  yy *= yf;
  yz *= yf;
  yo *= yf;
  zx *= zf;
  zy *= zf;
  zz *= zf;
  zo *= zf;
}


/** Translate the origin */
void AMatrix3D::translate(float x, float y, float z)
{
  DEBUG_OUT<<"AMatrix3D::translate("<<x<<","<<y<<","<<z<<")\n";
  xo += x;
  yo += y;
  zo += z;
}


/** Translate the origin */
void AMatrix3D::translateTo(float x, float y, float z)
{
  DEBUG_OUT<<"AMatrix3D::translate("<<x<<","<<y<<","<<z<<")\n";
  xo=x;
  yo=y;
  zo=z;
}


/** rotate theta degrees about the y axis */
void AMatrix3D::yRotBy(float theta)
{
  DEBUG_OUT<<"AMatrix3D::yrot("<<theta<<")\n";
  theta *= (M_PI / 180.0F);
  //DEBUG_OUT<<"theta is now "<<theta<<"\n";
  float ct = cos(theta);
  float st = sin(theta);

  float Nxx = (float) (xx * ct + zx * st);
  float Nxy = (float) (xy * ct + zy * st);
  float Nxz = (float) (xz * ct + zz * st);
  float Nxo = (float) (xo * ct + zo * st);

  float Nzx = (float) (zx * ct - xx * st);
  float Nzy = (float) (zy * ct - xy * st);
  float Nzz = (float) (zz * ct - xz * st);
  float Nzo = (float) (zo * ct - xo * st);

  xo = Nxo;
  xx = Nxx;
  xy = Nxy;
  xz = Nxz;
  zo = Nzo;
  zx = Nzx;
  zy = Nzy;
  zz = Nzz;
}


/** rotate theta degrees about the x axis */
void AMatrix3D::xRotBy(float theta)
{
  DEBUG_OUT<<"AMatrix3D::xrot("<<theta<<")\n";
  theta *= (M_PI / 180.0F);
  //DEBUG_OUT<<"theta is now "<<theta<<"\n";
  float ct = cos(theta);
  float st = sin(theta);

  float Nyx = (float) (yx * ct + zx * st);
  float Nyy = (float) (yy * ct + zy * st);
  float Nyz = (float) (yz * ct + zz * st);
  float Nyo = (float) (yo * ct + zo * st);

  float Nzx = (float) (zx * ct - yx * st);
  float Nzy = (float) (zy * ct - yy * st);
  float Nzz = (float) (zz * ct - yz * st);
  float Nzo = (float) (zo * ct - yo * st);

  yo = Nyo;
  yx = Nyx;
  yy = Nyy;
  yz = Nyz;
  zo = Nzo;
  zx = Nzx;
  zy = Nzy;
  zz = Nzz;
}


/** rotate theta degrees about the z axis */
void AMatrix3D::zRotBy(float theta)
{
  DEBUG_OUT<<"AMatrix3D::zrot("<<theta<<")\n";
  theta *= (M_PI / 180.0F);
  //DEBUG_OUT<<"theta is now "<<theta<<"\n";
  float ct = cos(theta);
  float st = sin(theta);

  float Nyx = (float) (yx * ct + xx * st);
  float Nyy = (float) (yy * ct + xy * st);
  float Nyz = (float) (yz * ct + xz * st);
  float Nyo = (float) (yo * ct + xo * st);

  float Nxx = (float) (xx * ct - yx * st);
  float Nxy = (float) (xy * ct - yy * st);
  float Nxz = (float) (xz * ct - yz * st);
  float Nxo = (float) (xo * ct - yo * st);

  yo = Nyo;
  yx = Nyx;
  yy = Nyy;
  yz = Nyz;
  xo = Nxo;
  xx = Nxx;
  xy = Nxy;
  xz = Nxz;
}


// this=this*a
// NOTE: This is an optimized 3x4 that pretends its 4x4
void AMatrix3D::mult(AMatrix3D *a)
{
  //DEBUG_OUT<<"AMatrix3D::mult()\n";
  float lxx = xx * a->xx + yx * a->xy + zx * a->xz;
  float lxy = xy * a->xx + yy * a->xy + zy * a->xz;
  float lxz = xz * a->xx + yz * a->xy + zz * a->xz;
  float lxo = xo * a->xx + yo * a->xy + zo * a->xz + a->xo;

  float lyx = xx * a->yx + yx * a->yy + zx * a->yz;
  float lyy = xy * a->yx + yy * a->yy + zy * a->yz;
  float lyz = xz * a->yx + yz * a->yy + zz * a->yz;
  float lyo = xo * a->yx + yo * a->yy + zo * a->yz + a->yo;

  float lzx = xx * a->zx + yx * a->zy + zx * a->zz;
  float lzy = xy * a->zx + yy * a->zy + zy * a->zz;
  float lzz = xz * a->zx + yz * a->zy + zz * a->zz;
  float lzo = xo * a->zx + yo * a->zy + zo * a->zz + a->zo;

  xx = lxx;
  xy = lxy;
  xz = lxz;
  xo = lxo;

  yx = lyx;
  yy = lyy;
  yz = lyz;
  yo = lyo;

  zx = lzx;
  zy = lzy;
  zz = lzz;
  zo = lzo;
}


// b=this*a
// NOTE: This is an optimized 3x4 that pretends its 4x4
void AMatrix3D::mult(AMatrix3D *a,AMatrix3D *b)
{
  //DEBUG_OUT<<"AMatrix3D::mult()\n";
  b->xx= xx * a->xx + yx * a->xy + zx * a->xz;
  b->xy= xy * a->xx + yy * a->xy + zy * a->xz;
  b->xz= xz * a->xx + yz * a->xy + zz * a->xz;
  b->xo= xo * a->xx + yo * a->xy + zo * a->xz + a->xo;

  b->yx= xx * a->yx + yx * a->yy + zx * a->yz;
  b->yy= xy * a->yx + yy * a->yy + zy * a->yz;
  b->yz= xz * a->yx + yz * a->yy + zz * a->yz;
  b->yo= xo * a->yx + yo * a->yy + zo * a->yz + a->yo;

  b->zx= xx * a->zx + yx * a->zy + zx * a->zz;
  b->zy= xy * a->zx + yy * a->zy + zy * a->zz;
  b->zz= xz * a->zx + yz * a->zy + zz * a->zz;
  b->zo= xo * a->zx + yo * a->zy + zo * a->zz + a->zo;
}


// b=this*a
// NOTE: This is an optimized 3x4 that pretends its 4x4
void AMatrix3D::mult(AVertexF3D *a,AVertexF3D *b)
{
  //DEBUG_OUT<<"AMatrix3D::mult()\n";
  b->x= xx * a->x + yx * a->y + zx * a->z + xo;
  b->y= xy * a->x + yy * a->y + zy * a->z + yo;
  b->z= xz * a->x + yz * a->y + zz * a->z + zo;
}


/** Reinitialize to the unit matrix */
void AMatrix3D::unit()
{
  //DEBUG_OUT<<"AMatrix3D::unit()\n";
  xo = 0.0;
  xx = 1.0;
  xy = 0.0;
  xz = 0.0;
  yo = 0.0;
  yx = 0.0;
  yy = 1.0;
  yz = 0.0;
  zo = 0.0;
  zx = 0.0;
  zy = 0.0;
  zz = 1.0;
}



void AMatrix3D::transformI(AVertexF3D *v, AVertexI3D *tv, int nvert)
{
  //DEBUG_OUT<<"AMatrix3D::transform(,,"<<nvert<<")\n";
  if(!v) return;
  if(!tv) return;
  float lxx = xx, lxy = xy, lxz = xz, lxo = xo;
  float lyx = yx, lyy = yy, lyz = yz, lyo = yo;
  float lzx = zx, lzy = zy, lzz = zz, lzo = zo;
  for (int i = 0;i<nvert;i++) {
    float x = v[i].x;
    float y = v[i].y;
    float z = v[i].z;
    tv[i].x =(x * lxx + y * lxy + z * lxz + lxo);
    tv[i].y =(x * lyx + y * lyy + z * lyz + lyo);
    tv[i].z =(x * lzx + y * lzy + z * lzz + lzo);
    //DEBUG_OUT<<"v "<<i<<": "<<v[i].x<<","<<v[i].y<<","<<v[i].z<<"\n";
    //DEBUG_OUT<<"tv "<<i<<": "<<tv[i].x<<","<<tv[i].y<<","<<tv[i].z<<"\n";
  }
}


void AMatrix3D::transformF(AVertexF3D *v, AVertexF3D *tv, int nvert)
{
  //DEBUG_OUT<<"AMatrix3D::transform(,,"<<nvert<<")\n";
  if(!v) return;
  if(!tv) return;
  float lxx = xx, lxy = xy, lxz = xz, lxo = xo;
  float lyx = yx, lyy = yy, lyz = yz, lyo = yo;
  float lzx = zx, lzy = zy, lzz = zz, lzo = zo;
  for (int i = 0;i<nvert;i++) {
      float x = v[i].x;
      float y = v[i].y;
      float z = v[i].z;
      tv[i].x =(x * lxx + y * lxy + z * lxz + lxo);
      tv[i].y =(x * lyx + y * lyy + z * lyz + lyo);
      tv[i].z =(x * lzx + y * lzy + z * lzz + lzo);
      //DEBUG_OUT<<"v "<<i<<": "<<v[i].x<<","<<v[i].y<<","<<v[i].z<<"\n";
      //DEBUG_OUT<<"tv "<<i<<": "<<tv[i].x<<","<<tv[i].y<<","<<tv[i].z<<"\n";
  }
}


void AMatrix3D::transform(float *v, int *tv, int nvert)
{
  //DEBUG_OUT<<"AMatrix3D::transform(,,"<<nvert<<")\n";
  if(!v) return;
  if(!tv) return;
  float lxx = xx, lxy = xy, lxz = xz, lxo = xo;
  float lyx = yx, lyy = yy, lyz = yz, lyo = yo;
  float lzx = zx, lzy = zy, lzz = zz, lzo = zo;
  for (int i = 0;i<nvert;i++) {
      float x = v[i*3];
      float y = v[i*3+1];
      float z = v[i*3+2];
      tv[i*3] =(x * lxx + y * lxy + z * lxz + lxo);
      tv[i*3+1] =(x * lyx + y * lyy + z * lyz + lyo);
      tv[i*3+2] =(x * lzx + y * lzy + z * lzz + lzo);
      //DEBUG_OUT<<"v "<<i<<": "<<v[i].x<<","<<v[i].y<<","<<v[i].z<<"\n";
      //DEBUG_OUT<<"tv "<<i<<": "<<tv[i].x<<","<<tv[i].y<<","<<tv[i].z<<"\n";
  }
}
