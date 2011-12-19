
//#define DEBUG_OUT std::cerr
#define DEBUG_OUT *aNullStream


#include <iostream>

#include "AClock.h"


#ifdef ALIB_HAVE_TIME_H
#include <time.h>
#endif // ALIB_HAVE_TIME_H


#ifdef ASYS_MAC
#ifdef ALIB_NO_PCH
//#include <guilib/alib_carbon.h>
#endif // ALIB_NO_PCH
#endif // ASYS_MAC

#ifdef ASYS_WIN32
//#include <guilib/alib_win.h>
#endif // ASYS_WIN32


////////////////////////////////////////////////////////////////////////////////
//  AClock Class
////////////////////////////////////////////////////////////////////////////////

unsigned long AClock::read()
{
  unsigned long ret=0;
#ifdef ALIB_HAVE_CLOCK
  //ret=(unsigned long)::clock();
#endif // ALIB_HAVE_CLOCK
  return ret;
}


unsigned long AClock::getTick()
{
  unsigned long ret=0;
#ifdef ALIB_HAVE_CLOCK
  //ret=(unsigned long)::clock();
#endif // ALIB_HAVE_CLOCK
#ifdef ASYS_MAC
  ret=TickCount();
#endif // ASYS_MAC
#ifdef ASYS_WIN32
#ifndef ASYS_WIN386
  LARGE_INTEGER li;
  bool err=(bool)QueryPerformanceCounter(&li);
#ifdef __WATCOMC__
  unsigned int high=li.u.HighPart,low=li.u.LowPart;
#else
  unsigned int high=li.HighPart,low=li.LowPart;
#endif // __WATCOMC__
  //DEBUG_OUT<<"QueryPerformanceCounter: "<<high<<","<<low<<"\n";
  ret=low;
#endif // ASYS_WIN386
#endif // ASYS_WIN32
  return ret;
}


unsigned long AClock::getMilli()
{
  unsigned long ret=0;
#ifdef ALIB_HAVE_CLOCK
  //ret=(unsigned long)::clock();
#endif // ALIB_HAVE_CLOCK
#ifdef ASYS_MAC
  UnsignedWide Microsecs;
  Microseconds(&Microsecs);
  double Time;
  Time=(double)Microsecs.hi*(double)0xFFFFFFFF;
  Time=Time+(double)Microsecs.lo;
  Time=Time/1000.0;
  while(Time>(double)0xFFFFFFFF) Time=Time-(double)0xFFFFFFFF;
  ret=(unsigned long)Time;
#endif // ASYS_MAC
#ifdef ASYS_WINDOWS
  //ret=timeGetTime();
  ret=GetTickCount();
#endif // ASYS_WINDOWS
  return ret;
}


unsigned long AClock::getFreq()
{
  unsigned long freq=60;
#ifdef ASYS_WIN32
#ifndef ASYS_WIN386
  LARGE_INTEGER li;
  bool err=(bool)QueryPerformanceFrequency(&li);
#ifdef __WATCOMC__
  unsigned int high=li.u.HighPart,low=li.u.LowPart;
#else
  unsigned int high=li.HighPart,low=li.LowPart;
#endif // __WATCOMC__
  //DEBUG_OUT<<"QueryPerformanceFrequency: "<<high<<","<<low<<"\n";
  freq=low;
#endif // ASYS_WIN386
#endif // ASYS_WIN32
  return freq;
}
