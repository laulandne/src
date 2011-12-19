#ifndef DECODERLIB_ASMARTSOUNDDECODER_H
#define DECODERLIB_ASMARTSOUNDDECODER_H


#include <portable/ndecoder/ASmartDecoder.h>

#include "ASoundDecoder.h"


class ASmartSoundDecoder : ASmartDecoder
{
public:
  static ASoundDecoder *pickSoundDecoder(const char *theFilename);
};


#endif // DECODERLIB_ASMARTSOUNDDECODER_H
