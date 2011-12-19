
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#include <iostream>

#define FORCE_QUIT_EVENTS 1


#include <guilib/AEvent.h>
#include <guilib/AWindowAbstract.h>
#include <guilib/ADisplay.h>

#include <portable/nickcpp/ANullStream.h>


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

#ifdef ASYS_GLUT
#include <guilib/alib_glut.h>
#endif // ASYS_GLUT

#ifdef ASYS_OS2
#include <guilib/alib_os2.h>
#endif // ASYS_OS2

#ifdef ASYS_MAC
#include <guilib/alib_carbon.h>
#endif // ASYS_MAC

#ifdef ASYS_COCOA
#include <guilib/alib_cocoa.h>
#include <guilib/AApp.h>
#endif // ASYS_COCOA

#ifdef ASYS_AMIGA
#include <exec/tasks.h>
#include <intuition/intuition.h>
#include <clib/intuition_protos.h>
#include <clib/exec_protos.h>
#endif  // ASYS_AMIGA


AEvent *alib_curEvent=NULL;


////////////////////////////////////////////////////////////////////////////////
//  AEvent Class
////////////////////////////////////////////////////////////////////////////////

bool AEvent::quitASAP=false;
#ifdef ASYS_WINDOWS
//bool AEvent::sync=false;
#endif // ASYS_WINDOWS


AEvent::AEvent(bool debugFlag)
{
  init((AWindowAbstract *)NULL);
  //debug=debugFlag;
  //if(debug) DEBUG_OUT<<"AEvent created with debug flag set!\n";
}


void AEvent::init(AWindowAbstract *theWindow)
{
  type=A_EVENT_NONE;
  x=0;  y=0;  code=0;  modifier=0;  number=0;
  //data=(char *)NULL;
  theWin=(AWindowAbstract *)NULL;
  //theRegion=(ARegion *)NULL;
  nullWinFlag=false;
  if(!theWindow) {
    //DEBUG_OUT.Warning("!  theWindow is NULL!\n";
    //theWin=aThisApp.lastWindow;
    nullWinFlag=true;
  }
  else theWin=theWindow;
  shouldEcho=true;
  // we can't reset this here
  //debug=false;
#ifdef ASYS_AMIGA
  appsig=0;  winsig=0;
#endif // ASYS_AMIGA
#ifdef ASYS_X11
  x11type=0;
#endif // ASYS_X11
}


AEvent::~AEvent()
{
  type=A_EVENT_NONE;
  x=0;  y=0;  code=0;  modifier=0;  number=0;
  //data=(char *)NULL;
  theWin=(AWindowAbstract *)NULL;
  nullWinFlag=false;
  shouldEcho=false;
  //debug=false;
}


void AEvent::becomeCloneOfThisEvent(AEvent *e)
{
  type=e->type;
  x=e->x;  y=e->y;  code=e->code;  modifier=e->modifier;  number=e->number;
  theWin=e->theWin;
  //theRegion=e->theRegion;
  nullWinFlag=e->nullWinFlag;  shouldEcho=e->shouldEcho;
}


