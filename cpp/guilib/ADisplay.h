#ifndef GUILIB_ADISPLAY_H
#define GUILIB_ADISPLAY_H


#include <guilib/ATypeDefs.h>

#include <portable/nickcpp/AFrameAbstract.h>

#include "ADisplayMode.h"


class AWindowAbstract;
class APrivateBitmap;
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
//#ifdef ALIB_NO_PCH
#undef true
//#include <Quickdraw.h>
#include <Carbon/Carbon.h>
//#endif // ALIB_NO_PCH
#endif // ASYS_MAC


class ADisplay : public AFrameAbstract {
public:
  // Open one specifying at least a name
  ADisplay(char *name,unsigned int dWidth=320, unsigned int dHeight=200,
    unsigned int dDepth=1);
  // Open a generic display
  ADisplay(unsigned int dWidth=0, unsigned int dHeight=0,
    unsigned int dDepth=0);
  // Open a display matching the specs of the image if possible.
  ADisplay(APrivateBitmap *someImage);
  virtual ~ADisplay();
  // Set pen to rgb colors, won't do anything on a True Color display
  bool setColor(unsigned int colorNum, unsigned int r, unsigned int g,
    unsigned int b);
  bool setColor(unsigned int colorNum, unsigned long l);
  bool setColor(unsigned int index,AColor *aColor);
  // Check if user has changed screen size or depth and make sure this object's
  //   attributes match what the system is actually using
  void sync();
  void focus();
  void reallyFocus();
  // Try to match the display's palette (if indexed) to image if possible.
  bool matchColors(APalette *someColors);
  bool matchColors(APrivateBitmap *im);
  bool matchColors();
  // The next few will return information about the display's attributes
  //unsigned int getWidth() { sync(); return width-xOffset-xMargin; }
  //unsigned int getHeight() { sync(); return height-yOffset-yMargin; }
  unsigned int getDepth() { sync(); return depth; }
  unsigned int getNPlanes() { sync(); return nPlanes; }
  unsigned int getXOffset() { sync(); return xOffset; }
  unsigned int getYOffset() { sync(); return yOffset; }
  ABitmapType getBitmapType() { sync(); return bitmapType; }
  APalette *getColors() const { return colors; }
  unsigned int getBitsTriplet() const { return bitsTriplet; }
  unsigned int getAvgCharWidth() const { return avgCharWidth; }
  unsigned int getBytesPerLine() { calcBPL(); return bytesPerLine; }
  ABitmap *getTheFont(bool tryToLoad=true);
  bool isStaticColor() const { return staticColor; }
  bool isGrayscale() const { return grayscale; }
  bool canSetAnyColor() const { return canSetAnyColorFlag; }
  bool canUseAnyImage() const { return canUseAnyImageFlag; }
  // These next few are mainly for internal use...
  void unfocus();
  unsigned long fixForeColor(unsigned long c);
  unsigned long fixBackColor(unsigned long c);
  unsigned long fixRGBColor(unsigned long c);
  UINT calcScaleFactorX(UINT w);
  UINT calcScaleFactorY(UINT h);
  APrivateBitmap *getCompatibleBitmap(ADisplayMode *d=NULL);
#ifdef ASYS_DOS
  // SVGA memory window info
  unsigned long getWSize() const { return wsize; }
  unsigned long getWGran() const { return wgran; }
  // Check config
  bool getBiosSupp() const { return biossupp; }
  bool getMouseSupp() const { return mousesupp; }
  bool isTextMode();
  unsigned long getBiosMode() const { return choice->mode; }
  ABitmap *getTheBits() const { return theBits; }
  bool hideMouse();
  bool showMouse();
  // Hercules card modes
  static void setHercText();
  static void setHercGfx();
  void setPaletteRegs();
#ifdef ASYS_DOS16
  void *getFunc();
#endif // ASYS_DOS16
#endif // ASYS_DOS
#ifdef ASYS_AMIGA
  Screen *getScreen() const { return TheScreen; }
#endif // ASYS_AMIGA
#ifdef ASYS_X11_BASED
  Display *getDisplay() const { return theDisplay; }
  Colormap getCMap() const { if(useCMap) return theCMap; else return (Colormap)0; }
#ifdef USE_OPENGL
  XVisualInfo *getVisInfo() const { return visInfo; }
#endif
#endif // ASYS_X11_BASED
#ifdef ASYS_WIN32
#ifdef USE_DIRECTDRAW
  LPDIRECTDRAW getDdPtr() { return ddPtr; }
  HWND getDdrawWin() { return ddrawWin; }
  LPDIRECTDRAWSURFACE getOffscreenPtr() { return offscreenPtr; }
  LPDIRECTDRAWSURFACE getSurfPtr() { return surfPtr; }
  LPDIRECTDRAWPALETTE getWinPalette() { return winPalette; }
  BYTE *getDdrawBits() { return ddrawBits; }
  bool getCanPageFlip() { return canPageFlip; }
  bool getChangedMode() { return changedMode; }
  bool getDidInitDDraw() { return didInitDDraw; }
#endif // USE_DIRECTDRAW
#endif // ASYS_WIN32
private:
  void init();
  void findModes();
  void queryHardware();
  ADisplayMode *query(unsigned int w, unsigned int h, unsigned int d,
    unsigned int np);
  bool set(ADisplayMode *tryMode);
  void openDis(char *tname,unsigned int w, unsigned int h,
    unsigned int d, unsigned int np);
  void syncCurrentColors();
  void calcRGBBits();
#ifdef ASYS_DOS
  bool vesaGetInfo(struct AVesaInfo *v);
  bool vesaGetModeInfo(unsigned long mode, struct AVesaModeInfo *i);
  unsigned int getCurVideoMode();
  bool setVideoMode(ADisplayMode *tryMode);
  bool setBiosVideoMode(unsigned int mode);
  unsigned int getEquipStatus();
#endif // ASYS_DOS
#ifdef ASYS_WIN32
#ifdef USE_DIRECTDRAW
  bool initDirectDraw(ADisplayMode *tryMode);
#endif // USE_DIRECTDRAW
#endif // ASYS_WIN32
  APalette *colors;
  // true if we are currently changing the palette
  bool colorLock;
  char *name;
  ADisplayMode *choice;
  unsigned int desiredWidth,desiredHeight,desiredDepth;
  unsigned int bitsTriplet;
  unsigned int rbits,gbits,bbits;  // Only valid for true color
  unsigned long rmask,gmask,bmask;  // Only valid for true color
  unsigned long rshift,gshift,bshift;  // Only valid for true color
  bool staticColor,grayscale;
  bool canSetAnyColorFlag;
  bool canLockBits;
  bool canUseAnyImageFlag;
  unsigned int avgCharWidth;
  bool haveFocus;  // don't trust this...
  //AList *windows;
  AWindowAbstract *focusWindow;
  // May be NULL, but if not, points to offscreen buffer...
  unsigned int xOffset,yOffset;
  unsigned int xMargin,yMargin;
  bool bitsLocked;
  bool areDefault;  // we're the defaultDisplay
  ABitmap *theFont;  // image for for drawing text ourselves...
  //
  static unsigned int numDisplays;
  //static ADisplay *defaultDisplay;
  static AList *dmList;
  static unsigned int videoHardware;
  static unsigned int defaultDepth,defaultPlanes;
  //
#ifdef ASYS_AMIGA
  // Is the display really the default public screen?
  bool isWorkbench;
  // Is the display mode a HAM or HBrite mode?
  bool isHAM,isHBrite;
  // Have we tried turning on HBrite or turning HIRES off to get screen to open?
  bool triedHB, triedHIRES;
  // If we change the default public screen's colors, we save them here
  APalette *wbColors;
  Screen *TheScreen;
#endif // ASYS_AMIGA
#ifdef ASYS_MAC
#ifdef USE_QUARTZ
#else
  unsigned long curMode;
  GDHandle gfxDevice;
#endif // USE_QUARTZ
#endif // ASYS_MAC
#ifdef ASYS_DOS
  ABitmap *theBits;
  // Is the video memory interleaved like CGA?
  bool interleaved;
  // video bios mode the system was in before we opened
  ADisplayMode origMode;
  // How many text rows and columns the screen has in current mode
  unsigned int numRows,numCols;
  // The location and size of VRAM
  unsigned long vptr;  // Really a pointer...
  unsigned long vsize;
  // The size and granularity of VRAM for SVGA
  unsigned long wsize,wgran;
  // Does the bios support standard video bios funcs for this mode?
  bool biossupp,mousesupp;
#endif // ASYS_DOS
#ifdef ASYS_X11_BASED
  Display *theDisplay;
  Screen *theScreen;
  Visual *theVis;
  VisualID vid;
#ifdef USE_OPENGL
  XVisualInfo *visInfo;
#endif
  Colormap theCMap;
  bool useCMap;
#endif // ASYS_X11_BASED
#ifdef ASYS_WIN32
#ifdef USE_DIRECTDRAW
  HINSTANCE dDrawHandle;
  LPDIRECTDRAW ddPtr;
  LPDIRECTDRAWSURFACE offscreenPtr;
  LPDIRECTDRAWSURFACE surfPtr;
  LPDIRECTDRAWPALETTE winPalette;
  HWND ddrawWin;
  BYTE *ddrawBits;
  bool changedMode;
  bool canPageFlip;
  bool didInitDDraw;
  unsigned int numPages;
#endif // USE_DIRECTDRAW
#endif // ASYS_WIN32
};


extern ADisplay *aDefaultADisplay;


#endif // GUILIB_ADISPLAY_H
