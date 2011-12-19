#ifndef CPULIB_EALPHA_H
#define CPULIB_EALPHA_H


#include <cpulib/ECPURisc.h>
#include <cpulib/ECPUOperand.h>


class EMem;
class EMap;
class EMemMap;


// Address/operand modes
#define EALPHA_NONE 0
#define EALPHA_SPECIAL 1
#define EALPHA_ILLEGAL 2
#define EALPHA_PALCODE 3
// Inst formats...
#define EALPHA_REG_DISP 10
#define EALPHA_REG_REG_DISP 11
#define EALPHA_REG_REG_REG 12
#define EALPHA_REG_REG 13
// Operand formats...
#define EALPHA_REG 20
#define EALPHA_INDREG 21
#define EALPHA_IMM 22
#define EALPHA_DISP 23


class EAlpha : public ECPURisc
{
public:
  // Public member functions
  EAlpha();
  virtual ~EAlpha();
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
  void disOperand(ULONG loc,ECPUOperand *op);
  ULONG readOperand(ECPUOperand *op);
  void writeOperand(ECPUOperand *op,unsigned int val);
  ULONG getOperandEA(ECPUOperand *op);
  void do_store(ULONG addr,ULONG val,unsigned int size);
  ULONG do_load(ULONG addr,unsigned int size);
  void do_jump(unsigned int loc,unsigned int opcode);
  //
  void exec_lda(unsigned int loc,unsigned int opcode);
  void exec_ldah(unsigned int loc,unsigned int opcode);
  void exec_ldq(unsigned int loc,unsigned int opcode);
  void exec_ldl(unsigned int loc,unsigned int opcode);
  void exec_stl(unsigned int loc,unsigned int opcode);
  void exec_stq(unsigned int loc,unsigned int opcode);
  void exec_mov(unsigned int loc,unsigned int opcode);
  void exec_jsr(unsigned int loc,unsigned int opcode);
  void exec_jsr_coroutine(unsigned int loc,unsigned int opcode);
  void exec_jmp(unsigned int loc,unsigned int opcode);
  void exec_ret(unsigned int loc,unsigned int opcode);
  // Private data members
  ULONG rrrr[32];
  double F[32];
  // op3 is typically the destination
  ECPUOperand op1,op2,op3;
  unsigned int mode;
};


#endif // CPULIB_ALPHA_H
