#ifndef ELIB_ESOUNDIOSID_H
#define ELIB_ESOUNDIOSID_H


#include <elib/ESoundIOAbstract.h>


class ESoundIOSid : public ESoundIOAbstract
{
public:
  ESoundIOSid(EHandler *parent, ULONG address);
  virtual ~ESoundIOSid();
  virtual BYTE write8(ULONG addr,BYTE data);
  virtual BYTE read8(ULONG addr);
  virtual void ioregs();
  virtual void io_cycle();
  virtual void reset();
  //
private:
  //
};


#endif
