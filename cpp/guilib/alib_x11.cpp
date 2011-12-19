
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#ifdef ASYS_X11
#include <iostream>

#include "alib_x11.h"

#include <guilib/AApp.h>

#include <guilib/AEvent.h>
#include <guilib/AWindow.h>

#include <portable/nickcpp/ANullStream.h>
#endif // ASYS_X11


#ifdef ASYS_X11_BASED
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/Xutil.h>
#endif // ASYS_X11_BASED


#ifdef ASYS_X11_BASED
#ifdef USE_JOYSTICK
bool AEvent::linuxJoystick()
{
  bool ret=false;
  int size=0;
  js_event je;
  if(aThisApp.hasJoystick) {
    //DEBUG_OUT<<"Checking joystick...\n";
    size=read(aThisApp.joyfd,&je,sizeof(js_event));
    if(size==sizeof(js_event)) {
      //DEBUG_OUT<<"Got a joy event: "<<je.type<<" "<<je.number<<" "<<je.value<<"\n";
      if((je.type==1)&&(!je.value)) {
        // a button release
        //DEBUG_OUT<<"Joy button released: "<<je.number<<"\n";
        ret=true; // Don't check for an X event if we got a joy event
        type=A_EVENT_CLICK;  code=je.number;
      }
    }
  }
  return ret;
}
#endif // USE_JOYSTICK
#endif // ASYS_X11_BASED


#ifdef ASYS_X11
unsigned long AEvent::xFixKeypress(void *ee)
{
  bool numpadJoystick=true;
  XEvent *e=(XEvent *)ee;
  KeySym ks;
  char buf[10];
  buf[0]=0;  buf[1]=0;  buf[2]=0;
  XLookupString(&(e->xkey),buf,3,&ks,NULL);
  unsigned long kcode=buf[0];
  DEBUG_OUT<<"("<<e->xkey.state<<") "<<(int)buf[0]<<" "<<(int)buf[1];
  DEBUG_OUT<<" "<<(int)buf[2]<<"\n";
  switch(kcode) {
    case XK_BackSpace: kcode=A_KEY_DELETE; break;
    case XK_Delete: kcode=A_KEY_DELETE; break;
    case 127: kcode=A_KEY_DELETE; break;
    case XK_Linefeed: kcode=A_KEY_NEWLINE; break;
    case XK_Return: kcode=A_KEY_NEWLINE; break;
    default: break;
  }
  if(!kcode) {
    // For keys that don't map to strings
    KeySym ksym;
    ksym=XLookupKeysym(&e->xkey,0);
    DEBUG_OUT<<"ksym="<<ksym<<"\n";
    switch(ksym) {
      //
      // F keys
      //
      case XK_F1: kcode=A_KEY_F1; break;
      case XK_F2: kcode=A_KEY_F2; break;
      case XK_F19: kcode=A_KEY_F2; break;
      case XK_F3: kcode=A_KEY_F3; break;
      case XK_F20: kcode=A_KEY_F3; break;
      case XK_F4: kcode=A_KEY_F4; break;
      case XK_F16: kcode=A_KEY_F4; break;
      case XK_F5: kcode=A_KEY_F5; break;
      case XK_F18: kcode=A_KEY_F5; break;
      case XK_F6: kcode=A_KEY_F6; break;
      case XK_F7: kcode=A_KEY_F7; break;
      case XK_F8: kcode=A_KEY_F8; break;
      case XK_F9: kcode=A_KEY_F9; break;
      case XK_F10: kcode=A_KEY_F10; break;
      case XK_F11: kcode=A_KEY_F11; break;
      case XK_F12: kcode=A_KEY_F12; break;
      //
      // Cursor motion
      //
      case XK_Insert: kcode=A_KEY_INSERT; break;
      case XK_Home: kcode=A_KEY_HOME; break;
      case XK_End: kcode=A_KEY_END; break;
      case XK_Page_Up: kcode=A_KEY_PAGEUP; break;
      case XK_Page_Down: kcode=A_KEY_PAGEDOWN; break;
      case XK_Up: kcode=A_KEY_UP; break;
      case XK_Down: kcode=A_KEY_DOWN; break;
      case XK_Left: kcode=A_KEY_LEFT; break;
      case XK_Right: kcode=A_KEY_RIGHT; break;
      //
      // some odder keys...
      case XK_Scroll_Lock: kcode=A_KEY_SCROLLLOCK; break;
      case XK_Break: kcode=A_KEY_BREAK; break;
      case XK_Pause: kcode=A_KEY_PAUSE; break;
      case XK_Cancel: kcode=A_KEY_CANCEL; break;
      case XK_Help: kcode=A_KEY_HELP; break;
      //
      // keypad
      //
      case XK_KP_0: if(numpadJoystick) { } else kcode='0'; break;
      case XK_KP_1: if(numpadJoystick) { } else kcode='1'; break;
      case XK_KP_2: if(numpadJoystick) { } else kcode='2'; break;
      case XK_KP_3: if(numpadJoystick) { } else kcode='3'; break;
      case XK_KP_4: if(numpadJoystick) { } else kcode='4'; break;
      case XK_KP_5: if(numpadJoystick) { } else kcode='5'; break;
      case XK_KP_6: if(numpadJoystick) { } else kcode='6'; break;
      case XK_KP_7: if(numpadJoystick) { } else kcode='7'; break;
      case XK_KP_8: if(numpadJoystick) { } else kcode='8'; break;
      case XK_KP_9: if(numpadJoystick) { } else kcode='9'; break;
      case XK_KP_Begin: if(numpadJoystick) { } else kcode='5'; break;  // unlabeled on most pc keyboards
      case XK_KP_Divide: if(numpadJoystick) { } else kcode='/'; break;
      case XK_KP_Multiply: if(numpadJoystick) { } else kcode='*'; break;
      case XK_KP_Subtract: if(numpadJoystick) { } else kcode='-'; break;
      case XK_KP_Add: if(numpadJoystick) { } else kcode='+'; break;
      case XK_KP_Decimal: if(numpadJoystick) { } else kcode='.'; break;
      case XK_KP_Up: if(numpadJoystick) kcode=A_KEY_JOY_UP; else kcode=A_KEY_UP; break;
      case XK_KP_Down: if(numpadJoystick) kcode=A_KEY_JOY_DOWN; else kcode=A_KEY_DOWN; break;
      case XK_KP_Left: if(numpadJoystick) kcode=A_KEY_JOY_LEFT; else kcode=A_KEY_LEFT; break;
      case XK_KP_Right: if(numpadJoystick) kcode=A_KEY_JOY_RIGHT; else kcode=A_KEY_RIGHT; break;
      case XK_KP_Page_Up: if(numpadJoystick) { } else kcode=A_KEY_PAGEUP; break;
      case XK_KP_Page_Down: if(numpadJoystick) { } else kcode=A_KEY_PAGEDOWN; break;
      case XK_KP_Home: if(numpadJoystick) { } else kcode=A_KEY_HOME; break;
      case XK_KP_End: if(numpadJoystick) { } else kcode=A_KEY_END; break;
      // NOTE: Doesn't handle fire button number...
      case XK_KP_Insert: if(numpadJoystick) { kcode=A_KEY_JOY_FIRE; } else kcode=A_KEY_INSERT; break;
      // NOTE: Doesn't handle fire button number...
      case XK_KP_Delete: if(numpadJoystick) { kcode=A_KEY_JOY_FIRE; } else kcode=A_KEY_DELETE; break;
      //
      // mod keys by themselves
      //
      case XK_Shift_L: kcode=A_KEY_LSHIFT; break;
      case XK_Shift_R: kcode=A_KEY_RSHIFT; break;
      case XK_Control_L: kcode=A_KEY_LCONTROL; break;
      case XK_Control_R: kcode=A_KEY_RCONTROL; break;
      case XK_Alt_L: kcode=A_KEY_LALT; break;
      case XK_Alt_R: kcode=A_KEY_RALT; break;
      case XK_Super_L: kcode=A_KEY_LLOGO; break;
      case XK_Super_R: kcode=A_KEY_RLOGO; break;
      case XK_Menu: kcode=A_KEY_MENU; break;
      //
      default:
        if(ksym!=NoSymbol) DEBUG_OUT<<"unhandled keysym="<<(int)ksym<<"\n";
        kcode=0;
        break;
    }
  }
  DEBUG_OUT<<"kcode is "<<kcode<<"\n";
  return kcode;
}
#endif // ASYS_X11



