#ifndef ELIB_EVIDEOIOTEXTONLY_H
#define ELIB_EVIDEOIOTEXTONLY_H


#include "EVideoIOAbstract.h"


class EVideoIOTextOnly : public EVideoIOAbstract {
public:
  // Public Member Functions
  EVideoIOTextOnly(EHandler *parent,unsigned int ncol,unsigned int nrow, unsigned int depthHint=1);
  virtual ~EVideoIOTextOnly();
  //
  virtual void redraw();
  virtual BYTE write8(ULONG addr,BYTE value);
  //
  // Public Data Members
  //
protected:
  //
  // Private Member Functions
  virtual void init();
  void drawChar(ULONG ea);
  //
  // Public Data Members
  char *theFontName;
  unsigned int theFontSize;
  //
};


#endif
