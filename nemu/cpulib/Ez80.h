#ifndef CPULIB_EZ80_H
#define CPULIB_EZ80_H


#include <cpulib/ECPU8Bit.h>


class EMem;
class EMap;
class EMemMap;


// Interrupt vectors
#define Ez80_INT_NMI    0xfffa
#define Ez80_INT_RESET  0xfffc
#define Ez80_INT_IRQ    0xfffe


// condition codes
#define Ez80_CC_NZ 0
#define Ez80_CC_Z  1
#define Ez80_CC_NC 2
#define Ez80_CC_C  3
#define Ez80_CC_NP 4
#define Ez80_CC_P  5
#define Ez80_CC_NS 6
#define Ez80_CC_S  7


// single regs
#define Ez80_REG_B 0
#define Ez80_REG_C 1
#define Ez80_REG_D 2
#define Ez80_REG_E 3
#define Ez80_REG_H 4
#define Ez80_REG_L 5
#define Ez80_REG_X 6
#define Ez80_REG_A 7
// Use readReg1 and writeReg1 for these...
#define Ez80_REG_B1 8
#define Ez80_REG_C1 9
#define Ez80_REG_D1 10
#define Ez80_REG_E1 11
#define Ez80_REG_H1 12
#define Ez80_REG_L1 13
#define Ez80_REG_A1 14


// reg pairs
#define Ez80_REG_BC 0
#define Ez80_REG_DE 1
#define Ez80_REG_HL 2
#define Ez80_REG_SP 3
// Use readReg1 and writeReg1 for these...
#define Ez80_REG_BC1 4
#define Ez80_REG_DE1 5
#define Ez80_REG_HL1 6


// Address modes
#define Ez80_NONE 0
#define Ez80_SPECIAL 1
#define Ez80_ILLEGAL 2
#define Ez80_CONST 3
#define Ez80_REGB 4
#define Ez80_REGW 5
#define Ez80_REGB_REGB 6
#define Ez80_REGW_REGW 7
#define Ez80_REGB_BYTE 8
#define Ez80_REGW_WORD 9
#define Ez80_INDREGW 10
#define Ez80_INDREGW_BYTE 11
#define Ez80_INDBYTE_REGB 12
#define Ez80_REGW_INDABS 13
#define Ez80_REGW_ABS 14
#define Ez80_INDABS_REGW 15
#define Ez80_INDABS_REGB 16
#define Ez80_REGB_INDABS 17
#define Ez80_REGB_INDREGW 18
#define Ez80_INDREGW_REGW 19
#define Ez80_INDREGW_REGB 20
#define Ez80_BYTE 21
#define Ez80_REGB_INDBYTE 22
#define Ez80_DJ_REL 23
#define Ez80_J_REL 24
#define Ez80_JABS 25
#define Ez80_ABS 26
#define Ez80_JCC_REL 27
#define Ez80_JCC_ABS 28
#define Ez80_REGB2 29
#define Ez80_REGB2 29
#define Ez80_A_REGB 30


// register sizes
#define Ez80_SIZE_BAD  0
#define Ez80_SIZE_BYTE 1
#define Ez80_SIZE_WORD 2


// Status bits
#define Ez80_STAT_S  1
#define Ez80_STAT_Z  2
#define Ez80_STAT_E1 4
#define Ez80_STAT_H  8
#define Ez80_STAT_E2 16
#define Ez80_STAT_P 32
#define Ez80_STAT_V 32  // same bit as P
#define Ez80_STAT_N  64
#define Ez80_STAT_C  128
#define Ez80_STAT_ALL 0xff


#ifndef A_BIG_ENDIAN
struct Ez80StatusBits  {
  volatile unsigned int s:1;
  volatile unsigned int z:1;
  volatile unsigned int e1:1;
  volatile unsigned int h:1;
  volatile unsigned int e2:1;
  volatile unsigned int p:1;
  volatile unsigned int n:1;
  volatile unsigned int c:1;
};
#else
struct Ez80StatusBits  {
  volatile unsigned int c:1;
  volatile unsigned int n:1;
  volatile unsigned int p:1;
  volatile unsigned int e2:1;
  volatile unsigned int h:1;
  volatile unsigned int e1:1;
  volatile unsigned int z:1;
  volatile unsigned int s:1;
};
#endif


