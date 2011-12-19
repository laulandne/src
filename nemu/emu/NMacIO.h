#ifndef EMU_NMACIO_H
#define EMU_NMACIO_H


#include <elib/EMemIO.h>


class EIOVia6522;
class EIOScc8530;
class DControllerIWM;


class NMacIO : public EMemIO {
public:
  // Public Member Functions
  NMacIO(NMacHandler *parent);
  virtual ~NMacIO();
  virtual void reset();
  virtual void io_cycle();
  virtual void ioregs();
  virtual BYTE write8(ULONG addr,BYTE data);
  virtual BYTE read8(ULONG addr);
  // Public Data Members
private:
  // Private Member Functions
  BYTE write8_macplus(ULONG address, BYTE value);
  BYTE write8_mac24(ULONG address, BYTE value);
  BYTE write8_mac32(ULONG address, BYTE value);
  BYTE read8_macplus(ULONG address);
  BYTE read8_mac24(ULONG address);
  BYTE read8_mac32(ULONG address);
  BYTE write8_scc(ULONG address, BYTE value);
  BYTE write8_scsi(ULONG address, BYTE value);
  BYTE write8_iwm(ULONG address, BYTE value);
  BYTE write8_via(ULONG address, BYTE value);
  BYTE write8_io24(ULONG address, BYTE value);
  BYTE write8_io32(ULONG address, BYTE value);
  BYTE write8_slot24(ULONG address,unsigned int num,BYTE value);
  BYTE write8_slot32(ULONG address,unsigned int num,BYTE value);
  BYTE read8_slot32(ULONG address,unsigned int num);
  BYTE read8_slot24(ULONG address,unsigned int num);
  BYTE read8_io32(ULONG address);
  BYTE read8_io24(ULONG address);
  BYTE read8_via(ULONG address);
  BYTE read8_iwm(ULONG address);
  BYTE read8_scsi(ULONG address);
  BYTE read8_scc(ULONG address);
  BYTE remapRom(ULONG address);
  //
  // Private Data Members
  EIOVia6522 *via1,*via2;
  DControllerIWM *iwm;
  EIOScc8530 *scc;
  ULONG romAddr,romSize;
};


#endif
