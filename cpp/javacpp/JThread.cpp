
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#include <portable/nickcpp/ANullStream.h>


#include <javacpp/JThread.h>
#include <javacpp/JApplet.h>


extern JApplet *aDefaultApplet;


JThread *aDefaultJThread;


////////////////////////////////////////////////////////////////////////////////
//  JThread Class
////////////////////////////////////////////////////////////////////////////////

JThread::JThread(JApplet *a)
{
  DEBUG_OUT<<"JThread::JThread()\n";
  myApplet=a;
  if(!myApplet) DEBUG_OUT<<"NOTE: myApplet is NULL\n";
  priority=0;
#ifdef NICK_USE_GUILIB
  thread=new AThread();
#endif // NICK_USE_GUILIB
  aDefaultJThread=this;
  state=JAVACPP_STATE_INITED;
}


JThread::~JThread()
{
#ifdef NICK_USE_GUILIB
  if(thread) delete thread;
  thread=NULL;
#endif // NICK_USE_GUILIB
}


/* STATIC */
JThread *JThread::currentThread()
{
  return aDefaultJThread;
}


void JThread::setPriority(unsigned int p)
{
  DEBUG_OUT<<"JThread::setPriority("<<p<<")\n";
  priority=p;
}


void JThread::start()
{
  DEBUG_OUT<<"JThread::start()\n";
  state=JAVACPP_STATE_STARTED;
  if(!myApplet) { DEBUG_OUT<<"Can't run, no Applet!\n"; return; }
#ifdef NICK_USE_GUILIB
  if(thread) thread->start();
#endif // NICK_USE_GUILIB
}


void JThread::stop()
{
  DEBUG_OUT<<"JThread::stop()\n";
  state=JAVACPP_STATE_STOPPED;
#ifdef NICK_USE_GUILIB
  if(thread) thread->stop();
#endif // NICK_USE_GUILIB
}




