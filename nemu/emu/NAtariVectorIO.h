#ifndef EMU_NATARIVECTORIO_H
#define EMU_NATARIVECTORIO_H


#include <elib/EMemIO.h>


class NAtariVectorIO : public EMemIO {
public:
  // Public Member Functions
  NAtariVectorIO(NAtariVectorHandler *parent);
  virtual BYTE write8(ULONG addr,BYTE data);
  virtual BYTE read8(ULONG addr);
  // Public Data Members
private:
  // Private Member Functions
  virtual BYTE write8_asteroids(ULONG address, BYTE value);
  virtual BYTE write8_tempest(ULONG address, BYTE value);
  virtual BYTE read8_tempest(ULONG address);
  virtual BYTE read8_asteroids(ULONG address);
  // Private Data Members
};


#endif
