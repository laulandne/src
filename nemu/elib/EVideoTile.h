#ifndef ELIB_EVIDEOTILE_H
#define ELIB_EVIDEOTILE_H


#include <elib/EMemAbstract.h>


class APrivateBitmap;


class EVideoTile {
public:
  EVideoTile();
  virtual ~EVideoTile();
  //
  virtual void init();
  //
  bool active;
  int x,y;
  unsigned int sx,sy;
  unsigned int width,height;
  unsigned int color;
  unsigned int gfx;
  int type;
  BYTE *gfxData;
  APrivateBitmap *source;
  int dir;
  //
protected:
  //
  //
};


#endif