void AEvent::operator<<(AWindowAbstract *theWindow)
{
  DEBUG_OUT<<"In AEvent::operator<<...\n";
  init(theWindow);
#ifdef ASYS_PALM
  alib_curEvent=this;
  palmAppEventLoopOnce();
  //DEBUG_OUT<<"AEvent was "<<type<<"\n";
  return;
#endif // ASYS_PALM
#ifdef ASYS_DOS
  // FIXME: Could handle more event types for DOS
  // Keyboard first
  if(dosMyKbhit()) {
    type=A_EVENT_KEYPRESS;
    code=getch();
    if(aThisApp.extendedKey) {
      aThisApp.extendedKey=false;
      code=dosHandleExtended(code);
    }
    if(!code) { aThisApp.extendedKey=true; type=A_EVENT_NONE; }
    if(code==27) type=A_EVENT_DESTROY;  // Esc key
    if(code==' ') {
      if(aThisApp.joyNotKeys) { number=aThisApp.whichJoy; code=A_KEY_JOY_FIRE; }
    }
  }
  REGS16 rin,rout;
  ROUT_W_BX=0;
  // Then mouse
  if(aThisApp.hasMouse) {
    RIN_W_AX=0x03;
    int86(0x33,&rin,&rout);
  }
  if(ROUT_W_BX) type=A_EVENT_MOUSEUP;
#ifdef USE_JOYSTICK
  // This only reads the first joystick
  unsigned long c=dosJoystick(0);
  if(c) {
    type=A_EVENT_KEYPRESS;  code=c;
    DEBUG_OUT<<"<<: "<<(int)c<<"\n";
    //cout<<"<<: "<<(int)c<<"\n";
  }
#endif // USE_JOYSTICK
  return;
#endif // ASYS_DOS
#ifdef ASYS_OS2
  alib_curEvent=this;
  sync=false;
  bool ready=WinPeekMsg(aThisApp.ahAB,&aThisApp.aQMsg,0L,0,0,PM_REMOVE);
  if(ready) {
    WinDispatchMsg(aThisApp.ahAB,&aThisApp.aQMsg);
    if(sync) theWin->sync();
  }
  return;
#endif // ASYS_OS2
#ifdef ASYS_WINDOWS
  alib_curEvent=this;
  MSG msg;
  //sync=false;
  unsigned int ready=PeekMessage(&msg,(HWND)NULLHANDLE,0,0,PM_REMOVE);
  if(ready) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
    //if(sync) theWin->sync();
  }
  alib_awParam=msg.wParam;
  return;
#endif // ASYS_WINDOWS
#ifdef ASYS_AMIGA
  // FIXME: Need to handle ^C and other events
  type=A_EVENT_NONE;
  Task *thisTask=FindTask(NULL);
  winsig=1L<<theWin->getAmyWindow()->UserPort->mp_SigBit;
  appsig=theWin->getAppSignal();
  unsigned long signals=thisTask->tc_SigRecvd;
  amigaHandleEvent(signals);
  return;
#endif // ASYS_AMIGA
#ifdef ASYS_MAC
#ifdef USE_CARBON_EVENTS
  alib_curEvent=this;
  DEBUG_OUT<<"Going to call RunApplicationEventLoop...\n";
#if __LP64__
  RunCurrentEventLoop(kEventDurationForever);
#else
  RunApplicationEventLoop();
#endif
  DEBUG_OUT<<"Back from RunApplicationEventLoop.\n";
#else
  EventRecord event;
#ifdef USE_QUARTZ
#else
  MacSetPort(theWin->getMacGrafPtr());
#endif // USE_QUARTZ
  if(EventAvail(everyEvent,&event)) {
    WaitNextEvent(everyEvent,&event,0,(RgnHandle)nil);
#ifdef DEBUG_VERBOSE
    //DEBUG_OUT<<"mac << native event type "<<event.what<<" ("<<debugMacEventType(event.what)<<")\n";
#endif
    macHandleEvent(event);
  }
  // FIXME: I think I commented this out because it slowed us down
  //else SystemTask();
#endif // USE_CARBON_EVENTS
  return;
#endif // ASYS_MAC
#ifdef ASYS_X11
  type=A_EVENT_NONE;
  XEvent xe;
  if(theWin) {
    if(theWin->getParent()) {
      if(theWin->getParent()->getDisplay()) {
        XFlush(theWin->getParent()->getDisplay());
      }
    }
  }
  bool ret=false;
  bool w=true;
  while(w) {
    w=false;
#ifdef USE_JOYSTICK
    ret=!linuxJoystick();
#else
    ret=true;
#endif // USE_JOYSTICK
    //bool ret=XCheckMaskEvent(theWin->getParent()->getDisplay(),-1,&xe);
    if(ret) ret=XPending(theWin->getParent()->getDisplay());
    if(ret) {
      XNextEvent(theWin->getParent()->getDisplay(),&xe);
      x11_handle_event(&xe);
    }
  }
  return;
#endif // ASYS_X11
#ifdef ASYS_GNOME
  alib_curEvent=this;
  DEBUG_OUT<<"Going to call gtk_main...\n";
  gtk_main();
  DEBUG_OUT<<"Back from gtk_main.\n";
  return;
#endif // ASYS_GNOME
#ifdef ASYS_COCOA
  alib_curEvent=this;
  DEBUG_OUT<<"Going to call cocoaMain...\n";
  cocoaMain(aThisApp.argc,(char **)aThisApp.argv);
  DEBUG_OUT<<"Back from cocoaMain.\n";
  return;
