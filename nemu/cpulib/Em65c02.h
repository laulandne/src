#ifndef CPULIB_EM65C02_H
#define CPULIB_EM65C02_H


#include <cpulib/Em6502.h>


class Em65c02 : public Em6502
{
public:
  // Public member functions
  Em65c02();
  virtual ~Em65c02();
  static bool recognize(const char *name);
  virtual const char *getName();
  virtual void setKind(const char *name);
  virtual void reset();
  virtual void wregs();
  virtual void executeInstead(ULONG opcode);
  virtual void execute();
  virtual ULONG disasm(ULONG loc);
  // Public data members
protected:
  // Private member functions
  virtual void handle_adc(unsigned int aa);
  virtual void handle_sbc(unsigned int aa);
  virtual UINT16 handle_adc_d(unsigned int aa, unsigned int bb);
  virtual UINT16 handle_sbc_d(unsigned int aa, unsigned int bb);
  virtual bool execMain();
  // Private data members
};


#endif
