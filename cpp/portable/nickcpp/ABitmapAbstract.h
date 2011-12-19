#ifndef NICKCPP_ABITMAPABSTRACT_H
#define NICKCPP_ABITMAPABSTRACT_H


#include <portable/nickcpp/AFrameAbstract.h>


class ABitmapAbstract : public AFrameAbstract
{
public:
  ABitmapAbstract();
  ABitmapAbstract(unsigned int w, unsigned int h, unsigned int d, unsigned int np=1);
  virtual ~ABitmapAbstract();
protected:
  virtual void init();
};


#endif

