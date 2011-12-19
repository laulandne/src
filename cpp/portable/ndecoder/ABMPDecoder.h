#ifndef DECODERLIB_ABMPDECODER_H
#define DECODERLIB_ABMPDECODER_H


#include "AImageDecoder.h"


class ABitmap;


// Microsoft's BMP bitmap decoder
class ABMPDecoder : public AImageDecoder
{
public:
  ABMPDecoder(const char *imgFname);
  virtual ~ABMPDecoder();
  static bool recognize(uint8_t *buf,char *fname=(char *)NULL);
  virtual uint8_t *readLine(unsigned int lineNum, unsigned int numPlane);
private:
  virtual void header();
  virtual void color();
  // true if this is an OS/2 style bitmap array
  bool isArray;
  // true if we need to find the best image of many
  bool findBest;
  // true if this is an OS/2 icon or pointer
  bool isIcon;
  // true if this is an "old" bitmap with a bitmapheader size of 12
  bool oldBMP;
  // This is the compression field from the bitmap header...
  unsigned long compField;
  // The RLE methods of compression use these
  unsigned int lineSkip,byteSkip;
  // This marks that we hit the end of rle in a compressed stream
  bool endRLE;
  unsigned long bitsSeek;
};


#endif // DECODERLIB_ABMPDECODER_H
