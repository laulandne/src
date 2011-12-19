#ifndef ALIB_APLGDECODER_H
#define ALIB_APLGDECODER_H


#include "AMeshDecoder.h"


// PoLyGon format from Rend386 and Gossamer and friends...
class APLGDecoder : public AMeshDecoder
{
public:
  APLGDecoder(const char *fname);
  static bool recognize(uint8_t *str);
protected:
  void readObjects();
  //
};


#endif // ALIB_APLGDECODER_H

