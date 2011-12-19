#ifndef CPULIB_EMIPS_H
#define CPULIB_EMIPS_H


#include <cpulib/ECPURisc.h>


// The operator formats
#define EMIPS_FORMAT_UNKNOWN           0
#define EMIPS_FORMAT_VARIOUS           1
#define EMIPS_FORMAT_NONE              2
#define EMIPS_FORMAT_CONST             3
#define EMIPS_FORMAT_REG_REG_CONST     4
#define EMIPS_FORMAT_REG_REG_REG       5
#define EMIPS_FORMAT_REG_REG_CONST_OP  6


// The operator sub-formats
#define EMIPS_SUBFORMAT_NONE   0
#define EMIPS_SUBFORMAT_BASED  1


// Flags
#define EMIPS_BRANCH 1


// The operator type
#define EMIPS_NORMAL     0


class EMips : public ECPURisc {
public:
  // Public member functions
  EMips();
  virtual ~EMips();
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
  virtual const char *getRegName(unsigned int num);
protected:
  // Private member functions
  virtual void init();
  //
  void exec_special();
  void exec_regimm();
  void exec_add();
  void exec_addu();
  void exec_dadd();
  void exec_daddu();
  void exec_addi();
  void exec_addiu();
  void exec_and();
  void exec_andi();
  void exec_beq();
  void exec_bne();
  void exec_blez();
  void exec_bgtz();
  void exec_beql();
  void exec_bnel();
  void exec_blezl();
  void exec_bgtzl();
  void exec_bltz();
  void exec_bgez();
  void exec_bgezl();
  void exec_bltzl();
  void exec_bltzal();
  void exec_bgezal();
  void exec_bltzall();
  void exec_bgezall();
  void exec_break();
  void exec_cache();
  void exec_copx();
  void exec_daddi();
  void exec_daddiu();
  void exec_div();
  void exec_divu();
  void exec_ddiv();
  void exec_ddivu();
  void exec_j();
  void exec_jal();
  void exec_jr();
  void exec_jalr();
  void exec_lui();
  void exec_lb();
  void exec_lh();
  void exec_lwl();
  void exec_lwr();
  void exec_lw();
  void exec_lwu();
  void exec_lbu();
  void exec_lhu();
  void exec_ll();
  void exec_ldl();
  void exec_ldr();
  void exec_lld();
  void exec_ld();
  void exec_lwcx();
  void exec_ldcx();
  void exec_mtlo();
  void exec_mthi();
  void exec_mflo();
  void exec_mfhi();
  void exec_mult();
  void exec_multu();
  void exec_dmult();
  void exec_dmultu();
  void exec_nor();
  void exec_or();
  void exec_ori();
  void exec_slti();
  void exec_sltiu();
  void exec_sll();
  void exec_srl();
  void exec_sra();
  void exec_sllv();
  void exec_srlv();
  void exec_srav();
  void exec_dsll();
  void exec_dsrl();
  void exec_dsra();
  void exec_dsll32();
  void exec_dsrl32();
  void exec_dsra32();
  void exec_dsllv();
  void exec_dsrlv();
  void exec_dsrav();
  void exec_sb();
  void exec_sh();
  void exec_swl();
  void exec_sw();
  void exec_swr();
  void exec_sc();
  void exec_swcx();
  void exec_sdcx();
  void exec_sdc();
  void exec_sdl();
  void exec_sdr();
  void exec_sd();
  void exec_slt();
  void exec_sltu();
  void exec_sub();
  void exec_subu();
  void exec_dsub();
  void exec_dsubu();
  void exec_sync();
  void exec_syscall();
  void exec_tge();
  void exec_tgeu();
  void exec_tlt();
  void exec_tltu();
  void exec_teq();
  void exec_tne();
  void exec_tgei();
  void exec_tgeiu();
  void exec_tlti();
  void exec_tltiu();
  void exec_teqi();
  void exec_tnei();
  void exec_xor();
  void exec_xori();
  //
  void dishelper(const char *str=(const char *)NULL);
  void decode(ULONG inst);
  void disVarious();
  ULONG readReg(unsigned int n);
  void writeReg(unsigned int n, ULONG val);
  char *getFloatRegName(unsigned int num);
  double readFloatReg(unsigned int n);
  void writeFloatReg(unsigned int n, double val);
  void wmregs();
  void wfregs();
  // Private data members
  ULONG G[32];
  ULONG cpr[32];
  double F[32];
  const char *opName;
  unsigned int opFormat,opSubFormat,opFlags;
  bool disFlag;
  // All the below are used for instruction decoding...
  // (There's a lot of duplication of fields for now...)
  unsigned int top,bop,op0,op1;
  unsigned int rs,rt,rd,sa;
  int imm;  // signed
  char n[10];
};


#endif
