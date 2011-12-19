#ifndef EMU_NAMIGAIO_H
#define EMU_NAMIGAIO_H


#include <elib/EMemIO.h>


class NAmigaHandler;
class EIOCia8520;


class NAmigaIO : public EMemIO {
public:
  // Public Member Functions
  NAmigaIO(NAmigaHandler *parent);
  virtual ~NAmigaIO();
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
  BYTE write8_cia(ULONG address, BYTE value);
  BYTE write8_rtc(ULONG address, BYTE value);
  BYTE write8_custom(ULONG address, BYTE value);
  BYTE write8_autoconfig(ULONG address, BYTE value);
  BYTE read8_autoconfig(ULONG address);
  BYTE read8_custom(ULONG address);
  BYTE read8_rtc(ULONG address);
  BYTE read8_cia(ULONG address);
  BYTE remapRom(ULONG address);
  //
  // Private Data Members
  EIOCia8520 *cia1,*cia2;
  bool lastLEDState;
};


#endif
