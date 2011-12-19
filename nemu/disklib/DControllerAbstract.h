#ifndef ELIB_EDISKCONTROLLER_H
#define ELIB_EDISKCONTROLLER_H


#include <elib/EIOChip.h>


// This should be big enough for most uses...
#define EDISK_BUFFER_SIZE 16*1024


class DControllerAbstract : public EIOChip {
public:
  DControllerAbstract(EHandler *parent, ULONG address);
  virtual ~DControllerAbstract();
  //
  virtual void ioregs();
  virtual void io_cycle();
  virtual void reset();
  virtual BYTE write8Reg(unsigned int reg,BYTE data);
  virtual BYTE read8Reg(unsigned int reg);
  //
protected:
  //
  virtual void init();
  //
  unsigned int motorState,previousMotorState;
  bool stepperOn,motorOn;
  bool writeMode;
  unsigned int curDriveNum;
  unsigned int headTrackIsOn;
  BYTE dataLatch;
  unsigned int trackBufferPtr;
  BYTE trackBuffer[EDISK_BUFFER_SIZE];
  BYTE tempBuffer[EDISK_BUFFER_SIZE];\
  //
};


#endif

