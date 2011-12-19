#ifndef ELIB_EMAPHANDLER_H
#define ELIB_EMAPHANDLER_H


#include "EMemAbstract.h"


class EMapHandler : public EMemAbstract {
public:
  virtual BYTE read8(ULONG addr);
  virtual BYTE write8(ULONG addr,BYTE data);
protected:
};


#endif
