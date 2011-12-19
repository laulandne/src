#ifndef DECODERLIB_AGIFDECODER_H
#define DECODERLIB_AGIFDECODER_H


#include "ADecoderHelper.h"


// CompuServe's GIF decoder
class AGIFDecoder : public ADecoderHelper
{
public:
  AGIFDecoder(const char *imgFname);
  static bool recognize(uint8_t *buf,char *fname=(char *)NULL);
  virtual uint8_t *readLine(unsigned int lineNum, unsigned int numPlane);
private:
  virtual void header();
  virtual void color();
};


#endif // DECODERLIB_AGIFDECODER_H
