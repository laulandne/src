#ifndef DECODERLIB_ASMARTMESHDECODER_H
#define DECODERLIB_ASMARTMESHDECODER_H


#include <portable/ndecoder/ASmartDecoder.h>

#include "AMeshDecoder.h"


class ASmartMeshDecoder : ASmartDecoder
{
public:
  static AMeshDecoder *pickMeshDecoder(const char *theFilename);
};


#endif // DECODERLIB_ASMARTMESHDECODER_H
