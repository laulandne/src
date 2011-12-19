#ifndef DECODERLIB_AAUDECODER_H
#define DECODERLIB_AAUDECODER_H


#include "ASoundDecoder.h"


// Sun's AU format...
class AAUDecoder : public ASoundDecoder
{
public:
  AAUDecoder(const char *sndFname);
  static bool recognize(uint8_t *str);
protected:
  void header();
  void body();
  unsigned long dataLoc,dataSize;
};


#endif // DECODERLIB_AAUDECODER_H
