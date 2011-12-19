
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#include <iostream>
#include <stdlib.h>
#include <string.h>

#include <portable/nickcpp/ANullStream.h>

#include "ASound.h"
#include "AAudioDevice.h"


#ifdef ASYS_AMIGA
#include <exec/memory.h>
#include <clib/exec_protos.h>
#endif // ASYS_AMIGA



////////////////////////////////////////////////////////////////////////////////
//  Global junk
////////////////////////////////////////////////////////////////////////////////

struct aSoundFormat2 {
  bool avail;
  unsigned long format;
  unsigned long rate;
  unsigned int bits;
  bool stereo;
};


////////////////////////////////////////////////////////////////////////////////
//  ASound Class
////////////////////////////////////////////////////////////////////////////////

ASound::ASound()
{
  DEBUG_OUT<<"ASound() contructed.\n";
  init();
  createBuffers();
}


ASound::ASound(unsigned long pitch,unsigned long len)
{
  DEBUG_OUT<<"ASound("<<pitch<<") contructed.\n";
  init();
  // FIXME: Fill in some default squarewave data...
  createBuffers();
}


ASound::ASound(unsigned char *theSamples,unsigned long theNumSamples,unsigned long theSampleRate,unsigned int theNumBits)
{
  DEBUG_OUT<<"ASound(theSamples,"<<theNumSamples<<","<<theSampleRate<<","<<theNumBits<<") contructed.\n";
  init();
  createBuffers();
  samples=theSamples;
  numSamples=theNumSamples;
  sampleRate=theSampleRate;
  numBits=theNumBits;
}


// FIXME: Do this!
ASound::ASound(unsigned int bits,unsigned long rate,unsigned long bytes)
{
  DEBUG_OUT<<"ASound("<<bits<<","<<rate<<","<<bytes<<") contructed.\n";
  init();
  createBuffers();
  numBits=bits;
  sampleRate=rate;
  numSamples=bytes;
  stereo=false; // FIXME: Is this right?
}


ASound::~ASound()
{
  DEBUG_OUT<<"ASound::~ASound()\n";
#ifdef ASYS_WINDOWS
#ifdef USE_DIRECTSOUND
  if(dsBuffer) {
    dsBuffer->Stop();
    dsBuffer->Release();
  }
  dsBuffer=NULL;
#endif // USE_DIRECTSOUND
#endif // ASYS_WINDOWS
#ifdef ASYS_AMIGA
  if(samples) FreeMem(samples,numSamples);
#else
  if(samples) free(samples);
#endif // ASYS_AMIGA
  samples=NULL;
}


void ASound::init()
{
  ASoundAbstract::init();
  numSamples=0;
  samples=(unsigned char *)NULL;
  stereo=false;
  compressed=false;
  sampleRate=0;
  numBits=0;
#ifdef ASYS_WINDOWS
#ifdef USE_DIRECTSOUND
  dsBuffer=NULL;
#endif // USE_DIRECTSOUND
#endif // ASYS_WINDOWS
}


