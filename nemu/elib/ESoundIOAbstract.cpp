
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#include <iostream>

#include <elib/ESoundIOAbstract.h>

//#include <soundlib/ASound.h>

#include <portable/nickcpp/ANullStream.h>

#include <portable/nickcpp/AStringHelper.h>


////////////////////////////////////////////////////////////////////////////////
//  ESoundIOAbstract Class
////////////////////////////////////////////////////////////////////////////////

ESoundIOAbstract::ESoundIOAbstract(EHandler *parent,unsigned int nvoices) : EMemIO(parent)
{
  DEBUG_OUT<<"ESoundIOAbstract::ESoundIOAbstract()\n";
  init();
  letter='s';
  setName("sound_memory");
  DEBUG_OUT<<"Going to malloc the wavePtr's...\n";
  unsigned int t=0;
  for(t=0;t<8;t++) {
    wavePtr[t]=(BYTE *)malloc(3072000/32);
    if(!wavePtr[t]) DEBUG_OUT<<"Didn't get wavePtr["<<t<<"]!\n";
  }
  DEBUG_OUT<<"Going to create the dsBuffers...\n";
  for(t=0;t<3;t++) {
    // buffers are for 1 second...
/*
    dsBuffer[t]=new ASound(8,(3072000/32),(3072000/32));
    if(dsBuffer[t]) {
      if(!dsBuffer[t]->err.error()) {
        dsBuffer[t]->write((BYTE *)wavePtr[0],dsBuffer[t]->getSize());
      }
      else { DEBUG_OUT<<"ASound["<<t<<"] has error...\n";  }
    }
    else { DEBUG_OUT<<"Didn't get ASound["<<t<<"]...\n"; }
*/
  }
  DEBUG_OUT<<"(done creating dsBuffers)\n";
}


ESoundIOAbstract::~ESoundIOAbstract()
{
  DEBUG_OUT<<"Destroying ESoundIOAbstract\n";
  unsigned int t=0;
  for(t=0;t<3;t++) {
/*
    if(dsBuffer[t]) {
      dsBuffer[t]->stop();
      delete dsBuffer[t];
      dsBuffer[t]=(ASound *)NULL;
    }
*/
  }
  for(t=0;t<8;t++) {
    if(wavePtr[t]) {
      free(wavePtr[t]);
      wavePtr[t]=(BYTE *)NULL;
    }
  }
}


void ESoundIOAbstract::init()
{
  // Init all data members
  unsigned int t=0;
  for(t=0;t<3;t++) {
    //dsBuffer[t]=(ASound *)NULL;
    frequency[t]=0; lastFrequency[t]=0;
    volume[t]=0; lastVolume[t]=0;
    wave[t]=0; lastWave[t]=0;
  }
  for(t=0;t<8;t++) wavePtr[t]=(BYTE *)NULL;
}


void ESoundIOAbstract::io_cycle()
{
}


void ESoundIOAbstract::reset()
{
}


void ESoundIOAbstract::ioregs()
{
}


void ESoundIOAbstract::stop()
{
  for(unsigned int t=0;t<3;t++) {
    //if(dsBuffer[t]->isPlaying()) dsBuffer[t]->stop();
  }
}


void ESoundIOAbstract::copyWaves(unsigned int srcOffset,unsigned int len,unsigned int num,ULONG offset)
{
  memcpy(&(wavePtr[num][offset]),&(waveBuffer[srcOffset]),len);
}


void ESoundIOAbstract::play()
{
  for(unsigned int t=0;t<3;t++) {
    //if(frequency[t]) dsBuffer[t]->playASyncLooped();
  }
}


void ESoundIOAbstract::setVoiceWave(unsigned int voiceNum, unsigned int waveNum)
{
  //if(dsBuffer[voiceNum]->isPlaying()) dsBuffer[voiceNum]->stop();
  //dsBuffer[voiceNum]->write((BYTE *)wavePtr[waveNum],dsBuffer[voiceNum]->getSize());
  //dsBuffer[voiceNum]->playASyncLooped();
}
