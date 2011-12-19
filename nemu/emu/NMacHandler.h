#ifndef EMU_NMACHANDLER_H
#define EMU_NMACHANDLER_H


#include <elib/Em68kHandler.h>

#include "NMacZone.h"
#include "NMacApp.h"


// Types of I/O Emulation
#define N_MAC_IO_NONE  0x00
#define N_MAC_IO_PLUS 0x01
#define N_MAC_IO_GENERIC24 0x02
#define N_MAC_IO_GENERIC32 0x03
#define N_MAC_IO_MAC68k_NUBUS 0x04
#define N_MAC_IO_POWERMAC_NUBUS 0x05
#define N_MAC_IO_POWERMAC_PCI 0x06


// Location in memory that PEF image is loaded if needed
#define N_MAC_PEF_OFF  0x100000


class NMacHandler : public Em68kHandler {
public:
  // Public Member Functions
  NMacHandler();
  virtual ~NMacHandler();
  //
  static unsigned int name2type(const char *name);
  static bool recognize(const char *name);
  virtual void setKind(const char *name);
  virtual const char *getName();
  //
  virtual void my_reset();
  virtual void my_reset_postCPU();
  virtual bool systemCall(unsigned int num=0);
  virtual bool interp(const char *cmd,const char *a1,const char *a2,const char *a3);
  virtual bool ioDisHelper(ULONG addr);
  //
  virtual bool read8ret(ULONG addr,BYTE *ret);
  virtual bool write8ret(ULONG addr,BYTE val,BYTE *ret);
  //
  bool isCFM() { return CFM; }
  bool isPPC() { return iPPC; }
  bool isPCI() { return PCI; }
  //
  // Public Data Members
  //
private:
  //
  // Private Member Functions
  virtual void io_init(BYTE type);
  //
  void setupMacWorld();
  void showHeaps();
  void showZones();
  void addGlobalSymbols();
  //
  // Private Data Members
  bool iPPC,PCI,CFM;
  NMacZone sysZone;
  NMacApp appZone;
  unsigned int stackMax;
  bool didGlobalSymbols;
  //
};


#endif
