#ifndef GUILIB_AAPPLICATIONLOOP_H
#define GUILIB_AAPPLICATIONLOOP_H


#include <guilib/ATypeDefs.h>

#include <portable/nickcpp/AError.h>

#include "AEvent.h"


class AWindow;
class AWindowInputStream;
class ARegion;


// Event handler types
#define AAPPLICATIONLOOP_NONE 0
#define AAPPLICATIONLOOP_AEVENT 1
#define AAPPLICATIONLOOP_SPECIAL 2


// Global default event handler
void alib_globalhandleDefaultEvent(AEvent *theEvent);


class AApplicationLoop {
public:
  AApplicationLoop();
  AApplicationLoop(AWindow *win);
  void associateWithWindow(AWindow *w);
  void associateWithWindow(AWindowInputStream *iw);
  virtual ~AApplicationLoop();
  //
  // check events, returning if none, dispatching to registered event
  //   handlers as they occur.
  void loopOnce();
  void loopOnceWait();
  // Repeatably check/dispatch events to handlers and then loop...
  void runOnceNoWait();
  void runOnce();
  void run();
  //
  // Register an event handler for a keypress, key is ASCII
  void regKeypress(char key, void (*funToCall)(AEvent *));
  // Register an event handler for an arbitrary event type
  // NOTE: Do NOT assume any events of this type will occur.
  void regEvent(AEventType etype, void (*funToCall)(AEvent *));
  void regEventKeyboard(void (*funToCall)(unsigned char,int,int));
  void regEventMouse(void (*funToCall)(int,int,int,int));
  void regEventMotion(void (*funToCall)(int,int));
  void regEventResize(void (*funToCall)(int,int));
  void regEventRedraw(void (*funToCall)());
  void regEventFocus(void (*funToCall)());
  void regDefaultEvent(void (*funToCall)(AEvent *));  // This one'll handle ALL otherwise unhandled events
  //
  bool dispatchEvent(AEvent *theEvent);
  bool handleDefaultEvent(AEvent *theEvent);
  //
  bool getQuitASAP(); // Don't trust this
  void setQuitASAP(); // This won't always be handled!
  //
  AError err;
protected:
  void init();
  //
  AWindow *theWin;
  bool needFakeEvent;  // TODO: Is this really needed?
  bool nothingRegistered;  // TODO: Is this really needed?
  // Currently we only support 8-bit ASCII character code for keys
  bool keyAssigned[256];
  unsigned int eventHandlerType[A_EVENT_LAST];
  void (*keyFunctions[256])(AEvent *);
  void (*eventFunctions[A_EVENT_LAST])(AEvent *);
  void (*defaultEventFunc)(AEvent *);
  // The following are for the new-style (ala GLUT) handlers and may all be NULL
  void (*keyboardFunction)(unsigned char,int,int);
  void (*mouseFunction)(int,int,int,int);
  void (*motionFunction)(int,int);
  void (*resizeFunction)(int,int);
  void (*redrawFunction)();
  void (*focusFunction)();
};


extern AApplicationLoop *aDefaultAApplicationLoop;


#endif // GUILIB_AAPPLICATIONLOOP_H