class Ez80 : public ECPU8Bit
{
public:
  // Public member functions
  Ez80();
  virtual ~Ez80();
  static bool recognize(const char *name);
  virtual const char *getName();
  virtual void setKind(const char *name);
  virtual void execute();
  virtual void handle_int(ULONG value);
  virtual void cause_int(ULONG value);
  virtual void ints();
  virtual bool setRegister(const char *name,ULONG value);
  virtual ULONG readRegister(const char *name);
  virtual ULONG disasm(ULONG loc);
  virtual void reset();
  virtual void wregs();
  virtual void set_flags_zn(int i);
  virtual void set_flags_szhpn(int i);
  virtual void set_flags_szhpc(int i,int j);
  virtual void set_flags_szp(int i);
  virtual void set_flags_znc(int i, int j);
  // Public data members
protected:
  // Private member functions
  virtual void init();
  unsigned int dis_special(ULONG loc,BYTE topcode,unsigned int len);
  void disasmAddrMode(ULONG loc,unsigned int mode);
  void printCondCode(unsigned int num);
  void printReg(unsigned int num,unsigned int size=Ez80_SIZE_BYTE);
  char *getRegString(unsigned int num,unsigned int size=Ez80_SIZE_BYTE);
  unsigned int readReg(unsigned int n,unsigned int size=Ez80_SIZE_BYTE);
  void writeReg(unsigned int n,unsigned int val,unsigned int size=Ez80_SIZE_BYTE);
  unsigned int readReg1(unsigned int n,unsigned int size=Ez80_SIZE_BYTE);
  void writeReg1(unsigned int n,unsigned int val,unsigned int size=Ez80_SIZE_BYTE);
  void doOutByte(ULONG addr,BYTE val);
  BYTE doInByte(ULONG addr);
  void exec_adc(ULONG loc,unsigned int op);
  void exec_add(ULONG loc,unsigned int op);
  void exec_and(ULONG loc,unsigned int op);
  void exec_call(ULONG loc,unsigned int op);
  void exec_ccf(ULONG loc,unsigned int op);
  void exec_cp(ULONG loc,unsigned int op);
  void exec_cpl(ULONG loc,unsigned int op);
  void exec_daa(ULONG loc,unsigned int op);
  void exec_dec(ULONG loc,unsigned int op);
  void exec_di(ULONG loc,unsigned int op);
  void exec_djnz(ULONG loc,unsigned int op);
  void exec_ei(ULONG loc,unsigned int op);
  void exec_ex(ULONG loc,unsigned int op);
  void exec_exx(ULONG loc,unsigned int op);
  void exec_halt(ULONG loc,unsigned int op);
  void exec_in(ULONG loc,unsigned int op);
  void exec_inc(ULONG loc,unsigned int op);
  void exec_jp(ULONG loc,unsigned int op);
  void exec_jr(ULONG loc,unsigned int op);
  void exec_ld(ULONG loc,unsigned int op);
  void exec_nop(ULONG loc,unsigned int op);
  void exec_or(ULONG loc,unsigned int op);
  void exec_out(ULONG loc,unsigned int op);
  void exec_pop(ULONG loc,unsigned int op);
  void exec_push(ULONG loc,unsigned int op);
  void exec_ret(ULONG loc,unsigned int op);
  void exec_rla(ULONG loc,unsigned int op);
  void exec_rlca(ULONG loc,unsigned int op);
  void exec_rra(ULONG loc,unsigned int op);
  void exec_rrca(ULONG loc,unsigned int op);
  void exec_rst(ULONG loc,unsigned int op);
  void exec_sbc(ULONG loc,unsigned int op);
  void exec_scf(ULONG loc,unsigned int op);
  void exec_special(ULONG loc,unsigned int op);
  void exec_sub(ULONG loc,unsigned int op);
  void exec_xor(ULONG loc,unsigned int op);
  // Private data members
  UINT16 ix,iy;
  BYTE a;
  volatile BYTE f;  // Becayse sbits points to it
  BYTE b,c,d,e,h,l;
  BYTE a1;
  BYTE f1;
  BYTE b1,c1,d1,e1,h1,l1;
  BYTE i;
  volatile Ez80StatusBits *sbits;
  bool intsEnabled;
  unsigned int reg,reg2;  // for use when decoding...
};


#endif

