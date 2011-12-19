
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#include <elib/EVideoTile.h>


////////////////////////////////////////////////////////////////////////////////
//  EVideoTile Class
////////////////////////////////////////////////////////////////////////////////

EVideoTile::EVideoTile()
{
  init();
}


EVideoTile::~EVideoTile()
{
}


void EVideoTile::init()
{
  active=false;
  x=0;  y=0;
  sx=0;  sy=0;
  width=0;  height=0;
  color=0;
  gfx=0;
  //
  type=0;
  gfxData=(BYTE *)NULL;
  source=NULL;
  dir=0;
}

