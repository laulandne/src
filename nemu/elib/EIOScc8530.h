#ifndef ELIB_ESCC_H
#define ELIB_ESCC_H


#include <elib/EIOAcia.h>


class EIOScc8530 : public EIOAcia {
public:
  EIOScc8530(EHandler *parent, ULONG address);
  virtual void ioregs();
  virtual void io_cycle();
  virtual void reset();
  virtual BYTE write8Reg(unsigned int reg,BYTE data);
  virtual BYTE read8Reg(unsigned int reg);
protected:
};


#endif

