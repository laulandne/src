#ifndef DECODERLIB_ATGADECODER_H
#define DECODERLIB_ATGADECODER_H


#include "ADecoderHelper.h"


// Truevision Targa TGA decoder
class ATGADecoder : public AImageDecoder
{
public:
  ATGADecoder(const char *imgFname);
  static bool recognize(uint8_t *buf,char *fname=(char *)NULL);
  virtual uint8_t *readLine(unsigned int lineNum, unsigned int numPlane);
private:
  virtual void header();
  virtual void color();
};


#endif // DECODERLIB_ATGADECODER_H