void ASound::createBuffers()
{
  DEBUG_OUT<<"ASound::createBuffers()\n";
#ifdef ASYS_AMIGA
  samples=(unsigned char *)AllocMem(numSamples,MEMF_CHIP);
#else
  samples=(unsigned char *)malloc((int)numSamples);
#endif // ASYS_AMIGA
#ifdef ASYS_WINDOWS
#ifdef USE_DIRECTSOUND
  PCMWAVEFORMAT pcm;
  DSBUFFERDESC dsDesc;
  memset(&pcm,0,sizeof(PCMWAVEFORMAT));
  pcm.wf.wFormatTag=WAVE_FORMAT_PCM;
  pcm.wf.nChannels=1;
  pcm.wf.nSamplesPerSec=sampleRate;
  pcm.wBitsPerSample=numBits;
  // FIXME: should the /8 be /numBits?
  pcm.wf.nBlockAlign=pcm.wBitsPerSample*pcm.wf.nChannels/8;
  pcm.wf.nAvgBytesPerSec=pcm.wf.nSamplesPerSec*pcm.wf.nBlockAlign;
  memset(&dsDesc,0,sizeof(DSBUFFERDESC));
  dsDesc.dwSize=sizeof(DSBUFFERDESC);
  dsDesc.dwFlags=DSBCAPS_CTRLVOLUME|DSBCAPS_CTRLPAN|DSBCAPS_CTRLFREQUENCY;
  dsDesc.dwBufferBytes=numSamples;
  dsDesc.lpwfxFormat=(LPWAVEFORMATEX)&pcm;
  LPDIRECTSOUND ddSound=NULL;
  if(ddSound) {
    unsigned long err;
    err=ddSound->CreateSoundBuffer(&dsDesc,&dsBuffer,NULL);
    if(err==DS_OK) DEBUG_OUT<<"Sound buffer created okay.\n";
    else {
      DEBUG_OUT<<"Problem creating sound buffer!\n";
      dsBuffer=NULL;
    }
  }
  else {
    DEBUG_OUT<<"Couldn't get DirectSound.\n";
    dsBuffer=NULL;
  }
#endif // USE_DIRECTSOUND
#endif // ASYS_WINDOWS
  DEBUG_OUT<<"(createBuffers done)\n";
}


void ASound::play(unsigned long pitch)
{
  DEBUG_OUT<<"ASound::play()\n";
  if(!okayDokey) verifyOkay();
  if(!okayDokey) { DEBUG_OUT<<"still not okay!\n"; return; }
  //if(myDevice) {
    //myDevice->play(this,pitch);
  //}
  //else DEBUG_OUT<<"No output device!\n";
}


void ASound::playASync(unsigned long pitch)
{
  play(pitch);
}


void ASound::playLooped(unsigned long pitch)
{
  playASyncLooped(pitch);
}


// FIXME: Do this!
void ASound::playASyncLooped(unsigned long pitch)
{
  DEBUG_OUT<<"ASound::playASyncLooped()\n";
  if(!okayDokey) verifyOkay();
  if(!okayDokey) { DEBUG_OUT<<"still not okay!\n"; return; }
  if(isPlaying()) stop();
  bool didIt=false;
#ifdef ASYS_WINDOWS
#ifdef USE_DIRECTSOUND
  if(dsBuffer) {
    dsBuffer->Play(0,0,DSBPLAY_LOOPING);
    didIt=true;
  }
  else DEBUG_OUT<<"No dsBuffer!\n";
#endif // USE_DIRECTSOUND
#endif // ASYS_WINDOWS
  if(!didIt) play(pitch);
}


// FIXME: Do this!
void ASound::stop()
{
  DEBUG_OUT<<"ASound::stop()\n";
  if(!isPlaying()) { DEBUG_OUT<<"not isPlaying!\n"; return; }
#ifdef ASYS_WINDOWS
#ifdef USE_DIRECTSOUND
  if(dsBuffer) dsBuffer->Stop();
  else DEBUG_OUT<<"No dsBuffer!\n";
#endif // USE_DIRECTSOUND
#endif // ASYS_WINDOWS
}


// FIXME: Do this!
bool ASound::isPlaying()
{
  bool ret=false;
#ifdef ASYS_WINDOWS
#ifdef USE_DIRECTSOUND
  if(dsBuffer) {
    DWORD status;
    dsBuffer->GetStatus(&status);
    if(status&DSBSTATUS_PLAYING) ret=true;
  }
  else DEBUG_OUT<<"No dsBuffer!\n";
#endif // USE_DIRECTSOUND
#endif // ASYS_WINDOWS
  return ret;
}


