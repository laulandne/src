#ifndef EMU_NCBMIO_H
#define EMU_NCBMIO_H


#include <elib/EMemIO.h>
#include <elib/EIOChip.h>


class ERam;
class DController65;
class EIOPia;
class ESoundIOSid;
class EIOAcia6551;
class EIOTia6525;


class NCbmIO : public EMemIO {
public:
  // Public Member Functions
  NCbmIO(NCbmHandler *parent);
  virtual ~NCbmIO();
  virtual BYTE write8(ULONG addr,BYTE data);
  virtual BYTE read8(ULONG addr);
  virtual void io_cycle();
  virtual void reset();
  virtual void ioregs();
  //
  // Public Data Members
  bool colorRamExtend;  // c65: do we have cia's or another 1k of color ram
  bool paletteMode;  // c65: palette mode or standard c64 colors?
private:
  // Private Member Functions
  void move_timer(ULONG address, UINT16 value);
  //
  BYTE write8_vic2(ULONG address, BYTE value);
  BYTE write8_sid(ULONG address, BYTE value);
  BYTE write8_cias(ULONG address, BYTE value);
  BYTE write8_vic1(ULONG address, BYTE value);
  BYTE write8_tias(ULONG address, BYTE value);
  BYTE write8_vias(ULONG address, BYTE value);
  BYTE write8_c65uart(ULONG address, BYTE value);
  BYTE write8_c65dma(ULONG address, BYTE value);
  BYTE write8_c65rec(ULONG address, BYTE value);
  BYTE write8_c128mmu(ULONG address, BYTE value);
  BYTE write8_c128vdc(ULONG address, BYTE value);
  BYTE write8_via(ULONG address, BYTE value);
  BYTE write8_petvdc(ULONG address, BYTE value);
  BYTE write8_pias(ULONG address, BYTE value);
  BYTE write8_c128mmuff00(ULONG address, BYTE value);
  BYTE write8_c264(ULONG address, BYTE value);
  //
  BYTE read8_vic2(ULONG address);
  BYTE read8_sid(ULONG address);
  BYTE read8_tias(ULONG address);
  BYTE read8_cias(ULONG address);
  BYTE read8_vic1(ULONG address);
  BYTE read8_vias(ULONG address);
  BYTE read8_c65uart(ULONG address);
  BYTE read8_c65dma(ULONG address);
  BYTE read8_c65rec(ULONG address);
  BYTE read8_c128mmu(ULONG address);
  BYTE read8_c128vdc(ULONG address);
  BYTE read8_c264(ULONG address);
  BYTE read8_pias(ULONG address);
  BYTE read8_via(ULONG address);
  BYTE read8_petvdc(ULONG address);
  BYTE read8_c128mmuff00(ULONG address);
  //
  void c65DoDMA(ULONG address);
  //
  // Private Data Members
  BYTE irq1en,irq2en;
  unsigned int ta_int,tb_int,tc_int;
  unsigned int state1a,state1b,state1c,state2a,state2b;
  int tlatch1a,tlatch1b,tlatch1c,tlatch2a,tlatch2b;
  int timer1a,timer1b,timer1c,timer2a,timer2b;
  bool free1a,free1b,free1c,free2a,free2b;
  bool run1a,run1b,run1c,run2a,run2b;
  unsigned int IFR1,IFR2;
  unsigned int vic3ModeKey;  // c65: used when switching to/from native mode
  bool vic3ModeFlag; // c65: vic3 chip in native mode?
  BYTE *vic3Red,*vic3Green,*vic3Blue;  // c65: the palette regs
  ERam *vdcMem,*vdc; // c128's 80 column chip
  BYTE vdcReg;
  unsigned int vdcAddr;
  DController65 *fdc65;
  BYTE c65_df[256];
  BYTE dma0,dma1,dma2;
  EIOPia *xia1,*xia2;
  EIOAcia6551 *acia;
  EIOPia *tpia1,*tpia2;
  //ESoundIOSid *sid1,*sid2;
  bool RAST_IRQ;
  unsigned int raster_check;
  unsigned int nscreen_addr,nchar_addr,ncolor_addr;  // last known values
};


#endif

