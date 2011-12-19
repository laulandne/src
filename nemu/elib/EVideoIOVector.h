#ifndef ELIB_EVIDEOIOVECTOR_H
#define ELIB_EVIDEOIOVECTOR_H


#include "EVideoIOAbstract.h"


class EVideoIOVector : public EVideoIOAbstract {
public:
  // Public Member Functions
  EVideoIOVector(EHandler *parent,unsigned int ncol,unsigned int nrow, unsigned int depthHint=1);
  virtual ~EVideoIOVector();
  // Public Data Members
protected:
  // Private Member Functions
  virtual void init();
};


#endif
