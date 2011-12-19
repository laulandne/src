#ifndef ALIB_AMDLDECODER_H
#define ALIB_AMDLDECODER_H


#include <portable/nickcpp/AList.h>

#include <portable/nick3d/AMatrix3D.h>
#include <portable/nick3d/AVertex3D.h>

#include "AMeshDecoder.h"


// Quake 1 Model (not Half-Life)
class AMDLDecoder : public AMeshDecoder
{
public:
  AMDLDecoder(const char *fname);
  static bool recognize(uint8_t *str);
protected:
  void readHeader();
  void readTextures();
  void readTextCoords();
  void readTriangles();
  void readFrames();
  void buildAndFixFaceAndTextCoordLists();
  //
  AVertexF3D scale;
  AVertexF3D translate;
  float boundingradius;
  AVertexF3D eyeposition;
  unsigned long skinwidth;
  unsigned long skinheight;
  unsigned long synctype;
  unsigned long flags;
  unsigned long theSize;
  char *frontBack;  // temp array
  AFace3D *tfaces;  // temp array
  ATextCoord3D *ttcs;  // temp array
  bool *alreadyFixed;  // temp array
  bool *onSeam;  // temp array
};


#endif // ALIB_AMDLDECODER_H

