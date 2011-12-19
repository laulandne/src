#ifndef DECODERLIB_AJFIFDECODER_H
#define DECODERLIB_AJFIFDECODER_H


#include "ADecoderHelper.h"


// JPEG JFIF decoder
class AJFIFDecoder : public ADecoderHelper
{
public:
  AJFIFDecoder(const char *imgFname);
  static bool recognize(uint8_t *buf,char *fname=(char *)NULL);
  virtual uint8_t *readLine(unsigned int lineNum, unsigned int numPlane);
private:
  virtual void header();
  virtual void color();
  // true if the image is grayscale instead of color
  bool grayscale;
};


#endif // DECODERLIB_AJFIFDECODER_H
