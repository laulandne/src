#ifndef CPULIB_EM6800_H
#define CPULIB_EM6800_H


#include <cpulib/ECPU8Bit.h>


// Instruction flags
#define E6809_ONLY 1
#define E6811_ONLY 2


// Interrupt vectors
#define E6800_INT_SWI2   0xfff2
#define E6800_INT_SWI3   0xfff4
#define E6800_INT_FIRQ   0xfff6
#define E6800_INT_IRQ    0xfff8
#define E6800_INT_SWI    0xfffa
#define E6800_INT_NMI    0xfffc
#define E6800_INT_RESET  0xfffe


// Address modes
#define E6800_NONE     0
#define E6800_UNKNOWN  1
#define E6800_SPECIAL  2
#define E6800_ILLEGAL  3
#define E6800_VARIOUS  4
#define E6800_IMM8     5
#define E6800_IMM16    6
#define E6800_DP       7
#define E6800_ABS      8
#define E6800_REGS     9
#define E6800_REGMASK  10
#define E6800_REL      11
#define E6800_REL16    12
#define E6800_INDEXED  13
#define E6800_EXTEND   14


// Postbyte addressing modes
#define E6800_POST_AUTOINC1 0
#define E6800_POST_AUTOINC2 1
#define E6800_POST_AUTODEC1 2
#define E6800_POST_AUTODEC2 3
#define E6800_POST_NO_OFFSET_R 4
#define E6800_POST_B_OFFSET_R 5
#define E6800_POST_A_OFFSET_R 6
#define E6800_POST_8BIT_OFFSET_R 8
#define E6800_POST_16BIT_OFFSET_R 9
#define E6800_POST_D_OFFSET_R 11
#define E6800_POST_8BIT_OFFSET_PC 12
#define E6800_POST_16BIT_OFFSET_PC 13
#define E6800_POST_IND_AUTOINC2 17
#define E6800_POST_IND_AUTODEC2 19
#define E6800_POST_IND_NO_OFFSET_R 20
#define E6800_POST_IND_B_OFFSET_R 21
#define E6800_POST_IND_A_OFFSET_R 22
#define E6800_POST_IND_8BIT_OFFSET_R 24
#define E6800_POST_IND_16BIT_OFFSET_R 25
#define E6800_POST_IND_D_OFFSET_R 27
#define E6800_POST_IND_8BIT_OFFSET_PC 28
#define E6800_POST_IND_16BIT_OFFSET_PC 29
#define E6800_POST_IND_EXTEND 31


// Status bits
#define E6800_STAT_E  1
#define E6800_STAT_F  2
#define E6800_STAT_H  4
#define E6800_STAT_I  8
#define E6800_STAT_N  16
#define E6800_STAT_Z  32
#define E6800_STAT_V  64
#define E6800_STAT_C  128
#define E6800_STAT_ALL 0xff


#ifndef A_BIG_ENDIAN
struct Em6800StatusBits  {
  volatile unsigned int e:1;
  volatile unsigned int f:1;
  volatile unsigned int h:1;
  volatile unsigned int i:1;
  volatile unsigned int n:1;
  volatile unsigned int z:1;
  volatile unsigned int v:1;
  volatile unsigned int c:1;
};
#else
struct Em6800StatusBits  {
  volatile unsigned int c:1;
  volatile unsigned int v:1;
  volatile unsigned int z:1;
  volatile unsigned int n:1;
  volatile unsigned int i:1;
  volatile unsigned int h:1;
  volatile unsigned int f:1;
  volatile unsigned int e:1;
};
#endif


