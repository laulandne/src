#ifndef EMU_NPCIO_H
#define EMU_NPCIO_H


#include <elib/EMemIO.h>


class NPcIO : public EMemIO {
public:
  // Public Member Functions
  NPcIO(NPcHandler *parent);
  virtual BYTE write8(ULONG addr,BYTE data);
  virtual BYTE read8(ULONG addr);
  // Public Data Members
private:
  // Private Member Functions
  BYTE write8_pc_generic(ULONG address, BYTE value);
  BYTE read8_pc_generic(ULONG address);
  // Private Data Members
};


#endif
