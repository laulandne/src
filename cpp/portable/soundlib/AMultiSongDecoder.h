#ifndef DECODERLIB_AMULTISONGDECODER_H
#define DECODERLIB_AMULTISONGDECODER_H


#include "ASongDecoder.h"


class AMultiSongDecoder : public ASongDecoder
{
public:
  AMultiSongDecoder(const char *imgFname);
  //
  static bool recognize(uint8_t *buf,char *fname=(char *)NULL);
protected:
};


#endif // DECODERLIB_AMULTISONGDECODER_H