void ASound::verifyOkay()
{
  DEBUG_OUT<<"ASound::verifyOkay()\n";
  // Convert to format needed to play on current device...
  //if(!myDevice) { DEBUG_OUT<<"Can't verify.  No device!\n"; return; }
  //if(myDevice->verifyOkay(this)) okayDokey=true; else okayDokey=false;
  if(!okayDokey) DEBUG_OUT<<"Yowza!  Couldn't convert sound to match hardware!!!\n";
}


bool ASound::convert(unsigned long rate, unsigned int bits, bool st)
{
  DEBUG_OUT<<"ASound::convert()\n";
  bool ret=false;
  long signedRateDiff=(long)(sampleRate-rate);
  unsigned long rateDiff=abs((int)signedRateDiff);
  // If we fit arbitrarily close to the desired one, just let it go...
  if((rateDiff<1010)&&(numBits==bits)&&(stereo==st)) return true;
  // FIXME: actually convert the sound...
  if((rateDiff<1010)&&(numBits==bits)) {
    DEBUG_OUT<<"Rate and bits are close enough, but stereo is "<<stereo<<"!="<<st<<"\n";
    return false;
  }
  if(rateDiff<1010) {
    DEBUG_OUT<<"Rate is close enough, need to convert bits "<<numBits<<"!="<<bits<<"\n";
    return false;
  }
  DEBUG_OUT<<"Must be a rate mismatch "<<sampleRate<<"!="<<rate<<"\n";
  return ret;
}


unsigned long ASound::calcPlayingTime()
{
  if(!sampleRate) return 0;
  return (numSamples/sampleRate)*1000;
}


// FIXME: Do this!
void ASound::setPitch(unsigned long pitch)
{
#ifdef ASYS_WINDOWS
#ifdef USE_DIRECTSOUND
  if(dsBuffer) {
    dsBuffer->SetFrequency(pitch);
  }
  else DEBUG_OUT<<"No dsBuffer!\n";
#endif // USE_DIRECTSOUND
#endif // ASYS_WINDOWS
}


// FIXME: Do this!
void ASound::setVolume(unsigned long volume)
{
#ifdef ASYS_WINDOWS
#ifdef USE_DIRECTSOUND
  if(dsBuffer) {
    if(!volume) volume=DSBVOLUME_MIN;
    dsBuffer->SetVolume(volume);
    //if(!volume) stop();
  }
  else DEBUG_OUT<<"No dsBuffer!\n";
#endif // USE_DIRECTSOUND
#endif // ASYS_WINDOWS
}


// FIXME: Do this!
unsigned long ASound::getSize()
{
  return numSamples;
}


// FIXME: Do this!
void ASound::write(unsigned char *data,unsigned long len)
{
  DEBUG_OUT<<"ASound::write()\n";
  if(len>numSamples) len=numSamples;
  if(!len) { DEBUG_OUT<<"No len!\n"; return; }
  if(!data) { DEBUG_OUT<<"No data!\n"; return; }
  if(!samples) { DEBUG_OUT<<"No samples!\n"; return; }
  memcpy(samples,data,len);
#ifdef ASYS_WINDOWS
#ifdef USE_DIRECTSOUND
  if(dsBuffer) {
    unsigned int err;
    LPVOID p1,p2;
    DWORD s1,s2;
    // NOTE: 3072000/32 is samples per sec...
    err=dsBuffer->Lock(0,3072000/32,&p1,&s1,&p2,&s2,0);
    if(err==DS_OK) {
      DWORD ss1=s1,ss2=s2;
      if(len<(s1+s2)) { ss2=(s1+s2)-len; }
      if(len<s1) { ss1=len; ss2=0; }
      CopyMemory(p1,data,ss1);
      if(s2) CopyMemory(p2,data+ss1,ss2);
      dsBuffer->Unlock(p1,s1,p2,s2);
    }
    else DEBUG_OUT<<"Error locking Direct Sound buffer!\n";
  }
  else DEBUG_OUT<<"No dsBuffer!\n";
#endif // USE_DIRECTSOUND
#endif // ASYS_WINDOWS
}
