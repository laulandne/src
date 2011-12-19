#ifndef CPULIB_EARM_H
#define CPULIB_EARM_H


#include <cpulib/ECPURisc.h>
#include <cpulib/ECPUOperand.h>


// Address and operand modes
#define EARM_NONE 0
#define EARM_SPECIAL 1
#define EARM_ILLEGAL 2
#define EARM_REG_REG 3
#define EARM_REG_IMM 4
#define EARM_REG_REG_IMM 5
#define EARM_REG_REG_REG 6
#define EARM_REG_ADDR 7
#define EARM_INDREG 8
#define EARM_DISP 9
#define EARM_REG 10
#define EARM_IMM 11
#define EARM_REG_REG2 12
#define EARM_REG_IMM2 13
#define EARM_MULTREG 14


// cond codes
#define EARM_CC_EQ 0
#define EARM_CC_NE 1
#define EARM_CC_CS 2
#define EARM_CC_CC 3
#define EARM_CC_MI 4
#define EARM_CC_PL 5
#define EARM_CC_VS 6
#define EARM_CC_VC 7
#define EARM_CC_HI 8
#define EARM_CC_LS 9
#define EARM_CC_GE 10
#define EARM_CC_LT 11
#define EARM_CC_GT 12
#define EARM_CC_LE 13
#define EARM_CC_ALWAYS 14
#define EARM_CC_NEVER 15


// special register use
#define EARM_REG_FP 11
#define EARM_REG_IP 12
#define EARM_REG_SP 13
#define EARM_REG_LR 14
#define EARM_REG_PC 15


class EArm : public ECPURisc
{
public:
  // Public member functions
  EArm();
  virtual ~EArm();
  static bool recognize(const char *name);
  virtual const char *getName();
  virtual void setKind(const char *name);
  virtual void execute();
  virtual void handle_int(ULONG value);
  virtual void cause_int(ULONG value);
  virtual void ints();
  virtual bool setRegister(const char *name,ULONG value);
  virtual ULONG readRegister(const char *name);
  virtual ULONG readStack();
  virtual void setStack(ULONG location);
  virtual void setFramePtr(ULONG location);
  virtual ULONG readFramePtr();
  virtual ULONG disasm(ULONG loc);
  virtual unsigned int printAddrMode(ULONG loc,unsigned int topcode,unsigned int mode);
  virtual void reset();
  virtual void wregs();
  virtual const char *getRegName(unsigned int n);
  virtual void writeReg(unsigned int n,ULONG val);
  virtual ULONG readReg(unsigned int n);
  // Public data members
protected:
  // Private member functions
  virtual void init();
  void decode();
  void decode_special();
  void disOperand(ULONG loc,ECPUOperand *op);
  unsigned int readOperand(ECPUOperand *op);
  void writeOperand(ECPUOperand *op,unsigned int val);
  void printCC(unsigned int cond);
  //
  void exec_mov(unsigned int loc,unsigned int opcode);
  void exec_mvn(unsigned int loc,unsigned int opcode);
  void exec_and(unsigned int loc,unsigned int opcode);
  void exec_add(unsigned int loc,unsigned int opcode);
  void exec_sub(unsigned int loc,unsigned int opcode);
  void exec_cmp(unsigned int loc,unsigned int opcode);
  void exec_cmn(unsigned int loc,unsigned int opcode);
  void exec_str(unsigned int loc,unsigned int opcode);
  void exec_strb(unsigned int loc,unsigned int opcode);
  void exec_ldr(unsigned int loc,unsigned int opcode);
  void exec_ldrb(unsigned int loc,unsigned int opcode);
  void exec_ldm(unsigned int loc,unsigned int opcode);
  void exec_stm(unsigned int loc,unsigned int opcode);
  void exec_bl(unsigned int loc,unsigned int opcode);
  void exec_bcc(unsigned int loc,unsigned int opcode);
  // Private data members
  // NOTE: R15 is pc, R14 is lr, R13 is sp, R12 is ip and R11 is fp
  UINT32 rrrr[16];
  UINT32 cpsr;
  unsigned int cpuMode;
  // op1 is typically the dest
  ECPUOperand op1,op2,op3;
  unsigned int cond;
  unsigned int opcode1,opcode2;
  unsigned int mode;
  bool willExecute;
};


#endif // CPULIB_ARM_H

