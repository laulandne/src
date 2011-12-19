#ifndef ALIB_ASOUND_H
#define ALIB_ASOUND_H


#include "ASoundAbstract.h"


#ifdef ASYS_WINDOWS
#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>
#undef WIN32_LEAN_AND_MEAN
#ifdef USE_DIRECTSOUND
#include <mmsystem.h>
#include <dsound.h>
#endif // USE_DIRECTSOUND
#endif // ASYS_WINDOWS


class ASound : public ASoundAbstract
{
public:
  // Create a default tone
  ASound();
  // Create a tone of the specified pitch
  ASound(unsigned long pitch,unsigned long len);
  // create an empty sound to the given specs
  ASound(unsigned int bits,unsigned long rate,unsigned long bytes);
  ASound(unsigned char *theSamples,unsigned long theNumSamples,unsigned long theSampleRate,unsigned int theNumBits);
  virtual ~ASound();
  void play(unsigned long pitch=0);
  void playLooped(unsigned long pitch=0);
  void playASync(unsigned long pitch=0);
  void playASyncLooped(unsigned long pitch=0);
  void stop();
  unsigned long calcPlayingTime();
  unsigned long getSize();
  void write(unsigned char *data,unsigned long len);
  bool isPlaying();
  void setPitch(unsigned long pitch);
  void setVolume(unsigned long volume);
  unsigned long getSampleRate() { return sampleRate; }
  unsigned int getNumBits() { return numBits; }
  unsigned long getNumSamples() { return numSamples; }
  unsigned char *getSamples() { return samples; }
  bool getStereo() { return stereo; }
  bool getCompressed() const { return compressed; }
  bool convert(unsigned long rate, unsigned int bits, bool st);
protected:
  // Private member functions
  void init();
  void createBuffers();
  void verifyOkay();
  // Private data members
  bool stereo;
  bool compressed;
  unsigned long sampleRate;  // 0=variable
  unsigned int numBits;
  unsigned long numSamples;
  unsigned char *samples;
#ifdef ASYS_WINDOWS
#ifdef USE_DIRECTSOUND
  LPDIRECTSOUNDBUFFER dsBuffer;
#endif // USE_DIRECTSOUND
#endif // ASYS_WINDOWS
};


#endif // ALIB_ASOUND_H
