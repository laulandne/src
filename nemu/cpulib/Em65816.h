#ifndef CPULIB_EM65816_H
#define CPULIB_EM65816_H


#include <cpulib/Em6502.h>


// Interrupt vectors
#define E65816_INT_EIRQ   0xfffe
#define E65816_INT_RESET  0xfffc
#define E65816_INT_ENMI   0xfffa
#define E65816_INT_EABORT 0xfff8
#define E65816_INT_ECOP   0xfff4
#define E65816_INT_NIRQ   0xffee
#define E65816_INT_NNMI   0xffea
#define E65816_INT_NABORT 0xffe8
#define E65816_INT_NBRK   0xffe6
#define E65816_INT_NCOP   0xffe4


// Address modes
#define E65816_NONE 0
#define E65816_ZP   1
#define E65816_ZPZ  2
#define E65816_ZPX  3
#define E65816_ZPY  4
#define E65816_IMM  5
#define E65816_ABS  6
#define E65816_ABSX 7
#define E65816_ABSY 8
#define E65816_IND  9
#define E65816_INDX 10
#define E65816_INDY 11
#define E65816_INDZ 12
#define E65816_BXX  13
#define E65816_WREL 14
#define E65816_JABS 15
#define E65816_JIND 16
#define E65816_ABSINDX 17
#define E65816_INDYSP 18
#define E65816_SP   19
#define E65816_ZPWD 20
#define E65816_ABSWD 21
#define E65816_ABSL  22
#define E65816_JABSL  23
#define E65816_ZPBXX 24
#define E65816_JINDX 25
#define E65816_ABSW  26
#define E65816_IMMW  27
#define E65816_ABSLX  28
#define E65816_SR  29
#define E65816_IMMA  30
#define E65816_IMMX  31
#define E65816_IMMY  32
#define E65816_UNKNOWN 0xff


// Status bits
#define E65816_STAT_C  1
#define E65816_STAT_Z  2
#define E65816_STAT_I  4
#define E65816_STAT_D  8
#define E65816_STAT_Break  16
#define E65816_STAT_Extra  32
#define E65816_STAT_V  64
#define E65816_STAT_N  128
#define E65816_STAT_ALL 0xff
// For native mode
#define E65816_STAT_X  16
#define E65816_STAT_M  32



#ifndef A_BIG_ENDIAN
struct Em65816StatusBits  {
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
struct Em65816StatusBits  {
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


class Em65816 : public ECPU8Bit
{
public:
  // Public member functions
  Em65816();
  virtual ~Em65816();
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
  virtual void setBitness(unsigned int val);
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
  // NOTE: We've got our own stack handing funcs because of the wider address space
  virtual void stack_push(BYTE val);
  virtual BYTE stack_pop();
  void goEmulation();
  void goNative();
  unsigned int mapRead8(ULONG addr,bool echo=true);
  unsigned int mapRead16l(ULONG addr,bool echo=true);
  void mapWrite8(ULONG addr,unsigned int val,bool echo=true);
  void mapWrite16l(ULONG addr,unsigned int val,bool echo=true);
  virtual unsigned int read24code(ULONG addr);
  //virtual void write24code(ULONG addr,unsigned int val);
  virtual unsigned int read24data(ULONG addr);
  //virtual void write24data(ULONG addr,unsigned int val);
  virtual unsigned int readv(ULONG addr,unsigned int size);
  virtual void writev(ULONG addr,unsigned int val,unsigned int size);
  void adj_op_len(bool n);
  void fixOpLen(unsigned int mode);
  // Private data members
  unsigned int dbr;  // data offset (in bytes)
  unsigned int pbr;  // program offset (in bytes)
  UINT16 dp;  // direct page addr in bytes
  volatile BYTE statusReg;  // status
  volatile Em65816StatusBits *sbits;
  UINT16 a;
  UINT16 x,y;
  BYTE bHolder;
  unsigned int pbrHolder,dbrHolder,dpHolder;
  bool m16Bit,x16Bit;
  bool nativeMode;
  unsigned int mSizeMask,xSizeMask;
  unsigned int mHighBit,xHighBit;
  unsigned int mHalfBit,xHalfBit;
  unsigned int dpMask;
};


#endif
