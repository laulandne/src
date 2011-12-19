#ifndef DECODERLIB_AQTIMAGEDECODER_H
#define DECODERLIB_AQTIMAGEDECODER_H


#include "AQTDecoderHelper.h"


class AQTImageDecoder : public AImageDecoder
{
public:
  AQTImageDecoder(const char *fname);
  static bool recognize(uint8_t *str);
  virtual uint8_t *readLine(unsigned int lineNum, unsigned int numPlane);
protected:
  AQTDecoderHelper *helper;
};


#endif // DECODERLIB_AQTIMAGEDECODER_H
