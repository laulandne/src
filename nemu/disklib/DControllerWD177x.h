#ifndef ELIB_ECONTROLLERWD177X_H
#define ELIB_ECONTROLLERWD177X_H


#include <disklib/DControllerAbstract.h>


class DControllerWD177x : public DControllerAbstract {
public:
  DControllerWD177x(EHandler *parent, ULONG address);
  virtual ~DControllerWD177x();
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

