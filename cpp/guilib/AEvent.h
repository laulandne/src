#ifndef GUILIB_AEVENT_H
#define GUILIB_AEVENT_H


#ifdef ASYS_MAC
#include <guilib/alib_carbon.h>
#endif

#ifdef ASYS_WINDOWS
#include <guilib/alib_win.h>
#endif

#ifdef ASYS_MAC
#ifdef ALIB_NO_PCH
#undef true
//#include <Events.h>
#include <Carbon/Carbon.h>
#endif // ALIB_NO_PCH
#endif // ASYS_MAC


// These codes are returned in the "code" field
// of for cursor keys and joystick movement
// as an A_EVENT_KEYPRESS event
// FIXME: The codes WILL change in the near future (wide characters?)
// FIXME: These codes are not yet supported on all platforms
#define A_KEY_USER  128
#define A_KEY_UP    A_KEY_USER+1
#define A_KEY_DOWN  A_KEY_USER+2
#define A_KEY_LEFT  A_KEY_USER+3
#define A_KEY_RIGHT A_KEY_USER+4


// These F-keys definitely aren't supported on all platforms...
#define A_KEY_F1  A_KEY_USER+5
#define A_KEY_F2  A_KEY_USER+6
#define A_KEY_F3  A_KEY_USER+7
#define A_KEY_F4  A_KEY_USER+8
#define A_KEY_F5  A_KEY_USER+9
#define A_KEY_F6  A_KEY_USER+10
#define A_KEY_F7  A_KEY_USER+11
#define A_KEY_F8  A_KEY_USER+12
#define A_KEY_F9  A_KEY_USER+13
#define A_KEY_F10  A_KEY_USER+14
#define A_KEY_F11  A_KEY_USER+15
#define A_KEY_F12  A_KEY_USER+16
#define A_KEY_F13  A_KEY_USER+17
#define A_KEY_F14  A_KEY_USER+18
#define A_KEY_F15  A_KEY_USER+19


// More normal keys...
// TODO: We should probably handle the del and delete key separately...
#define A_KEY_DELETE      0x08
#define A_KEY_BACKSPACE   A_KEY_DELETE
#define A_KEY_NEWLINE     0x0d
#define A_KEY_ESC         0x1b
#define A_KEY_TAB         0x09


// These are virtual keycodes for keys that
// don't have widely implemented standard values
// Not all systems support all these keys
// NOTE: Some keyboards label the "insert" key as "help" also or solely...
#define A_KEY_INSERT      A_KEY_USER+20
#define A_KEY_HOME        A_KEY_USER+21
#define A_KEY_END         A_KEY_USER+22
#define A_KEY_PAGEUP      A_KEY_USER+23
#define A_KEY_PAGEDOWN    A_KEY_USER+24
//
#define A_KEY_SCROLLLOCK  A_KEY_USER+25
#define A_KEY_BREAK       A_KEY_USER+26
#define A_KEY_PAUSE       A_KEY_USER+27
#define A_KEY_CANCEL      A_KEY_USER+28
// NOTE: Some keyboards label the "insert" key as "help" also or solely...
#define A_KEY_HELP        A_KEY_USER+29
#define A_KEY_SYSRQ       A_KEY_USER+30
#define A_KEY_PRINTSCREEN A_KEY_USER+31
#define A_KEY_NUMLOCK     A_KEY_USER+32


// These codes are returned for mod keys by themselves
// Not all systems support all these keys (or mod keys by themselves at all)
// TODO: Test if there are two apple keys on apple keyboards...
#define A_KEY_LSHIFT   A_KEY_USER+40
#define A_KEY_RSHIFT   A_KEY_USER+41
#define A_KEY_LCONTROL A_KEY_USER+42
#define A_KEY_RCONTROL A_KEY_USER+43
#define A_KEY_LALT     A_KEY_USER+44
#define A_KEY_RALT     A_KEY_USER+45
#define A_KEY_LLOGO    A_KEY_USER+46
#define A_KEY_RLOGO    A_KEY_USER+47
#define A_KEY_MENU     A_KEY_USER+48
#define A_KEY_CAPSLOCK A_KEY_USER+49


