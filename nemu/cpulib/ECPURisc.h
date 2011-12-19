#ifndef ELIB_ERISCCPU_H
#define ELIB_ERISCCPU_H


#include "ECPUAbstract.h"


class ECPURisc : public ECPUAbstract {
public:
  // Public member functions
  ECPURisc();
  virtual ~ECPURisc();
  virtual ULONG getPC() { return pc; }
  virtual ULONG disasmPC() { return disasm(pc); }
  virtual ULONG readStack();
  virtual void setStack(ULONG location);
  //
  virtual void jump(ULONG location);
  virtual void willJump(ULONG location);
  virtual void jsr(ULONG location);
  virtual void rts();
  virtual void pushAddr(ULONG location);
  virtual ULONG pullAddr();
  virtual const char *getRegName(unsigned int n);
  virtual void writeReg(unsigned int n,ULONG val);
  virtual ULONG readReg(unsigned int n);
  //
protected:
  virtual void init();
  bool r0IsZero;
  bool branchesAreDelayed;
  bool tookBranch;
  bool anull;
  ULONG pc,npc;
  // NOTE: Most Risc cpu's use one of their 32 regs as the stack, so don't trust this
  ULONG sp;
  ULONG ir;
  ULONG branch_address;
};


#endif
