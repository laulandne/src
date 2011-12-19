
//#define DEBUG_OUT std::cerr
#define DEBUG_OUT *aNullStream


#include <iostream>

#define FORCE_QUIT_EVENTS 1


#include <guilib/AApplicationLoop.h>
#include <guilib/AEvent.h>
#include <guilib/AWindowInputStream.h>


#ifdef ASYS_X11_BASED
#include <X11/keysym.h>
#include <X11/Xutil.h>
#include <guilib/alib_x11.h>
#endif // ASYS_X11_BASED

#ifdef ASYS_GNOME
#include <gtk/gtkmain.h>
#endif // ASYS_GNOME

#ifdef ASYS_DOS
#include <guilib/alib_dos.h>
#endif // ASYS_DOS

#ifdef ASYS_WINDOWS
#include <guilib/alib_win.h>
#endif // ASYS_DOS

#ifdef ASYS_OS2
#include <guilib/alib_os2.h>
#endif // ASYS_OS2

#ifdef ASYS_MAC
#include <guilib/alib_carbon.h>
#endif // ASYS_MAC

#ifdef ASYS_COCOA
#include <guilib/alib_cocoa.h>
#endif // ASYS_COCOA

#ifdef ASYS_AMIGA
#include <exec/tasks.h>
#include <intuition/intuition.h>
#include <clib/intuition_protos.h>
#include <clib/exec_protos.h>
#endif  // ASYS_AMIGA


AApplicationLoop *aDefaultAApplicationLoop=NULL;


////////////////////////////////////////////////////////////////////////////////
//  AApplicationLoop Class
////////////////////////////////////////////////////////////////////////////////

AApplicationLoop::AApplicationLoop()
{
  DEBUG_OUT<<"AApplicationLoop::AApplicationLoop()\n";
  init();
  aDefaultAApplicationLoop=this;
  theWin=aDefaultAWindow;
  if(!theWin) {
    std::cerr<<"aDefaultAWindow NULL win!\n";
    exit(5);
  }
}


AApplicationLoop::AApplicationLoop(AWindow *win)
{
  DEBUG_OUT<<"AApplicationLoop::AApplicationLoop(win)\n";
  init();
  theWin=win;
  aDefaultAApplicationLoop=this;
  if(!theWin) {
    std::cerr<<"Passed NULL win!\n";
    exit(5);
  }
}


void AApplicationLoop::init()
{
  nothingRegistered=true;
  needFakeEvent=true;
  theWin=NULL;
  unsigned int t;
  for(t=0;t<256;t++) keyAssigned[t]=false;
  for(t=0;t<A_EVENT_LAST;t++) eventHandlerType[t]=AAPPLICATIONLOOP_NONE;
  defaultEventFunc=alib_globalhandleDefaultEvent;
  keyboardFunction=NULL;
  mouseFunction=NULL;
  motionFunction=NULL;
  resizeFunction=NULL;
  redrawFunction=NULL;
  focusFunction=NULL;
}


AApplicationLoop::~AApplicationLoop()
{
}


void AApplicationLoop::associateWithWindow(AWindow *w)
{
  if(!w) return;
  theWin=w;
  if(theWin->err.error()) {
    DEBUG_OUT<<"AWindow has an error!  Giving up!\n";
    err.set();
    return;
  }
}


void AApplicationLoop::associateWithWindow(AWindowInputStream *iw)
{
  if(!iw) return;
  associateWithWindow(iw->getWindow());
}


// TODO: what if you just want to generally catch keypresses?
void AApplicationLoop::loopOnceWait()
{
  if(err.error()) { DEBUG_OUT<<"We've got an error, exiting!\n"; exit(5); }
  if(alib_curEvent) { if(alib_curEvent->getQuitASAP()) exit(0); }
  if(!theWin) { DEBUG_OUT<<"AApplicationLoop can't loop, no theWin!\n"; err.set(); return; }
  DEBUG_OUT<<"AApplicationLoop::loopOnceWait...going to wait...\n";
  if(!alib_curEvent) alib_curEvent=new AEvent();
  alib_curEvent->wait(theWin);
  DEBUG_OUT<<"AApplicationLoop::loopOnceWait back from wait.\n";
  dispatchEvent(alib_curEvent);
}


// TODO: what if you just want to generally catch keypresses?
void AApplicationLoop::loopOnce()
{
  if(err.error()) { DEBUG_OUT<<"We've got an error, exiting!\n"; exit(5); }
  if(alib_curEvent) { if(alib_curEvent->getQuitASAP()) exit(0); }
  if(!theWin) { DEBUG_OUT<<"AApplicationLoop can't loop, no theWin!\n"; err.set(); return; }
  DEBUG_OUT<<"AApplicationLoop::loopOnce...going to <<...\n";
  if(!alib_curEvent) alib_curEvent=new AEvent();
  *alib_curEvent<<theWin;
  DEBUG_OUT<<"AApplicationLoop::loopOnce back from wait.\n";
  dispatchEvent(alib_curEvent);
}


