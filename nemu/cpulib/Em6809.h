#ifndef CPULIB_EM6809_H
#define CPULIB_EM6809_H


#include <cpulib/Em6800.h>


class Em6809 : public Em6800
{
public:
  // Public member functions
  Em6809();
  virtual ~Em6809();
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
  void exec_andcc(ULONG loc,unsigned int op);
  void exec_cmpd(ULONG loc,unsigned int op);
  void exec_cmpu(ULONG loc,unsigned int op);
  void exec_cmps(ULONG loc,unsigned int op);
  void exec_lbcc(ULONG loc,unsigned int op);
  void exec_lbcs(ULONG loc,unsigned int op);
  void exec_lbeq(ULONG loc,unsigned int op);
  void exec_lbge(ULONG loc,unsigned int op);
  void exec_lbgt(ULONG loc,unsigned int op);
  void exec_lbhi(ULONG loc,unsigned int op);
  void exec_lble(ULONG loc,unsigned int op);
  void exec_lbls(ULONG loc,unsigned int op);
  void exec_lblt(ULONG loc,unsigned int op);
  void exec_lbmi(ULONG loc,unsigned int op);
  void exec_lbne(ULONG loc,unsigned int op);
  void exec_lbpl(ULONG loc,unsigned int op);
  void exec_lbvc(ULONG loc,unsigned int op);
  void exec_lbvs(ULONG loc,unsigned int op);
  void exec_lbra(ULONG loc,unsigned int op);
  void exec_lbrn(ULONG loc,unsigned int op);
  void exec_lbsr(ULONG loc,unsigned int op);
  void exec_ldu(ULONG loc,unsigned int op);
  void exec_leas(ULONG loc,unsigned int op);
  void exec_leau(ULONG loc,unsigned int op);
  void exec_leax(ULONG loc,unsigned int op);
  void exec_leay(ULONG loc,unsigned int op);
  void exec_orcc(ULONG loc,unsigned int op);
  void exec_pshu(ULONG loc,unsigned int op);
  void exec_pulu(ULONG loc,unsigned int op);
  void exec_sex(ULONG loc,unsigned int op);
  void exec_stu(ULONG loc,unsigned int op);
  void exec_swi1(ULONG loc,unsigned int op);
  void exec_swi2(ULONG loc,unsigned int op);
};


#endif
