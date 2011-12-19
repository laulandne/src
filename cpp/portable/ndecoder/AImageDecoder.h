#ifndef DECODERLIB_AIMAGEDECODER_H
#define DECODERLIB_AIMAGEDECODER_H


#include <portable/nickcpp/ABitmap.h>

#include "ADecoder.h"


class APalette;


class AImageDecoder : public ADecoder
{
public:
  AImageDecoder(const char *fname);
  //
  ABitmap *createNewBitmapObject();
  //
  virtual uint8_t *readLine(unsigned int lineNum, unsigned int numPlane);
  bool getRGBReorder() const { return needsRGBReorder; }
  APalette *getColors() const { return colors; }
  uint8_t *getLineBuffer() const { return lineBuffer; }
  unsigned int getLineBufferSize() const { return lineBufferSize; }
  //
  int32_t getWidth() { return width; }
  int32_t getHeight() { return height; }
  int32_t getDepth() { return depth; }
  int32_t getBytesPerLine() { return bytesPerLine; }
  int32_t getNPlanes() { return nPlanes; }
  //
  static bool recognizeGIF(uint8_t *buf);
  static bool recognizeJPEG(uint8_t *buf);
  static bool recognizePNG(uint8_t *buf);
  static bool recognizeTGA(uint8_t *buf);
  static bool recognizeTIFF(uint8_t *buf);
  //
protected:
  //
  virtual void init();
  virtual void color();
  bool initLineBuffer();
  void readRawBitmap(bool needReorder);
  void reorderBitmapRGB();
  bool boundsValid();
  void calc();
  unsigned int calcBPL(unsigned int unit=0);
  void headerGIF();
  void headerJPEG();
  void headerPNG();
  void headerTGA();
  void headerTIFF();
  //
  uint8_t *lineBuffer;
  unsigned int lineBufferSize;
  // true if image decoded upsidedown
  bool needsRGBReorder;
  APalette *colors;
  ABitmap *bm;  // may or may not be used by child classes
  //
  int32_t width,height,depth;
  int32_t bytesPerLine,nPlanes;
  bool hasColorMap;
  uint8_t tBuffer[1024];
};


#endif // DECODERLIB_AIMAGEDECODER_H
