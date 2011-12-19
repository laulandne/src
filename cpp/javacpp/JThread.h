#ifndef JAVAGLUE_JTHREAD_H
#define JAVAGLUE_JTHREAD_H


#include <javacpp/javacpp.h>


#ifdef NICK_USE_GUILIB
#include <guilib/AThread.h>
#endif // NICK_USE_GUILIB


const int JThread_MIN_PRIORITY=1;


class JApplet;


class JThread {
public:
  JThread(JApplet *a);
  virtual ~JThread();
  void setPriority(unsigned int p);
  void start();
  void stop();
  static JThread *currentThread();
protected:
  unsigned int state;
  unsigned int priority;
  JApplet *myApplet;
#ifdef NICK_USE_GUILIB
  AThread *thread;
#endif // NICK_USE_GUILIB
};


#endif // JAVAGLUE_JTHREAD_H

