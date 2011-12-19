#ifndef EMU_NFAMICOMIO_H
#define EMU_NFAMICOMIO_H


#include <elib/EMemIO.h>


class NFamicomIO : public EMemIO {
public:
  // Public Member Functions
  NFamicomIO(NFamicomHandler *parent);
  virtual ~NFamicomIO();
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
  //
};


#endif
