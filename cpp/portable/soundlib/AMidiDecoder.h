#ifndef DECODERLIB_AMIDIDECODER_H
#define DECODERLIB_AMIDIDECODER_H


#include "ASongDecoder.h"


// Standard Midi file
class AMidiDecoder : public ASongDecoder
{
public:
  AMidiDecoder(const char *sngFname);
  static bool recognize(uint8_t *str);
protected:
  void header();
  void body();
};


#endif // DECODERLIB_AMIDIDECODER_H
