#ifndef EMU_NATARIIO_H
#define EMU_NATARIIO_H


#include <elib/EMemIO.h>


class EIOPia6520;
class ESoundIOPokey;


class NAtariIO : public EMemIO {
public:
  // Public Member Functions
  NAtariIO(NAtariHandler *parent);
  virtual ~NAtariIO();
  virtual void reset();
  virtual void io_cycle();
  virtual void ioregs();
  virtual BYTE write8(ULONG addr,BYTE data);
  virtual BYTE read8(ULONG addr);
  // Public Data Members
  ESoundIOPokey *pokey;
protected:
  // Private Member Functions
  BYTE strobe(ULONG address,BYTE val);
  BYTE write8_antic(ULONG address, BYTE value);
  BYTE write8_gtia(ULONG address, BYTE value);
  BYTE write8_pokey(ULONG address, BYTE value);
  BYTE write8_pia(ULONG address, BYTE value);
  BYTE read8_pia(ULONG address);
  BYTE read8_pokey(ULONG address);
  BYTE read8_gtia(ULONG address);
  BYTE read8_antic(ULONG address);
  // Private Data Members
  BYTE writeOnly;
  bool DLI_NMI,VBI_NMI;
  EIOPia6520 *pia;
};


#endif