class Em6800 : public ECPU8Bit
{
public:
  // Public member functions
  Em6800();
  virtual ~Em6800();
  static bool recognize(const char *name);
  virtual const char *getName();
  virtual void setKind(const char *name);
  virtual void execute();
  virtual ULONG disasm(ULONG loc);
  virtual void wregs();
  virtual void handle_int(ULONG value);
  virtual void cause_int(ULONG value);
  virtual void ints();
  virtual bool setRegister(const char *name,ULONG value);
  virtual ULONG readRegister(const char *name);
  virtual unsigned int printAddrMode(ULONG loc,unsigned int topcode,unsigned int mode);
  virtual void reset();
  virtual void set_flags_nz(int i);
  virtual void set_flags_nzv(int i);
  virtual void set_flags_nzc(int i, int j);
  // Public data members
protected:
  // Private member functions
  virtual void init();
  virtual void stack_push6800(UINT16 &s,BYTE val);
  virtual BYTE stack_pop6800(UINT16 &s);
  virtual void stack_pushw6800(UINT16 &s,UINT16 val);
  virtual UINT16 stack_popw6800(UINT16 &s);
  virtual unsigned int readIndReg(unsigned int r);
  virtual void writeIndReg(unsigned int r,unsigned int val);
  virtual unsigned int readRegD();
  virtual void writeRegD(unsigned int val);
  virtual unsigned int readReg(unsigned int n);
  virtual void writeReg(unsigned int n,unsigned int val);
  virtual unsigned int calcEAPostByte(ULONG addr);
  virtual void printIndReg(unsigned int r);
  virtual void printReg(unsigned int r);
  virtual void printRegs(unsigned int r);
  virtual void printRegMask(unsigned int r);
  virtual unsigned int disPostByte(ULONG addr);
  // common to all but 6809
  void exec_asld(ULONG loc,unsigned int op);
  void exec_cba(ULONG loc,unsigned int op);
  void exec_clv(ULONG loc,unsigned int op);
  void exec_clc(ULONG loc,unsigned int op);
  void exec_cec(ULONG loc,unsigned int op);
  void exec_cli(ULONG loc,unsigned int op);
  void exec_des(ULONG loc,unsigned int op);
  void exec_dex(ULONG loc,unsigned int op);
  void exec_ins(ULONG loc,unsigned int op);
  void exec_inx(ULONG loc,unsigned int op);
  void exec_pula(ULONG loc,unsigned int op);
  void exec_pulb(ULONG loc,unsigned int op);
  void exec_psha(ULONG loc,unsigned int op);
  void exec_pshb(ULONG loc,unsigned int op);
  void exec_pulx(ULONG loc,unsigned int op);
  void exec_pshx(ULONG loc,unsigned int op);
  void exec_sba(ULONG loc,unsigned int op);
  void exec_sev(ULONG loc,unsigned int op);
  void exec_sei(ULONG loc,unsigned int op);
  void exec_sec(ULONG loc,unsigned int op);
  void exec_tpa(ULONG loc,unsigned int op);
  void exec_tap(ULONG loc,unsigned int op);
  void exec_txs(ULONG loc,unsigned int op);
  void exec_tsx(ULONG loc,unsigned int op);
  void exec_tba(ULONG loc,unsigned int op);
  void exec_wai(ULONG loc,unsigned int op);
  void exec_lds(ULONG loc,unsigned int op);
  void exec_ldd(ULONG loc,unsigned int op);
  void exec_sts(ULONG loc,unsigned int op);
  void exec_subd(ULONG loc,unsigned int op);
  // Common for all?
  void exec_abx(ULONG loc,unsigned int op);
  void exec_adca(ULONG loc,unsigned int op);
  void exec_adcb(ULONG loc,unsigned int op);
  void exec_adda(ULONG loc,unsigned int op);
  void exec_addb(ULONG loc,unsigned int op);
  void exec_addd(ULONG loc,unsigned int op);
  void exec_anda(ULONG loc,unsigned int op);
  void exec_andb(ULONG loc,unsigned int op);
  void exec_asl(ULONG loc,unsigned int op);
  void exec_asla(ULONG loc,unsigned int op);
  void exec_aslb(ULONG loc,unsigned int op);
  void exec_asr(ULONG loc,unsigned int op);
  void exec_asra(ULONG loc,unsigned int op);
  void exec_asrb(ULONG loc,unsigned int op);
  void exec_bcc(ULONG loc,unsigned int op);
  void exec_bcs(ULONG loc,unsigned int op);
  void exec_beq(ULONG loc,unsigned int op);
  void exec_bge(ULONG loc,unsigned int op);
  void exec_bgt(ULONG loc,unsigned int op);
  void exec_bhi(ULONG loc,unsigned int op);
  void exec_bita(ULONG loc,unsigned int op);
  void exec_bitb(ULONG loc,unsigned int op);
  void exec_ble(ULONG loc,unsigned int op);
  void exec_bls(ULONG loc,unsigned int op);
  void exec_blt(ULONG loc,unsigned int op);
  void exec_bmi(ULONG loc,unsigned int op);
  void exec_bne(ULONG loc,unsigned int op);
  void exec_bpl(ULONG loc,unsigned int op);
  void exec_bra(ULONG loc,unsigned int op);
  void exec_brn(ULONG loc,unsigned int op);
  void exec_bsr(ULONG loc,unsigned int op);
  void exec_bvc(ULONG loc,unsigned int op);
  void exec_bvs(ULONG loc,unsigned int op);
  void exec_clr(ULONG loc,unsigned int op);
  void exec_clra(ULONG loc,unsigned int op);
  void exec_clrb(ULONG loc,unsigned int op);
  void exec_cmpa(ULONG loc,unsigned int op);
  void exec_cmpb(ULONG loc,unsigned int op);
  void exec_cmpx(ULONG loc,unsigned int op);
  void exec_cmpy(ULONG loc,unsigned int op);
  void exec_com(ULONG loc,unsigned int op);
  void exec_coma(ULONG loc,unsigned int op);
  void exec_comb(ULONG loc,unsigned int op);
  void exec_cpx(ULONG loc,unsigned int op);
  void exec_cwai(ULONG loc,unsigned int op);
  void exec_daa(ULONG loc,unsigned int op);
  void exec_dec(ULONG loc,unsigned int op);
  void exec_deca(ULONG loc,unsigned int op);
  void exec_decb(ULONG loc,unsigned int op);
  void exec_eora(ULONG loc,unsigned int op);
  void exec_eorb(ULONG loc,unsigned int op);
  void exec_exg(ULONG loc,unsigned int op);
  void exec_inc(ULONG loc,unsigned int op);
  void exec_inca(ULONG loc,unsigned int op);
  void exec_incb(ULONG loc,unsigned int op);
  void exec_jmp(ULONG loc,unsigned int op);
  void exec_jsr(ULONG loc,unsigned int op);
  void exec_lda(ULONG loc,unsigned int op);
  void exec_ldb(ULONG loc,unsigned int op);
  void exec_ldx(ULONG loc,unsigned int op);
  void exec_ldy(ULONG loc,unsigned int op);
  void exec_lsr(ULONG loc,unsigned int op);
  void exec_lsra(ULONG loc,unsigned int op);
  void exec_lsrb(ULONG loc,unsigned int op);
  void exec_mul(ULONG loc,unsigned int op);
  void exec_neg(ULONG loc,unsigned int op);
  void exec_nega(ULONG loc,unsigned int op);
  void exec_negb(ULONG loc,unsigned int op);
  void exec_nop(ULONG loc,unsigned int op);
  void exec_ora(ULONG loc,unsigned int op);
  void exec_orb(ULONG loc,unsigned int op);
  void exec_pshs(ULONG loc,unsigned int op);
  void exec_puls(ULONG loc,unsigned int op);
  void exec_rol(ULONG loc,unsigned int op);
  void exec_rola(ULONG loc,unsigned int op);
  void exec_rolb(ULONG loc,unsigned int op);
  void exec_ror(ULONG loc,unsigned int op);
  void exec_rora(ULONG loc,unsigned int op);
  void exec_rorb(ULONG loc,unsigned int op);
  void exec_rti(ULONG loc,unsigned int op);
  void exec_rts(ULONG loc,unsigned int op);
  void exec_sbca(ULONG loc,unsigned int op);
  void exec_sbcb(ULONG loc,unsigned int op);
  void exec_sta(ULONG loc,unsigned int op);
  void exec_stb(ULONG loc,unsigned int op);
  void exec_std(ULONG loc,unsigned int op);
  void exec_stx(ULONG loc,unsigned int op);
  void exec_sty(ULONG loc,unsigned int op);
  void exec_suba(ULONG loc,unsigned int op);
  void exec_subb(ULONG loc,unsigned int op);
  void exec_swi(ULONG loc,unsigned int op);
  void exec_tfr(ULONG loc,unsigned int op);
  void exec_tst(ULONG loc,unsigned int op);
  void exec_tsta(ULONG loc,unsigned int op);
  void exec_tstb(ULONG loc,unsigned int op);
  void exec_special(ULONG loc,unsigned int op);
  // Private data members
  BYTE a,b;  // or d with both together (a is high)
  UINT16 x,y;
  UINT16 u; // not valid on all variants
  UINT16 dp; // direct page in bytes, not valid on all variants
  volatile BYTE p; // status bits
  volatile Em6800StatusBits *sbits;
};


#endif
