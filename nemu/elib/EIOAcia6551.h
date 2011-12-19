#ifndef ELIB_EIOACIA6551_H
#define ELIB_EIOACIA6551_H


#include "EIOAcia.h"


class EIOAcia6551 : public EIOAcia
{
public:
  EIOAcia6551(EHandler *parent, ULONG address);
  virtual void ioregs();
  virtual void io_cycle();
  virtual void reset();
  virtual BYTE write8Reg(unsigned int reg,BYTE data);
  virtual BYTE read8Reg(unsigned int reg);
protected:
};


#endif
