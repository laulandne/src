#ifndef ELIB_ESOUNDIOABSTRACT_H
#define ELIB_ESOUNDIOABSTRACT_H


#include <elib/EMemIO.h>


//class ASound;


// (This is hard coded for now)
#define ESOUND_NUM_VOICES 3


class ESoundIOAbstract : public EMemIO {
public:
  // Public Member Functions
  ESoundIOAbstract(EHandler *parent,unsigned int nvoices=0);
  virtual ~ESoundIOAbstract();
  //
  virtual void io_cycle();
  virtual void ioregs();
  virtual void reset();
  //
  virtual void play();
  virtual void stop();
  void copyWaves(unsigned int srcOffset,unsigned int len,unsigned int num,ULONG offset);
  void setVoiceWave(unsigned int voiceNum, unsigned int waveNum);
  //
  // Public Data Members
protected:
  // Private Member Functions
  void init();
  // Private Data Members
  //ASound *dsBuffer[ESOUND_NUM_VOICES];
  BYTE *wavePtr[8];
  BYTE waveBuffer[256];
  long frequency[ESOUND_NUM_VOICES];
  long lastFrequency[ESOUND_NUM_VOICES];
  long volume[ESOUND_NUM_VOICES];
  long lastVolume[ESOUND_NUM_VOICES];
  long wave[ESOUND_NUM_VOICES];
  long lastWave[ESOUND_NUM_VOICES];
};


#endif