// Media keys which some keyboards have...
#define A_KEY_BACK     A_KEY_USER+50
#define A_KEY_FOREWARD A_KEY_USER+51
#define A_KEY_BSTOP    A_KEY_USER+52
#define A_KEY_RELOAD   A_KEY_USER+53
#define A_KEY_BHOME    A_KEY_USER+54
// These below may not be able to be read raw (os may grab them)
#define A_KEY_VOLDOWN  A_KEY_USER+60
#define A_KEY_VOLUP    A_KEY_USER+61
#define A_KEY_VOLMUTE  A_KEY_USER+62
#define A_KEY_EJECT    A_KEY_USER+63


// These are the codes returned by a joystick
// or the numeric keypad arrow keys
// x will be the joy number,
// y will have the analog value for the
// axe (if aplicable)
// TODO: What about diagnoals?
#define A_KEY_JOY_UP    A_KEY_USER+80
#define A_KEY_JOY_DOWN  A_KEY_USER+81
#define A_KEY_JOY_LEFT  A_KEY_USER+82
#define A_KEY_JOY_RIGHT A_KEY_USER+83
#define A_KEY_JOY_FIRE  A_KEY_USER+84
// x will be the button number,
// y will have the analog value for the
// button (if aplicable)


// These are mouse button numbers
// They are returned in the code field of
// an A_EVENT_CLICK event.
// These are also returned in the modifier field
// for joystick button presses.
// NOTE: Not all platforms support all three buttons
// Don't require use of a specific function, always have another way to do it
// Don't assume anything about the value of the code field of a click event
#define A_BUTTON_LEFT    1
#define A_BUTTON_MIDDLE  2
#define A_BUTTON_RIGHT   3


// These are different values for the modifier field
// NOTE: This is a bit field, so 3=shift AND control
// These aren't implemented for all platforms
#define A_MOD_NONE    0
#define A_MOD_SHIFT   1
#define A_MOD_CONTROL 2
#define A_MOD_ALT     4
#define A_MOD_LOGO    8
#define A_MOD_MENU    16


enum AEventType {
  A_EVENT_NONE=0,    // Nothing happened
  A_EVENT_DESTROY=1,   // The window was closed
  A_EVENT_KEYDOWN=2,   // A key was pressed
  A_EVENT_KEYUP=3,     // A key was released
  A_EVENT_MENU=4,      // A system or user-defined menu was selected
  A_EVENT_BREAK=5,     // Break, control-C or Quit was pressed/selected
  A_EVENT_MOUSEDOWN=6, // A mouse button was pressed
  A_EVENT_MOUSEUP=7,   // A mouse button was released
  A_EVENT_MOUSEMOVE=8, // The mouse was moved
  A_EVENT_MOVED=9,     // The window's position was changed
  A_EVENT_REDRAW=10,    // The window needs to be redrawn
  A_EVENT_RESIZE=11,    // The window's size was changed
  A_EVENT_FOCUS=12,     // The window has been made active
  A_EVENT_UNFOCUS=13,   // The window has been made inactive
  A_EVENT_DRAGNDROP=14, // An icon was dropped on our window or app icon
  A_EVENT_SOUND=15,     // A sound stopped playing, see ASound class
  A_EVENT_NETWORK=16,   // A network message has arrived, see ANet classes
  A_EVENT_NETACK=17,    // A network acknowledgement has arrived, see ANet
  A_EVENT_TICK=18,      // A timing tick, see timing classes
  A_EVENT_ALARM=19,     // A timimg alarm, see timing classes
  A_EVENT_DISPLAY=20,   // The display mode or size was changed
  A_EVENT_UNKNOWN=21,   // Not sure what happened...treat as A_EVENT_NONE
  A_EVENT_LAST=22       // Make sure this is the last one...
};


