
//#define DEBUG_OUT std::cerr
#define DEBUG_OUT *aNullStream


#include "ACamera3D.h"

#include <iostream>

#include <portable/nickcpp/ANullStream.h>


////////////////////////////////////////////////////////////////////////////////
//  ACamera3D Class
////////////////////////////////////////////////////////////////////////////////

ACamera3D::ACamera3D()
{
  DEBUG_OUT<<"ACamera3D created!\n";
  init();
}


ACamera3D::ACamera3D(float nx,float ny,float nz,float sa, float sb, float sc)
{
  DEBUG_OUT<<"ACamera3D created!\n";
  init();
  x=nx;  y=ny;  z=nz;
  a=sa;  b=sb;  c=sc;
  transDirty=true;
  calcBoundingBox();
}


void ACamera3D::init()
{
  x=0.0;  y=0.0;  z=0.0;
  a=0.0;  b=0.0;  c=0.0;
  sx=1.0;  sy=1.0;  sz=1.0;
  bmin.x=0.0;  bmax.x=0.0;  bmin.y=0.0;  bmax.y=0.0;  bmin.z=0.0;  bmax.z=0.0;
  transDirty=false;
  trans.unit();
  vwidth=0;
}


void ACamera3D::setViewSize(unsigned int w)
{
  DEBUG_OUT<<"ACamera3D view now "<<w<<"x"<<w<<"x"<<w<<".\n";
  vwidth=w/2;
}


void ACamera3D::calcBoundingBox()
{
  // Treat it like a point
  bmin.x=x; bmax.x=x;
  bmin.y=y; bmax.y=y;
  bmin.z=z; bmax.z=z;
}


void ACamera3D::recalcTrans()
{
  DEBUG_OUT<<"ACamera3D::recalcTrans "<<vwidth<<"x"<<vwidth<<"x"<<vwidth<<" "<<x<<","<<y<<","<<z<<" "<<a<<","<<b<<","<<c<<" "<<sx<<","<<sy<<","<<sz<<"\n";
  transDirty=false;
  trans.unit();
  trans.scale(sx*vwidth,sy*vwidth,sz*vwidth);
  trans.xRotBy(-a);
  trans.yRotBy(-b);
  trans.zRotBy(-c);
}


void ACamera3D::scale(float nx, float ny, float nz)
{
  sx=nx;  sy=ny;  sz=nz;
  transDirty=true;
}


void ACamera3D::translate(float sx, float sy, float sz)
{
  x+=sx;  y+=sy;  z+=sz;
  transDirty=true;
}


void ACamera3D::translateTo(float sx, float sy, float sz)
{
  x=sx;  y=sy;  z=sz;
  transDirty=true;
}


void ACamera3D::xRotTo(float theta)
{
  a=theta;
  transDirty=true;
}


void ACamera3D::yRotTo(float theta)
{
  b=theta;
  transDirty=true;
}


void ACamera3D::zRotTo(float theta)
{
  c=theta;
  transDirty=true;
}


void ACamera3D::xRotBy(float theta)
{
  a+=theta;
  transDirty=true;
}


void ACamera3D::yRotBy(float theta)
{
  b+=theta;
  transDirty=true;
}


void ACamera3D::zRotBy(float theta)
{
  c+=theta;
  transDirty=true;
}


void ACamera3D::debugDump()
{
  DEBUG_OUT<<"ACamera3D: x=("<<bmin.x<<","<<bmax.x<<") y=("<<bmin.y<<","<<bmax.y<<") z=("<<bmin.z<<","<<bmax.z<<")\n";
  // TODO: more here...
}
