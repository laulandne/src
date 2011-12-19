#ifndef ELIB_ECISCCPU_H
#define ELIB_ECISCCPU_H


#include "ECPUAbstract.h"


class ECPUCisc : public ECPUAbstract {
public:
  // Public member functions
  ECPUCisc();
  virtual ~ECPUCisc();
  virtual ULONG getPC() { return pc; }
  virtual ULONG disasmPC() { return disasm(pc); }
  virtual ULONG readStack();
  virtual void setStack(ULONG location);
  //
  virtual void jump(ULONG location);
  virtual void jsr(ULONG location);
  virtual void rts();
  virtual void pushAddr(ULONG location);
  virtual ULONG pullAddr();
  //
protected:
  virtual void init();
  ULONG pc,sp;
};


#endif
