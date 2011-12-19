#ifndef GUILIB_AWINDOWSTREAM_H
#define GUILIB_AWINDOWSTREAM_H


#include "AWindow.h"


class ADisplay;
class ABitmap;
class APalette;
class AColor;


#ifdef ASYS_X11_BASED
#include <X11/X.h>
#include <X11/Xlib.h>
#endif // ASYS_X11_BASED

#ifdef ASYS_WINDOWS
#ifdef USE_DIRECTDRAW
#include <ddraw.h>
#endif // USE_DIRECTDRAW
#endif // ASYS_WINDOWS

#ifdef ASYS_AMIGA
struct AppWindow;
struct Window;
struct RastPort;
struct MsgPort;
#endif // ASYS_AMIGA

#ifdef ASYS_MAC
#ifdef ALIB_NO_PCH
#undef true
//#include <Quickdraw.h>
#include <Carbon/Carbon.h>
#endif // ALIB_NO_PCH
#endif // ASYS_MAC


class AWindowStream {
public:
  AWindowStream(char *nm=(char *)NULL);
  virtual ~AWindowStream();
  AWindowStream& fatal(const char *s=(char *)NULL);
  AWindowStream& warning(const char *s=(char *)NULL);
  AWindowStream& stop(const char *s=(char *)NULL);
  AWindowStream& operator<<(unsigned long s);
  AWindowStream& operator<<(long s);
  AWindowStream& operator<<(unsigned int s);
  AWindowStream& operator<<(int s);
  AWindowStream& operator<<(const char *s);
  AWindowStream& operator<<(char s);
  AWindowStream& operator<<(float s);
  AWindowStream& operator<<(double s);
protected:
  void init();
  AWindow *w;
  //ADebug *d;
};


#endif // GUILIB_AWINDOWSTREAM_H


