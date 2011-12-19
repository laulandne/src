#ifndef NICKCPP_ABITMAP_H
#define NICKCPP_ABITMAP_H


#include <portable/nickcpp/ADrawableAbstract.h>


class AColor;
class AConvertingBitLine;
class APalette;


// For now this is the maximum...
// (We're using a static array of this size, but will change to a
//   dynamically allocated array in the future)
#define NUM_PLANES	4*8


class ABitmap : public ADrawableAbstract {
public:
  ABitmap();
  //ABitmap(unsigned char *theBits,AWindowAbstract *someWindow);
  //ABitmap(ADisplay *someDisplay, ADisplayMode *d);
  ABitmap(unsigned char *data, unsigned int w, unsigned int h,unsigned int d, unsigned int np=1);
  ABitmap(unsigned int w, unsigned int h, unsigned int d, unsigned int np=1);
  virtual ~ABitmap();
  //
  // begin drawing primatives...
  //
  // Draw any other image into this image at dx, dy
  virtual void draw(ABitmap *source, unsigned int dx, unsigned int dy);
  // Draw any other image into this image at dx, dy
  virtual void copyBlock(ABitmap *source, unsigned int sx, unsigned int sy,unsigned int dx,unsigned int dy,unsigned bw,unsigned bh);
  // Rotate to an arbitrary angle
  virtual void rotate(ABitmap *dest,int angle);
  // Flip horizontally or vertically, or both
  virtual void flip(bool dx, bool dy);
  // Scale image size by given factor
  virtual void scale(int xFactor, int yFactor);
  virtual void scale(ABitmap *dest,int xFactor, int yFactor);
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
  virtual int getPenX();
  virtual int getPenY();
  virtual void scroll(int dx,int dy);
  virtual void moveTo(unsigned int newX, unsigned int newY);
  virtual void lineTo(unsigned int newX, unsigned int newY);
  virtual void eraseRect(unsigned int rwidth,unsigned int rheight);
  virtual void drawRect(unsigned int rwidth,unsigned int rheight);
  virtual void drawFilledRect(unsigned int rwidth,unsigned int rheight);
  //
  // ...end drawing primatives
  //
  virtual void copyPlane(ABitmap *source,unsigned int p);
  static unsigned long bits2Bytes(unsigned int bits);
  virtual ABitmapType getBitmapType() const { return bitmapType; }
  virtual bool checkLine(unsigned int row);  // is row within legal bounds?
  virtual bool checkPlane(unsigned int p);   // is p within legal bounds?
  virtual unsigned char *getPlane(unsigned int n) const {
    if(n<nPlanes) return plane[n]; else return (unsigned char *)NULL;
  }
  virtual int getLetter() const { return letter; }
  virtual void setLetter(char c) { letter=c; }
  // Don't use this unless you're really careful!
  virtual void setPlane(unsigned int p, unsigned char *data);
  virtual void stealPlane(unsigned int np) { if(np<nPlanes) needFree[np]=false; }
  // Return an AConvertingBitLine object based on line y of this object
  AConvertingBitLine *getLine(unsigned int y);
  AConvertingBitLine *getCopyOfLine(unsigned int y);
  // Copy the bits of given row to already created line object
  // (as created by getLine above)
  virtual void copyLineTo(AConvertingBitLine *dest,unsigned int row);
  virtual void copyToLine(AConvertingBitLine *dest,unsigned int row);
  virtual void copyFromLine(AConvertingBitLine *source,unsigned int row);
  // Write arbitrary data to any line and any plane of this object
  virtual void writeLine(unsigned int lineNum, unsigned char *lineData,unsigned int lineLen, unsigned int planeNum);
  virtual bool isTrueColor() const { return trueColor; }
  // Convert image to given format and depth if possible
  virtual void convert(unsigned int suggestDepth,ABitmapType tp,APalette *pal);
  virtual void computeHistogram(unsigned int eBits);
  virtual void convertEightBits();
  virtual void convertTwentyFourBits();
  virtual APalette *getColors() const { return colors; }
  virtual bool setColor(unsigned int colorNum, unsigned int r, unsigned int g, unsigned int b);
  virtual bool setColor(unsigned int colorNum, unsigned long l);
  virtual bool setColor(unsigned int index,AColor *aColor);
  virtual void touch();
  virtual void update();
  // Is the image "okay"?  (i.e. is it ready to be drawn?)
  virtual bool isOkay() const { return okayDokey; };
  //virtual bool verifyOkay(ADisplay *dis);
  //virtual bool verifyOkay(AWindowAbstract *win);
  // Avoid using these functions as they are mainly for internal use...
  virtual void adjustRGBChunky();
  virtual void adjustRGBPlanes();
  //
protected:
  //
  // member functions
  //
  virtual void freeBitmap();  // Call this in destructors
  virtual void init();
  // Change palette's size as needed...also create or destroy.
  virtual void calcColors();
  virtual void readLine(unsigned int lineNum, unsigned char *lineData,unsigned int lineLen, unsigned int planeNum);
  // Will be an optimized memcpy()
  virtual void memcpyl(unsigned char *dest, unsigned char *src, unsigned long len);
  // Make ourselves a clone of source
  virtual void copyFields(ABitmap *source);
  virtual void duplicate(ABitmap *source);
  virtual void reorderRGB();
  virtual void reorderRGBChunky();
  virtual void reorderRGBPlanes();
  virtual void *acalloc(unsigned int num,unsigned int size);
  virtual bool tooBig();
  virtual void checkPaletteSize();
  virtual void snarfAndMungeBits(char *bits,unsigned int bitsize,unsigned int unit,bool mirror);
  //
  // data members
  //
  APalette *colors;  // This can and will be NULL sometimes
  bool ownColors;
  unsigned long foreground, background;
  int letter;
  unsigned char *plane[NUM_PLANES];
  bool needsRGBReorder;
  bool needFree[NUM_PLANES];
  unsigned int *lineOffset;
   // true if the PrivateBits have been created
  bool privateBitsDone;
  // true if we are in the middle of creating the PrivateBits
  bool privateLock;
  // true if we are in the middle of a verify (i.e. Making the image "Okay")
  bool verifyLock;
  // true if verify was happy with the image
  bool okayDokey;
  bool allPlanesSame;
  //bool needsUnmapping;
  //bool updateImmediately;
  //unsigned int bitsHint;
};


#endif

