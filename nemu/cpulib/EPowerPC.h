#ifndef CPULIB_EPPC_H
#define CPULIB_EPPC_H


#include <cpulib/ECPURisc.h>


// The operator formats
#define EPPC_FORMAT_UNKNOWN  0
#define EPPC_FORMAT_VARIOUS  1
#define EPPC_FORMAT_D        2
#define EPPC_FORMAT_B        3
#define EPPC_FORMAT_I        4
#define EPPC_FORMAT_M        5
#define EPPC_FORMAT_SC       6
#define EPPC_FORMAT_X        7
#define EPPC_FORMAT_X2       8
#define EPPC_FORMAT_XL       9
#define EPPC_FORMAT_XS       10
#define EPPC_FORMAT_XO       11
#define EPPC_FORMAT_XFX      12
#define EPPC_FORMAT_XFXr     13


// The operator sub-formats
#define EPPC_SUBFORMAT_NONE   0
#define EPPC_SUBFORMAT_CMPI   1
#define EPPC_SUBFORMAT_LDST   2
#define EPPC_SUBFORMAT_IMMED  3
#define EPPC_SUBFORMAT_REG    4


// The operator type
#define EPPC_NONE       0
#define EPPC_NORMAL     0
#define EPPC_STANDARD   0
#define EPPC_POWER      1
#define EPPC_POWER2     2
#define EPPC_32ONLY     4
#define EPPC_64ONLY     8
#define EPPC_PRIV       16
#define EPPC_OPRIV      32


// The CR and XER flag bits
// CR integer...
#define EPPC_CR_LT  0
#define EPPC_CR_GT  1
#define EPPC_CR_EQ  2
#define EPPC_CR_SO  3
// ...CR floating point...
#define EPPC_CR_FX  0
#define EPPC_CR_FEX 1
#define EPPC_CR_VX  2
#define EPPC_CR_OX  3
// ...and XER integer...
#define EPPC_XER_SO 31
#define EPPC_XER_OV 30
#define EPPC_XER_CA 29


