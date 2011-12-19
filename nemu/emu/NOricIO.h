#ifndef EMU_NORICIO_H
#define EMU_NORICIO_H


#include <elib/EMemIO.h>


class EIOVia6522;


class NOricIO : public EMemIO {
public:
  // Public Member Functions
  NOricIO(NOricHandler *parent);
  virtual ~NOricIO();
  virtual void reset();
  virtual void io_cycle();
  virtual void ioregs();
  virtual BYTE write8(ULONG addr,BYTE data);
  virtual BYTE read8(ULONG addr);
  // Public Data Members
protected:
  // Private Member Functions
  // Private Data Members
  EIOVia6522 *via;
};


#endif