// TODO: Why do we muck with alib_curEvent, if we have theEvent?!?
bool AApplicationLoop::dispatchEvent(AEvent *theEvent)
{
  DEBUG_OUT<<"AApplicationLoop::dispatchEvent (type="<<theEvent->debugEventType()<<")...\n";
  if(nothingRegistered) {
    DEBUG_OUT<<"dispatchEvent, nothingRegistered...\n";
    if(!alib_curEvent) {
      alib_curEvent=new AEvent;
      DEBUG_OUT<<"(created new alib_curEvent)\n";
    }
    if(alib_curEvent) alib_curEvent->becomeCloneOfThisEvent(theEvent);
    defaultEventFunc(theEvent);
    return true;
  }
  bool handled=false;
  if(theEvent->type!=A_EVENT_NONE) {
    switch(theEvent->type) {
      case A_EVENT_KEYUP:
        if(theEvent->code<256) {
          DEBUG_OUT<<"(Keypress code: "<<theEvent->code<<")\n";
          if(keyAssigned[(int)theEvent->code]) { (*keyFunctions[(int)theEvent->code])(theEvent); handled=true; }
        }
        break;
      default:
        break;
    }
  }
  if(!handled) {
    if(theEvent->type<A_EVENT_LAST) {
      if(eventHandlerType[theEvent->type]) {
        if(eventHandlerType[theEvent->type]==AAPPLICATIONLOOP_AEVENT) (*eventFunctions[theEvent->type])(theEvent);
        else  {
          // TODO: Crack event and pass correct parameters to event handlers!
          switch(theEvent->type) {
            case A_EVENT_FOCUS:
              (*focusFunction)();
              break;
            case A_EVENT_REDRAW:
              (*redrawFunction)();
              break;
            case A_EVENT_RESIZE:
              (*resizeFunction)(aDefaultAWindow->getWidth(),aDefaultAWindow->getHeight());
              break;
            case A_EVENT_KEYUP:
              (*keyboardFunction)(theEvent->code,theEvent->x,theEvent->y);
              break;
            case A_EVENT_MOUSEUP:
              // TODO: Check params are correct
              (*mouseFunction)(theEvent->code,0,theEvent->x,theEvent->y);
              break;
            case A_EVENT_MOUSEMOVE:
              (*motionFunction)(theEvent->x,theEvent->y);
              break;
            default:
              std::cerr<<"Unknown special event handler!\n";
              exit(5);
              break;
          }
        }
      }
      else defaultEventFunc(theEvent);
      handled=true;
    }
  }
  return handled;
}


void alib_globalhandleDefaultEvent(AEvent *theEvent)
{
  if(alib_curEvent) {
    if(aDefaultAApplicationLoop) {
      aDefaultAApplicationLoop->handleDefaultEvent(theEvent);
    }
    else DEBUG_OUT<<"alib_globalhandleDefaultEvent lost event...no alib_curEvent->defaultLoop!\n";
  }
  else DEBUG_OUT<<"alib_globalhandleDefaultEvent lost event...no alib_curEvent!\n";
}


bool AApplicationLoop::handleDefaultEvent(AEvent *theEvent)
{
  DEBUG_OUT<<"AApplicationLoop::handleDefaultEvent (type="<<theEvent->debugEventType()<<")...\n";
  if(theEvent->getQuitASAP()) exit(0);  // The app didn't handle it, so we will.
  bool ret=false;
  switch(theEvent->type) {
    case A_EVENT_FOCUS:
      if(theWin) theWin->reallyFocus();
      else DEBUG_OUT<<"theWin was NULL\n";
      ret=true;
      break;
    case A_EVENT_DESTROY:
      theEvent->setQuitASAP();
      DEBUG_OUT<<"Setting quitASAP!\n";
      // Set this and give the app a chance to handle it...but we'll exit if we find it ourselves
      ret=true;
      break;
    default:
      DEBUG_OUT<<"Ignoring unregistered event type "<<(int)theEvent->type<<" ("<<theEvent->debugEventType()<<")\n";
      break;
  }
  return ret;
}