#ifdef ASYS_X11
void AApp::x11_start()
{
  //DEBUG_OUT<<"x11_start()...\n";
#ifdef USE_OPENGL
  //DEBUG_OUT<<"Using OpenGL for 3D.\n";
#endif
#ifdef ASYS_GNOME
  //DEBUG_OUT<<"Using the GNOME framework.\n";
  alibGnomeMain(theArgc,theArgv);
#endif // ASYS_GNOME
#ifdef USE_JOYSTICK
  joyfd=open("/dev/js0",O_RDONLY|O_NONBLOCK);
  if(joyfd!= -1) {
    ioctl(joyfd,JSIOCGAXES,&numJoyAxes);
    ioctl(joyfd,JSIOCGBUTTONS,&numJoyButtons);
    // FIXME: check joystick driver version, make sure its at least 1.0
    if(numJoyAxes&&numJoyButtons) {
      hasJoystick=true;
      //DEBUG_OUT<<"Linux /dev/js0 joystick found.\n";
      //DEBUG_OUT<<"Axes: "<<(int)numJoyAxes<<" Buttons: "<<(int)numJoyButtons<<"\n";
      // FIXME: read all axes and record their value...
      // assume they represent the "centered" values
    }
  }
#endif // USE_JOYSTICK
  visualWarned=false;
}
#endif // ASYS_X11


