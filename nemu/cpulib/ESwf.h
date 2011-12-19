#ifndef CPULIB_ESWF_H
#define CPULIB_ESWF_H


#include <cpulib/ECPUByteCode.h>

#include <portable/nickcpp/ARect.h>


// Address modes
#define ESWF_NONE 0
#define ESWF_SPECIAL 1
#define ESWF_ILLEGAL 2

#define EACTION_ADDR_NONE 0
#define EACTION_ADDR_ONE 1
#define EACTION_ADDR_TWO 2
#define EACTION_ADDR_THREE 3
#define EACTION_ADDR_VAR 4


class ESwf : public ECPUByteCode
{
public:
  // Public member functions
  ESwf();
  virtual ~ESwf();
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
  void handleHeader();
  unsigned int readRect(ARect *r,ULONG addr); // returns size of rect in bytes
  void executeAction();
  ULONG disasmAction(ULONG loc);
  unsigned int printAddrModeAction(ULONG loc,unsigned int topcode,unsigned int mode);
  //
  void exec_end_tag(ULONG loc,unsigned int opcode);
  void exec_show_frame(ULONG loc,unsigned int opcode);
  void exec_define_shape(ULONG loc,unsigned int opcode);
  void exec_free_character(ULONG loc,unsigned int opcode);
  void exec_place_object(ULONG loc,unsigned int opcode);
  void exec_remove_object(ULONG loc,unsigned int opcode);
  void exec_define_bits(ULONG loc,unsigned int opcode);
  void exec_define_button(ULONG loc,unsigned int opcode);
  void exec_jpeg_tables(ULONG loc,unsigned int opcode);
  void exec_set_background_color(ULONG loc,unsigned int opcode);
  void exec_define_font(ULONG loc,unsigned int opcode);
  void exec_define_text(ULONG loc,unsigned int opcode);
  void exec_do_action(ULONG loc,unsigned int opcode);
  void exec_define_font_info(ULONG loc,unsigned int opcode);
  void exec_define_sound(ULONG loc,unsigned int opcode);
  void exec_start_sound(ULONG loc,unsigned int opcode);
  void exec_stop_sound(ULONG loc,unsigned int opcode);
  void exec_define_button_sound(ULONG loc,unsigned int opcode);
  void exec_sound_stream_head(ULONG loc,unsigned int opcode);
  void exec_sound_stream_block(ULONG loc,unsigned int opcode);
  void exec_define_bits_lossless(ULONG loc,unsigned int opcode);
  void exec_define_bits_jpeg2(ULONG loc,unsigned int opcode);
  void exec_define_shape2(ULONG loc,unsigned int opcode);
  void exec_define_bitton_cxform(ULONG loc,unsigned int opcode);
  void exec_protect(ULONG loc,unsigned int opcode);
  void exec_paths_are_postscript(ULONG loc,unsigned int opcode);
  void exec_place_object2(ULONG loc,unsigned int opcode);
  void exec_remove_object2(ULONG loc,unsigned int opcode);
  void exec_sync_frame(ULONG loc,unsigned int opcode);
  void exec_free_all(ULONG loc,unsigned int opcode);
  void exec_define_shape3(ULONG loc,unsigned int opcode);
  void exec_define_text2(ULONG loc,unsigned int opcode);
  void exec_define_button2(ULONG loc,unsigned int opcode);
  void exec_define_bits_jpeg3(ULONG loc,unsigned int opcode);
  void exec_define_bits_lossless2(ULONG loc,unsigned int opcode);
  void exec_define_edit_text(ULONG loc,unsigned int opcode);
  void exec_define_video(ULONG loc,unsigned int opcode);
  void exec_define_sprite(ULONG loc,unsigned int opcode);
  void exec_name_character(ULONG loc,unsigned int opcode);
  void exec_serial_number(ULONG loc,unsigned int opcode);
  void exec_define_text_format(ULONG loc,unsigned int opcode);
  void exec_frame_label(ULONG loc,unsigned int opcode);
  void exec_sound_stream_head2(ULONG loc,unsigned int opcode);
  void exec_define_morph_shape(ULONG loc,unsigned int opcode);
  void exec_generate_frame(ULONG loc,unsigned int opcode);
  void exec_define_font2(ULONG loc,unsigned int opcode);
  void exec_generator_command(ULONG loc,unsigned int opcode);
  void exec_define_command_object(ULONG loc,unsigned int opcode);
  void exec_character_set(ULONG loc,unsigned int opcode);
  void exec_external_font(ULONG loc,unsigned int opcode);
  void exec_export(ULONG loc,unsigned int opcode);
  void exec_import(ULONG loc,unsigned int opcode);
  void exec_protect_debug(ULONG loc,unsigned int opcode);
  void exec_do_init_action(ULONG loc,unsigned int opcode);
  void exec_define_video_stream(ULONG loc,unsigned int opcode);
  void exec_video_frame(ULONG loc,unsigned int opcode);
  void exec_define_font_info2(ULONG loc,unsigned int opcode);
  void exec_illgeal(ULONG loc,unsigned int opcode);
  void exec_protect_debug2(ULONG loc,unsigned int opcode);
  void exec_script_limits(ULONG loc,unsigned int opcode);
  void exec_set_tab_index(ULONG loc,unsigned int opcode);
  void exec_place_object3(ULONG loc,unsigned int opcode);
  //
  void exec_end(ULONG loc,unsigned int opcode);
  void exec_nextframe(ULONG loc,unsigned int opcode);
  void exec_prevframe(ULONG loc,unsigned int opcode);
  void exec_play(ULONG loc,unsigned int opcode);
  void exec_stop(ULONG loc,unsigned int opcode);
  void exec_togglequality(ULONG loc,unsigned int opcode);
  void exec_stopsounds(ULONG loc,unsigned int opcode);
  void exec_add(ULONG loc,unsigned int opcode);
  void exec_subtract(ULONG loc,unsigned int opcode);
  void exec_multiply(ULONG loc,unsigned int opcode);
  void exec_divide(ULONG loc,unsigned int opcode);
  void exec_equals(ULONG loc,unsigned int opcode);
  void exec_lessthan(ULONG loc,unsigned int opcode);
  void exec_logicaland(ULONG loc,unsigned int opcode);
  void exec_logicalor(ULONG loc,unsigned int opcode);
  void exec_logicalnot(ULONG loc,unsigned int opcode);
  void exec_stringeq(ULONG loc,unsigned int opcode);
  void exec_stringlength(ULONG loc,unsigned int opcode);
  void exec_substring(ULONG loc,unsigned int opcode);
  void exec_pop(ULONG loc,unsigned int opcode);
  void exec_int(ULONG loc,unsigned int opcode);
  void exec_getvariable(ULONG loc,unsigned int opcode);
  void exec_setvariable(ULONG loc,unsigned int opcode);
  void exec_settargetexpression(ULONG loc,unsigned int opcode);
  void exec_stringconcat(ULONG loc,unsigned int opcode);
  void exec_getproperty(ULONG loc,unsigned int opcode);
  void exec_setproperty(ULONG loc,unsigned int opcode);
  void exec_duplicateclip(ULONG loc,unsigned int opcode);
  void exec_removeclip(ULONG loc,unsigned int opcode);
  void exec_trace(ULONG loc,unsigned int opcode);
  void exec_startdragmovie(ULONG loc,unsigned int opcode);
  void exec_stopdragmovie(ULONG loc,unsigned int opcode);
  void exec_stringlessthan(ULONG loc,unsigned int opcode);
  void exec_random(ULONG loc,unsigned int opcode);
  void exec_mblength(ULONG loc,unsigned int opcode);
  void exec_ord(ULONG loc,unsigned int opcode);
  void exec_chr(ULONG loc,unsigned int opcode);
  void exec_gettimer(ULONG loc,unsigned int opcode);
  void exec_mbsubstring(ULONG loc,unsigned int opcode);
  void exec_mbord(ULONG loc,unsigned int opcode);
  void exec_mbchr(ULONG loc,unsigned int opcode);
  void exec_delete(ULONG loc,unsigned int opcode);
  void exec_varequals(ULONG loc,unsigned int opcode);
  void exec_callfunction(ULONG loc,unsigned int opcode);
  void exec_return(ULONG loc,unsigned int opcode);
  void exec_modulo(ULONG loc,unsigned int opcode);
  void exec_new(ULONG loc,unsigned int opcode);
  void exec_var(ULONG loc,unsigned int opcode);
  void exec_initarray(ULONG loc,unsigned int opcode);
  void exec_initobject(ULONG loc,unsigned int opcode);
  void exec_typeof(ULONG loc,unsigned int opcode);
  void exec_targetpath(ULONG loc,unsigned int opcode);
  void exec_enumerate(ULONG loc,unsigned int opcode);
  void exec_newadd(ULONG loc,unsigned int opcode);
  void exec_newlessthan(ULONG loc,unsigned int opcode);
  void exec_newequals(ULONG loc,unsigned int opcode);
  void exec_dup(ULONG loc,unsigned int opcode);
  void exec_swap(ULONG loc,unsigned int opcode);
  void exec_getmember(ULONG loc,unsigned int opcode);
  void exec_setmember(ULONG loc,unsigned int opcode);
  void exec_increment(ULONG loc,unsigned int opcode);
  void exec_decrement(ULONG loc,unsigned int opcode);
  void exec_callmethod(ULONG loc,unsigned int opcode);
  void exec_bitwiseand(ULONG loc,unsigned int opcode);
  void exec_bitwiseor(ULONG loc,unsigned int opcode);
  void exec_bitwisexor(ULONG loc,unsigned int opcode);
  void exec_shiftleft(ULONG loc,unsigned int opcode);
  void exec_shiftright(ULONG loc,unsigned int opcode);
  void exec_shiftright2(ULONG loc,unsigned int opcode);
  void exec_gotoframe(ULONG loc,unsigned int opcode);
  void exec_geturl(ULONG loc,unsigned int opcode);
  void exec_setregister(ULONG loc,unsigned int opcode);
  void exec_constantpool(ULONG loc,unsigned int opcode);
  void exec_waitforframe(ULONG loc,unsigned int opcode);
  void exec_settarget(ULONG loc,unsigned int opcode);
  void exec_gotolabel(ULONG loc,unsigned int opcode);
  void exec_waitforframeexpression(ULONG loc,unsigned int opcode);
  void exec_with(ULONG loc,unsigned int opcode);
  void exec_pushdata(ULONG loc,unsigned int opcode);
  void exec_branchalways(ULONG loc,unsigned int opcode);
  void exec_geturl2(ULONG loc,unsigned int opcode);
  void exec_definefunction(ULONG loc,unsigned int opcode);
  void exec_branchiftrue(ULONG loc,unsigned int opcode);
  void exec_callframe(ULONG loc,unsigned int opcode);
  void exec_gotoexpression(ULONG loc,unsigned int opcode);
  void exec_strict_mode(ULONG loc,unsigned int opcode);
  void exec_declarefunction2(ULONG loc,unsigned int opcode);
  void exec_try(ULONG loc,unsigned int opcode);
  // Private data members
  ARect frame;
  unsigned int frameRate1,frameRate2;
  unsigned int frameCount;
  bool action;
};


#endif // CPULIB_ESWF_H
