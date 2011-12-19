#ifndef ELIB_EIOVia6522_H
#define ELIB_EIOVia6522_H


#include "EIOPia.h"


class EIOVia6522 : public EIOPia
{
public:
  EIOVia6522(EHandler *parent, ULONG address);
  virtual void ioregs();
  virtual void io_cycle();
  virtual void reset();
  virtual BYTE write8Reg(unsigned int reg,BYTE data);
  virtual BYTE read8Reg(unsigned int reg);
protected:
};


#endif
