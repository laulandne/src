#ifndef DECODERLIB_AWAVDECODER_H
#define DECODERLIB_AWAVDECODER_H


#include "ASoundDecoder.h"


// MicroSoft's WAV format...
class AWAVDecoder : public ASoundDecoder
{
public:
  AWAVDecoder(const char *sndFname);
  static bool recognize(uint8_t *str);
protected:
  void header();
  void body();
};


#endif // DECODERLIB_AWAVDECODER_H
