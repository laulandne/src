#ifndef ELIB_EIOCHIP_H
#define ELIB_EIOCHIP_H


#include <elib/EMemIO.h>


class EIOChip : public EMemIO
{
public:
  // Public member functions
  EIOChip(EHandler *parent, ULONG address); // NOTE: address not used unless mapped!
  virtual ~EIOChip();
  virtual void ioregs();
  virtual void io_cycle();
  virtual void reset();
  void setReadNotify();
  void setWriteNotify();
  virtual BYTE write8(ULONG addr,BYTE data);
  virtual BYTE read8(ULONG addr);
  virtual BYTE write8Reg(unsigned int reg,BYTE data);
  virtual BYTE read8Reg(unsigned int reg);
  virtual void dontHandlePorts() { handlePorts=false; }
  virtual void dontHandleTimers() { handleTimers=false; }
  virtual void dontHandleSerial() { handleSerial=false; }
  // Public data members
protected:
  // Private member functions
  void init();
  void move_timer(ULONG address, UINT16 value);
  // Private data members
  BYTE irqen;
  unsigned int IFR;
  BYTE key_max,key_count;
  BYTE keys[8];
  unsigned int joyKey[2];
  unsigned int keyScan;
  BYTE reg,tval;
  bool readNotify,writeNotify;
  bool handleTimers,handlePorts,handleSerial;
};


#endif