#endif // ASYS_COCOA
#ifdef ASYS_GLUT
  DEBUG_OUT<<"Going to call glutMainLoop...\n";
  glutMainLoop();
  DEBUG_OUT<<"Back from glutMainLoop.\n";
#endif // ASYS_GLUT
  DEBUG_OUT<<"FELL THROUGH TO DEFAULT EVENT HANDLER (<<)\n";
  // FIXME: This will actually wait until a keypress...
  type=A_EVENT_NONE;
  code=fgetc(stdin);
  //cout<<"AEvent wait code was "<<code<<"\n";
  switch(code) {
    case 10:
      code=A_KEY_NEWLINE;
      type=A_EVENT_KEYUP;
      break;
    case 27:
      type=A_EVENT_DESTROY;
      break;
    default:
      type=A_EVENT_KEYUP;
      DEBUG_OUT<<"(Keypress code: "<<code<<")\n";
      break;
  }
}


void AEvent::wait(AWindowAbstract *theWindow)
{
  DEBUG_OUT<<"In AEvent::wait...\n";
  init(theWindow);
#ifdef ASYS_PALM
  alib_curEvent=this;
  palmAppEventLoopOnce();
  //DEBUG_OUT<<"AEvent was "<<type<<"\n";
  return;
#endif // ASYS_PALM
#ifdef ASYS_DOS
  // FIXME: Could handle more event types for DOS
  REGS16 rin,rout;
  bool waiting=true;
  while(waiting) {
    if(dosMyKbhit()) {
      type=A_EVENT_KEYPRESS;
      code=getch();
      if(aThisApp.extendedKey) {
        aThisApp.extendedKey=false;
        code=dosHandleExtended(code);
      }
      if(!code) { aThisApp.extendedKey=true; type=A_EVENT_NONE; }
      if(code==27) { type=A_EVENT_DESTROY; }  // Esc key
      if(code==' ') {
        if(aThisApp.joyNotKeys) { number=aThisApp.whichJoy; code=A_KEY_JOY_FIRE; }
      }
      waiting=false;
    }
    ROUT_W_BX=0;
    if(aThisApp.hasMouse) { RIN_W_AX=0x03;  int86(0x33,&rin,&rout); }
    if(ROUT_W_BX) waiting=false;
  }
  x=ROUT_W_CX; y=ROUT_W_DX;
  unsigned int h=ROUT_W_BX;
  // Correct X coord for lo-res screens
  if(theWin->getWidth()<512) x=x/2;
  rin.h.ah=0x02;  int86(0x16,&rin,&rout);
  if(rout.h.al&3) modifier|=A_MOD_SHIFT;
  if(rout.h.al&4) modifier|=A_MOD_CONTROL;
  if(h) { type=A_EVENT_MOUSEUP; code=0; }
  return;
#endif // ASYS_DOS
#ifdef ASYS_WINDOWS
  alib_curEvent=this;
  MSG msg;
  bool waiting=true;
  //sync=false;
  while(waiting) {
    int val=GetMessage(&msg,(HWND)NULLHANDLE,0,0);
    TranslateMessage(&msg);
    DispatchMessage(&msg);
    if(!val) waiting=false;
    if(type!=A_EVENT_NONE) waiting=false;
  }
  //if(sync) theWin->sync();
  alib_awParam=msg.wParam;
  return;
#endif // ASYS_WINDOWS
#ifdef ASYS_OS2
  alib_curEvent=this;
  sync=false;
  bool waiting=true;
  while(waiting) {
    int val=WinGetMsg(aThisApp.ahAB,&aThisApp.aQMsg,0L,0,0);
    WinDispatchMsg(aThisApp.ahAB,&aThisApp.aQMsg);
    if(!val) waiting=false;
    if(alib_curEvent->type!=A_EVENT_NONE) waiting=false;
  }
  if(sync) theWin->sync();
  return;
#endif // ASYS_OS2
#ifdef ASYS_AMIGA
  // FIXME: Need to handle ^C and other events
  // FIXME: Bug, this won't stop waiting for a joystick move!
  // (We read the hardware directly instead of using an actual event)
  bool waiting=true;
  type=A_EVENT_NONE;
  winsig=1L<<theWin->getAmyWindow()->UserPort->mp_SigBit;
  appsig=theWin->getAppSignal();
  while(waiting) {
    unsigned long signals=Wait(winsig|appsig);
    //WaitPort(theWin->getAmyWindow()->UserPort);
    waiting=amigaHandleEvent(signals);
  }  // while(waiting)
  return;
#endif // ASYS_AMIGA
#ifdef ASYS_MAC
#ifdef USE_CARBON_EVENTS
  alib_curEvent=this;
  DEBUG_OUT<<"Going to call RunApplicationEventLoop...\n";
#if __LP64__
  RunCurrentEventLoop(kEventDurationForever);
#else
  RunApplicationEventLoop();
#endif
  DEBUG_OUT<<"Back from RunApplicationEventLoop.\n";
#else
  EventRecord event;
#ifdef USE_QUARTZ
#else
  MacSetPort(theWin->getMacGrafPtr());
#endif // USE_QUARTZ
  // Idle until window manager says there's an event for our process
  unsigned long limit=GetCaretTime();
  while(!WaitNextEvent(everyEvent,&event,limit,(RgnHandle)nil)) {
    aThisApp.yieldToOtherProcesses();
  }
  //DEBUG_OUT<<"mac wait native event type "<<event.what<<"\n";
  macHandleEvent(event);
#endif // USE_CARBON_EVENTS
  return;
#endif // ASYS_MAC
#ifdef ASYS_X11
  type=A_EVENT_NONE;
  XEvent xe;
  //XSync(theWin->getParent()->getDisplay(),false);
  if(theWin) {
    if(theWin->getParent()) {
      if(theWin->getParent()->getDisplay()) {
        XFlush(theWin->getParent()->getDisplay());
        XNextEvent(theWin->getParent()->getDisplay(),&xe);
      }
    }
  }
  x11_handle_event(&xe);
  return;
#endif // ASYS_X11
#ifdef ASYS_GNOME
  alib_curEvent=this;
  DEBUG_OUT<<"Going to call gtk_main...\n";
  gtk_main();
  DEBUG_OUT<<"Back from gtk_main.\n";
  return;
#endif // ASYS_GNOME
#ifdef ASYS_COCOA
  alib_curEvent=this;
  DEBUG_OUT<<"Going to call cocoaMain...\n";
  cocoaMain(aThisApp.argc,(char **)aThisApp.argv);
  DEBUG_OUT<<"Back from cocoaMain.\n";
  return;
#endif // ASYS_COCOA
#ifdef ASYS_GLUT
  DEBUG_OUT<<"Going to call glutMainLoop...\n";
  glutMainLoop();
  DEBUG_OUT<<"Back from glutMainLoop.\n";
#endif // ASYS_GLUT
  //
  std::cerr<<"FELL THROUGH TO DEFAULT EVENT HANDLER (wait)\n";
  std::cerr<<"You probably forgot to define an ASYS_*...\n";
  exit(5);
  //
  type=A_EVENT_NONE;
  code=fgetc(stdin);
  //cout<<"AEvent wait code was "<<code<<"\n";
  switch(code) {
    case 10:
      code=A_KEY_NEWLINE;
      type=A_EVENT_KEYUP;
      break;
    case 27:
      type=A_EVENT_DESTROY;
      break;
    default:
      type=A_EVENT_KEYUP;
      DEBUG_OUT<<"(Keypress code: "<<code<<")\n";
      break;
  }
}


