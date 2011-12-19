#ifndef DECODERLIB_ASMARTSCENEDECODER_H
#define DECODERLIB_ASMARTSCENEDECODER_H


#include <portable/ndecoder/ASmartDecoder.h>

#include "ASceneDecoder.h"


class ASmartSceneDecoder : ASmartDecoder
{
public:
  static ASceneDecoder *pickSceneDecoder(const char *theFilename);
};


#endif // DECODERLIB_ASMARTSCENEDECODER_H
