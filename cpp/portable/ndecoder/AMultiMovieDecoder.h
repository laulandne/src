#ifndef DECODERLIB_AMULTIMOVIEDECODER_H
#define DECODERLIB_AMULTIMOVIEDECODER_H


#include "AMovieDecoder.h"


class AMultiMovieDecoder : public AMovieDecoder
{
public:
  AMultiMovieDecoder(const char *imgFname);
  //
  static bool recognize(uint8_t *buf,char *fname=(char *)NULL);
protected:
};


#endif // DECODERLIB_AMULTIMOVIEDECODER_H

