#ifndef EMU_NATARIARCADEIO_H
#define EMU_NATARIARCADEIO_H


#include <elib/EMemIO.h>


class ESoundIOPokey;


class NAtariArcadeIO : public EMemIO {
public:
  // Public Member Functions
  NAtariArcadeIO(NAtariArcadeHandler *parent);
  virtual ~NAtariArcadeIO();
  virtual BYTE write8(ULONG addr,BYTE data);
  virtual BYTE read8(ULONG addr);
  virtual void reset();
  virtual void io_cycle();
  virtual void ioregs();
  // Public Data Members
private:
  // Private Member Functions
  BYTE write8_centi(ULONG address, BYTE value);
  BYTE write8_mili(ULONG address, BYTE value);
  BYTE read8_mili(ULONG address);
  BYTE read8_centi(ULONG address);
  // Private Data Members
  bool vflag;
  ESoundIOPokey *pokey1,*pokey2;
};


#endif

