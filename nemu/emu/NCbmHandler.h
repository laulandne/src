#ifndef EMU_NCBMHANDLER_H
#define EMU_NCBMHANDLER_H


#include <elib/EHandler.h>


class ERom;
class NCbmKernHelper;


#define N_CBM_D000_IO   0
#define N_CBM_D000_RAM  1
#define N_CBM_D000_CHAR 2


#define NCBMDOS_NONE    0
#define NCBMDOS_CLOSED  0
#define NCBMDOS_ERROR   1
#define NCBMDOS_TALK    2
#define NCBMDOS_LISTEN  3
#define NCBMDOS_OPEN    4
#define NCBMDOS_UNKNOWN 5
#define NCBMDOS_COMMAND 6
#define NCBMDOS_DIRECT  7


#define NCBM_NUM_CHANNELS 256

#define NCBM_MAX_KEY_ROWS 16


// Types of I/O Emulation
#define N_CBM_IO_NONE      0x00
#define N_CBM_IO_C64       0x01
#define N_CBM_IO_VIC       0x02
#define N_CBM_IO_TED       0x04
#define N_CBM_IO_C128      0x08
#define N_CBM_IO_C65       0x10
#define N_CBM_IO_PET       0x20
#define N_CBM_IO_B         0x40
#define N_CBM_IO_ALL       0xff


class NCbmHandler : public EHandler {
friend class NCBMVideo;
friend class NCBMIO;
friend class NCBMIOLow;
friend class NCBMIOHigh;
public:
  // Public Member Functions
  NCbmHandler();
  virtual ~NCbmHandler();
  //
  virtual bool read8ret(ULONG addr,BYTE *ret);
  virtual bool write8ret(ULONG addr,BYTE val,BYTE *ret);
  //
  virtual void init();
  static bool recognize(const char *name);
  virtual void keypress(int c);
  virtual void setKind(const char *name);
  virtual const char *getName();
  //
  void handleVic3MapC65(BYTE d030Byte);
  void handleMMUC264(BYTE cr);
  void handleMMUC128(BYTE cr);
  BYTE getCRC128(unsigned int num);
  void setCRC128(unsigned int num,BYTE val);
  void setCommonC128(unsigned int val);
  bool getC64Mode() const { return c64Mode; }
  void setC64Mode(bool t) { c64Mode=t; }
  //
  // Public Data Members
  BYTE key_max,key_count;
  BYTE keys[NCBM_MAX_KEY_ROWS];
  unsigned int joyKey[2];
  unsigned int keyScan;
private:
  // Private Member Functions
  virtual void io_init(BYTE type);
  //
  void initMMUC264();
  void initMMUC128();
  void initMMUC65();
  BYTE read8ZeroOne(ULONG addr);
  BYTE write8ZeroOne(ULONG addr,BYTE val);
  // Private Data Members
  bool c64Mode;  // For machines that can go into 64 mode (and the 64 itself)
  bool mmuInited;
  BYTE mmuCR;
  BYTE c128pcr[4];
  bool c128io,c128lo,c128mid,c128hi;
  bool c65magic;
  BYTE c128bank;
  // NOTE: Not all of these rom's will be used by all emulations
  ERom *kernRom,*basARom,*basBRom;
  ERom *editRom,*charRom,*monitorRom;
  ERom *magicRom,*dosRom,*graphicsRom;
  EMemAbstract *ioMem,*ramHandle;
  unsigned int basicStartp,basicEndp;
  bool mapLock;
  bool loram,hiram,charen;
  BYTE value0,value1;
};


#endif
