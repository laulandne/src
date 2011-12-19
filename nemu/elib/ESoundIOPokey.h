#ifndef ELIB_EPOKEY_H
#define ELIB_EPOKEY_H


#include <elib/ESoundIOAbstract.h>


class ESoundIOPokey : public ESoundIOAbstract
{
public:
  ESoundIOPokey(EHandler *parent, ULONG address);
  virtual ~ESoundIOPokey();
  virtual BYTE write8(ULONG addr,BYTE data);
  virtual BYTE read8(ULONG addr);
  virtual void ioregs();
  virtual void io_cycle();
  virtual void reset();
  //
  BYTE getIRQen() const { return irq1en; }
  void setKeyCountMax() { key_count=key_max; }
  //
private:
  //
  BYTE irq1en,skctl;
  BYTE audf1,audf2,audf3,audf4;
  unsigned int key_count,key_max;
};


#endif