// NOTE: where is a pointer into the string "ptr"
// and size is the leng of ptr's buffer
bool AEvent::input_string(char *ptr, unsigned int *where, unsigned int size)
{
  char tc[2];
  if(type!=A_EVENT_KEYUP) return false;
  if(*where>=size) return false;
  //if(*where<0) *where=0;
  handleModKeys();
  tc[1]=0;
  if(code==0x0a) {
    ptr[*where]=0;
    if(shouldEcho) theWin->eraseCursor();
    return true;
  }
  else
  if(code==A_KEY_NEWLINE) {
    ptr[*where]=0;
    if(shouldEcho) theWin->eraseCursor();
    return true;
  }
  else
  if(code==A_KEY_BACKSPACE) {
    if(*where) {
      if(shouldEcho) { theWin->eraseCursor(); *theWin<<(char)A_KEY_BACKSPACE; }
      (*where)--;
    }
  }
  else {
    tc[0]=(char)code;
    if(shouldEcho) { theWin->eraseCursor(); *theWin<<tc; }
    if(code<32) {
      // Control codes
      if(shouldEcho) { theWin->eraseCursor(); *theWin<<(int)code; }
    }
    ptr[*where]=(char)code;  (*where)++;
  }
  if(shouldEcho) theWin->drawCursor();
  return false;
}


