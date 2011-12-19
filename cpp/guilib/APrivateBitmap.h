#ifndef GUILIB_APRIVATEBITMAP_H
#define GUILIB_APRIVATEBITMAP_H


#include <guilib/ATypeDefs.h>

#include <portable/nickcpp/ABitmap.h>


#ifdef ASYS_X11
#include <X11/X.h>
#include <X11/Xlib.h>
#endif // ASYS_X11

#ifdef ASYS_AMIGA
struct BitMap;
#endif // ASYS_AMIGA

#ifdef ASYS_MAC
//#ifdef ALIB_NO_PCH
//#undef true
//#include <QDOffscreen.h>
#include <Carbon/Carbon.h>
//#endif // ALIB_NO_PCH
#endif // ASYS_MAC


#ifdef ASYS_WINDOWS
#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>
#undef WIN32_LEAN_AND_MEAN
#endif // ASYS_WINDOWS


class AWindowAbstract;
class ADisplayMode;
class ADisplay;


class APrivateBitmap : public ABitmap {
public:
  APrivateBitmap();
  APrivateBitmap(ABitmap *bm);
  APrivateBitmap(unsigned char *theBits,AWindowAbstract *someWindow);
  APrivateBitmap(ADisplay *someDisplay, ADisplayMode *d);
  APrivateBitmap(unsigned char *data, unsigned int w, unsigned int h,unsigned int d, unsigned int np=1);
  APrivateBitmap(unsigned int w, unsigned int h, unsigned int d, unsigned int np=1);
  virtual ~APrivateBitmap();
  //
  // begin drawing primatives...
  //
  virtual void draw(APrivateBitmap *source, unsigned int dx, unsigned int dy);
  // Draw any other image into this image at dx, dy
  virtual void copyBlock(APrivateBitmap *source, unsigned int sx, unsigned int sy,unsigned int dx,unsigned int dy,unsigned bw,unsigned bh);
  //
  // ...end drawing primatives
  //
  virtual void copyPlane(APrivateBitmap *source,unsigned int p);
  // Don't use this unless you're really careful!
  virtual void setPlane(unsigned int p, unsigned char *data);
  // Return an AConvertingBitLine object based on line y of this object
  virtual AConvertingBitLine *getLine(unsigned int y);
  virtual AConvertingBitLine *getCopyOfLine(unsigned int y);
  // Copy the bits of given row to already created line object
  // (as created by getLine above)
  virtual void copyLineTo(AConvertingBitLine *dest,unsigned int row);
  virtual void copyToLine(AConvertingBitLine *dest,unsigned int row);
  virtual void copyFromLine(AConvertingBitLine *source,unsigned int row);
  // Write arbitrary data to any line and any plane of this object
  virtual void writeLine(unsigned int lineNum, unsigned char *lineData,unsigned int lineLen, unsigned int planeNum);
  virtual void touch();
  virtual void update();
  virtual bool verifyOkay(AWindowAbstract *win);
  virtual bool verifyOkay(ADisplay *dis);
  // Avoid using these functions as they are mainly for internal use...
  virtual void updatePrivateBits();
  //
#ifdef ASYS_AMIGA
  BitMap *getBits() const { return TheBM; }
#endif // ASYS_AMIGA
#ifdef ASYS_WINDOWS
#ifndef USE_DIRECTDRAW
  HBITMAP getBits() const { return winBits; }
  BITMAPINFO *getBMI() const { return bmi; }
#endif // USE_DIRECTDRAW
#endif // ASYS_WINDOWS
#ifdef ASYS_OS2
  HBITMAP getBits() const { return winBits; }
  POINTL *getPoints() { return point; }
#endif // ASYS_OS2
#ifdef ASYS_MAC
#ifdef USE_QUARTZ
#else
  //__PixMap *getPBits() const { return ThePM; }
  //CTabHandle getCTab() const { return ctab; }
  GWorldPtr getTheWorld() const { return theWorld; }
#endif // USE_QUARTZ
#endif // ASYS_MAC
#ifdef ASYS_X11_BASED
  Pixmap getPBits() const { return ThePM; }
  XImage *getImage() const { return TheImage; }
#endif // ASYS_X11_BASED
  //
protected:
  //
  // member functions
  //
  virtual void freeBitmap();  // Call this in destructors
  virtual void init();
  virtual void readLine(unsigned int lineNum, unsigned char *lineData,unsigned int lineLen, unsigned int planeNum);
  // Make ourselves a clone of source
  virtual void copyFields(APrivateBitmap *source);
  virtual void duplicate(APrivateBitmap *source);
  //
  virtual void initPrivateBits();
  virtual void createPrivateBits(bool yeah);
  virtual void copyPrivateBits(APrivateBitmap *source);
  virtual void freePrivateBits();
  // Set the EGA/VGA writing plane number
  static void setVGAWrite(unsigned int p);
  // Set the EGA/VGA reading plane number
  static void setVGARead(unsigned int p);
  // Set the EGA/VGA video memory to write to all planes at the same time
  static void setVGAAll();
  // Set the SVGA bank number.
  virtual void setVGABank(unsigned int b);
#ifdef ASYS_MAC
  virtual void macBlastBits();
#endif // ASYS_MAC
  //
  // data members
  //
#ifdef ASYS_WINDOWS
#ifndef USE_DIRECTDRAW
  HBITMAP winBits;
  BITMAPINFO *bmi;
  char *extra;
#endif // USE_DIRECTDRAW
#endif // ASYS_WINDOWS
#ifdef ASYS_OS2
  HBITMAP winBits;
#ifdef ASYS_OS2_32
  BITMAPINFO2 *bmi;
#else
  BITMAPINFO *bmi;
#endif // ASYS_OS2_16
  POINTL point[4];
#endif // ASYS_OS2
#ifdef ASYS_MAC
#ifdef USE_QUARTZ
#else
  //__PixMapPtr ThePM;
  GWorldPtr theWorld;
  CTabHandle ctab;  // if image is indexed colors
#endif // USE_QUARTZ
#endif // ASYS_MAC
#ifdef ASYS_AMIGA
  BitMap *TheBM;
#endif // ASYS_AMIGA
#ifdef ASYS_X11
  Pixmap ThePM;
  // Not used much, will probably be removed (only used in setPrivateBits)
  XImage *TheImage;
  unsigned char *xbits;
  unsigned long xbitsize,xplanesize,xbpl;
#endif // ASYS_X11
#ifdef ASYS_DOS
  // Is this image only to be used by a particular instance of ADisplay?
  bool displayPrivate;
  // Are we being used in one of those 16 color 4 plane video modes?
  bool needPlaneSwitch;
  // Are we being used for an SVGA mode where its more than 64k?
  bool needBankSwitch;
#endif // ASYS_DOS
#ifdef ASYS_DOS16
  // pointer to VESA video ram page switching function
  void *pageFunc;
#endif // ASYS_DOS16
#ifdef ASYS_PALM
  BitmapType *bits;
  WinHandle win;
#endif // ASYS_PALM
};


#endif

