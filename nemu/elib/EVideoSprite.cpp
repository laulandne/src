
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#include <elib/EVideoSprite.h>


////////////////////////////////////////////////////////////////////////////////
//  EVideoSprite Class
////////////////////////////////////////////////////////////////////////////////

EVideoSprite::EVideoSprite()
{
  init();
}


EVideoSprite::~EVideoSprite()
{
}


void EVideoSprite::init()
{
  EVideoTile::init();
}

