#ifndef ELIB_EBYTECODECPU_H
#define ELIB_EBYTECODECPU_H


#include <cpulib/ECPUAbstract.h>
#include <cpulib/ECPUByteCodeObject.h>


class ECPUByteCode : public ECPUAbstract {
public:
  // Public member functions
  ECPUByteCode();
  virtual ~ECPUByteCode();
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
  virtual void pushObj(ECPUByteCodeObject *obj);
  virtual ECPUByteCodeObject *popObj();
  virtual ECPUByteCodeObject *topObj();
  virtual ECPUByteCodeObject *getNObj(unsigned int n);
  virtual void stackInc();
  virtual void stackDec();
  ULONG pc,sp;
  ECPUByteCodeObject obj;
  AList methods;
  AList classes;
  AList objects;
};


#endif
