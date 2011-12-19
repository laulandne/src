#ifndef GUILIB_charMAPPEDWINDOW_H
#define GUILIB_charMAPPEDWINDOW_H


#include "AWindow.h"


class ADisplay;

class ABitmap;
class APalette;
class AColor;
class ADebug;


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


// To be done...
class AWindowCharMapped : public AWindow
{
public:
  AWindowCharMapped(char *newName,unsigned int newWidth,unsigned int newHeight);
  void drawCharMap();
  void setCharMapping(bool t);
protected:
  void freeCharMap();
  void newCharMap();
  void clearCharMap();
  void calcCharMapSize();
  void resizeCharMap();
};


#endif // GUILIB_charMAPPEDWINDOW_H


