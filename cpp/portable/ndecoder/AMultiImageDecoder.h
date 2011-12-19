#ifndef DECODERLIB_AMULTIIMAGEDECODER_H
#define DECODERLIB_AMULTIIMAGEDECODER_H


#include "AImageDecoder.h"


class ABitmap;
class AConvertingBitLine;


class AMultiImageDecoder : public AImageDecoder
{
public:
  AMultiImageDecoder(const char *imgFname);
  //
  static bool recognize(uint8_t *buf,char *fname=(char *)NULL);
  virtual uint8_t *readLine(unsigned int lineNum, unsigned int numPlane);
protected:
  //
  virtual void header();
  virtual void color();
  bool readPNM(const char *ttname);
  //
  ABitmap *theImage;
  AConvertingBitLine *bitLine;
  char tname[256];
};


#endif

