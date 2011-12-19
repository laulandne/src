#ifndef ELIB_EFDC65_H
#define ELIB_EFDC65_H


#include <disklib/DControllerAbstract.h>


class DController65 : public DControllerAbstract {
public:
  DController65(EHandler *parent, ULONG address);
  virtual ~DController65();
  //
  virtual void ioregs();
  virtual void io_cycle();
  virtual void reset();
  virtual BYTE write8Reg(unsigned int reg,BYTE data);
  virtual BYTE read8Reg(unsigned int reg);
  //
protected:
};


#endif

