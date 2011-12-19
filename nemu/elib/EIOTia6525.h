#ifndef ELIB_EIOTIA6525_H
#define ELIB_EIOTIA6525_H


#include "EIOPia.h"


class EIOTia6525 : public EIOPia
{
public:
  EIOTia6525(EHandler *parent, ULONG address);
  virtual void ioregs();
  virtual void io_cycle();
  virtual void reset();
  virtual BYTE write8Reg(unsigned int reg,BYTE data);
  virtual BYTE read8Reg(unsigned int reg);
protected:
};


#endif
