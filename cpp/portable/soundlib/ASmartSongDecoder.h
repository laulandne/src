#ifndef DECODERLIB_ASMARTSONGDECODER_H
#define DECODERLIB_ASMARTSONGDECODER_H


#include <portable/ndecoder/ASmartDecoder.h>

#include "ASongDecoder.h"


class ASmartSongDecoder : ASmartDecoder
{
public:
  static ASongDecoder *pickSongDecoder(const char *theFilename);
};


#endif // DECODERLIB_ASMARTSONGDECODER_H
