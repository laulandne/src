#ifndef ELIB_ECONTROLLER827X_H
#define ELIB_ECONTROLLER827X_H


#include <disklib/DControllerAbstract.h>


class DController827x : public DControllerAbstract {
public:
  DController827x(EHandler *parent, ULONG address);
  virtual ~DController827x();
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

