#ifndef DECODERLIB_ASOUNDDECODER_H
#define DECODERLIB_ASOUNDDECODER_H


#include <portable/ndecoder/ADecoder.h>


class ASound;


class ASoundDecoder : public ADecoder
{
public:
  ASoundDecoder(const char *fname);
  virtual ~ASoundDecoder();
  //
  ASound *createNewSoundObject();
  //
  unsigned long getNumSamples() const { return numSamples; }
  uint8_t *getSamples() const { return samples; }
  unsigned long getSampleRate() const { return sampleRate; }
  unsigned int getNumBits() const { return numBits; }
  bool getStereo() const { return stereo; }
  bool getCompressed() const { return compressed; }
protected:
  //
  void init();
  bool initBuffer();
  //
  unsigned long numSamples;
  uint8_t *samples;
  unsigned long sampleRate;
  unsigned int numBits;
  bool stereo;
  bool compressed;
};


#endif // DECODERLIB_ASOUNDDECODER_H
