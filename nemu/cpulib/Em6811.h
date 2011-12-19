#ifndef CPULIB_EM6811_H
#define CPULIB_EM6811_H


#include <cpulib/Em6800.h>


class Em6811 : public Em6800
{
public:
  // Public member functions
  Em6811();
  virtual ~Em6811();
  static bool recognize(const char *name);
  virtual const char *getName();
  virtual void execute();
  virtual ULONG disasm(ULONG loc);
  virtual void wregs();
  // Public data members
protected:
  // Private member functions
  virtual void init();
  void exec_special(ULONG loc,unsigned int op);
  void exec_aba(ULONG loc,unsigned int op);
  void exec_asrd(ULONG loc,unsigned int op);
  void exec_brset(ULONG loc,unsigned int op);
  void exec_brclr(ULONG loc,unsigned int op);
  void exec_bset(ULONG loc,unsigned int op);
  void exec_bclr(ULONG loc,unsigned int op);
  void exec_fdiv(ULONG loc,unsigned int op);
  void exec_idiv(ULONG loc,unsigned int op);
  void exec_lsrd(ULONG loc,unsigned int op);
  void exec_stop(ULONG loc,unsigned int op);
  void exec_tab(ULONG loc,unsigned int op);
  void exec_test(ULONG loc,unsigned int op);
  void exec_xgdx(ULONG loc,unsigned int op);
};


#endif
