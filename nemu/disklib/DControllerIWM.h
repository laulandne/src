#ifndef ELIB_EIWM_H
#define ELIB_EIWM_H


#include <disklib/DControllerAbstract.h>


class DControllerIWM : public DControllerAbstract {
public:
  DControllerIWM(EHandler *parent, ULONG address);
  virtual ~DControllerIWM();
  //
  virtual void ioregs();
  virtual void io_cycle();
  virtual void reset();
  virtual BYTE write8Reg(unsigned int reg,BYTE data);
  virtual BYTE read8Reg(unsigned int reg);
  //
protected:
  //
  void strobe8Reg(unsigned int reg);
  //
  bool ca0,ca1,ca2;
  bool lstrb;
  bool enable;
  bool select;
  bool q6,q7;
  //
};


#endif

