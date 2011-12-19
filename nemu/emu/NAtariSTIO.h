#ifndef EMU_NATARISTIO_H
#define EMU_NATARISTIO_H


#include <elib/EMemIO.h>


class NAtariSTIO : public EMemIO {
public:
  // Public Member Functions
  NAtariSTIO(NAtariSTHandler *parent);
  virtual ~NAtariSTIO();
  //
  virtual void reset();
  virtual void io_cycle();
  virtual void ioregs();
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
  //
};


#endif
