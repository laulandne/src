#ifndef GUILIB_ABITMAPBANKS_H
#define GUILIB_ABITMAPBANKS_H


// NOTE: This class is intended for use only to match obscure video card buffer setups!


#include <guilib/APrivateBitmap.h>


class ABitmapBanks : public APrivateBitmap
{
public:
  ABitmapBanks(ADisplay *someDisplay, ADisplayMode *d=NULL);
  virtual ~ABitmapBanks();
  // Draw any other image into this image at dx, dy
  virtual void draw(ABitmap *source, unsigned int dx, unsigned int dy);
  virtual void writePixel(unsigned int x, unsigned int y);
  virtual unsigned int readPixel(unsigned int x, unsigned int y);
protected:
  unsigned long wsize,wgran;
};


#endif // GUILIB_ABITMAPBANKS_H

