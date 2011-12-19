#ifndef ALIB_AMAYADECODER_H
#define ALIB_AMAYADECODER_H


#include <portable/nickcpp/AList.h>

#include <portable/nick3d/AMatrix3D.h>
#include <portable/nick3d/AVertex3D.h>

#include "ASceneDecoder.h"


// NOTE: This only handles the .ma ASCII format!

// Maya model
class AMayaDecoder : public ASceneDecoder
{
public:
  AMayaDecoder(const char *fname);
  static bool recognize(uint8_t *str);
protected:
  void readObjects();
};


#endif // ALIB_AMAYADECODER_H

