#ifndef DECODERLIB_AIFFDECODER_H
#define DECODERLIB_AIFFDECODER_H


#include "ADecoderHelper.h"


// IFF (ILBM), the popular amiga format originated by Electronic Arts
class AIFFDecoder : public AImageDecoder
{
public:
  AIFFDecoder(const char *imgFname);
  static bool recognize(uint8_t *buf,char *fname=(char *)NULL);
  virtual uint8_t *readLine(unsigned int lineNum, unsigned int numPlane);
private:
  virtual void header();
  virtual void color();
  // This used to see if we're in the middle of the "BODY" chunk for readLine
  unsigned int iffChunk;
  // This is the Amiga viewmodes value which describes the display mode id
  unsigned long iffViewModes;
  // true if the image includes a mask plane (ala DPaint's stencil)
  bool hasMask;
  // if true we won't actual read a line, but skip it instead (for skipping the mask)
  bool skipRead;
  // true if the image is in amiga Hold-And-Modify mode
  bool isHam;
  // true if the image has palette change data on per line basis (ala Digiview)
  bool isDynamic;
  // true if this is a pc dpaint 2e pbm file and not ilbm
  bool isPBM;
  unsigned int lineBytes;
  unsigned long tchunkOffset;
};


#endif // DECODERLIB_AIFFDECODER_H
