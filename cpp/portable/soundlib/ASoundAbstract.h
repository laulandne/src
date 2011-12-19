#ifndef ALIB_ASOUNDABSTRACT_H
#define ALIB_ASOUNDABSTRACT_H


#include <portable/nickcpp/AError.h>


#ifdef ASYS_MAC
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


class AAudioDevice;


class ASoundAbstract
{
public:
  // Public member functions
  ASoundAbstract();
  virtual ~ASoundAbstract();
  //
  char *getName() const { return name; }
  //
  AError err;
  //
protected:
  //
  // Private member functions
  virtual void init();
  //
  // Private data members
  char *name;
  bool okayDokey;
};


#endif // ALIB_ASOUNDABSTRACT_H

