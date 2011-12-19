#ifndef ALIB_ADXFDECODER_H
#define ALIB_ADXFDECODER_H


#include <portable/nickcpp/AList.h>

#include <portable/nick3d/AMatrix3D.h>
#include <portable/nick3d/AVertex3D.h>

#include "ASceneDecoder.h"


// DXF model
class ADXFDecoder : public ASceneDecoder
{
public:
  ADXFDecoder(const char *fname);
  static bool recognize(uint8_t *str);
protected:
  void readObjects();
};


#endif // ALIB_ADXFDECODER_H

