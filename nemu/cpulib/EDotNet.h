#ifndef CPULIB_EDOTNET_H
#define CPULIB_EDOTNET_H


#include <cpulib/ECPUByteCode.h>


// Address modes
#define EDOTNET_NONE 0
#define EDOTNET_SPECIAL 1
#define EDOTNET_ILLEGAL 2


class EDotNet : public ECPUByteCode
{
public:
  // Public member functions
  EDotNet();
  virtual ~EDotNet();
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
  // Private data members
  void exec_nop(ULONG loc,unsigned int opcode);
  void exec_break(ULONG loc,unsigned int opcode);
  void exec_ldarg_0(ULONG loc,unsigned int opcode);
  void exec_ldarg_1(ULONG loc,unsigned int opcode);
  void exec_ldarg_2(ULONG loc,unsigned int opcode);
  void exec_ldarg_3(ULONG loc,unsigned int opcode);
  void exec_ldloc_0(ULONG loc,unsigned int opcode);
  void exec_ldloc_1(ULONG loc,unsigned int opcode);
  void exec_ldloc_2(ULONG loc,unsigned int opcode);
  void exec_ldloc_3(ULONG loc,unsigned int opcode);
  void exec_stloc_0(ULONG loc,unsigned int opcode);
  void exec_stloc_1(ULONG loc,unsigned int opcode);
  void exec_stloc_2(ULONG loc,unsigned int opcode);
  void exec_stloc_3(ULONG loc,unsigned int opcode);
  void exec_ldarg_s(ULONG loc,unsigned int opcode);
  void exec_ldarga_s(ULONG loc,unsigned int opcode);
  void exec_starg_s(ULONG loc,unsigned int opcode);
  void exec_ldloc_s(ULONG loc,unsigned int opcode);
  void exec_ldloca_s(ULONG loc,unsigned int opcode);
  void exec_stloc_s(ULONG loc,unsigned int opcode);
  void exec_ldnull(ULONG loc,unsigned int opcode);
  void exec_ldc_i4_m1(ULONG loc,unsigned int opcode);
  void exec_ldc_i4_0(ULONG loc,unsigned int opcode);
  void exec_ldc_i4_1(ULONG loc,unsigned int opcode);
  void exec_ldc_i4_2(ULONG loc,unsigned int opcode);
  void exec_ldc_i4_3(ULONG loc,unsigned int opcode);
  void exec_ldc_i4_4(ULONG loc,unsigned int opcode);
  void exec_ldc_i4_5(ULONG loc,unsigned int opcode);
  void exec_ldc_i4_6(ULONG loc,unsigned int opcode);
  void exec_ldc_i4_7(ULONG loc,unsigned int opcode);
  void exec_ldc_i4_8(ULONG loc,unsigned int opcode);
  void exec_ldc_i4_s(ULONG loc,unsigned int opcode);
  void exec_ldc_i4(ULONG loc,unsigned int opcode);
  void exec_ldc_i8(ULONG loc,unsigned int opcode);
  void exec_ldc_r4(ULONG loc,unsigned int opcode);
  void exec_ldc_r8(ULONG loc,unsigned int opcode);
  void exec_dup(ULONG loc,unsigned int opcode);
  void exec_pop(ULONG loc,unsigned int opcode);
  void exec_jmp(ULONG loc,unsigned int opcode);
  void exec_call(ULONG loc,unsigned int opcode);
  void exec_calli(ULONG loc,unsigned int opcode);
  void exec_ret(ULONG loc,unsigned int opcode);
  void exec_br_s(ULONG loc,unsigned int opcode);
  void exec_brfalse_s(ULONG loc,unsigned int opcode);
  void exec_brtrue_s(ULONG loc,unsigned int opcode);
  void exec_beq_s(ULONG loc,unsigned int opcode);
  void exec_bge_s(ULONG loc,unsigned int opcode);
  void exec_bgt_s(ULONG loc,unsigned int opcode);
  void exec_ble_s(ULONG loc,unsigned int opcode);
  void exec_blt_s(ULONG loc,unsigned int opcode);
  void exec_bne_un_s(ULONG loc,unsigned int opcode);
  void exec_bge_un_s(ULONG loc,unsigned int opcode);
  void exec_bgt_un_s(ULONG loc,unsigned int opcode);
  void exec_ble_un_s(ULONG loc,unsigned int opcode);
  void exec_blt_un_s(ULONG loc,unsigned int opcode);
  void exec_br(ULONG loc,unsigned int opcode);
  void exec_brfalse(ULONG loc,unsigned int opcode);
  void exec_brtrue(ULONG loc,unsigned int opcode);
  void exec_beq(ULONG loc,unsigned int opcode);
  void exec_bge(ULONG loc,unsigned int opcode);
  void exec_bgt(ULONG loc,unsigned int opcode);
  void exec_ble(ULONG loc,unsigned int opcode);
  void exec_blt(ULONG loc,unsigned int opcode);
  void exec_bne_un(ULONG loc,unsigned int opcode);
  void exec_bge_un(ULONG loc,unsigned int opcode);
  void exec_bgt_un(ULONG loc,unsigned int opcode);
  void exec_ble_un(ULONG loc,unsigned int opcode);
  void exec_blt_un(ULONG loc,unsigned int opcode);
  void exec_switch(ULONG loc,unsigned int opcode);
  void exec_ldind_i1(ULONG loc,unsigned int opcode);
  void exec_ldind_u1(ULONG loc,unsigned int opcode);
  void exec_ldind_i2(ULONG loc,unsigned int opcode);
  void exec_ldind_u2(ULONG loc,unsigned int opcode);
  void exec_ldind_i4(ULONG loc,unsigned int opcode);
  void exec_ldind_u4(ULONG loc,unsigned int opcode);
  void exec_ldind_i8(ULONG loc,unsigned int opcode);
  void exec_ldind_i(ULONG loc,unsigned int opcode);
  void exec_ldind_r4(ULONG loc,unsigned int opcode);
  void exec_ldind_r8(ULONG loc,unsigned int opcode);
  void exec_ldind_ref(ULONG loc,unsigned int opcode);
  void exec_stind_ref(ULONG loc,unsigned int opcode);
  void exec_stind_i1(ULONG loc,unsigned int opcode);
  void exec_stind_i2(ULONG loc,unsigned int opcode);
  void exec_stind_i4(ULONG loc,unsigned int opcode);
  void exec_stind_i8(ULONG loc,unsigned int opcode);
  void exec_stind_r4(ULONG loc,unsigned int opcode);
  void exec_stind_r8(ULONG loc,unsigned int opcode);
  void exec_add(ULONG loc,unsigned int opcode);
  void exec_sub(ULONG loc,unsigned int opcode);
  void exec_mul(ULONG loc,unsigned int opcode);
  void exec_div(ULONG loc,unsigned int opcode);
  void exec_div_un(ULONG loc,unsigned int opcode);
  void exec_rem(ULONG loc,unsigned int opcode);
  void exec_rem_un(ULONG loc,unsigned int opcode);
  void exec_and(ULONG loc,unsigned int opcode);
  void exec_or(ULONG loc,unsigned int opcode);
  void exec_xor(ULONG loc,unsigned int opcode);
  void exec_shl(ULONG loc,unsigned int opcode);
  void exec_shr(ULONG loc,unsigned int opcode);
  void exec_shr_un(ULONG loc,unsigned int opcode);
  void exec_neg(ULONG loc,unsigned int opcode);
  void exec_not(ULONG loc,unsigned int opcode);
  void exec_conv_i1(ULONG loc,unsigned int opcode);
  void exec_conv_i2(ULONG loc,unsigned int opcode);
  void exec_conv_i4(ULONG loc,unsigned int opcode);
  void exec_conv_i8(ULONG loc,unsigned int opcode);
  void exec_conv_r4(ULONG loc,unsigned int opcode);
  void exec_conv_r8(ULONG loc,unsigned int opcode);
  void exec_conv_u4(ULONG loc,unsigned int opcode);
  void exec_conv_u8(ULONG loc,unsigned int opcode);
  void exec_callvirt(ULONG loc,unsigned int opcode);
  void exec_cpobj(ULONG loc,unsigned int opcode);
  void exec_ldobj(ULONG loc,unsigned int opcode);
  void exec_ldstr(ULONG loc,unsigned int opcode);
  void exec_newobj(ULONG loc,unsigned int opcode);
  void exec_castclass(ULONG loc,unsigned int opcode);
  void exec_isinst(ULONG loc,unsigned int opcode);
  void exec_conv_r_un(ULONG loc,unsigned int opcode);
  void exec_unbox(ULONG loc,unsigned int opcode);
  void exec_throw(ULONG loc,unsigned int opcode);
  void exec_ldfld(ULONG loc,unsigned int opcode);
  void exec_ldflda(ULONG loc,unsigned int opcode);
  void exec_stfld(ULONG loc,unsigned int opcode);
  void exec_ldsfld(ULONG loc,unsigned int opcode);
  void exec_ldsflda(ULONG loc,unsigned int opcode);
  void exec_stsfld(ULONG loc,unsigned int opcode);
  void exec_stobj(ULONG loc,unsigned int opcode);
  void exec_conv_ovf_i1_un(ULONG loc,unsigned int opcode);
  void exec_conv_ovf_i2_un(ULONG loc,unsigned int opcode);
  void exec_conv_ovf_i4_un(ULONG loc,unsigned int opcode);
  void exec_conv_ovf_i8_un(ULONG loc,unsigned int opcode);
  void exec_conv_ovf_u1_un(ULONG loc,unsigned int opcode);
  void exec_conv_ovf_u2_un(ULONG loc,unsigned int opcode);
  void exec_conv_ovf_u4_un(ULONG loc,unsigned int opcode);
  void exec_conv_ovf_u8_un(ULONG loc,unsigned int opcode);
  void exec_conv_ovf_i_un(ULONG loc,unsigned int opcode);
  void exec_conv_ovf_u_un(ULONG loc,unsigned int opcode);
  void exec_box(ULONG loc,unsigned int opcode);
  void exec_newarr(ULONG loc,unsigned int opcode);
  void exec_ldlen(ULONG loc,unsigned int opcode);
  void exec_ldelema(ULONG loc,unsigned int opcode);
  void exec_ldelem_i1(ULONG loc,unsigned int opcode);
  void exec_ldelem_u1(ULONG loc,unsigned int opcode);
  void exec_ldelem_i2(ULONG loc,unsigned int opcode);
  void exec_ldelem_u2(ULONG loc,unsigned int opcode);
  void exec_ldelem_i4(ULONG loc,unsigned int opcode);
  void exec_ldelem_u4(ULONG loc,unsigned int opcode);
  void exec_ldelem_i8(ULONG loc,unsigned int opcode);
  void exec_ldelem_i(ULONG loc,unsigned int opcode);
  void exec_ldelem_r4(ULONG loc,unsigned int opcode);
  void exec_ldelem_r8(ULONG loc,unsigned int opcode);
  void exec_ldelem_ref(ULONG loc,unsigned int opcode);
  void exec_stelem_i(ULONG loc,unsigned int opcode);
  void exec_stelem_i1(ULONG loc,unsigned int opcode);
  void exec_stelem_i2(ULONG loc,unsigned int opcode);
  void exec_stelem_i4(ULONG loc,unsigned int opcode);
  void exec_stelem_i8(ULONG loc,unsigned int opcode);
  void exec_stelem_r4(ULONG loc,unsigned int opcode);
  void exec_stelem_r8(ULONG loc,unsigned int opcode);
  void exec_stelem_ref(ULONG loc,unsigned int opcode);
  void exec_conv_ovf_i1(ULONG loc,unsigned int opcode);
  void exec_conv_ovf_u1(ULONG loc,unsigned int opcode);
  void exec_conv_ovf_i2(ULONG loc,unsigned int opcode);
  void exec_conv_ovf_u2(ULONG loc,unsigned int opcode);
  void exec_conv_ovf_i4(ULONG loc,unsigned int opcode);
  void exec_conv_ovf_u4(ULONG loc,unsigned int opcode);
  void exec_conv_ovf_i8(ULONG loc,unsigned int opcode);
  void exec_conv_ovf_u8(ULONG loc,unsigned int opcode);
  void exec_refanyval(ULONG loc,unsigned int opcode);
  void exec_ckfinite(ULONG loc,unsigned int opcode);
  void exec_mkrefany(ULONG loc,unsigned int opcode);
  void exec_ldtoken(ULONG loc,unsigned int opcode);
  void exec_conv_u2(ULONG loc,unsigned int opcode);
  void exec_conv_u1(ULONG loc,unsigned int opcode);
  void exec_conv_i(ULONG loc,unsigned int opcode);
  void exec_conv_ovf_i(ULONG loc,unsigned int opcode);
  void exec_conv_ovf_u(ULONG loc,unsigned int opcode);
  void exec_add_ovf(ULONG loc,unsigned int opcode);
  void exec_add_ovf_un(ULONG loc,unsigned int opcode);
  void exec_mul_ovf(ULONG loc,unsigned int opcode);
  void exec_mul_ovf_un(ULONG loc,unsigned int opcode);
  void exec_sub_ovf(ULONG loc,unsigned int opcode);
  void exec_sub_ovf_un(ULONG loc,unsigned int opcode);
  void exec_endfinally(ULONG loc,unsigned int opcode);
  void exec_leave(ULONG loc,unsigned int opcode);
  void exec_leave_s(ULONG loc,unsigned int opcode);
  void exec_stind_i(ULONG loc,unsigned int opcode);
  void exec_conv_u(ULONG loc,unsigned int opcode);
  void exec_special(ULONG loc,unsigned int opcode);
};


#endif // CPULIB_DOTNET_H
