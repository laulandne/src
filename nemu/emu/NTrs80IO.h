#ifndef EMU_NTRS80IO_H
#define EMU_NTRS80IO_H


#include <elib/EMemIO.h>


class NTrs80IO : public EMemIO {
public:
  // Public Member Functions
  NTrs80IO(NTrs80Handler *parent);
  virtual ~NTrs80IO();
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
