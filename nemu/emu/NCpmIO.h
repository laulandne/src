#ifndef EMU_NCPMIO_H
#define EMU_NCPMIO_H


#include <elib/EMemIO.h>


class NCpmIO : public EMemIO {
public:
  // Public Member Functions
  NCpmIO(NCpmHandler *parent);
  virtual ~NCpmIO();
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
