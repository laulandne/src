#ifndef GUILIB_ADRAWABLEABSTRACT_H
#define GUILIB_ADRAWABLEABSTRACT_H


#include <portable/nickcpp/AFrameAbstract.h>


class AColor;


class ADrawableAbstract : public AFrameAbstract
{
public:
  ADrawableAbstract();
  virtual ~ADrawableAbstract();
  //
  // begin drawing primatives...
  //
  virtual unsigned int getFontHeight();
  virtual char *queryFontAvail(char *name,unsigned int size=0,unsigned int flags=0);
  virtual void setFont(char *name,unsigned int size=0,unsigned int flags=0);
  virtual void setFontSize(unsigned int size,unsigned int flags=0);
  // Set given pen to given rgb values...doesn't do anything on truecolor
  virtual bool setColor(unsigned int colorNum, unsigned int r, unsigned int g,unsigned int b);
  virtual bool setColor(unsigned int colorNum, unsigned long l);
  virtual bool setColor(unsigned int index,AColor *aColor);
  virtual void drawText(const char *str,unsigned int len,bool remember=true);
  // Draw any other image into this image at dx, dy
  virtual void draw(ADrawableAbstract *source, unsigned int dx, unsigned int dy);
  // Draw any other image into this image at dx, dy
  virtual void copyBlock(ADrawableAbstract *source, unsigned int sx, unsigned int sy,unsigned int dx,unsigned int dy,unsigned bw,unsigned bh);
  // Rotate to an arbitrary angle
  virtual void rotate(ADrawableAbstract *dest,int angle);
  // Flip horizontally or vertically, or both
  virtual void flip(bool dx, bool dy);
  // Scale image size by given factor
  virtual void scale(int xFactor, int yFactor);
  virtual void scale(ADrawableAbstract *dest,int xFactor, int yFactor);
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
protected:
};


#endif

