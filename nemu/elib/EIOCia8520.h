#ifndef ELIB_EIOCia8520_H
#define ELIB_EIOCia8520_H


#include "EIOVia6522.h"


class EIOCia8520 : public EIOVia6522
{
public:
  EIOCia8520(EHandler *parent, ULONG address);
  virtual void ioregs();
  virtual void io_cycle();
  virtual void reset();
  virtual BYTE write8Reg(unsigned int reg,BYTE data);
  virtual BYTE read8Reg(unsigned int reg);
protected:
  ULONG tod;
};


#endif
