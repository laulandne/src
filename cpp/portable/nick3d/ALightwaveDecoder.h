#ifndef ALIB_ALIGHTWAVEDECODER_H
#define ALIB_ALIGHTWAVEDECODER_H


#include <portable/nickcpp/AList.h>

#include <portable/nick3d/AMatrix3D.h>
#include <portable/nick3d/AVertex3D.h>

#include "ASceneDecoder.h"


// Lightwave model
class ALightwaveDecoder : public ASceneDecoder
{
public:
  ALightwaveDecoder(const char *fname);
  static bool recognize(uint8_t *str);
protected:
  void readObjects();
  void readSurf();
  void readPols();
  void readPnts();
  void readPtag();
  void readTags();
  void readBbox();
  void readLayr();
  void readClip();
};


#endif // ALIB_ALIGHTWAVEDECODER_H

