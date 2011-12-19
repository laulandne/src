#ifndef CPULIB_EM68K_H
#define CPULIB_EM68K_H


#include <cpulib/ECPUCisc.h>


// Interrupt vectors
// (For ease of use, there are actually many more)
#define E68k_INT_RESET     1
#define E68k_INT_BUSERR    2
#define E68k_INT_ADDRESS   3
#define E68k_INT_ILLEGAL   4
#define E68k_INT_DIVZERO   5
#define E68k_INT_PRIVILEGE 8
#define E68k_INT_A_LINE    10
#define E68k_INT_F_LINE    11
#define E68K_INT_SPURIOUS  24
#define E68k_INT_AUTO1     25
#define E68k_INT_IRQ       25  /* An alias */
// ...5 more between here...
#define E68k_INT_AUTO7     31
#define E68k_INT_NMI       31  /* An Alias */
#define E68k_INT_TRAP0     32
// ...13 more between here...
#define E68k_INT_TRAP15    47


// Instruction encoding methods
#define E68k_UNIMPLEMENTED  0
#define E68k_SIZE_EA  1
#define E68k_EA_EA  2
#define E68k_REG_MODE_EA  3
#define E68k_DATA_SIZE_EA  4
#define E68k_COND_EA  5
#define E68k_COND_DISP 6
#define E68k_COUNT_SIZE_REG  7
#define E68k_EA  8
#define E68k_REG 9
#define E68k_REG_DATA 10
#define E68k_SIZE_EA_IMM 11
#define E68k_REG_LEA 12
#define E68k_REG_EA 13
#define E68k_MOVEM 14
#define E68k_REG_MODE_REG 15
#define E68k_A_LINE 16
#define E68k_F_LINE 17
#define E68k_SPECIAL 18


// Cond code modes...
#define E68k_CC_NORMAL 0


// Status bits
#define E68k_STAT_C  1
#define E68k_STAT_V  2
#define E68k_STAT_Z  4
#define E68k_STAT_N  8
#define E68k_STAT_X  16
#define E68k_STAT_E1 32
#define E68k_STAT_E2 64
#define E68k_STAT_E3 128
#define E68k_STAT_I0 256
#define E68k_STAT_I1 512
#define E68k_STAT_I2 1024
#define E68k_STAT_E4 2048
#define E68k_STAT_M  4096
#define E68k_STAT_S  8192
#define E68k_STAT_T0 16384
#define E68k_STAT_T1 32768
#define E68k_STAT_ALL 0xffff


// NOTE: These MUST be in numerical order!
#define E_CPU_NONE     0x00
#define E_CPU_68000    0x01
#define E_CPU_68010    0x02
#define E_CPU_68020    0x03
#define E_CPU_68030    0x04
#define E_CPU_68040    0x05
#define E_CPU_68060    0x06  /* Mostly treated like an '040 */
#define E_CPU_ColdFire 0x07  /* Mostly treated like an '040 */


extern char *E68kopcode[];
extern unsigned int E68kdecode[];


