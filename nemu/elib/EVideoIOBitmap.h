#ifndef ELIB_EVIDEOIOBITMAP_H
#define ELIB_EVIDEOIOBITMAP_H


#include "EVideoIOAbstract.h"


class EVideoIOBitmap : public EVideoIOAbstract {
public:
  // Public Member Functions
  EVideoIOBitmap(EHandler *parent,unsigned int ncol,unsigned int nrow, unsigned int depthHint=1);
  virtual ~EVideoIOBitmap();
  // Public Data Members
protected:
  // Private Member Functions
  virtual void init();
};


#endif
