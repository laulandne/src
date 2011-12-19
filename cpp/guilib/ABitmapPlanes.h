#ifndef GUILIB_ABITMAPPLANES_H
#define GUILIB_ABITMAPPLANES_H


// NOTE: This class is intended for use only to match obscure video card buffer setups!


#include <guilib/APrivateBitmap.h>


class ABitmapPlanes : public APrivateBitmap
{
public:
  ABitmapPlanes(ADisplay *someDisplay, ADisplayMode *d=NULL);
  virtual ~ABitmapPlanes();
  // Draw any other image into this image at dx, dy
  virtual void draw(ABitmap *source, unsigned int dx, unsigned int dy);
  virtual void writePixel(unsigned int x, unsigned int y);
  virtual unsigned int readPixel(unsigned int x, unsigned int y);
protected:
  bool needPlaneSwitch;
};


#endif // GUILIB_ABITMAPPLANES_H

