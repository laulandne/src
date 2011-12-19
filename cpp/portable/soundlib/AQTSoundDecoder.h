#ifndef DECODERLIB_AQTSOUNDDECODER_H
#define DECODERLIB_AQTSOUNDDECODER_H


#include <portable/ndecoder/AQTDecoderHelper.h>

#include "ASoundDecoder.h"


class AQTSoundDecoder : public ASoundDecoder
{
public:
  AQTSoundDecoder(const char *fname);
  static bool recognize(uint8_t *str);
protected:
  AQTDecoderHelper *helper;
};


#endif // DECODERLIB_AQTSOUNDDECODER_H
