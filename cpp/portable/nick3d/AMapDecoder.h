#ifndef ALIB_AMAPDECODER_H
#define ALIB_AMAPDECODER_H


#include <portable/nickcpp/AList.h>

#include <portable/nick3d/AMatrix3D.h>
#include <portable/nick3d/AVertex3D.h>

#include "ASceneDecoder.h"


// A "Brush" in the Quake sense
// i.e. a static part of the level architecture
// NOTE: We assume it is geometrically flat!
class ABrush2D
{
public:
  ABrush2D();
  AVertexF3D v[4];
  float fRed,fGreen,fBlue;
  unsigned int textID;
  bool solid;
};


class AMapDecoder : public ASceneDecoder
{
public:
  AMapDecoder(const char *fname);
  //
  virtual bool isMap() { return true; }
  virtual unsigned int getNumMaps();
  virtual void setMapNumber(unsigned int n);
  virtual bool readTheCurrentMap();
  //
protected:
  //
  virtual void init();
  void brushes2Mesh();
  //
  unsigned int numMaps;
  unsigned int curMapNum;
  unsigned int numBrushes;
  AList brushes;
  //
};


#endif // ALIB_AMAPDECODER_H

