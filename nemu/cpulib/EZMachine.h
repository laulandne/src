#ifndef CPULIB_EZMACHINE_H
#define CPULIB_EZMACHINE_H


#include <cpulib/ECPUByteCode.h>


// Address modes (numbers are arbitrary)
#define EZ_ADDR_NONE 0
#define EZ_ADDR_0OP 1
#define EZ_ADDR_1OP_LARGE 4
#define EZ_ADDR_1OP_SMALL 5
#define EZ_ADDR_1OP_VAR 6
#define EZ_ADDR_2OP_SMALL_SMALL 8
#define EZ_ADDR_2OP_SMALL_VAR 9
#define EZ_ADDR_2OP_VAR_SMALL 10
#define EZ_ADDR_2OP_VAR_VAR 11
#define EZ_ADDR_VAROP 16
#define EZ_ADDR_VAROP2 17
#define EZ_ADDR_EXTEND 18
#define EZ_ADDR_PRINT 19
#define EZ_ADDR_SPECIAL 20
#define EZ_ADDR_ILLEGAL 21


// flags and subformats
// NOTE: EZ_NONE is reused as an operand type and must be 3
#define EZ_STORE 1
#define EZ_BRANCH 2
#define EZ_NONE 3
#define EZ_CALL 4


class EZMachine : public ECPUByteCode
{
public:
  // Public member functions
  EZMachine();
  virtual ~EZMachine();
  static bool recognize(const char *name);
  virtual const char *getName();
  virtual void setKind(const char *name);
  virtual void execute();
  virtual bool setRegister(const char *name,ULONG value);
  virtual ULONG readRegister(const char *name);
  virtual ULONG readStack() { return sp; }
  virtual ULONG disasm(ULONG loc);
  virtual void reset();
  virtual void wregs();
  // Public data members
protected:
  // Private member functions
  virtual void init();
  unsigned int decodeAddrMode(bool print,ULONG loc,unsigned int topcode,unsigned int mode);
  void handleHeader();
  void execSpecial(ULONG loc,unsigned int topcode);
  void disSpecial(ULONG loc,unsigned int topcode);
  void disExtended(ULONG loc,unsigned int topcode);
  ULONG unpackAddress(unsigned int val);
  ULONG getBranch(ULONG loc);
  void printVar(unsigned int var);
  unsigned int getVar(unsigned int var);
  void setVar(unsigned int var,unsigned int val);
  unsigned int getOperand(bool print,ULONG loc,unsigned int type);
  void setOperand(ULONG loc,unsigned int type,unsigned int val);
  void doCall(ULONG loc,unsigned int opcode,unsigned int op1);
  //
  void exec_je(ULONG loc,unsigned int opcode);
  void exec_jl(ULONG loc,unsigned int opcode);
  void exec_jg(ULONG loc,unsigned int opcode);
  void exec_dec_chk(ULONG loc,unsigned int opcode);
  void exec_inc_chk(ULONG loc,unsigned int opcode);
  void exec_jin(ULONG loc,unsigned int opcode);
  void exec_test(ULONG loc,unsigned int opcode);
  void exec_or(ULONG loc,unsigned int opcode);
  void exec_and(ULONG loc,unsigned int opcode);
  void exec_test_attr(ULONG loc,unsigned int opcode);
  void exec_set_attr(ULONG loc,unsigned int opcode);
  void exec_clear_attr(ULONG loc,unsigned int opcode);
  void exec_store(ULONG loc,unsigned int opcode);
  void exec_insert_obj(ULONG loc,unsigned int opcode);
  void exec_loadw(ULONG loc,unsigned int opcode);
  void exec_loadb(ULONG loc,unsigned int opcode);
  void exec_get_prop(ULONG loc,unsigned int opcode);
  void exec_get_prop_addr(ULONG loc,unsigned int opcode);
  void exec_get_next_prop(ULONG loc,unsigned int opcode);
  void exec_add(ULONG loc,unsigned int opcode);
  void exec_sub(ULONG loc,unsigned int opcode);
  void exec_mul(ULONG loc,unsigned int opcode);
  void exec_div(ULONG loc,unsigned int opcode);
  void exec_mod(ULONG loc,unsigned int opcode);
  void exec_call_vs(ULONG loc,unsigned int opcode);
  void exec_call_1n(ULONG loc,unsigned int opcode);
  void exec_call_2s(ULONG loc,unsigned int opcode);
  void exec_call_2n(ULONG loc,unsigned int opcode);
  void exec_set_colour(ULONG loc,unsigned int opcode);
  void exec_catch(ULONG loc,unsigned int opcode);
  void exec_throw(ULONG loc,unsigned int opcode);
  void exec_jz(ULONG loc,unsigned int opcode);
  void exec_get_sibling(ULONG loc,unsigned int opcode);
  void exec_get_child(ULONG loc,unsigned int opcode);
  void exec_get_parent(ULONG loc,unsigned int opcode);
  void exec_get_prop_len(ULONG loc,unsigned int opcode);
  void exec_inc(ULONG loc,unsigned int opcode);
  void exec_dec(ULONG loc,unsigned int opcode);
  void exec_print_addr(ULONG loc,unsigned int opcode);
  void exec_call_1s(ULONG loc,unsigned int opcode);
  void exec_remove_obj(ULONG loc,unsigned int opcode);
  void exec_print_obj(ULONG loc,unsigned int opcode);
  void exec_ret(ULONG loc,unsigned int opcode);
  void exec_jump(ULONG loc,unsigned int opcode);
  void exec_print_paddr(ULONG loc,unsigned int opcode);
  void exec_load(ULONG loc,unsigned int opcode);
  void exec_not(ULONG loc,unsigned int opcode);
  void exec_rtrue(ULONG loc,unsigned int opcode);
  void exec_rfalse(ULONG loc,unsigned int opcode);
  void exec_print(ULONG loc,unsigned int opcode);
  void exec_print_ret(ULONG loc,unsigned int opcode);
  void exec_nop(ULONG loc,unsigned int opcode);
  void exec_save(ULONG loc,unsigned int opcode);
  void exec_restore(ULONG loc,unsigned int opcode);
  void exec_restart(ULONG loc,unsigned int opcode);
  void exec_ret_popped(ULONG loc,unsigned int opcode);
  void exec_pop(ULONG loc,unsigned int opcode);
  void exec_quit(ULONG loc,unsigned int opcode);
  void exec_new_line(ULONG loc,unsigned int opcode);
  void exec_show_status(ULONG loc,unsigned int opcode);
  void exec_verify(ULONG loc,unsigned int opcode);
  void exec_extended(ULONG loc,unsigned int opcode);
  void exec_piracy(ULONG loc,unsigned int opcode);
  void exec_call(ULONG loc,unsigned int opcode);
  void exec_storew(ULONG loc,unsigned int opcode);
  void exec_storeb(ULONG loc,unsigned int opcode);
  void exec_put_prop(ULONG loc,unsigned int opcode);
  void exec_aread(ULONG loc,unsigned int opcode);
  void exec_sread(ULONG loc,unsigned int opcode);
  void exec_print_char(ULONG loc,unsigned int opcode);
  void exec_print_num(ULONG loc,unsigned int opcode);
  void exec_random(ULONG loc,unsigned int opcode);
  void exec_push(ULONG loc,unsigned int opcode);
  void exec_pull(ULONG loc,unsigned int opcode);
  void exec_split_window(ULONG loc,unsigned int opcode);
  void exec_set_window(ULONG loc,unsigned int opcode);
  void exec_call_vs2(ULONG loc,unsigned int opcode);
  void exec_erase_window(ULONG loc,unsigned int opcode);
  void exec_erase_line(ULONG loc,unsigned int opcode);
  void exec_set_cursor(ULONG loc,unsigned int opcode);
  void exec_get_cursor(ULONG loc,unsigned int opcode);
  void exec_set_text_style(ULONG loc,unsigned int opcode);
  void exec_buffer_mode(ULONG loc,unsigned int opcode);
  void exec_output_stream(ULONG loc,unsigned int opcode);
  void exec_input_stream(ULONG loc,unsigned int opcode);
  void exec_sound_effect(ULONG loc,unsigned int opcode);
  void exec_read_char(ULONG loc,unsigned int opcode);
  void exec_scan_table(ULONG loc,unsigned int opcode);
  void exec_call_vn(ULONG loc,unsigned int opcode);
  void exec_call_vn2(ULONG loc,unsigned int opcode);
  void exec_tokenise(ULONG loc,unsigned int opcode);
  void exec_encode_text(ULONG loc,unsigned int opcode);
  void exec_copy_table(ULONG loc,unsigned int opcode);
  void exec_print_table(ULONG loc,unsigned int opcode);
  void exec_check_arg_count(ULONG loc,unsigned int opcode);
  // Private data members
  unsigned int highBase,staticBase;
  unsigned int initialPC;
  unsigned int dictLoc,objLoc,globalLoc,abvLoc;
  unsigned int mode,branch,store,op1,op2;
};


#endif
