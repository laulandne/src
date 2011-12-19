#ifndef DECODERLIB_AQTSONGDECODER_H
#define DECODERLIB_AQTSONGDECODER_H


#include <portable/ndecoder/AQTDecoderHelper.h>

#include "ASongDecoder.h"


class AQTSongDecoder : public ASongDecoder
{
public:
  AQTSongDecoder(const char *fname);
  static bool recognize(uint8_t *str);
protected:
  AQTDecoderHelper *helper;
};


#endif // DECODERLIB_AQTSONGDECODER_H
