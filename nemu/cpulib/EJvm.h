#ifndef CPULIB_EJVM_H
#define CPULIB_EJVM_H


#include <cpulib/ECPUByteCode.h>

#include <emu/NJavaHandler.h>

#include <binarylib/NJavaClassDecoder.h>


class EMapSymbol;


// Address modes
#define EJVM_ADDR_NONE 0
#define EJVM_ADDR_ONE 1
#define EJVM_ADDR_TWO 2
#define EJVM_ADDR_IMM8 3
#define EJVM_ADDR_IMM16 4
#define EJVM_ADDR_SPECIAL 5
#define EJVM_ADDR_ILLEGAL 6


class EJvm : public ECPUByteCode
{
public:
  // Public member functions
  EJvm();
  virtual ~EJvm();
  static bool recognize(const char *name);
  virtual const char *getName();
  virtual void setKind(const char *name);
  virtual void execute();
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
  void exec_nop(ULONG loc,unsigned int opcode);
  void exec_aconst_null(ULONG loc,unsigned int opcode);
  void exec_iconst_m1(ULONG loc,unsigned int opcode);
  void exec_iconst_0(ULONG loc,unsigned int opcode);
  void exec_iconst_1(ULONG loc,unsigned int opcode);
  void exec_iconst_2(ULONG loc,unsigned int opcode);
  void exec_iconst_3(ULONG loc,unsigned int opcode);
  void exec_iconst_4(ULONG loc,unsigned int opcode);
  void exec_iconst_5(ULONG loc,unsigned int opcode);
  void exec_lconst_0(ULONG loc,unsigned int opcode);
  void exec_lconst_1(ULONG loc,unsigned int opcode);
  void exec_fconst_0(ULONG loc,unsigned int opcode);
  void exec_fconst_1(ULONG loc,unsigned int opcode);
  void exec_fconst_2(ULONG loc,unsigned int opcode);
  void exec_dconst_0(ULONG loc,unsigned int opcode);
  void exec_dconst_1(ULONG loc,unsigned int opcode);
  void exec_bipush(ULONG loc,unsigned int opcode);
  void exec_sipush(ULONG loc,unsigned int opcode);
  void exec_ldc(ULONG loc,unsigned int opcode);
  void exec_ldc_w(ULONG loc,unsigned int opcode);
  void exec_ldc2_w(ULONG loc,unsigned int opcode);
  void exec_iload(ULONG loc,unsigned int opcode);
  void exec_lload(ULONG loc,unsigned int opcode);
  void exec_fload(ULONG loc,unsigned int opcode);
  void exec_dload(ULONG loc,unsigned int opcode);
  void exec_aload(ULONG loc,unsigned int opcode);
  void exec_iload_0(ULONG loc,unsigned int opcode);
  void exec_iload_1(ULONG loc,unsigned int opcode);
  void exec_iload_2(ULONG loc,unsigned int opcode);
  void exec_iload_3(ULONG loc,unsigned int opcode);
  void exec_lload_0(ULONG loc,unsigned int opcode);
  void exec_lload_1(ULONG loc,unsigned int opcode);
  void exec_lload_2(ULONG loc,unsigned int opcode);
  void exec_lload_3(ULONG loc,unsigned int opcode);
  void exec_fload_0(ULONG loc,unsigned int opcode);
  void exec_fload_1(ULONG loc,unsigned int opcode);
  void exec_fload_2(ULONG loc,unsigned int opcode);
  void exec_fload_3(ULONG loc,unsigned int opcode);
  void exec_dload_0(ULONG loc,unsigned int opcode);
  void exec_dload_1(ULONG loc,unsigned int opcode);
  void exec_dload_2(ULONG loc,unsigned int opcode);
  void exec_dload_3(ULONG loc,unsigned int opcode);
  void exec_aload_0(ULONG loc,unsigned int opcode);
  void exec_aload_1(ULONG loc,unsigned int opcode);
  void exec_aload_2(ULONG loc,unsigned int opcode);
  void exec_aload_3(ULONG loc,unsigned int opcode);
  void exec_iaload(ULONG loc,unsigned int opcode);
  void exec_laload(ULONG loc,unsigned int opcode);
  void exec_faload(ULONG loc,unsigned int opcode);
  void exec_daload(ULONG loc,unsigned int opcode);
  void exec_aaload(ULONG loc,unsigned int opcode);
  void exec_baload(ULONG loc,unsigned int opcode);
  void exec_caload(ULONG loc,unsigned int opcode);
  void exec_saload(ULONG loc,unsigned int opcode);
  void exec_istore(ULONG loc,unsigned int opcode);
  void exec_lstore(ULONG loc,unsigned int opcode);
  void exec_fstore(ULONG loc,unsigned int opcode);
  void exec_dstore(ULONG loc,unsigned int opcode);
  void exec_astore(ULONG loc,unsigned int opcode);
  void exec_istore_0(ULONG loc,unsigned int opcode);
  void exec_istore_1(ULONG loc,unsigned int opcode);
  void exec_istore_2(ULONG loc,unsigned int opcode);
  void exec_istore_3(ULONG loc,unsigned int opcode);
  void exec_lstore_0(ULONG loc,unsigned int opcode);
  void exec_lstore_1(ULONG loc,unsigned int opcode);
  void exec_lstore_2(ULONG loc,unsigned int opcode);
  void exec_lstore_3(ULONG loc,unsigned int opcode);
  void exec_fstore_0(ULONG loc,unsigned int opcode);
  void exec_fstore_1(ULONG loc,unsigned int opcode);
  void exec_fstore_2(ULONG loc,unsigned int opcode);
  void exec_fstore_3(ULONG loc,unsigned int opcode);
  void exec_dstore_0(ULONG loc,unsigned int opcode);
  void exec_dstore_1(ULONG loc,unsigned int opcode);
  void exec_dstore_2(ULONG loc,unsigned int opcode);
  void exec_dstore_3(ULONG loc,unsigned int opcode);
  void exec_astore_0(ULONG loc,unsigned int opcode);
  void exec_astore_1(ULONG loc,unsigned int opcode);
  void exec_astore_2(ULONG loc,unsigned int opcode);
  void exec_astore_3(ULONG loc,unsigned int opcode);
  void exec_iastore(ULONG loc,unsigned int opcode);
  void exec_lastore(ULONG loc,unsigned int opcode);
  void exec_fastore(ULONG loc,unsigned int opcode);
  void exec_dastore(ULONG loc,unsigned int opcode);
  void exec_aastore(ULONG loc,unsigned int opcode);
  void exec_bastore(ULONG loc,unsigned int opcode);
  void exec_castore(ULONG loc,unsigned int opcode);
  void exec_sastore(ULONG loc,unsigned int opcode);
  void exec_pop(ULONG loc,unsigned int opcode);
  void exec_pop2(ULONG loc,unsigned int opcode);
  void exec_dup(ULONG loc,unsigned int opcode);
  void exec_dup_x1(ULONG loc,unsigned int opcode);
  void exec_dup_x2(ULONG loc,unsigned int opcode);
  void exec_dup2(ULONG loc,unsigned int opcode);
  void exec_dup2_x1(ULONG loc,unsigned int opcode);
  void exec_dup2_x2(ULONG loc,unsigned int opcode);
  void exec_swap(ULONG loc,unsigned int opcode);
  void exec_iadd(ULONG loc,unsigned int opcode);
  void exec_ladd(ULONG loc,unsigned int opcode);
  void exec_fadd(ULONG loc,unsigned int opcode);
  void exec_dadd(ULONG loc,unsigned int opcode);
  void exec_isub(ULONG loc,unsigned int opcode);
  void exec_lsub(ULONG loc,unsigned int opcode);
  void exec_fsub(ULONG loc,unsigned int opcode);
  void exec_dsub(ULONG loc,unsigned int opcode);
  void exec_imul(ULONG loc,unsigned int opcode);
  void exec_lmul(ULONG loc,unsigned int opcode);
  void exec_fmul(ULONG loc,unsigned int opcode);
  void exec_dmul(ULONG loc,unsigned int opcode);
  void exec_idiv(ULONG loc,unsigned int opcode);
  void exec_ldiv(ULONG loc,unsigned int opcode);
  void exec_fdiv(ULONG loc,unsigned int opcode);
  void exec_ddiv(ULONG loc,unsigned int opcode);
  void exec_irem(ULONG loc,unsigned int opcode);
  void exec_lrem(ULONG loc,unsigned int opcode);
  void exec_frem(ULONG loc,unsigned int opcode);
  void exec_drem(ULONG loc,unsigned int opcode);
  void exec_ineg(ULONG loc,unsigned int opcode);
  void exec_lneg(ULONG loc,unsigned int opcode);
  void exec_fneg(ULONG loc,unsigned int opcode);
  void exec_dneg(ULONG loc,unsigned int opcode);
  void exec_ishl(ULONG loc,unsigned int opcode);
  void exec_lshl(ULONG loc,unsigned int opcode);
  void exec_ishr(ULONG loc,unsigned int opcode);
  void exec_lshr(ULONG loc,unsigned int opcode);
  void exec_iushr(ULONG loc,unsigned int opcode);
  void exec_lushr(ULONG loc,unsigned int opcode);
  void exec_iand(ULONG loc,unsigned int opcode);
  void exec_land(ULONG loc,unsigned int opcode);
  void exec_ior(ULONG loc,unsigned int opcode);
  void exec_lor(ULONG loc,unsigned int opcode);
  void exec_ixor(ULONG loc,unsigned int opcode);
  void exec_lxor(ULONG loc,unsigned int opcode);
  void exec_iinc(ULONG loc,unsigned int opcode);
  void exec_i2l(ULONG loc,unsigned int opcode);
  void exec_i2f(ULONG loc,unsigned int opcode);
  void exec_i2d(ULONG loc,unsigned int opcode);
  void exec_l2i(ULONG loc,unsigned int opcode);
  void exec_l2f(ULONG loc,unsigned int opcode);
  void exec_l2d(ULONG loc,unsigned int opcode);
  void exec_f2i(ULONG loc,unsigned int opcode);
  void exec_f2l(ULONG loc,unsigned int opcode);
  void exec_f2d(ULONG loc,unsigned int opcode);
  void exec_d2i(ULONG loc,unsigned int opcode);
  void exec_d2l(ULONG loc,unsigned int opcode);
  void exec_d2f(ULONG loc,unsigned int opcode);
  void exec_i2b(ULONG loc,unsigned int opcode);
  void exec_i2c(ULONG loc,unsigned int opcode);
  void exec_i2s(ULONG loc,unsigned int opcode);
  void exec_lcmp(ULONG loc,unsigned int opcode);
  void exec_fcmpl(ULONG loc,unsigned int opcode);
  void exec_fcmpg(ULONG loc,unsigned int opcode);
  void exec_dcmpl(ULONG loc,unsigned int opcode);
  void exec_dcmpg(ULONG loc,unsigned int opcode);
  void exec_ifeq(ULONG loc,unsigned int opcode);
  void exec_ifne(ULONG loc,unsigned int opcode);
  void exec_iflt(ULONG loc,unsigned int opcode);
  void exec_ifge(ULONG loc,unsigned int opcode);
  void exec_ifgt(ULONG loc,unsigned int opcode);
  void exec_ifle(ULONG loc,unsigned int opcode);
  void exec_if_icmpeq(ULONG loc,unsigned int opcode);
  void exec_if_icmpne(ULONG loc,unsigned int opcode);
  void exec_if_icmplt(ULONG loc,unsigned int opcode);
  void exec_if_icmpge(ULONG loc,unsigned int opcode);
  void exec_if_icmpgt(ULONG loc,unsigned int opcode);
  void exec_if_icmple(ULONG loc,unsigned int opcode);
  void exec_if_acmpeq(ULONG loc,unsigned int opcode);
  void exec_ifacmpne(ULONG loc,unsigned int opcode);
  void exec_goto(ULONG loc,unsigned int opcode);
  void exec_jsr(ULONG loc,unsigned int opcode);
  void exec_ret(ULONG loc,unsigned int opcode);
  void exec_tableswitch(ULONG loc,unsigned int opcode);
  void exec_lookupswitch(ULONG loc,unsigned int opcode);
  void exec_ireturn(ULONG loc,unsigned int opcode);
  void exec_lreturn(ULONG loc,unsigned int opcode);
  void exec_freturn(ULONG loc,unsigned int opcode);
  void exec_dreturn(ULONG loc,unsigned int opcode);
  void exec_areturn(ULONG loc,unsigned int opcode);
  void exec_return(ULONG loc,unsigned int opcode);
  void exec_getstatic(ULONG loc,unsigned int opcode);
  void exec_putstatic(ULONG loc,unsigned int opcode);
  void exec_getfield(ULONG loc,unsigned int opcode);
  void exec_putfield(ULONG loc,unsigned int opcode);
  void exec_invokevirtual(ULONG loc,unsigned int opcode);
  void exec_invokespecial(ULONG loc,unsigned int opcode);
  void exec_invokestatic(ULONG loc,unsigned int opcode);
  void exec_invokeinterface(ULONG loc,unsigned int opcode);
  void exec_new(ULONG loc,unsigned int opcode);
  void exec_newarray(ULONG loc,unsigned int opcode);
  void exec_anewarray(ULONG loc,unsigned int opcode);
  void exec_arraylength(ULONG loc,unsigned int opcode);
  void exec_athrow(ULONG loc,unsigned int opcode);
  void exec_checkcast(ULONG loc,unsigned int opcode);
  void exec_instanceof(ULONG loc,unsigned int opcode);
  void exec_monitorenter(ULONG loc,unsigned int opcode);
  void exec_monitorexit(ULONG loc,unsigned int opcode);
  void exec_wide(ULONG loc,unsigned int opcode);
  void exec_multinewarray(ULONG loc,unsigned int opcode);
  void exec_ifnull(ULONG loc,unsigned int opcode);
  void exec_ifnonnull(ULONG loc,unsigned int opcode);
  void exec_goto_w(ULONG loc,unsigned int opcode);
  void exec_jsr_w(ULONG loc,unsigned int opcode);
  void exec_breakpoint(ULONG loc,unsigned int opcode);
  void exec_ldc_quick(ULONG loc,unsigned int opcode);
  void exec_ldc_w_quick(ULONG loc,unsigned int opcode);
  void exec_ldc2_w_quick(ULONG loc,unsigned int opcode);
  void exec_getfield_quick(ULONG loc,unsigned int opcode);
  void exec_putfield_quick(ULONG loc,unsigned int opcode);
  void exec_getfield2_quick(ULONG loc,unsigned int opcode);
  void exec_putfield2_quick(ULONG loc,unsigned int opcode);
  void exec_getstatic_quick(ULONG loc,unsigned int opcode);
  void exec_putstatic_quick(ULONG loc,unsigned int opcode);
  void exec_getstatic2_quick(ULONG loc,unsigned int opcode);
  void exec_putstatic2_quick(ULONG loc,unsigned int opcode);
  void exec_invokevirtual_quick(ULONG loc,unsigned int opcode);
  void exec_invokenonvirtual_quick(ULONG loc,unsigned int opcode);
  void exec_invokesuper_quick(ULONG loc,unsigned int opcode);
  void exec_invokestatic_quick(ULONG loc,unsigned int opcode);
  void exec_invokeinterface_quick(ULONG loc,unsigned int opcode);
  void exec_invokevirtualobject_quick(ULONG loc,unsigned int opcode);
  void exec_new_quick(ULONG loc,unsigned int opcode);
  void exec_anewarray_quick(ULONG loc,unsigned int opcode);
  void exec_multinewarray_quick(ULONG loc,unsigned int opcode);
  void exec_checkcast_quick(ULONG loc,unsigned int opcode);
  void exec_instanceof_quick(ULONG loc,unsigned int opcode);
  void exec_invokevirtual_quick_w(ULONG loc,unsigned int opcode);
  void exec_getfield_quick_w(ULONG loc,unsigned int opcode);
  void exec_putfield_quick_w(ULONG loc,unsigned int opcode);
  void exec_impdep1(ULONG loc,unsigned int opcode);
  void exec_impdep2(ULONG loc,unsigned int opcode);
  // misc helper methods
  void invoke_helper(ULONG loc,unsigned int opcode);
  void iload_helper(ULONG loc,unsigned int opcode,unsigned int num);
  void aload_helper(ULONG loc,unsigned int opcode,unsigned int num);
  void lload_helper(ULONG loc,unsigned int opcode,unsigned int num);
  void fload_helper(ULONG loc,unsigned int opcode,unsigned int num);
  void dload_helper(ULONG loc,unsigned int opcode,unsigned int num);
  void iconst_helper(ULONG loc,unsigned int opcode,unsigned int num);
  void lconst_helper(ULONG loc,unsigned int opcode,unsigned int num);
  void fconst_helper(ULONG loc,unsigned int opcode,unsigned int num);
  void dconst_helper(ULONG loc,unsigned int opcode,unsigned int num);
  // symbol helper methods
  void getStringAtAddr(char *s,unsigned int len,ULONG addr);
  unsigned int getNameString(char *s,unsigned int index);
  unsigned int getNameStringVal(char *s,unsigned int index);
  unsigned int getNameStringVal2(char *s,unsigned int index);
  unsigned int getNameStringMisc(char *s,unsigned int index);
  void getConstant(EMapSymbol *c,unsigned int num);
  // Private data members
  char s[256];
};


#endif
