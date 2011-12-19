#ifndef DECODERLIB_AMULTISOUNDDECODER_H
#define DECODERLIB_AMULTISOUNDDECODER_H


#include "ASoundDecoder.h"


class AMultiSoundDecoder : public ASoundDecoder
{
public:
  AMultiSoundDecoder(const char *imgFname);
  //
  static bool recognize(uint8_t *buf,char *fname=(char *)NULL);
protected:
};


#endif // DECODERLIB_AMULTISOUNDDECODER_H

