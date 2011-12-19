#ifndef GUILIB_ABITMAPSIMPLE_H
#define GUILIB_ABITMAPSIMPLE_H


#include <portable/nickcpp/AFrameAbstract.h>


class ABitmapSimple : public AFrameAbstract
{
public:
  ABitmapSimple();
  ABitmapSimple(unsigned int w, unsigned int h, unsigned int d, unsigned int np=1);
  virtual ~ABitmapSimple();
protected:
  virtual void init();
};


#endif // GUILIB_ABITMAPSIMPLE_H

