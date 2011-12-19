#ifndef CPULIB_ESH_H
#define CPULIB_ESH_H


#include <cpulib/ECPURisc.h>
#include <cpulib/ECPUOperand.h>


// Address/operand modes
#define ESH_NONE 0
#define ESH_SPECIAL 1
#define ESH_ILLEGAL 2
#define ESH_PALCODE 3
// Inst formats...
#define ESH_IMM_REG 10
#define ESH_REG_DISPREG4 11
#define ESH_DISPREG4_REG 12
#define ESH_DISPPC2_REG 13
#define ESH_DISPPC4_REG 14
#define ESH_REG_REG 15
#define ESH_MOV 16
// Operand formats...
#define ESH_REG 20
#define ESH_SPECIALREG 21
#define ESH_INDREG 22
#define ESH_PREDECINDREG 23
#define ESH_POSTINCINDREG 24
#define ESH_IMM 25
#define ESH_DISPREG 26
#define ESH_DISPPC 27


class ESh : public ECPURisc
{
public:
  // Public member functions
  ESh();
  virtual ~ESh();
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
  virtual ULONG disasm(ULONG loc);
  virtual unsigned int printAddrMode(ULONG loc,unsigned int topcode,unsigned int mode);
  virtual void reset();
  virtual void wregs();
  virtual void writeReg(unsigned int n,ULONG val);
  virtual ULONG readReg(unsigned int n);
  virtual const char *getRegName(unsigned int n);
  // Public data members
protected:
  // Private member functions
  virtual void init();
  const char *getSpecialRegName(unsigned int num);
  void decode();
  void disOperand(ULONG loc,ECPUOperand *op);
  ULONG readSpecialReg(unsigned int num);
  void writeSpecialReg(unsigned int num,ULONG val);
  ULONG readOperand(ECPUOperand *op);
  void writeOperand(ECPUOperand *op,ULONG val);
  void decode_special();
  void decode_special0();
  void decode_special4();
  //
  void exec_add(unsigned int loc,unsigned int opcode);
  void exec_bcc(unsigned int loc,unsigned int opcode);
  void exec_bra(unsigned int loc,unsigned int opcode);
  void exec_bsr(unsigned int loc,unsigned int opcode);
  void exec_nop(unsigned int loc,unsigned int opcode);
  void exec_mov(unsigned int loc,unsigned int opcode);
  void exec_mov_w(unsigned int loc,unsigned int opcode);
  void exec_mov_l(unsigned int loc,unsigned int opcode);
  void exec_special0(unsigned int loc,unsigned int opcode);
  void exec_special4(unsigned int loc,unsigned int opcode);
  // Private data members
  ULONG rrrr[16];
  ULONG pr;
  ULONG dsr,a0,x0,x1,y0,y1;
  USHORT mach,macl;
  const char *opname;
  // op2 is typically the dest
  ECPUOperand op1,op2;
  unsigned int r1,r2,imm4,imm8;
  unsigned int mode;
};


#endif // CPULIB_SH_H
