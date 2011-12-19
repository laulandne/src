#ifndef ALIB_AWAVEFRONTDECODER_H
#define ALIB_AWAVEFRONTDECODER_H


#include "AMeshDecoder.h"


// Wavefront text based OBJ file
class AWavefrontDecoder : public AMeshDecoder
{
public:
  AWavefrontDecoder(const char *fname);
  virtual ~AWavefrontDecoder();
  static bool recognize(uint8_t *str);
protected:
  void readObjects();
  //
  bool hasvns,hasvts;
};


#endif // ALIB_AWAVEFRONTDECODER_H

