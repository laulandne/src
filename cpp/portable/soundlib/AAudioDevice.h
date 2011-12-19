#ifndef ALIB_ASOUNDOBJECT_H
#define ALIB_ASOUNDOBJECT_H


#ifdef ASYS_MAC
#include <guilib/alib_carbon.h>
#ifndef USE_UMBRELLA_HEADERS
//#include <Sound.h>
#endif // USE_UMBRELLA_HEADERS
#endif // ASYS_MAC

#ifdef ASYS_WINDOWS
#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>
#undef WIN32_LEAN_AND_MEAN
#include <mmsystem.h>
#ifdef USE_DIRECTSOUND
#include <dsound.h>
#endif
#endif // ASYS_WINDOWS

#ifdef USE_QUICKTIME
#ifdef USE_UMBRELLA_HEADERS
#include <Quicktime/Quicktime.h>
#else
#include <QuickTimeComponents.h>
#ifndef MAC_OLD_HEADERS
#include <QuickTimeMusic.h>
#endif // MAC_OLD_HEADERS
#endif // USE_UMBRELLA_HEADERS
#endif // USE_QUICKTIME


class ASound;
class ASong;


class AAudioDevice
{
public:
  // Public member functions
  AAudioDevice();
  virtual ~AAudioDevice();
  void play(ASound *s,unsigned long pitch=0);
  void playLooped(ASound *s,unsigned long pitch=0);
  void playASync(ASound *s,unsigned long pitch=0);
  void playASyncLooped(ASound *s,unsigned long pitch=0);
  void play(ASong *s);
  void playLooped(ASong *s);
  void playASync(ASong *s);
  void playASyncLooped(ASong *s);
  void playTone(unsigned long pitch=440,unsigned int len=1000);  //freq
  void playNote(unsigned int key,unsigned int len=1000,unsigned int vel=127, unsigned int chan=0); // midi
  void setPatch(unsigned int num, unsigned int chan=0);
  bool verifyOkay(ASound *s);
  bool verifyOkay(ASong *s);
  void stop();
  unsigned long calcPlayingTime();
  bool isPlaying();
  void setVolume(unsigned long volume);
protected:
  // Private member functions
  void init();
  void open();
  unsigned int lookupKeyByFreq(unsigned long freq);
  unsigned int lookupFreqByKey(unsigned int key);
#ifdef ASYS_MAC
#if TARGET_RT_LITTLE_ENDIAN
  BigEndianFixed makeSureBigEndianFixed(unsigned long val);
  BigEndianShort makeSureBigEndianShort(unsigned short val);
  BigEndianLong makeSureBigEndianLong(unsigned long val);
  BigEndianOSType makeSureBigEndianOSType(unsigned long val);
#endif // TARGET_RT_LITTLE_ENDIAN
#endif // ASYS_MAC
  // Private data members
#ifdef ASYS_MAC
#if TARGET_RT_LITTLE_ENDIAN
  BigEndianFixed bf;  BigEndianShort bs;  BigEndianLong bl;  BigEndianOSType bo;
#endif // TARGET_RT_LITTLE_ENDIAN
#endif // ASYS_MAC
  bool stereo;
  bool doesMidi;
  bool doesWaves;
  unsigned int patch;
#ifdef ASYS_WINDOWS
  // This is only for Win16 where we link at run time...
#ifdef ASYS_WIN16
  HINSTANCE mmSystemHandle;
#endif // ASYS_WIN16
#ifndef ASYS_WIN32CE
#ifndef REALLY_OLD_WINDOWS
  HMIDIOUT midiHand;
#endif // REALLY_OLD_WINDOWS
#endif // ASYS_WIN32CE
#ifdef USE_DIRECTSOUND
  HINSTANCE dSoundHandle;
  LPDIRECTSOUND ddSound;
#endif // USE_DIRECTSOUND
#endif // ASYS_WINDOWS
#ifdef ASYS_MAC
#ifdef USE_QUICKTIME
#ifdef MAC_USE_QT_MUSIC
  NoteAllocator theNA;
  NoteChannel myNC;
  NoteRequest myNR;
#endif
#endif
#ifdef MAC_USE_SOUND_MANAGER
  SndCommand cmd;
  ExtSoundHeader hdr;
#endif
#endif // ASYS_MAC
};


#endif // ALIB_ASOUNDOBJECT_H

