#ifndef GUILIB_ADISPLAYMODE_H
#define GUILIB_ADISPLAYMODE_H


#include <guilib/ATypeDefs.h>


class AWindowAbstract;
class ABitmap;
class APalette;
class AColor;
class AList;


#ifdef ASYS_X11_BASED
#include <X11/X.h>
#include <X11/Xlib.h>
#ifdef USE_OPENGL
#include <GL/gl.h>
#include <GL/glx.h>
#endif
#endif // ASYS_X11_BASED

#ifdef ASYS_WINDOWS
#ifdef USE_DIRECTDRAW
#include <ddraw.h>
#endif // USE_DIRECTDRAW
#endif // ASYS_WINDOWS

#ifdef ASYS_AMIGA
struct Screen;
#endif // ASYS_AMIGA

#ifdef ASYS_MAC
#ifdef ALIB_NO_PCH
#undef true
//#include <Quickdraw.h>
#include <Carbon/Carbon.h>
#endif // ALIB_NO_PCH
#endif // ASYS_MAC


// options for creating displays (may be left out)
enum ADisplayStrictVals {
  A_DISPLAY_MATCH_LOOSE=0,  // we don't care if it doesn't exactly match
  A_DISPLAY_MATCH_STICT=1,  // must match given dimensions/depth
  A_DISPLAY_MATCH_BEST=2  // give us the best possible, even better than we asked for
};

// Flags etc for ADisplayMode
enum ADisplayFlagVals {
  A_DISPLAYMODE_FLAG_NONE=0,     // Video has not been detected (yet)
  A_DISPLAYMODE_FLAG_LESS=1,      // Mode can handle its depth or lower (usually planes)
  A_DISPLAYMODE_FLAG_MONOCHROME=2,      // Mode is grayscale only
  A_DISPLAYMODE_FLAG_TOO_MUCH=4, // Mode is too much for current hardware
  A_DISPLAYMODE_FLAG_DEFAULT=8 // This mode is actually the default screen
};


// Flags etc for AApp.videoHardware
enum AVideoHardwareVals {
  A_VIDEO_HARDWARE_NONE=0,     // Video has not been detected (yet)
  A_VIDEO_HARDWARE_STANDARD=1 // Minimal video
};


// NOTE: This class MUST not have a base class!
class ADisplayMode {
public:
  unsigned long mode;
  unsigned int width,height;
  unsigned int depth,rdepth,nPlanes;
  unsigned int bytesPerLine;
  unsigned long vMemPtr;
  unsigned long vMemSize;
  unsigned int hardware;
  unsigned int bitsTriplet;
  unsigned int bitmapType;
  unsigned int flags;
};


#endif // GUILIB_ADISPLAYMODE_H
