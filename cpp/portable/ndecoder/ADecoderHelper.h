#ifndef DECODERLIB_ADECODERHELPER_H
#define DECODERLIB_ADECODERHELPER_H


#include "AImageDecoder.h"


// Contains routines that may be useful to
// more than one decoder, but not too many
// (Or they go in AImageDecoder...)
class ADecoderHelper : public AImageDecoder
{
public:
  ADecoderHelper(const char *fname);
  //
  static bool recognizeAIFF(uint8_t *buf);
  static bool recognizeRIFF(uint8_t *buf);
  //
  static bool recognizeMidi(uint8_t *buf);
  //
  static bool recognizeMPEG(uint8_t *buf);
  //
protected:
  void init();
  //
  void headerAIFF();
  void headerWAV();
  //
  void headerMidi();
  //
  void headerMPEG();
  //
  uint8_t tBuffer[1024];
};


#endif // DECODERLIB_ADECODERHELPER_H
