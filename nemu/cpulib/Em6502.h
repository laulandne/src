#ifndef CPULIB_EM6502_H
#define CPULIB_EM6502_H


#include <cpulib/ECPU8Bit.h>


class EMem;
class EMap;
class EMemMap;


// Interrupt vectors
#define E6502_INT_IRQ     0xfffe
#define E6502_INT_RESET   0xfffc
#define E6502_INT_NMI     0xfffa


// Address modes
#define E6502_NONE 0
#define E6502_ZP   1
#define E6502_ZPZ  2
#define E6502_ZPX  3
#define E6502_ZPY  4
#define E6502_IMM  5
#define E6502_ABS  6
#define E6502_ABSX 7
#define E6502_ABSY 8
#define E6502_IND  9
#define E6502_INDX 10
#define E6502_INDY 11
#define E6502_INDZ 12
#define E6502_BXX  13
#define E6502_WREL 14
#define E6502_JABS 15
#define E6502_JIND 16
#define E6502_ABSINDX 17
#define E6502_INDYSP 18
#define E6502_SP   19
#define E6502_ZPWD 20
#define E6502_ABSWD 21
#define E6502_ABSL  22
#define E6502_JABSL  23
#define E6502_ZPBXX 24
#define E6502_JINDX 25
#define E6502_ABSW  26
#define E6502_IMMW  27
#define E6502_UNKNOWN 0xff


// Status bits
#define E6502_STAT_C  1
#define E6502_STAT_Z  2
#define E6502_STAT_I  4
#define E6502_STAT_D  8
#define E6502_STAT_Break  16
#define E6502_STAT_Extra  32
#define E6502_STAT_V  64
#define E6502_STAT_N  128
#define E6502_STAT_ALL 0xff

// For the C65's 4510 cpu
#define E6502_STAT_ExtendDisable 32


#ifndef A_BIG_ENDIAN
struct Em6502StatusBits  {
  volatile unsigned int c:1;
  volatile unsigned int z:1;
  volatile unsigned int i:1;
  volatile unsigned int d:1;
  volatile unsigned int breakFlag:1;
  volatile unsigned int extraFlag:1;
  volatile unsigned int v:1;
  volatile unsigned int n:1;
};
#else
struct Em6502StatusBits  {
  volatile unsigned int n:1;
  volatile unsigned int v:1;
  volatile unsigned int extraFlag:1;
  volatile unsigned int breakFlag:1;
  volatile unsigned int d:1;
  volatile unsigned int i:1;
  volatile unsigned int z:1;
  volatile unsigned int c:1;
};
#endif


class Em6502 : public ECPU8Bit
{
public:
  // Public member functions
  Em6502();
  virtual ~Em6502();
  static bool recognize(const char *name);
  virtual const char *getName();
  virtual void setKind(const char *name);
  virtual void executeInstead(ULONG opcode);
  virtual void execute();
  virtual void handle_int(ULONG value);
  virtual void cause_int(ULONG value);
  virtual void ints();
  virtual bool setRegister(const char *name,ULONG value);
  virtual ULONG readRegister(const char *name);
  virtual ULONG disasm(ULONG loc);
  virtual void changeMap(EMemMapAbstract *mmap);
  virtual void nmi();
  virtual void reset();
  virtual void wregs();
  // Public data members
protected:
  // Private member functions
  virtual void init();
  virtual void handle_brk();
  virtual bool execMain();
  virtual void setStatusReg(unsigned int newValue);
  virtual BYTE getStatusReg();
  virtual void setStatusBits(BYTE mask);
  virtual void resetStatusBits(BYTE mask);
  virtual void handle_adc(unsigned int aa);
  virtual void handle_sbc(unsigned int aa);
  virtual UINT16 handle_adc_d(unsigned int aa, unsigned int bb);
  virtual UINT16 handle_sbc_d(unsigned int aa, unsigned int bb);
  virtual void set_flags_nz(BYTE i);
  virtual void set_flags_nzc(BYTE i, BYTE j);
  virtual void set_flags_nz(UINT16 i,UINT16 sizemask);
  virtual void set_flags_nzc(UINT16 i, UINT16 j,UINT16 sizemask);
  virtual void disasmAddrMode(ULONG loc,unsigned int mode);
  // Private data members
  BYTE a;
  BYTE x,y;
  volatile BYTE statusReg;  // status
  volatile Em6502StatusBits *sbits;
};


#endif
