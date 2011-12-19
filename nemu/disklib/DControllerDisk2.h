#ifndef ELIB_EDISK2_H
#define ELIB_EDISK2_H


#include <disklib/DControllerAbstract.h>


class DControllerDisk2 : public DControllerAbstract {
public:
  DControllerDisk2(EHandler *parent, ULONG address);
  virtual ~DControllerDisk2();
  //
  virtual void ioregs();
  virtual void io_cycle();
  virtual void reset();
  virtual BYTE write8Reg(unsigned int reg,BYTE data);
  virtual BYTE read8Reg(unsigned int reg);
  //
  virtual void strobe8Reg(unsigned int reg);
  //
protected:
  //
  BYTE readEncodedDiskByte();
  void readAndEncodeWholeTrack(unsigned int trackNum);
};


#endif

