#ifndef ELIB_EIOACIA_H
#define ELIB_EIOACIA_H


#include "EIOChip.h"


#define EIOACIA_BUFFER_SIZE 256
#define EIOACIA_MAX_NUM_PORTS 2


class EIOAcia : public EIOChip
{
public:
  EIOAcia(EHandler *parent, ULONG address);
  //
  virtual void ioregs();
  virtual void io_cycle();
  virtual void reset();
  //
  virtual BYTE write8Reg(unsigned int reg,BYTE data);
  virtual BYTE read8Reg(unsigned int reg);
  //
  void setBaud(unsigned int pnum,unsigned int b);
  unsigned int getBaud(unsigned int pnum);
  void writeSerialByte(unsigned int pnum,unsigned int b);
  unsigned int readSerialByte(unsigned int pnum);
  //
protected:
  //
  unsigned int baud[EIOACIA_MAX_NUM_PORTS];
  BYTE serialBuffer[EIOACIA_MAX_NUM_PORTS][EIOACIA_BUFFER_SIZE];
  unsigned int serialBufPtr[EIOACIA_MAX_NUM_PORTS];
  //
};


#endif
