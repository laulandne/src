#ifndef ELIB_E8BITCPU_H
#define ELIB_E8BITCPU_H


#include "ECPUAbstract.h"


class ECPU8Bit : public ECPUAbstract {
public:
  // Public member functions
  ECPU8Bit();
  virtual ~ECPU8Bit();
  virtual ULONG getPC() { return pc; }
  virtual ULONG disasmPC() { return disasm(pc); }
  virtual ULONG readStack();
  virtual void setStack(ULONG location);
  virtual void setFramePtr(ULONG location) { }
  virtual ULONG readFramePtr() { return sp; }
  //
  virtual void jump(ULONG location);
  virtual void jsr(ULONG location);
  virtual void rts();
  virtual void pushAddr(ULONG location);
  virtual ULONG pullAddr();
  //
protected:
  virtual void init();
  virtual void stack_push(BYTE val);
  virtual void stack_pushw(UINT16 val);  // NOTE: Little-endian
  virtual BYTE stack_pop();
  virtual UINT16 stack_popw();  // NOTE: Little-endian
  virtual void stack_inc();
  virtual void stack_dec();
  virtual unsigned int getImmByte(ULONG addr);
  virtual ULONG getImmWord(ULONG addr);  // NOTE: Little-endian
  UINT16 pc,sp;
  UINT16 opw,valw;
  UINT16 stackBase,stackMask;
  UINT16 stackCorrection;
};


#endif
