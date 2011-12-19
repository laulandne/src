#ifndef DECODERLIB_ASMARTDECODER_H
#define DECODERLIB_ASMARTDECODER_H


#include "ADecoder.h"


class ASmartDecoder
{
public:
  static bool getBuffer(const char *theFilename,uint8_t *theBuffer);
  //static bool recognize(char *imgFile,char *fname=(char *)NULL);
  static ADecoder *pickBestDecoder(const char *theFilename);
};


#endif // DECODERLIB_ASMARTDECODER_H
