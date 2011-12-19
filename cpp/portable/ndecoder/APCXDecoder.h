#ifndef DECODERLIB_APCXDECODER_H
#define DECODERLIB_APCXDECODER_H


#include "ADecoderHelper.h"


// PCX, the popular PC-paintbrush format
class APCXDecoder : public AImageDecoder
{
public:
  APCXDecoder(const char *imgFname);
  static bool recognize(uint8_t *buf,char *fname=(char *)NULL);
  virtual uint8_t *readLine(unsigned int lineNum, unsigned int numPlane);
protected:
  virtual void header();
  virtual void color();
  unsigned int lineBytes;
  bool readPlanes;
};


#endif // DECODERLIB_APCXDECODER_H