class Em68k : public ECPUCisc
{
public:
  // Public member functions
  Em68k();
  virtual ~Em68k();
  static bool recognize(const char *name);
  virtual const char *getName();
  virtual void setKind(const char *name);
  virtual bool setRegister(const char *name,ULONG value);
  virtual ULONG readRegister(const char *name);
  virtual void setReturnReg(ULONG val);
  virtual ULONG readReturnReg();
  virtual void setParamReg(unsigned int num,ULONG val);
  virtual ULONG readParamReg(unsigned int num);
  virtual ULONG readStack();
  virtual void setStack(ULONG val);
  virtual ULONG readFramePtr();
  virtual void setFramePtr(ULONG val);
  virtual void execute();
  virtual ULONG disasm(ULONG loc);
  virtual void reset();
  virtual void wregs();
  virtual void handle_int(ULONG value); // Uses native int number only
  virtual void cause_int(ULONG value);  // Uses native int number only
  virtual void ints();
  virtual bool interp(const char *cmd,const char *a1,const char *a2,const char *a3);
  virtual void setCFlag(ULONG val);
  virtual void setVFlag(ULONG val);
  virtual void setZFlag(ULONG val);
  virtual void setNFlag(ULONG val);
  // Public data members
protected:
  // Private member functions
  virtual void init();
  void wmregs();
  void wfregs();
  unsigned int handle_dis_special(ULONG loc,unsigned int opcode);
  void print_size(unsigned int size);
  void print_sr();
  void print_ea_op_len(ULONG loc, unsigned int mode, unsigned int reg, unsigned int size=ECPUABSTRACT_SIZE_LONG);
  void print_reg(unsigned int reg);
  void print_movem_maskx(unsigned int mask,unsigned int mode,unsigned int reg);
  void print_fmove(unsigned int opmode);
  unsigned int reverse_mask(unsigned int mask);
  void do_movemrm(ULONG loc,unsigned int mode,unsigned int reg,unsigned int size, unsigned int mask);
  void do_movemmr(ULONG loc,unsigned int mode,unsigned int reg,unsigned int size, unsigned int mask);
  void push(unsigned int val,unsigned int reg,unsigned int size);
  unsigned int pull(unsigned int reg,unsigned int size);
  unsigned int calc_ea_op_len(ULONG loc, unsigned int mode, unsigned int reg, unsigned int size);
  unsigned int calc_size_bytes(unsigned int size,unsigned int reg);
  void write_reg(unsigned int reg, unsigned int val, unsigned int size);
  // Returns EA addr if any...
  unsigned int write_ea_op_len(ULONG loc, unsigned int mode, unsigned int reg, unsigned int val, unsigned int size);
  // Returns EA addr if any...
  unsigned int write_ea(ULONG loc, unsigned int mode, unsigned int reg, unsigned int val, unsigned int size);
  unsigned int read_reg(unsigned int reg, unsigned int size);
  unsigned int read_ea_op_len(ULONG loc, unsigned int mode, unsigned int reg, unsigned int size);
  unsigned int read_ea(ULONG loc, unsigned int mode, unsigned int reg, unsigned int size);
  //
  void dis_040ext(ULONG loc,unsigned int opcode);
  void dis_pxxx(ULONG loc, unsigned int op, unsigned int data);
  void dis_fxxx(ULONG loc, unsigned int op, unsigned int data);
  void dis_special(ULONG loc, unsigned int opcode);
  void dis_a_line(ULONG loc, unsigned int opcode);
  void dis_f_line(ULONG loc, unsigned int opcode);
  void dis_special_0x(ULONG loc, unsigned int opcode);
  void dis_special_4e(ULONG loc, unsigned int opcode);
  void dis_special_ex(ULONG loc, unsigned int opcode);
  void dis_bxxx(ULONG loc, unsigned int opcode, unsigned int srcSize, unsigned int type, unsigned int bits);
  void dis_dbcc(ULONG loc, unsigned int opcode);
  void dis_move_sr_ccr(ULONG loc, unsigned int opcode);
  void dis_movem(ULONG loc, unsigned int opcode);
  void dis_move(ULONG loc, unsigned int topcode, unsigned int reg2, unsigned int mode2, unsigned int mode,
    unsigned int reg);
  void dis_moveq(ULONG loc, unsigned int reg, unsigned int data);
  void dis_lea(ULONG loc, unsigned int reg2, unsigned int mode, unsigned int reg);
  void dis_clr(ULONG loc, unsigned int mode, unsigned int reg, unsigned int size);
  void dis_neg(ULONG loc, unsigned int mode, unsigned int reg, unsigned int size);
  void dis_not(ULONG loc, unsigned int mode, unsigned int reg, unsigned int size);
  void dis_tst(ULONG loc, unsigned int mode, unsigned int reg, unsigned int size);
  void dis_trapcc(ULONG loc, unsigned int opcode, unsigned int mode);
  void dis_scc(ULONG loc, unsigned int mode, unsigned int reg);
  void dis_bcc(ULONG loc, unsigned int opcode);
  void dis_addq(ULONG loc, unsigned int mode, unsigned int reg, unsigned int reg2, unsigned int size);
  void dis_subq(ULONG loc, unsigned int mode, unsigned int reg, unsigned int reg2, unsigned int size);
  void dis_or(ULONG loc, unsigned int reg2, unsigned int mode2, unsigned int mode, unsigned int reg);
  void dis_eor(ULONG loc, unsigned int reg2, unsigned int mode2, unsigned int mode, unsigned int reg);
  void dis_sub(ULONG loc, unsigned int reg2, unsigned int mode2, unsigned int mode, unsigned int reg);
  void dis_cmp(ULONG loc, unsigned int reg2, unsigned int mode2, unsigned int mode, unsigned int reg);
  void dis_and(ULONG loc, unsigned int reg2, unsigned int mode2, unsigned int mode, unsigned int reg);
  void dis_add(ULONG loc, unsigned int reg2, unsigned int mode2, unsigned int mode, unsigned int reg);
  void dis_ori(ULONG loc, unsigned int opcode, unsigned int mode, unsigned int reg, unsigned int size);
  void dis_eori(ULONG loc, unsigned int opcode, unsigned int mode, unsigned int reg, unsigned int size);
  void dis_andi(ULONG loc, unsigned int opcode, unsigned int mode, unsigned int reg, unsigned int size);
  void dis_subi(ULONG loc, unsigned int opcode, unsigned int mode, unsigned int reg, unsigned int size);
  void dis_addi(ULONG loc, unsigned int opcode, unsigned int mode, unsigned int reg, unsigned int size);
  void dis_cmpi(ULONG loc, unsigned int opcode, unsigned int mode, unsigned int reg, unsigned int size);
  void dis_exg(ULONG loc, unsigned int opcode);
  void dis_ext(ULONG loc, unsigned int opcode);
  void dis_xxrm(ULONG loc, unsigned int opcode);
  void dis_xxlm(ULONG loc, unsigned int opcode);
  void dis_xxrr(ULONG loc, unsigned int opcode, unsigned int count, unsigned int size);
  void dis_xxlr(ULONG loc, unsigned int opcode, unsigned int count, unsigned int size);
  void dis_swap(ULONG loc, unsigned int opcode);
  void dis_pea(ULONG loc, unsigned int opcode);
  void exec_040ext(ULONG loc,unsigned int opcode);
  void exec_pxxx(ULONG loc, unsigned int op, unsigned int data);
  void exec_fxxx(ULONG loc, unsigned int op, unsigned int data);
  void exec_special(ULONG loc, unsigned int opcode);
  void exec_a_line(ULONG loc, unsigned int opcode);
  void exec_f_line(ULONG loc, unsigned int opcode);
  void exec_special_0x(ULONG loc, unsigned int opcode);
  void exec_special_4e(ULONG loc, unsigned int opcode);
  void exec_special_ex(ULONG loc, unsigned int opcode);
  void exec_rts(ULONG loc);
  void exec_move_sr_ccr(ULONG loc,unsigned int opcode);
  void exec_movec(ULONG loc, unsigned int opcode);
  void exec_jmp(ULONG loc, unsigned int mode, unsigned int reg);
  void exec_jsr(ULONG loc, unsigned int mode, unsigned int reg);
  void exec_bsr(ULONG loc, unsigned int opcode);
  void exec_bxxx(ULONG loc, unsigned int opcode, unsigned int srcSize, unsigned int type, unsigned int bits);
  void exec_dbcc(ULONG loc, unsigned int opcode);
  void exec_link(ULONG loc, unsigned int opcode);
  void exec_unlk(ULONG loc, unsigned int opcode);
  void exec_trap(ULONG loc, unsigned int opcode);
  void exec_movem(ULONG loc, unsigned int opcode);
  void exec_move(ULONG loc, unsigned int topcode, unsigned int reg2, unsigned int mode2, unsigned int mode,
    unsigned int reg);
  void exec_moveq(ULONG loc, unsigned int reg, unsigned int data);
  void exec_lea(ULONG loc, unsigned int reg2, unsigned int mode, unsigned int reg);
  void exec_clr(ULONG loc, unsigned int mode, unsigned int reg, unsigned int size);
  void exec_neg(ULONG loc, unsigned int mode, unsigned int reg, unsigned int size);
  void exec_not(ULONG loc, unsigned int mode, unsigned int reg, unsigned int size);
  void exec_tst(ULONG loc, unsigned int mode, unsigned int reg, unsigned int size);
  void exec_trapcc(ULONG loc, unsigned int opcode, unsigned int reg);
  void exec_scc(ULONG loc, unsigned int opcode, unsigned int mode);
  void exec_bcc(ULONG loc, unsigned int opcode);
  void exec_addq(ULONG loc, unsigned int mode, unsigned int reg, unsigned int reg2, unsigned int size);
  void exec_subq(ULONG loc, unsigned int mode, unsigned int reg, unsigned int reg2, unsigned int size);
  void exec_or(ULONG loc, unsigned int reg2, unsigned int mode2, unsigned int mode, unsigned int reg);
  void exec_eor(ULONG loc, unsigned int reg2, unsigned int mode2, unsigned int mode, unsigned int reg);
  void exec_sub(ULONG loc, unsigned int reg2, unsigned int mode2, unsigned int mode, unsigned int reg);
  void exec_cmp(ULONG loc, unsigned int reg2, unsigned int mode2, unsigned int mode, unsigned int reg);
  void exec_and(ULONG loc, unsigned int reg2, unsigned int mode2, unsigned int mode, unsigned int reg);
  void exec_add(ULONG loc, unsigned int reg2, unsigned int mode2, unsigned int mode, unsigned int reg);
  void exec_ori(ULONG loc, unsigned int opcode, unsigned int mode, unsigned int reg, unsigned int size);
  void exec_eori(ULONG loc, unsigned int opcode, unsigned int mode, unsigned int reg, unsigned int size);
  void exec_andi(ULONG loc, unsigned int opcode, unsigned int mode, unsigned int reg, unsigned int size);
  void exec_subi(ULONG loc, unsigned int opcode, unsigned int mode, unsigned int reg, unsigned int size);
  void exec_addi(ULONG loc, unsigned int opcode, unsigned int mode, unsigned int reg, unsigned int size);
  void exec_cmpi(ULONG loc, unsigned int opcode, unsigned int mode, unsigned int reg, unsigned int size);
  void exec_ext(ULONG loc, unsigned int opcode);
  void exec_exg(ULONG loc, unsigned int opcode);
  void exec_lsrm(ULONG loc, unsigned int opcode);
  void exec_lslm(ULONG loc, unsigned int opcode);
  void exec_asrm(ULONG loc, unsigned int opcode);
  void exec_aslm(ULONG loc, unsigned int opcode);
  void exec_roxrm(ULONG loc, unsigned int opcode);
  void exec_roxlm(ULONG loc, unsigned int opcode);
  void exec_rorm(ULONG loc, unsigned int opcode);
  void exec_rolm(ULONG loc, unsigned int opcode);
  void exec_lsrr(ULONG loc, unsigned int opcode, unsigned int count, unsigned int size);
  void exec_lslr(ULONG loc, unsigned int opcode, unsigned int count, unsigned int size);
  void exec_asrr(ULONG loc, unsigned int opcode, unsigned int count, unsigned int size);
  void exec_aslr(ULONG loc, unsigned int opcode, unsigned int count, unsigned int size);
  void exec_roxrr(ULONG loc, unsigned int opcode, unsigned int count, unsigned int size);
  void exec_roxlr(ULONG loc, unsigned int opcode, unsigned int count, unsigned int size);
  void exec_rorr(ULONG loc, unsigned int opcode, unsigned int count, unsigned int size);
  void exec_rolr(ULONG loc, unsigned int opcode, unsigned int count, unsigned int size);
  void exec_swap(ULONG loc, unsigned int opcode);
  void exec_pea(ULONG loc, unsigned int opcode);
  unsigned int do_lsr_once(unsigned int val,unsigned int size);
  unsigned int do_lsl_once(unsigned int val,unsigned int size);
  unsigned int do_asr_once(unsigned int val,unsigned int size);
  unsigned int do_asl_once(unsigned int val,unsigned int size);
  unsigned int do_ror_once(unsigned int val,unsigned int size);
  unsigned int do_rol_once(unsigned int val,unsigned int size);
  unsigned int do_roxr_once(unsigned int val,unsigned int size);
  unsigned int do_roxl_once(unsigned int val,unsigned int size);
  //
  unsigned int decode_mode_size(unsigned int reg,unsigned int mode);
  unsigned int decode_mode_dir(unsigned int reg,unsigned int mode);
  unsigned int decode_mode_reg(unsigned int reg,unsigned int mode);
  unsigned int getImm_op_len(ULONG loc, unsigned int size);
  void setFlagsVCX(ULONG val1, ULONG val2, ULONG res, unsigned int size);
  unsigned int getMaskSize68k(unsigned int size);
  unsigned int getInverseMaskSize68k(unsigned int size);
  unsigned int getTopBitSize68k(unsigned int size);
  unsigned int getNotTopMaskSize68k(unsigned int size);
  int sign_extend68k(int val, unsigned int size);
  void decode(ULONG loc);
  void setXFlag(ULONG val);
  bool isSSet() const { return ((sr|E68k_STAT_S)==0) ? false : true; }
  bool isMSet() const { return ((sr|E68k_STAT_M)==0) ? false : true; }
  void goUser();
  void goSupervisor();
  void setSR(unsigned int newVal);
  // Private data members
  unsigned int d[8];
  unsigned int a[8];
  double fp[8];
  // These are used more as storage than as active registers...
  unsigned int ssp,msp;  // NOTE: ssp=isp and parent's sp=usp
  USHORT sr;
  // These next regs are for '010 or better
  ULONG vbr;
  unsigned int sfc,dfc;
  // These next regs are for '020 or better
  unsigned int caar,cacr;
  // These are for chips with MMU's
  // NOTE: URP=CRP, TC=TCR, and MMUSR=PSR
  // NOTE: '040 and higher only use urp_l and srp_l
  unsigned int tc,mmusr,urp_l,urp_h,srp_l,srp_h;
  unsigned int tt0,tt1;  // Only for '030
  unsigned int drp_l,drp_h,val,cal,scc,ac; // Only for '020+68851
  // These next regs are for '040 (or better?)
  unsigned int dtt0,dtt1;  // NOTE: TT0=ITT0 and TT1=ITT1
  unsigned int fpcr,fpsr;  /// NOTE: fpiar=pc always here
};

#endif

