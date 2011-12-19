#ifndef GUILIB_ABITMAPBANKSPLANES_H
#define GUILIB_ABITMAPBANKSPLANES_H


// NOTE: This class is intended for use only to match obscure video card buffer setups!


#include <guilib/APrivateBitmap.h>


class ABitmapBanksPlanes : public APrivateBitmap
{
public:
  ABitmapBanksPlanes(ADisplay *someDisplay, ADisplayMode *d=NULL);
  virtual ~ABitmapBanksPlanes();
  // Draw any other image into this image at dx, dy
  virtual void draw(ABitmap *source, unsigned int dx, unsigned int dy);
  virtual void writePixel(unsigned int x, unsigned int y);
  virtual unsigned int readPixel(unsigned int x, unsigned int y);
protected:
  unsigned long wsize,wgran;
  bool needPlaneSwitch;
};


#endif // GUILIB_ABITMAPBANKSPLANES_H

