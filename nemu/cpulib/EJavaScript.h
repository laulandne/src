#ifndef CPULIB_EJAVASCRIPT_H
#define CPULIB_EJAVASCRIPT_H


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


class EJavascript : public ECPUByteCode
{
public:
  // Public member functions
  EJavascript();
  virtual ~EJavascript();
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
  void exec_push(ULONG loc,unsigned int opcode);
  void exec_popv(ULONG loc,unsigned int opcode);
  void exec_enterwith(ULONG loc,unsigned int opcode);
  void exec_leavewith(ULONG loc,unsigned int opcode);
  void exec_return(ULONG loc,unsigned int opcode);
  void exec_goto(ULONG loc,unsigned int opcode);
  void exec_ifeq(ULONG loc,unsigned int opcode);
  void exec_ifne(ULONG loc,unsigned int opcode);
  void exec_js_arguments_str(ULONG loc,unsigned int opcode);
  void exec_forarg(ULONG loc,unsigned int opcode);
  void exec_forvar(ULONG loc,unsigned int opcode);
  void exec_dup(ULONG loc,unsigned int opcode);
  void exec_dup2(ULONG loc,unsigned int opcode);
  void exec_setconst(ULONG loc,unsigned int opcode);
  void exec_bitor(ULONG loc,unsigned int opcode);
  void exec_bitxor(ULONG loc,unsigned int opcode);
  void exec_bitand(ULONG loc,unsigned int opcode);
  void exec_eq(ULONG loc,unsigned int opcode);
  void exec_ne(ULONG loc,unsigned int opcode);
  void exec_lt(ULONG loc,unsigned int opcode);
  void exec_le(ULONG loc,unsigned int opcode);
  void exec_gt(ULONG loc,unsigned int opcode);
  void exec_ge(ULONG loc,unsigned int opcode);
  void exec_lsh(ULONG loc,unsigned int opcode);
  void exec_rsh(ULONG loc,unsigned int opcode);
  void exec_ursh(ULONG loc,unsigned int opcode);
  void exec_add(ULONG loc,unsigned int opcode);
  void exec_sub(ULONG loc,unsigned int opcode);
  void exec_mul(ULONG loc,unsigned int opcode);
  void exec_div(ULONG loc,unsigned int opcode);
  void exec_mod(ULONG loc,unsigned int opcode);
  void exec_not(ULONG loc,unsigned int opcode);
  void exec_bitnot(ULONG loc,unsigned int opcode);
  void exec_neg(ULONG loc,unsigned int opcode);
  void exec_js_new_str(ULONG loc,unsigned int opcode);
  void exec_delname(ULONG loc,unsigned int opcode);
  void exec_delprop(ULONG loc,unsigned int opcode);
  void exec_delelem(ULONG loc,unsigned int opcode);
  void exec_js_typeof_str(ULONG loc,unsigned int opcode);
  void exec_js_void_str(ULONG loc,unsigned int opcode);
  void exec_incname(ULONG loc,unsigned int opcode);
  void exec_incprop(ULONG loc,unsigned int opcode);
  void exec_incelem(ULONG loc,unsigned int opcode);
  void exec_decname(ULONG loc,unsigned int opcode);
  void exec_decprop(ULONG loc,unsigned int opcode);
  void exec_decelem(ULONG loc,unsigned int opcode);
  void exec_nameinc(ULONG loc,unsigned int opcode);
  void exec_propinc(ULONG loc,unsigned int opcode);
  void exec_eleminc(ULONG loc,unsigned int opcode);
  void exec_namedec(ULONG loc,unsigned int opcode);
  void exec_propdec(ULONG loc,unsigned int opcode);
  void exec_elemdec(ULONG loc,unsigned int opcode);
  void exec_getprop(ULONG loc,unsigned int opcode);
  void exec_setprop(ULONG loc,unsigned int opcode);
  void exec_getelem(ULONG loc,unsigned int opcode);
  void exec_setelem(ULONG loc,unsigned int opcode);
  void exec_pushobj(ULONG loc,unsigned int opcode);
  void exec_call(ULONG loc,unsigned int opcode);
  void exec_name(ULONG loc,unsigned int opcode);
  void exec_number(ULONG loc,unsigned int opcode);
  void exec_string(ULONG loc,unsigned int opcode);
  void exec_zero(ULONG loc,unsigned int opcode);
  void exec_one(ULONG loc,unsigned int opcode);
  void exec_js_null_str(ULONG loc,unsigned int opcode);
  void exec_js_this_str(ULONG loc,unsigned int opcode);
  void exec_js_false_str(ULONG loc,unsigned int opcode);
  void exec_js_true_str(ULONG loc,unsigned int opcode);
  void exec_or(ULONG loc,unsigned int opcode);
  void exec_and(ULONG loc,unsigned int opcode);
  void exec_tableswitch(ULONG loc,unsigned int opcode);
  void exec_lookupswitch(ULONG loc,unsigned int opcode);
  void exec_closure(ULONG loc,unsigned int opcode);
  void exec_exportall(ULONG loc,unsigned int opcode);
  void exec_exportname(ULONG loc,unsigned int opcode);
  void exec_importall(ULONG loc,unsigned int opcode);
  void exec_importprop(ULONG loc,unsigned int opcode);
  void exec_importelem(ULONG loc,unsigned int opcode);
  void exec_object(ULONG loc,unsigned int opcode);
  void exec_pop(ULONG loc,unsigned int opcode);
  void exec_pos(ULONG loc,unsigned int opcode);
  void exec_trap(ULONG loc,unsigned int opcode);
  void exec_getarg(ULONG loc,unsigned int opcode);
  void exec_setarg(ULONG loc,unsigned int opcode);
  void exec_getvar(ULONG loc,unsigned int opcode);
  void exec_setvar(ULONG loc,unsigned int opcode);
  void exec_uint16(ULONG loc,unsigned int opcode);
  void exec_newinit(ULONG loc,unsigned int opcode);
  void exec_endinit(ULONG loc,unsigned int opcode);
  void exec_initprop(ULONG loc,unsigned int opcode);
  void exec_initelem(ULONG loc,unsigned int opcode);
  void exec_defsharp(ULONG loc,unsigned int opcode);
  void exec_usesharp(ULONG loc,unsigned int opcode);
  void exec_incarg(ULONG loc,unsigned int opcode);
  void exec_incvar(ULONG loc,unsigned int opcode);
  void exec_decarg(ULONG loc,unsigned int opcode);
  void exec_decvar(ULONG loc,unsigned int opcode);
  void exec_arginc(ULONG loc,unsigned int opcode);
  void exec_varinc(ULONG loc,unsigned int opcode);
  void exec_argdec(ULONG loc,unsigned int opcode);
  void exec_vardec(ULONG loc,unsigned int opcode);
  void exec_toobject(ULONG loc,unsigned int opcode);
  void exec_forname(ULONG loc,unsigned int opcode);
  void exec_forprop(ULONG loc,unsigned int opcode);
  void exec_forelem(ULONG loc,unsigned int opcode);
  void exec_pop2(ULONG loc,unsigned int opcode);
  void exec_bindname(ULONG loc,unsigned int opcode);
  void exec_setname(ULONG loc,unsigned int opcode);
  void exec_throw(ULONG loc,unsigned int opcode);
  void exec_js_in_str(ULONG loc,unsigned int opcode);
  void exec_js_instanceof_str(ULONG loc,unsigned int opcode);
  void exec_debugger(ULONG loc,unsigned int opcode);
  void exec_gosub(ULONG loc,unsigned int opcode);
  void exec_retsub(ULONG loc,unsigned int opcode);
  void exec_exception(ULONG loc,unsigned int opcode);
  void exec_setsp(ULONG loc,unsigned int opcode);
  void exec_condswitch(ULONG loc,unsigned int opcode);
  void exec_case(ULONG loc,unsigned int opcode);
  void exec_default(ULONG loc,unsigned int opcode);
  void exec_eval(ULONG loc,unsigned int opcode);
  void exec_enumelem(ULONG loc,unsigned int opcode);
  void exec_js_getter_str(ULONG loc,unsigned int opcode);
  void exec_js_setter_str(ULONG loc,unsigned int opcode);
  void exec_deffun(ULONG loc,unsigned int opcode);
  void exec_defconst(ULONG loc,unsigned int opcode);
  void exec_defvar(ULONG loc,unsigned int opcode);
  void exec_anonfunobj(ULONG loc,unsigned int opcode);
  void exec_namedfunobj(ULONG loc,unsigned int opcode);
  void exec_initcatchvar(ULONG loc,unsigned int opcode);
  void exec_group(ULONG loc,unsigned int opcode);
  void exec_setcall(ULONG loc,unsigned int opcode);
  void exec_try(ULONG loc,unsigned int opcode);
  void exec_finally(ULONG loc,unsigned int opcode);
  void exec_swap(ULONG loc,unsigned int opcode);
  void exec_argsub(ULONG loc,unsigned int opcode);
  void exec_argcnt(ULONG loc,unsigned int opcode);
  void exec_deflocalfun(ULONG loc,unsigned int opcode);
  void exec_gotox(ULONG loc,unsigned int opcode);
  void exec_ifeqx(ULONG loc,unsigned int opcode);
  void exec_ifnex(ULONG loc,unsigned int opcode);
  void exec_orx(ULONG loc,unsigned int opcode);
  void exec_andx(ULONG loc,unsigned int opcode);
  void exec_gosubx(ULONG loc,unsigned int opcode);
  void exec_casex(ULONG loc,unsigned int opcode);
  void exec_defaultx(ULONG loc,unsigned int opcode);
  void exec_tableswitchx(ULONG loc,unsigned int opcode);
  void exec_lookupswitchx(ULONG loc,unsigned int opcode);
  void exec_backpatch(ULONG loc,unsigned int opcode);
  void exec_backpatch_pop(ULONG loc,unsigned int opcode);
  void exec_backpatch_push(ULONG loc,unsigned int opcode);
  void exec_setrval(ULONG loc,unsigned int opcode);
  void exec_retrval(ULONG loc,unsigned int opcode);
  void exec_getgvar(ULONG loc,unsigned int opcode);
  void exec_setgvar(ULONG loc,unsigned int opcode);
  void exec_incgvar(ULONG loc,unsigned int opcode);
  void exec_decgvar(ULONG loc,unsigned int opcode);
  void exec_gvarinc(ULONG loc,unsigned int opcode);
  void exec_gvardec(ULONG loc,unsigned int opcode);
  void exec_regexp(ULONG loc,unsigned int opcode);
  void exec_defxmlns(ULONG loc,unsigned int opcode);
  void exec_anyname(ULONG loc,unsigned int opcode);
  void exec_qnamepart(ULONG loc,unsigned int opcode);
  void exec_qnameconst(ULONG loc,unsigned int opcode);
  void exec_qname(ULONG loc,unsigned int opcode);
  void exec_toattrname(ULONG loc,unsigned int opcode);
  void exec_toattrval(ULONG loc,unsigned int opcode);
  void exec_addattrname(ULONG loc,unsigned int opcode);
  void exec_addattrval(ULONG loc,unsigned int opcode);
  void exec_bindxmlname(ULONG loc,unsigned int opcode);
  void exec_setxmlname(ULONG loc,unsigned int opcode);
  void exec_xmlname(ULONG loc,unsigned int opcode);
  void exec_descendants(ULONG loc,unsigned int opcode);
  void exec_filter(ULONG loc,unsigned int opcode);
  void exec_endfilter(ULONG loc,unsigned int opcode);
  void exec_toxml(ULONG loc,unsigned int opcode);
  void exec_toxmllist(ULONG loc,unsigned int opcode);
  void exec_xmltagexpr(ULONG loc,unsigned int opcode);
  void exec_xmleltexpr(ULONG loc,unsigned int opcode);
  void exec_xmlobject(ULONG loc,unsigned int opcode);
  void exec_xmlcdata(ULONG loc,unsigned int opcode);
  void exec_xmlcomment(ULONG loc,unsigned int opcode);
  void exec_xmlpi(ULONG loc,unsigned int opcode);
  void exec_getmethod(ULONG loc,unsigned int opcode);
  void exec_getfunns(ULONG loc,unsigned int opcode);
  void exec_foreach(ULONG loc,unsigned int opcode);
  void exec_deldesc(ULONG loc,unsigned int opcode);
  void exec_uint24(ULONG loc,unsigned int opcode);
  void exec_literal(ULONG loc,unsigned int opcode);
  void exec_findname(ULONG loc,unsigned int opcode);
  void exec_litopx(ULONG loc,unsigned int opcode);
  void exec_startxml(ULONG loc,unsigned int opcode);
  void exec_startxmlexpr(ULONG loc,unsigned int opcode);
  void exec_setmethod(ULONG loc,unsigned int opcode);
  void exec_stop(ULONG loc,unsigned int opcode);
  void exec_getxprop(ULONG loc,unsigned int opcode);
  void exec_getxelem(ULONG loc,unsigned int opcode);
  // Private data members
};


#endif
