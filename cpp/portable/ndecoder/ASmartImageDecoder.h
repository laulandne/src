#ifndef DECODERLIB_ASMARTIMAGEDECODER_H
#define DECODERLIB_ASMARTIMAGEDECODER_H


#include "ASmartDecoder.h"
#include "AImageDecoder.h"


class ASmartImageDecoder : ASmartDecoder
{
public:
  static AImageDecoder *pickImageDecoder(const char *theFilename);
};


#endif // DECODERLIB_ASMARTIMAGEDECODER_H