class EPowerPC : public ECPURisc {
public:
  // Public member functions
  EPowerPC();
  virtual ~EPowerPC();
  static bool recognize(const char *name);
  virtual void execute();
  virtual void handle_int(ULONG value);
  virtual void cause_int(ULONG value);
  virtual const char *getName();
  virtual void setKind(const char *name);
  virtual bool setRegister(const char *name,ULONG value);
  virtual void setReturnReg(ULONG val);
  virtual ULONG readReturnReg();
  virtual void setParamReg(unsigned int num,ULONG val);
  virtual ULONG readParamReg(unsigned int num);
  virtual ULONG readStack();
  virtual void setStack(ULONG val);
  virtual ULONG readFramePtr();
  virtual void setFramePtr(ULONG val);
  virtual ULONG disasm(ULONG loc);
  virtual void reset();
  virtual void wregs();
  virtual bool interp(const char *cmd,const char *a1,const char *a2,const char *a3);
protected:
  // Private member functions
  virtual void init();
  //
  void exec_cmp();
  void exec_tw();
  void exec_subfc();
  void exec_mulhdu();
  void exec_addc();
  void exec_mulhwu();
  void exec_mfcr();
  void exec_lwarx();
  void exec_ldx();
  void exec_lwzx();
  void exec_slw();
  void exec_cntlzw();
  void exec_sld();
  void exec_and();
  void exec_maskg();
  void exec_cmpl();
  void exec_subf();
  void exec_rfi();
  void exec_ldux();
  void exec_dcbst();
  void exec_lwzux();
  void exec_cntlzd();
  void exec_andc();
  void exec_td();
  void exec_mulhd();
  void exec_mulhw();
  void exec_mfmsr();
  void exec_ldarx();
  void exec_dcbf();
  void exec_lbzx();
  void exec_neg();
  void exec_mul();
  void exec_clf();
  void exec_lbzux();
  void exec_nor();
  void exec_subfe();
  void exec_adde();
  void exec_mtcrf();
  void exec_mtmsr();
  void exec_isync();
  void exec_stwx();
  void exec_slq();
  void exec_sle();
  void exec_stdux();
  void exec_stwux();
  void exec_sliq();
  void exec_subfze();
  void exec_addze();
  void exec_mtsr();
  void exec_stdcx_();
  void exec_stbx();
  void exec_sllq();
  void exec_sleq();
  void exec_subfne();
  void exec_mulld();
  void exec_addme();
  void exec_mullw();
  void exec_mtsrin();
  void exec_dcbtst();
  void exec_stbux();
  void exec_slliq();
  void exec_doz();
  void exec_add();
  void exec_lscbx();
  void exec_dcbt();
  void exec_lhzx();
  void exec_eqv();
  void exec_tlbie();
  void exec_eciwx();
  void exec_lhzux();
  void exec_xor();
  void exec_div();
  void exec_mfspr();
  void exec_lwax();
  void exec_lhax();
  void exec_abs();
  void exec_divs();
  void exec_tlbia();
  void exec_mftb();
  void exec_lwaux();
  void exec_lhaux();
  void exec_sthx();
  void exec_orc();
  void exec_sradi();
  void exec_slbie();
  void exec_ecowx();
  void exec_sthux();
  void exec_or();
  void exec_divdu();
  void exec_divwu();
  void exec_mtspr();
  void exec_dcbi();
  void exec_nand();
  void exec_nabs();
  void exec_divd();
  void exec_divw();
  void exec_slbia();
  void exec_cli();
  void exec_mcrxr();
  void exec_clcs();
  void exec_lswx();
  void exec_lwbrx();
  void exec_lfsx();
  void exec_srw();
  void exec_rrib();
  void exec_srd();
  void exec_maskir();
  void exec_tlbsync();
  void exec_lfsux();
  void exec_mfsr();
  void exec_lswi();
  void exec_sync();
  void exec_lfdx();
  void exec_mfsri();
  void exec_dclst();
  void exec_lfdux();
  void exec_mfsrin();
  void exec_stswx();
  void exec_stwbrx();
  void exec_stfax();
  void exec_srq();
  void exec_sre();
  void exec_stfsux();
  void exec_sriq();
  void exec_stswi();
  void exec_stfdx();
  void exec_srlq();
  void exec_sreq();
  void exec_stfdux();
  void exec_srliq();
  void exec_lhbrx();
  void exec_lfqx();
  void exec_sraw();
  void exec_srad();
  void exec_rac();
  void exec_lfqux();
  void exec_srawi();
  void exec_eieio();
  void exec_sthbrx();
  void exec_stfqx();
  void exec_sraq();
  void exec_srea();
  void exec_extsh();
  void exec_stfqux();
  void exec_sraiq();
  void exec_extsb();
  void exec_icbi();
  void exec_stfiwx();
  void exec_extsw();
  void exec_dcbz();
  void exec_dozi();
  void exec_rlmi();
  void exec_tdi();
  void exec_twi();
  void exec_mulli();
  void exec_subfic();
  void exec_cmpli();
  void exec_cmpi();
  void exec_addic();
  void exec_addic_();
  void exec_addi();
  void exec_addis();
  void exec_bc();
  void exec_sc();
  void exec_b();
  void exec_rlwimi();
  void exec_rlwinm();
  void exec_rlwnm();
  void exec_ori();
  void exec_oris();
  void exec_xori();
  void exec_xoris();
  void exec_andi_();
  void exec_andis_();
  void exec_lwz();
  void exec_lwzu();
  void exec_lbz();
  void exec_lbzu();
  void exec_stw();
  void exec_stwu();
  void exec_stb();
  void exec_stbu();
  void exec_lhz();
  void exec_lhzu();
  void exec_lha();
  void exec_lhau();
  void exec_sth();
  void exec_sthu();
  void exec_lmw();
  void exec_stmw();
  void exec_lfs();
  void exec_lfsu();
  void exec_lfd();
  void exec_lfdu();
  void exec_stfs();
  void exec_stfsu();
  void exec_std();
  void exec_stfdu();
  void exec_sfd();
  void exec_various();
  void exec_bclr();
  void exec_crnor();
  void exec_crandc();
  void exec_crxor();
  void exec_crnand();
  void exec_crand();
  void exec_creqv();
  void exec_crorc();
  void exec_cror();
  void disHelper(const char *oname=(const char *)NULL,unsigned int oformat=EPPC_FORMAT_UNKNOWN,unsigned int osubformat=0);
  void disVarious();
  //
  void printAddr(unsigned int val);
  void printReg(unsigned int num);
  ULONG virtual readReg(unsigned int n);
  ULONG readReg0(unsigned int n);
  void writeReg(unsigned int n, ULONG val);
  void writeReg0(unsigned int n, ULONG val);
  void printFloatReg(unsigned int num);
  double readFloatReg(unsigned int n);
  void writeFloatReg(unsigned int n, double val);
  void wmregs();
  void wfregs();
  void decodeAndCalc();
  unsigned int readCR();  // translates CRz to match what a real hardware CR looks like
  void writeCR(unsigned int val);
  unsigned int readXER();
  void writeXER(unsigned int val);
  void writeFPSCR(unsigned int val);
  unsigned int readFPSCR();
  void setCRbit(unsigned int bit);
  bool getCRbit(unsigned int bit);
  void clrCRbit(unsigned int bit);
  void setXERbit(unsigned int bit);
  bool getXERbit(unsigned int bit);
  void clrXERbit(unsigned int bit);
  void setXER_signed(int a,int b,int result);
  void setXER_unsigned(unsigned int a,unsigned int b,unsigned int result);
  void setCR_signed(unsigned int field,int a,int b);
  void setCR_unsigned(unsigned int field,unsigned int a,unsigned int b);
  void setCR_float(unsigned int field,double a,double b,double result);
  // Private data members
  ULONG G[32];
  double F[32];
  // NOTE: pc is called "CIA" in some docs
  ULONG LR;
  ULONG CTR;
  ULONG FPSCRz,XERz,CRz; // NOTE: don't read these directly!
  bool didOverflow;
  const char *opName;
  ULONG opFormat,opSubFormat;
  // All the below are used for instruction decoding...
  // (There's a lot of duplication of fields for now...)
  unsigned int top,bop;
  unsigned int rd;
  unsigned int bt,ba,bb,lk,spr,rc,oe;
  unsigned int ra,rb,mb,me,rt,rs,d,bd,aa,bo,bi,li,bf;
  int imm;  // signed...but not always extended to 32 bits...
};


#endif