void AApplicationLoop::runOnce()
{
  DEBUG_OUT<<"AApplicationLoop::runOnce...\n";
  if(err.error()) { DEBUG_OUT<<"We've got an error, exiting!\n"; exit(5); }
  if(alib_curEvent) { if(alib_curEvent->getQuitASAP()) exit(0); }
  if(!theWin) { DEBUG_OUT<<"AApplicationLoop can't runOnce, no theWin!\n"; err.set(); return; }
  if(needFakeEvent) {
    DEBUG_OUT<<"(runOnce is creating a fake event to prime the pump...)\n";
    if(!alib_curEvent) alib_curEvent=new AEvent;
    alib_curEvent->type=A_EVENT_REDRAW;
    alib_curEvent->setWindow(theWin);
    dispatchEvent(alib_curEvent);
    needFakeEvent=false;
  }
  loopOnceWait();
}


void AApplicationLoop::runOnceNoWait()
{
  DEBUG_OUT<<"AApplicationLoop::runOnceNoWait...\n";
  if(err.error()) { DEBUG_OUT<<"We've got an error, exiting!\n"; exit(5); }
  if(alib_curEvent) { if(alib_curEvent->getQuitASAP()) exit(0); }
  if(!theWin) { DEBUG_OUT<<"AApplicationLoop can't runOnceNoWait, no theWin!\n"; err.set(); return; }
  if(needFakeEvent) {
    DEBUG_OUT<<"(runOnce is creating a fake event to prime the pump...)\n";
    if(!alib_curEvent) alib_curEvent=new AEvent;
    alib_curEvent->type=A_EVENT_REDRAW;
    alib_curEvent->setWindow(theWin);
    dispatchEvent(alib_curEvent);
	  needFakeEvent=false;
  }
  loopOnce();
}


void AApplicationLoop::run()
{
  DEBUG_OUT<<"AApplicationLoop::run...\n";
  if(err.error()) { DEBUG_OUT<<"We've got an error, exiting!\n"; exit(5); }
  if(alib_curEvent) { if(alib_curEvent->getQuitASAP()) exit(0); }
  if(!theWin) { DEBUG_OUT<<"AApplicationLoop can't run, no theWin!\n"; err.set(); return; }
  bool working=true;
  while(working) {
    runOnce();
  }
}


void AApplicationLoop::regKeypress(char key, void (*funToCall)(AEvent *))
{
  unsigned int offset=key;
  if(offset<256) {
    keyAssigned[offset]=true;
    keyFunctions[offset]=funToCall;
    nothingRegistered=false;
  }
}


void AApplicationLoop::regEvent(AEventType etype, void (*funToCall)(AEvent *))
{
  if(etype<A_EVENT_LAST) {
    eventHandlerType[etype]=AAPPLICATIONLOOP_AEVENT;
    eventFunctions[etype]=funToCall;
    nothingRegistered=false;
  }
}


// TODO: Handle keydown too!
void AApplicationLoop::regEventKeyboard(void (*funToCall)(unsigned char,int,int))
{
  eventHandlerType[A_EVENT_KEYUP]=AAPPLICATIONLOOP_SPECIAL;
  keyboardFunction=funToCall;
  nothingRegistered=false;
}


// TODO: handle other mouse events too!
void AApplicationLoop::regEventMouse(void (*funToCall)(int,int,int,int))
{
  eventHandlerType[A_EVENT_MOUSEUP]=AAPPLICATIONLOOP_SPECIAL;
  mouseFunction=funToCall;
  nothingRegistered=false;
}


void AApplicationLoop::regEventResize(void (*funToCall)(int,int))
{
  eventHandlerType[A_EVENT_RESIZE]=AAPPLICATIONLOOP_SPECIAL;
  resizeFunction=funToCall;
  nothingRegistered=false;
}


void AApplicationLoop::regEventMotion(void (*funToCall)(int,int))
{
  eventHandlerType[A_EVENT_MOUSEMOVE]=AAPPLICATIONLOOP_SPECIAL;
  motionFunction=funToCall;
  nothingRegistered=false;
}


void AApplicationLoop::regEventRedraw(void (*funToCall)())
{
  eventHandlerType[A_EVENT_REDRAW]=AAPPLICATIONLOOP_SPECIAL;
  redrawFunction=funToCall;
  nothingRegistered=false;
}


// TODO: Handle unfocus too!
void AApplicationLoop::regEventFocus(void (*funToCall)())
{
  eventHandlerType[A_EVENT_FOCUS]=AAPPLICATIONLOOP_SPECIAL;
  focusFunction=funToCall;
  nothingRegistered=false;
}


void AApplicationLoop::regDefaultEvent(void (*funToCall)(AEvent *))
{
  defaultEventFunc=funToCall;
  nothingRegistered=false;
}


bool AApplicationLoop::getQuitASAP()
{
  if(!alib_curEvent) return alib_curEvent->getQuitASAP();
  else return false;
}


void AApplicationLoop::setQuitASAP()
{
  if(!alib_curEvent) alib_curEvent=new AEvent;
  alib_curEvent->setQuitASAP();
}
