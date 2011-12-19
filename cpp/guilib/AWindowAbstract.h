#ifndef GUILIB_AWINDOWABSTRACT_H
#define GUILIB_AWINDOWABSTRACT_H


#include <guilib/ATypeDefs.h>

#include <portable/nickcpp/ADrawableAbstract.h>


class ADisplay;

class ABitmap;
class APrivateBitmap;
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
//#ifdef ALIB_NO_PCH
//#undef true
//#include <Quickdraw.h>
#include <Carbon/Carbon.h>
//#endif // ALIB_NO_PCH
#endif // ASYS_MAC


// Font constants
#define A_FONT_NONPROP  1
#define A_FONT_PROP 2


class AWindowAbstract : public ADrawableAbstract {
public:
  AWindowAbstract();
  virtual ~AWindowAbstract();
  //
  // begin drawing primatives...
  //
  virtual unsigned int getFontHeight() { sync(); return fontHeight; }
  virtual char *queryFontAvail(char *name,unsigned int size=0,unsigned int flags=0);
  virtual void setFont(char *name,unsigned int size=0,unsigned int flags=0);
  virtual void setFontSize(unsigned int size,unsigned int flags=0);
  // Set given pen to given rgb values...doesn't do anything on truecolor
  virtual bool setColor(unsigned int colorNum, unsigned int r, unsigned int g,unsigned int b);
  virtual bool setColor(unsigned int colorNum, unsigned long l);
  virtual bool setColor(unsigned int index,AColor *aColor);
  virtual void drawText(const char *str,unsigned int len,bool remember=true);
  // Draw any other image into this window at dx, dy
  virtual void draw(APrivateBitmap *someImage, unsigned int atX=0, unsigned int atY=0);
  // Draw any other image into this window at dx, dy
  virtual void copyBlock(APrivateBitmap *source, unsigned int sx, unsigned int sy,unsigned int dx,unsigned int dy,unsigned bw,unsigned bh);
  // Rotate to an arbitrary angle
  virtual void rotate(APrivateBitmap *dest,int angle);
  // Flip horizontally or vertically, or both
  virtual void flip(bool dx, bool dy);
  // Scale image size by given factor
  virtual void scale(int xFactor, int yFactor);
  virtual void scale(APrivateBitmap *dest,int xFactor, int yFactor);
  // Scroll rectangle given by tx,ty,tw,th dx pixels horizontally, dy vertically
  virtual void scroll(unsigned int tx,unsigned int ty,unsigned int tw,unsigned int th, int dx, int dy);
  // Erase image to pixel value 0
  virtual void clear(bool preserveMap=false);
  // Erase a rectangle of pixels
  virtual void clear(unsigned int x, unsigned int y,unsigned int tw,unsigned int th);
  virtual void line(unsigned int x1, unsigned int y1,unsigned int x2, unsigned int y2);
  // Read a pixel's color value at x,y...return value depends on mode
  virtual unsigned int readPixel(unsigned int x, unsigned int y);
  virtual void writePixel(unsigned int x, unsigned int y);
  virtual void setForeground(unsigned long n,bool rgb=true);
  virtual void setBackground(unsigned long n,bool rgb=true);
  virtual void oval(int tx,int ty,int twidth, int theight);
  virtual void fillOval(int tx,int ty,int twidth, int theight);
  virtual void fillRect(int tx,int ty,int twidth, int theight);
  virtual int getPenX() { return penX; }
  virtual int getPenY() { return penY; }
  virtual void scroll(int dx,int dy);
  virtual void moveTo(unsigned int newX, unsigned int newY);
  virtual void lineTo(unsigned int newX, unsigned int newY);
  virtual void eraseRect(unsigned int rwidth,unsigned int rheight);
  virtual void drawRect(unsigned int rwidth,unsigned int rheight);
  virtual void drawFilledRect(unsigned int rwidth,unsigned int rheight);
  //
  // ...end drawing primatives
  //
  // Make this window the focus of input i.e. make it active
  AWindowAbstract& focus();
  AWindowAbstract& reallyFocus();
  // update private variables to keep this object and actual window in sync
  AWindowAbstract& sync();
  void drawBackground(void *foo=(void *)NULL);
  //
  // FIXME: Soon these functions will be replaced by using
  // C++'s stream library (similar to ALib's ADebug class)
  //
  AWindowAbstract& operator<<(unsigned long s);
  AWindowAbstract& operator<<(long s);
  AWindowAbstract& operator<<(unsigned int s);
  AWindowAbstract& operator<<(int s);
  AWindowAbstract& operator<<(const char *s);
  AWindowAbstract& operator<<(char s);
  AWindowAbstract& operator<<(float s);
  AWindowAbstract& operator<<(double s);
  //
  AWindowAbstract& operator<<(APrivateBitmap *someImage);
  AWindowAbstract& center(const char *str);
  AWindowAbstract& center(const char *str, unsigned int yCoord);
  AWindowAbstract& newline();
  // These next two are hacks to switch the mode of numeric output...
  AWindowAbstract& hex();
  AWindowAbstract& dec();
  //
  void setHexDigits(unsigned int digits) { sigHexDigits=digits; }
  unsigned int getHexDigits() const { return sigHexDigits; }
  // Attempt to match the window's color palette to given one...sometimes can't
  bool matchColors(APalette *someColors);
  unsigned int getBorderWidth() { sync(); return borderWidth; }
  unsigned int getBorderHeight() { sync(); return borderHeight; }
  bool resize(unsigned int newWidth, unsigned int newHeight);
  // Move the window to the global coords given in its parent display
  bool move(int newX, int newY);
  void centerOnDisplay();
  ADisplay *getParent() const { return parent; }
  //
  void setTextOutput(bool n) { textOutput=n; }
  void setDebugOutput(bool n) { debugOutput=n; }
  bool getTextOutput() const { return textOutput; }
  bool getDebugOutput() const { return debugOutput; }
  unsigned int getAvgCharWidth() const { return avgCharWidth; }
  //
  void startMore() { more=true;  moreLineNum=0; }
  void endMore() { more=false; }
  void setUseMore(bool t) { useMore=t; }
  bool getUseMore() const { return useMore; }
  //
  void drawCursor();
  void eraseCursor();
  void saveCursorPosition();
  void restoreCursorPosition();
  //
  APalette *getColors();
  void print();  // to debug, not to printer like you'd think...
  void redraw();  // try to redraw window contents...may not work
  void unfocus();
  //
#ifdef ASYS_MAC
  CWindowPtr getMacWindowPtr() const { return TheWP; }
#ifdef USE_QUARTZ
#else
  CGrafPtr getMacGrafPtr() const { return TheGP; }
#endif // USE_QUARTZ
#endif // ASYS_MAC
#ifdef ASYS_X11_BASED
  Window getXWindow() const { return TheWin; }
  void setXWindowHack(Window xwin);
  void waitForFirstFocus();
  void waitForFirstValidEvent();
#endif // ASYS_X11_BASED
#ifdef ASYS_AMIGA
  Window *getAmyWindow() const { return TheWin; }
  // These next two are for drag and drop app messages
  unsigned long getAppSignal() const { return appsignal; }
  char *handleAppMessage(unsigned long signals);
#endif // ASYS_AMIGA
#ifdef ASYS_WINDOWS
  HWND getWinWindow() const { return ahWnd; }
  HDC getWinHDC() const { return ahDC; }
  void paintBackground(HDC hdc);
#endif // ASYS_WINDOWS
#ifdef ASYS_OS2
  HWND getOS2Window() const { return ahWnd; }
#endif // ASYS_OS2
#ifdef ASYS_GLUT
  void glutSetSize(unsigned int w,unsigned int h);
  float glutTransX(unsigned x);
  float glutTransY(unsigned y);
#endif // ASYS_GLUT
  //
protected:
  //
  // Private member functions
  void init();
  // Actually open the window, called by all constructors
  void openWin(const char *newName,unsigned int newWidth,unsigned int newHeight, int newx=0, int newy=0);
  void drawImageChar(char c);
  void reallyMove(unsigned int tx,unsigned int ty);
  void reallyResize(unsigned int twidth, unsigned int theight);
  bool fixSizeMove(unsigned int tx,unsigned int ty,unsigned int twidth, unsigned int theight, bool sizeNotMove);
  unsigned int countLines(const char *msg);
  unsigned int howWideIsThis(const char *str,unsigned int len);
  //
  // Private data members
  char *name;
  unsigned int fontHeight;
  unsigned int oldFontHeight;
  unsigned int penX, penY;
  unsigned int oldPenX,oldPenY;
  // Current pen number values or rgb values for some systems/display modes
  unsigned long foreground, background;
  unsigned int borderHeight, borderWidth;
  unsigned int avgCharWidth;
  bool sizeCorrected,moveCorrected;
  bool syncLock;
  bool hexNums;
  unsigned int sigHexDigits;
  ADisplay *parent;
  bool debugOutput;
  bool textOutput;
  bool more;
  bool useMore;
  unsigned int moreLineNum,numRows;
  unsigned int scrollRows,normalScrollRows;
  bool haveFocus;  // don't trust this...
  bool charMapped;
  char *charMap;
  unsigned int mapSize;
  unsigned int mapCols,mapRows;
  char strBuf[256];
  // If non-NULL, this is a back buffer for a double-buffered window
  ABitmap *offscreenImage;
  //
  static bool needToDrawCursor;
  //static ADisplay *defaultDisplay;
  //static AWindowAbstract *defaultWindow;
  static unsigned int numWindows;
  //
#ifdef ASYS_WINDOWS
  HWND ahWnd;
  HDC ahDC;
  bool textProp;
#ifdef USE_DIRECTDRAW
  DDSURFACEDESC sDesc;
#else
  HFONT propFont,fixedFont;
  unsigned int pred,pgreen,pblue;
  unsigned int bred,bgreen,bblue;
  HPEN hPen;
  HBRUSH backBrush;
#endif // USE_DIRECTDRAW
#endif // ASYS_WINDOWS
#ifdef ASYS_OS2
  HWND ahWnd,ahWndFrame;
  // Cached for speed...
  HDC winDC;
  HPS winHPS;
  HDC bitsDC;
  HPS bitsPS;
#endif // ASYS_OS2
#ifdef ASYS_X11_BASED
  Window TheWin;
  GC TheGC;
  XFontStruct *xfs;  // NOTE: Will be NULL if we never changed the font from default
#endif // ASYS_X11_BASED
#ifdef ASYS_AMIGA
  Window *TheWin;
  RastPort *TheRP;
  // AppWindow support
  AppWindow *appwin;
  MsgPort *awport;
  unsigned long appsignal;
#endif // ASYS_AMIGA
#ifdef ASYS_MAC
  CWindowPtr TheWP;
#ifdef USE_QUARTZ
  CGContextRef TheContext;
#else
  CGrafPtr TheGP;
  RGBColor fcolor,bcolor;
#endif // USE_QUARTZ
#endif // ASYS_MAC
#ifdef ASYS_PALM
  bool textProp;
  unsigned int yOffset;
  unsigned int glyphOffset[256];
#endif // ASYS_PALM
};


#endif // GUILIB_AWINDOWABSTRACT_H


