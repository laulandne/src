#ifndef EMU_NPALMIO_H
#define EMU_NPALMIO_H


#include <elib/EMemIO.h>


class NPalmIO : public EMemIO {
public:
  // Public Member Functions
  NPalmIO(NPalmHandler *parent);
  virtual ~NPalmIO();
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
