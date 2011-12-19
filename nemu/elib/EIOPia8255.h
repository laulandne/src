#ifndef ELIB_EIOPia8255_H
#define ELIB_EIOPia8255_H


#include "EIOPia.h"


class EIOPia8255 : public EIOPia
{
public:
  EIOPia8255(EHandler *parent, ULONG address);
  virtual void ioregs();
  virtual void io_cycle();
  virtual void reset();
  virtual BYTE write8Reg(unsigned int reg,BYTE data);
  virtual BYTE read8Reg(unsigned int reg);
protected:
};


#endif
