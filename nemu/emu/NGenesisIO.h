#ifndef EMU_NGENESISIO_H
#define EMU_NGENESISIO_H


#include <elib/EMemIO.h>


class NGenesisIO : public EMemIO {
public:
  // Public Member Functions
  NGenesisIO(NGenesisHandler *parent);
  virtual ~NGenesisIO();
  //
  virtual void reset();
  virtual void io_cycle();
  virtual void ioregs();
  //
  virtual BYTE write8(ULONG addr,BYTE data);
  virtual BYTE read8(ULONG addr);
  //
  // Public Data Members
  //
private:
  //
  // Private Member Functions
  //
  // Private Data Members
  unsigned int segaLatch;
  //
};


#endif