void AEvent::handleModKeys()
{
  // if there are mod keys that match keys we know, modify the code
  // ex: code==2 and mod==A_MOD_SHIFT, we change code to an @ sign
  // FIXME: this depends on the key mapping...we'll resolve that later...
  bool handle=true;
  //
  // Here we'd let the OS handle it for us, if it could, so the keymap works
  //
  if(!handle) return;
  if(!modifier) return;
  if(modifier==A_MOD_SHIFT) {
    //
    // here we take care of simple cases like capital letters
    //
    if(handle) {
      switch(code) {
	    // this is stupid and wrong
		case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'h':
		case 'i': case 'j': case 'k': case 'l': case 'm':
		case 'n': case 'o': case 'p': case 'q': case 'r': case 's': case 't': case 'u':
		case 'v': case 'w': case 'x': case 'y': case 'z':
		  code+=32;
	      break;
        default:
          DEBUG_OUT<<"AEvent::handleModKeys, unhandled code="<<code;
          DEBUG_OUT<<" and modifier="<<modifier<<"\n";
          break;
      }
    }
  }
}


AWindowAbstract *AEvent::getWindow()
{
  return theWin;
}


/*
ARegion *AEvent::getDirtyRegion()
{
  return theRegion;
}
*/


// NOTE: Be sure to update this if any new event types are added!
char *AEvent::debugEventType()
{
  char *ret=NULL;
  switch(type) {
    case A_EVENT_NONE: ret=(char *)"A_EVENT_NONE"; break;
    case A_EVENT_DESTROY: ret=(char *)"A_EVENT_DESTROY"; break;
    case A_EVENT_KEYUP: ret=(char *)"A_EVENT_KEYUP"; break;
    case A_EVENT_KEYDOWN: ret=(char *)"A_EVENT_KEYDOWN"; break;
    case A_EVENT_MENU: ret=(char *)"A_EVENT_MENU"; break;
    case A_EVENT_BREAK: ret=(char *)"A_EVENT_BREAK"; break;
    case A_EVENT_MOUSEDOWN: ret=(char *)"A_EVENT_MOUSEDOWN"; break;
    case A_EVENT_MOUSEUP: ret=(char *)"A_EVENT_MOUSEUP"; break;
    case A_EVENT_MOUSEMOVE: ret=(char *)"A_EVENT_MOUSEMOVE"; break;
    case A_EVENT_MOVED: ret=(char *)"A_EVENT_MOVED"; break;
    case A_EVENT_REDRAW: ret=(char *)"A_EVENT_REDRAW"; break;
    case A_EVENT_RESIZE: ret=(char *)"A_EVENT_RESIZE"; break;
    case A_EVENT_FOCUS: ret=(char *)"A_EVENT_FOCUS"; break;
    case A_EVENT_UNFOCUS: ret=(char *)"A_EVENT_UNFOCUS"; break;
    case A_EVENT_DRAGNDROP: ret=(char *)"A_EVENT_DRAGNDROP"; break;
    case A_EVENT_SOUND: ret=(char *)"A_EVENT_SOUND"; break;
    case A_EVENT_NETWORK: ret=(char *)"A_EVENT_NETWORK"; break;
    case A_EVENT_NETACK: ret=(char *)"A_EVENT_NETACK"; break;
    case A_EVENT_TICK: ret=(char *)"A_EVENT_TICK"; break;
    case A_EVENT_ALARM: ret=(char *)"A_EVENT_ALARM"; break;
    case A_EVENT_DISPLAY: ret=(char *)"A_EVENT_DISPLAY"; break;
    case A_EVENT_UNKNOWN: ret=(char *)"A_EVENT_UNKNOWN"; break;
    default:
	  //DEBUG_OUT.warning("Unknown event in debugEventType!");
	  ret=(char *)"Unknown event in debugEventType!";
	  break;
  }
  return ret;
}

