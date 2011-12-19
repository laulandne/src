#ifndef DECODERLIB_AQTMOVIEDECODER_H
#define DECODERLIB_AQTMOVIEDECODER_H


#include "AQTDecoderHelper.h"

#include "AMovieDecoder.h"


class AQTMovieDecoder : public AMovieDecoder
{
public:
  AQTMovieDecoder(const char *fname);
  static bool recognize(uint8_t *str);
protected:
  AQTDecoderHelper *helper;
};


#endif // DECODERLIB_AQTMOVIEDECODER_H
