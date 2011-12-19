#ifndef DECODERLIB_A8SVXDECODER_H
#define DECODERLIB_A8SVXDECODER_H

#include "ASoundDecoder.h"


// EA's IFF 8svx format...
class A8SVXDecoder : public ASoundDecoder
{
public:
  A8SVXDecoder(const char *sndFname);
  static bool recognize(uint8_t *str);
protected:
  void header();
  void body();
};


// Apple's IFF AIFF format...
class AAIFFDecoder : public ASoundDecoder
{
public:
  AAIFFDecoder(const char *sndFname);
  static bool recognize(uint8_t *str);
protected:
  void header();
  void body();
};


#endif // DECODERLIB_A8SVXDECODER_H
