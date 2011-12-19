#ifndef ELIB_EIOPIA_H
#define ELIB_EIOPIA_H


#include "EIOChip.h"


#define EIOPIA_MAX_NUM_PORTS 4
#define EIOPIA_MAX_NUM_TIMERS 4


struct EIOPiaTimer
{
  int value;
  unsigned int irq;
  unsigned int state;
  int latch;
  bool free;
  bool run;
};


class EIOPia : public EIOChip
{
public:
  EIOPia(EHandler *parent, ULONG address);
  //
  virtual void ioregs();
  virtual void io_cycle();
  virtual void reset();
  //
  virtual BYTE write8Reg(unsigned int reg,BYTE data);
  virtual BYTE read8Reg(unsigned int reg);
  //
  void writePortByte(unsigned int pnum,unsigned int b);
  unsigned int readPortByte(unsigned int pnum);
  //
protected:
  //
  void cycle_timers();
  //
  BYTE portData[EIOPIA_MAX_NUM_PORTS];
  BYTE portDDR[EIOPIA_MAX_NUM_PORTS];
  EIOPiaTimer timer[EIOPIA_MAX_NUM_TIMERS];
  unsigned int irq_int;
  unsigned int numTimers;
  unsigned int numPorts;
  //
};


#endif
