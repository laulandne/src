
//#define DEBUG_OUT std::cerr
#define DEBUG_OUT *aNullStream


#include "ATexture3D.h"


////////////////////////////////////////////////////////////////////////////////
//  ATexture3D Class
////////////////////////////////////////////////////////////////////////////////

ATexture3D::ATexture3D(char *fname)
{
  init();
  //bm=new ABitmap(fname);
  //
}


ATexture3D::ATexture3D(unsigned int w,unsigned int h,unsigned int d,unsigned int np)
{
  init();
  //bm=new ABitmap(w,h,d,np);
  //
}


void ATexture3D::init()
{
  //bm=(ABitmap *)NULL;
#ifdef USE_OPENGL
  textureID=0;
#endif
}
