#ifndef EMU_NACORNIO_H
#define EMU_NACORNIO_H


#include <elib/EMemIO.h>


class EIOPia8255;
class EIOVia6522;
class EIOAcia6850;
class DController827x;


class NAcornIO : public EMemIO {
public:
  // Public Member Functions
  NAcornIO(NAcornHandler *parent);
  virtual ~NAcornIO();
  virtual void reset();
  virtual void io_cycle();
  virtual void ioregs();
  virtual BYTE write8(ULONG addr,BYTE data);
  virtual BYTE read8(ULONG addr);
  // Public Data Members
protected:
  // Private Member Functions
  BYTE write8atom(ULONG addr,BYTE data);
  BYTE write8bbc(ULONG addr,BYTE data);
  BYTE read8atom(ULONG addr);
  BYTE read8bbc(ULONG addr);
  // Private Data Members
  EIOPia8255 *pia;  // for atom
  EIOVia6522 *viaa;  // for bbc
  EIOVia6522 *viab;  // for bbc
  EIOAcia6850 *acia; // for bbc
  DController827x *diskio;  // for bbc
};


#endif
