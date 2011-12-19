#ifndef EMU_NCOCOIO_H
#define EMU_NCOCOIO_H


#include <elib/EMemIO.h>


class NCocoIO : public EMemIO {
public:
  // Public Member Functions
  NCocoIO(NCocoHandler *parent);
  virtual ~NCocoIO();
  virtual void reset();
  virtual void io_cycle();
  virtual void ioregs();
  virtual BYTE write8(ULONG addr,BYTE data);
  virtual BYTE read8(ULONG addr);
  // Public Data Members
protected:
  // Private Member Functions
  // Private Data Members
};


#endif