// NOTE: Not really implemented yet...
enum AMenuEventType {
  A_MENUEVENT_NONE=0,    // Nothing happened, or we are ignoring it
  A_MENUEVENT_NEW=1,   //
  A_MENUEVENT_OPEN=2,   //
  A_MENUEVENT_SAVE=3,   //
  A_MENUEVENT_SAVEAS=4,   //
  A_MENUEVENT_CLOSE=5,   //
  A_MENUEVENT_QUIT=6,   //
  A_MENUEVENT_CUT=7,   //
  A_MENUEVENT_COPY=8,   //
  A_MENUEVENT_PASTE=9,   //
  A_MENUEVENT_UNDO=10,   //
  A_MENUEVENT_REDO=11,   //
  A_MENUEVENT_LAST=12       // Make sure this is the last one...
};


class AWindowAbstract;
class AWindowInputStream;
class AApplicationLoop;


class AEvent {
public:
  //
  //  public member functions
  //
  AEvent(bool debugFlag=false);
  virtual ~AEvent();
  // Fill in event fields using current event for this window
  void operator<<(AWindowAbstract *theWindow);
  // Wait until any event (including A_EVENT_NONE!) happens in this window
  void wait(AWindowAbstract *theWindow);
  void becomeCloneOfThisEvent(AEvent *e);
  bool input_string(char *ptr, unsigned int *where, unsigned int size);
#ifdef UNICODE
  bool input_string(wchar_t *ptr, unsigned int *where, unsigned int size);
#endif // UNICODE
  bool getEcho() const { return shouldEcho; }
  void handleModKeys();
  void setWindow(AWindowAbstract *w) { theWin=w; }
  AWindowAbstract *getWindow();
  char *debugEventType();
  void setQuitASAP() { quitASAP=true; }
  bool getQuitASAP() { return quitASAP; }
  //
  //  public data members
  //
  AEventType type;
  long x,y;          // Mouse location, joystick change, etc.
  unsigned long code;         // Keycode, button/menu number, etc
  unsigned long modifier;     // Shift, control, etc.
  unsigned int number;
  //
#ifdef ASYS_X11
  int x11type;
#endif // ASYS_X11
protected:
  //
  //  private member functions
  //
  void init(AWindowAbstract *theWindow);
#ifdef ASYS_MAC
  void macHandleEvent(EventRecord& macEvent);
  void macMouseUp(EventRecord *macEvent, long *where);
  void macMouseDown(EventRecord *macEvent, long *where);
  //char *debugMacEventType(unsigned int theType);
#endif // ASYS_MAC
#ifdef ASYS_AMIGA
  bool amigaHandleEvent(unsigned long signals);
#ifdef USE_JOYSTICK
  unsigned long amigaJoystick(unsigned int joynum);
#endif // USE_JOYSTICK
#endif // ASYS_AMIGA
#ifdef ASYS_DOS
#ifdef USE_JOYSTICK
  unsigned long dosJoystick(unsigned int joynum);
#endif // USE_JOYSTICK
  unsigned long dosHandleExtended(unsigned long c);
#endif // ASYS_DOS
#ifdef ASYS_X11
#ifdef USE_JOYSTICK
  bool linuxJoystick();
#endif // USE_JOYSTICK
  unsigned long xFixKeypress(void *e);
  void x11_handle_event(void *xe);
  char *x11_event_name(void *xe);
#endif // ASYS_X11
  //
  //  private data members
  //
  bool nullWinFlag;
  bool shouldEcho;
  //char *data;                 // Event dependant data if any
  AWindowAbstract *theWin;  // Window in which the event occured
  //
  static bool quitASAP;
#ifdef ASYS_WINDOWS
  static bool sync;
#endif // ASYS_WIN32
  //
#ifdef ASYS_AMIGA
  unsigned long appsig,winsig;
#endif // ASYS_AMIGA
};


extern AEvent *alib_curEvent;


#endif // GUILIB_AEVENT_H
