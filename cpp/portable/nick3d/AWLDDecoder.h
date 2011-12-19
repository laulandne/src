#ifndef ALIB_AWLDDECODER_H
#define ALIB_AWLDDECODER_H


#include <portable/nickcpp/AList.h>

#include <portable/nick3d/AMatrix3D.h>
#include <portable/nick3d/AVertex3D.h>
#include <portable/nick3d/AMesh3D.h>

#include "ASceneDecoder.h"


struct AWLDDecoderSurfaceDef {
  char *name;
  unsigned int value;
};


struct AWLDDecoderSurfaceMap {
  char *name;
  AList surfaces;
};


struct AWLDDecoderSurface {
  unsigned int num;
  char *name;
};


struct AWLDDecoderObject {
  char *tag;  // optional
  char *name;  // filename
  AMesh3D *mesh;
  char *surfMapName;  // optional
};


// WorLD format from Rend386 and Gossamer and friends...
class AWLDDecoder : public ASceneDecoder
{
public:
  AWLDDecoder(const char *fname);
  static bool recognize(uint8_t *str);
protected:
  void readObjectsFromFile(FILE *fd);
  void fixObjectSurfMaps(AWLDDecoderObject *theObj);
  unsigned int lookupSurfNum(char *surfMapName,unsigned int surfNum);
  //
  AList surfaceDefs;
  AList surfaceMaps;
  AList objects;
  AWLDDecoderSurfaceMap *curSurfMap;
};


#endif // ALIB_AWLDDECODER_H

