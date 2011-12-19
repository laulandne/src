#ifndef DECODERLIB_AMP3DECODER_H
#define DECODERLIB_AMP3DECODER_H


#include "ASoundDecoder.h"


// MPEG Layer-3 Audio...
class AMP3Decoder : public ASoundDecoder
{
public:
  AMP3Decoder(const char *sndFname);
  static bool recognize(uint8_t *str);
protected:
  void header();
  void body();
};


#endif // DECODERLIB_AMP3DECODER_H
