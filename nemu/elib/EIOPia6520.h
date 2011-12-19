#ifndef ELIB_EIOPia6520_H
#define ELIB_EIOPia6520_H


#include "EIOPia.h"


class EIOPia6520 : public EIOPia
{
public:
  EIOPia6520(EHandler *parent, ULONG address);
  virtual void ioregs();
  virtual void io_cycle();
  virtual void reset();
  virtual BYTE write8Reg(unsigned int reg,BYTE data);
  virtual BYTE read8Reg(unsigned int reg);
protected:
};


#endif
