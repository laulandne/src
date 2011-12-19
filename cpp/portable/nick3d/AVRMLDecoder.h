#ifndef ALIB_AVRMLDECODER_H
#define ALIB_AVRMLDECODER_H


#include <portable/nickcpp/AList.h>

#include <portable/nick3d/AMatrix3D.h>
#include <portable/nick3d/AVertex3D.h>

#include "ASceneDecoder.h"


// VRML model
class AVRMLDecoder : public ASceneDecoder
{
public:
  AVRMLDecoder(const char *fname);
  static bool recognize(uint8_t *str);
protected:
  void readObjects();
};


#endif // ALIB_AVRMLDECODER_H

