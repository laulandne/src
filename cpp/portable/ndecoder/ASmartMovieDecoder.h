#ifndef DECODERLIB_ASMARTMOVIEDECODER_H
#define DECODERLIB_ASMARTMOVIEDECODER_H


#include "ASmartDecoder.h"
#include "AMovieDecoder.h"


class ASmartMovieDecoder : ASmartDecoder
{
public:
  static AMovieDecoder *pickMovieDecoder(const char *theFilename);
};


#endif // DECODERLIB_ASMARTMOVIEDECODER_H
