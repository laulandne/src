#ifndef ELIB_ESOUNDIOYMXXXX_H
#define ELIB_ESOUNDIOYMXXXX_H


#include <elib/ESoundIOAbstract.h>


class ESoundIOYmxxxx : public ESoundIOAbstract
{
public:
  ESoundIOYmxxxx(EHandler *parent, ULONG address);
  virtual ~ESoundIOYmxxxx();
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
