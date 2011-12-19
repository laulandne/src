#ifndef ELIB_EVIDEOIOMULTIMODE_H
#define ELIB_EVIDEOIOMULTIMODE_H


#include "EVideoIOTextOnly.h"


class EVideoIOMultiMode : public EVideoIOTextOnly {
public:
  // Public Member Functions
  EVideoIOMultiMode(EHandler *parent,unsigned int ncol,unsigned int nrow, unsigned int depthHint=1);
  virtual ~EVideoIOMultiMode();
  // Public Data Members
protected:
  // Private Member Functions
  virtual void init();
};


#endif