#ifdef ASYS_X11
void AEvent::x11_handle_event(void *xe)
{
  if(!xe) return;
  XEvent *e=(XEvent *)xe;
  //DEBUG_OUT<<"x11_handle_event type "<<e->type<<" "<<x11_event_name(xe)<<"\n";
  x11type=e->type;
  switch(e->type) {
    case Expose:  case GraphicsExpose:  type=A_EVENT_REDRAW;  break;
    case DestroyNotify:
      if(e->xdestroywindow.window==theWin->getXWindow())
        type=A_EVENT_DESTROY;
      else type=A_EVENT_NONE;
      break;
    case ButtonRelease:
      type=A_EVENT_MOUSEUP;
      x=e->xbutton.x;  y=e->xbutton.y;
      break;
    case ButtonPress:
      type=A_EVENT_MOUSEDOWN;
      x=e->xbutton.x;  y=e->xbutton.y;
      break;
    case MotionNotify:
      type=A_EVENT_MOUSEMOVE;
      x=e->xmotion.x;  y=e->xmotion.y;
      break;
    case FocusIn:  type=A_EVENT_FOCUS;  break;
    case FocusOut:  type=A_EVENT_UNFOCUS;  break;
    case ConfigureNotify:  type=A_EVENT_RESIZE;  /*theWin->sync();*/  break;
    case KeyPress:
      // FIXME: This doesn't handle modifiers at all...not even shift!
      type=A_EVENT_KEYUP;
      code=xFixKeypress(xe);
      if(!code) type=A_EVENT_NONE;
      break;
    case CreateNotify:
      type=A_EVENT_NONE; break;
    case ReparentNotify:
      type=A_EVENT_NONE; break;
    case MapNotify:
      type=A_EVENT_NONE; break;
    case UnmapNotify:
      type=A_EVENT_NONE; break;
    case NoExpose:
      type=A_EVENT_NONE; break;
    default:
      type=A_EVENT_NONE;
      //DEBUG_OUT<<"wait unhandled XEvent: "<<e->type<<"\n";
      break;
  }
}
#endif // ASYS_X11


#ifdef ASYS_X11
char *AEvent::x11_event_name(void *xe)
{
  if(!xe) return NULL;
  XEvent *e=(XEvent *)xe;
  char *ret=NULL;
  return x11_return_event_name(e->type);
}
#endif // ASYS_X11


#ifdef ASYS_X11
char *x11_return_event_name(unsigned int t)
{
  char *ret=NULL;
  switch(t) {
    case KeyPress:  ret=(char *)"KeyPress";  break;   // 2
    case KeyRelease:  ret=(char *)"KeyRelease";  break;   // 3
    case ButtonPress:  ret=(char *)"ButtonPress";  break;   // 4
    case ButtonRelease:  ret=(char *)"ButtonRelease";  break;    // 5
    case MotionNotify:  ret=(char *)"MotionNotify";  break;    // 6
    case EnterNotify:  ret=(char *)"EnterNotify";  break;    // 7
    case LeaveNotify:  ret=(char *)"LeaveNotify";  break;    // 8
    case FocusIn:  ret=(char *)"FocusIn";  break;    // 9
    case FocusOut:  ret=(char *)"FocusOut";  break;    // 10
    case KeymapNotify:  ret=(char *)"KeymapNotify";  break;    //11
    case Expose:  ret=(char *)"Expose";  break;    // 12
    case GraphicsExpose:  ret=(char *)"GraphicsExpose";  break;    // 13
    case NoExpose:  ret=(char *)"NoExpose";  break;    // 14
    case VisibilityNotify:  ret=(char *)"VisibilityNotify";  break;    // 15
    case CreateNotify:  ret=(char *)"CreateNotify";  break;    // 16
    case DestroyNotify:  ret=(char *)"DestroyNotify";  break;    // 17
    case UnmapNotify:  ret=(char *)"UnmapNotify";  break;    // 18
    case MapNotify:  ret=(char *)"MapNotify";  break;    // 19
    case MapRequest:  ret=(char *)"MapRequest";  break;    // 20
    case ReparentNotify:  ret=(char *)"ReparentNotify";  break;    // 21
    case ConfigureNotify:  ret=(char *)"ConfigureNotify";  break;    // 22
    case ConfigureRequest:  ret=(char *)"ConfigureRequest";  break;    // 23
    case GravityNotify:  ret=(char *)"GravityNotify";  break;    // 24
    case ResizeRequest:  ret=(char *)"ResizeRequest";  break;    // 25
    case CirculateNotify:  ret=(char *)"CirculateNotify";  break;    // 26
    case CirculateRequest:  ret=(char *)"CirculateRequest";  break;    // 27
    case PropertyNotify:  ret=(char *)"PropertyNotify";  break;    // 28
    case SelectionClear:  ret=(char *)"SelectionClear";  break;    // 29
    case SelectionRequest:  ret=(char *)"SelectionRequest";  break;    // 30
    case SelectionNotify:  ret=(char *)"SelectionNotify";  break;    // 31
    case ColormapNotify:  ret=(char *)"ColormapNotify";  break;    // 32
    case ClientMessage:  ret=(char *)"ClientMessage";  break;    // 33
    case MappingNotify:  ret=(char *)"MappingNotify";  break;    // 34
    default:
      ret=(char *)"unknown";
      break;
  }
  return ret;
}
#endif // ASYS_X11


