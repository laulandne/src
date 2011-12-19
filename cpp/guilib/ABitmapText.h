#ifndef GUILIB_ADIRECTBITMAPTEXT_H
#define GUILIB_ADIRECTBITMAPTEXT_H


// NOTE: This class is intended for use only to match obscure video card buffer setups!


#include <guilib/APrivateBitmap.h>


class ABitmapText : public APrivateBitmap
{
public:
  ABitmapText(ADisplay *someDisplay, ADisplayMode *d=NULL);
  virtual ~ABitmapText();
  // Draw any other image into this image at dx, dy
  virtual void draw(ABitmap *source, unsigned int dx, unsigned int dy);
  virtual void writePixel(unsigned int x, unsigned int y);
  virtual unsigned int readPixel(unsigned int x, unsigned int y);
protected:
};


#endif // GUILIB_ADIRECTBITMAPTEXT_H

