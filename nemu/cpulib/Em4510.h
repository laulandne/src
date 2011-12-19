#ifndef CPULIB_EM4510_H
#define CPULIB_EM4510_H


#include <cpulib/Em6502.h>


// The cpu in the Commodore 65 (C64dx) "Victor"
class Em4510 : public Em6502
{
public:
  // Public member functions
  Em4510();
  virtual ~Em4510();
  static bool recognize(const char *name);
  virtual const char *getName();
  virtual void setKind(const char *name);
  virtual void reset();
  virtual void nmi();
  virtual void ints();
  virtual void wregs();
  virtual void executeInstead(ULONG opcode);
  virtual void execute();
  virtual ULONG disasm(ULONG loc);
  virtual bool setRegister(const char *name,ULONG value);
  virtual ULONG readRegister(const char *name);
  virtual void changeMap(EMemMapAbstract *mmap);
  virtual void handle_int(ULONG value);
  virtual bool interp(const char *cmd,const char *a1,const char *a2,const char *a3);
  // Public data members
protected:
  // Private member functions
  void handle_map();
  virtual void handle_brk();
  virtual void handle_adc(unsigned int aa);
  virtual void handle_sbc(unsigned int aa);
  virtual UINT16 handle_adc_d(unsigned int aa, unsigned int bb);
  virtual UINT16 handle_sbc_d(unsigned int aa, unsigned int bb);
  virtual bool execMain();
  virtual void disasmAddrMode(ULONG loc,unsigned int mode);
  virtual void setStatusBits(BYTE mask);
  virtual void resetStatusBits(BYTE mask);
  virtual void setStatusReg(unsigned int newValue);
  virtual BYTE getStatusReg();
  // NOTE: We've got our own stack handing funcs because of the wider address space
  virtual void stack_push(BYTE val);
  virtual void stack_pushw(UINT16 val);
  virtual BYTE stack_pop();
  virtual UINT16 stack_popw();
  virtual void stack_inc();
  virtual void stack_dec();
  void show_map();
  void set_flags_w_nz(UINT16 i);
  void set_flags_w_nzc(UINT16 i, UINT16 j);
  // Private data members
  unsigned int translate[65536L];
  bool stackExtend;
  unsigned int mapUpper,mapLower;
  BYTE mapBlocks;
  BYTE z;     // another index register
  UINT16 dp;  // direct page address in bytes
};


#endif
