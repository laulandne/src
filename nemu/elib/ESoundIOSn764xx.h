#ifndef ELIB_ESOUNDIOSN764XX_H
#define ELIB_ESOUNDIOSN764XX_H


#include <elib/ESoundIOAbstract.h>


class ESoundIOSn764xx : public ESoundIOAbstract
{
public:
  ESoundIOSn764xx(EHandler *parent, ULONG address);
  virtual ~ESoundIOSn764xx();
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
