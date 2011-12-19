
//#define DEBUG_OUT std::cerr
#define DEBUG_OUT *aNullStream


#include "ALight3D.h"

#include <iostream>

#include <portable/nickcpp/ANullStream.h>


////////////////////////////////////////////////////////////////////////////////
//  ALight3D Class
////////////////////////////////////////////////////////////////////////////////


ALight3D::ALight3D(float x,float y,float z)
{
  DEBUG_OUT<<"ALight3D created!\n";
  init();
  // Treat it like a point
  bmin.x=x; bmax.x=x;
  bmin.y=y; bmax.y=y;
  bmin.z=z; bmax.z=z;
}


void ALight3D::init()
{
  bmin.x=0.0;  bmax.x=0.0;  bmin.y=0.0;  bmax.y=0.0;  bmin.z=0.0;  bmax.z=0.0;
}


void ALight3D::debugDump()
{
  DEBUG_OUT<<"ALight3D: x=("<<bmin.x<<","<<bmax.x<<") y=("<<bmin.y<<","<<bmax.y<<") z=("<<bmin.z<<","<<bmax.z<<")\n";
  // TODO: more here...
}
