#ifndef ALIB_ATHREAD_H
#define ALIB_ATHREAD_H


#include <guilib/ATypeDefs.h>


#ifdef ASYS_WINDOWS
#include <guilib/alib_win.h>
#endif // ASYS_WINDOWS


class AThread
{
public:
  AThread();
  ~AThread();
  void start();
  void stop();
protected:
};


#endif // ALIB_ATHREAD_H
