#ifndef ELIB_EIOCia6526_H
#define ELIB_EIOCia6526_H


#include "EIOVia6522.h"


class EIOCia6526 : public EIOVia6522
{
public:
  EIOCia6526(EHandler *parent, ULONG address);
  virtual void ioregs();
  virtual void io_cycle();
  virtual void reset();
  virtual BYTE write8Reg(unsigned int reg,BYTE data);
  virtual BYTE read8Reg(unsigned int reg);
protected:
  ULONG tod;
};


#endif
