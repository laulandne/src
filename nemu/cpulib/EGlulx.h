#ifndef CPULIB_EGLULX_H
#define CPULIB_EGLULX_H


#include <cpulib/ECPUByteCode.h>


/*
class EGLULX : public ECPUByteCode
{
public:
  // Public member functions
  EGLULX();
  virtual ~EGLULX();
  static bool recognize(const char *name);
  virtual const char *getName();
  virtual void setKind(const char *name);
  virtual void execute();
  virtual void handle_int(ULONG value);
  virtual void cause_int(ULONG value);
  virtual void ints();
  virtual bool setRegister(const char *name,ULONG value);
  virtual ULONG readRegister(const char *name);
  virtual ULONG readStack() { return sp; }
  virtual ULONG disasm(ULONG loc);
  virtual unsigned int printAddrMode(ULONG loc,unsigned int topcode,unsigned int mode);
  virtual void reset();
  virtual void wregs();
  // Public data members
protected:
  // Private member functions
  virtual void init();
  //
  void exec_nop(ULONG loc,unsigned int opcode);
  void exec_add(ULONG loc,unsigned int opcode);
  void exec_sub(ULONG loc,unsigned int opcode);
  void exec_mul(ULONG loc,unsigned int opcode);
  void exec_div(ULONG loc,unsigned int opcode);
  void exec_mod(ULONG loc,unsigned int opcode);
  void exec_neg(ULONG loc,unsigned int opcode);
  void exec_bitand(ULONG loc,unsigned int opcode);
  void exec_bitor(ULONG loc,unsigned int opcode);
  void exec_bitxor(ULONG loc,unsigned int opcode);
  void exec_bitnot(ULONG loc,unsigned int opcode);
  void exec_shiftl(ULONG loc,unsigned int opcode);
  void exec_sshiftr(ULONG loc,unsigned int opcode);
  void exec_ushiftr(ULONG loc,unsigned int opcode);
  void exec_jump(ULONG loc,unsigned int opcode);
  void exec_jz(ULONG loc,unsigned int opcode);
  void exec_jnz(ULONG loc,unsigned int opcode);
  void exec_jeq(ULONG loc,unsigned int opcode);
  void exec_jne(ULONG loc,unsigned int opcode);
  void exec_jlt(ULONG loc,unsigned int opcode);
  void exec_jge(ULONG loc,unsigned int opcode);
  void exec_jgt(ULONG loc,unsigned int opcode);
  void exec_jle(ULONG loc,unsigned int opcode);
  void exec_jltu(ULONG loc,unsigned int opcode);
  void exec_jgeu(ULONG loc,unsigned int opcode);
  void exec_jgtu(ULONG loc,unsigned int opcode);
  void exec_jleu(ULONG loc,unsigned int opcode);
  void exec_call(ULONG loc,unsigned int opcode);
  void exec_return(ULONG loc,unsigned int opcode);
  void exec_catch(ULONG loc,unsigned int opcode);
  void exec_throw(ULONG loc,unsigned int opcode);
  void exec_tailcall(ULONG loc,unsigned int opcode);
  void exec_copy(ULONG loc,unsigned int opcode);
  void exec_copys(ULONG loc,unsigned int opcode);
  void exec_copyb(ULONG loc,unsigned int opcode);
  void exec_sexs(ULONG loc,unsigned int opcode);
  void exec_sexb(ULONG loc,unsigned int opcode);
  void exec_aload(ULONG loc,unsigned int opcode);
  void exec_aloads(ULONG loc,unsigned int opcode);
  void exec_aloadb(ULONG loc,unsigned int opcode);
  void exec_aloadbit(ULONG loc,unsigned int opcode);
  void exec_astore(ULONG loc,unsigned int opcode);
  void exec_astores(ULONG loc,unsigned int opcode);
  void exec_astoreb(ULONG loc,unsigned int opcode);
  void exec_astorebit(ULONG loc,unsigned int opcode);
  void exec_stkcount(ULONG loc,unsigned int opcode);
  void exec_stkpeek(ULONG loc,unsigned int opcode);
  void exec_stkswap(ULONG loc,unsigned int opcode);
  void exec_stkroll(ULONG loc,unsigned int opcode);
  void exec_stkcopy(ULONG loc,unsigned int opcode);
  void exec_streamchar(ULONG loc,unsigned int opcode);
  void exec_streamnum(ULONG loc,unsigned int opcode);
  void exec_streamstr(ULONG loc,unsigned int opcode);
  void exec_gestalt(ULONG loc,unsigned int opcode);
  void exec_debugtrap(ULONG loc,unsigned int opcode);
  void exec_getmemsize(ULONG loc,unsigned int opcode);
  void exec_setmemsize(ULONG loc,unsigned int opcode);
  void exec_jumpabs(ULONG loc,unsigned int opcode);
  void exec_random(ULONG loc,unsigned int opcode);
  void exec_setrandom(ULONG loc,unsigned int opcode);
  void exec_quit(ULONG loc,unsigned int opcode);
  void exec_verify(ULONG loc,unsigned int opcode);
  void exec_restart(ULONG loc,unsigned int opcode);
  void exec_save(ULONG loc,unsigned int opcode);
  void exec_restore(ULONG loc,unsigned int opcode);
  void exec_saveundo(ULONG loc,unsigned int opcode);
  void exec_restoreundo(ULONG loc,unsigned int opcode);
  void exec_protect(ULONG loc,unsigned int opcode);
  void exec_glk(ULONG loc,unsigned int opcode);
  void exec_getstringtbl(ULONG loc,unsigned int opcode);
  void exec_setstringtbl(ULONG loc,unsigned int opcode);
  void exec_getiosys(ULONG loc,unsigned int opcode);
  void exec_setiosys(ULONG loc,unsigned int opcode);
  void exec_linearsearch(ULONG loc,unsigned int opcode);
  void exec_binarysearch(ULONG loc,unsigned int opcode);
  void exec_linkedsearch(ULONG loc,unsigned int opcode);
  void exec_callf(ULONG loc,unsigned int opcode);
  void exec_callfi(ULONG loc,unsigned int opcode);
  void exec_callfii(ULONG loc,unsigned int opcode);
  void exec_callfiii(ULONG loc,unsigned int opcode);
  // Private data members
};
*/


#endif
