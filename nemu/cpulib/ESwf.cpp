
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


//#define DEBUG_DIS_MODES 1

#define DEBUG_EXEC 1

#define CHECK_SAFE 1

#define SHOW_INTS  1


#include <cpulib/ESwf.h>

#include <elib/EMemMapAbstract.h>
#include <elib/EModule.h>

#include <portable/nickcpp/ANullStream.h>

#include <portable/nickcpp/AStringHelper.h>


#include <cpulib/generated/ESwf_tab.h>
#include <cpulib/generated/EAction_tab.h>


// The different cpus...
#define E_CPU_NONE   0x00
#define E_CPU_SWF1   0x01
#define E_CPU_SWF2   0x02
#define E_CPU_SWF3   0x03
#define E_CPU_SWF4   0x04
#define E_CPU_SWF5   0x05
#define E_CPU_SWF6   0x06
#define E_CPU_SWF7   0x07
#define E_CPU_SWF8   0x08
#define E_CPU_SWF9   0x09


////////////////////////////////////////////////////////////////////////////////
//  ESwf Class
////////////////////////////////////////////////////////////////////////////////

ESwf::ESwf()
{
  DEBUG_OUT<<"ESwf created...\n";
  init();
  type=E_CPU_SWF1;
  //w=ww;
  reset();
}


ESwf::~ESwf()
{
  DEBUG_OUT<<"ESwf destroyed.\n";
}


void ESwf::init()
{
  ECPUByteCode::init();
  // Generic features
  doTrace=false;  doRun=false;
  // Set up stack and registers
  int_pending=0;
  intLock=false;
  lsb=true;
  setStack(ECPUABSTRACT_DEFAULT_STACK);
  setFramePtr(ECPUABSTRACT_DEFAULT_STACK);
  setResetStack(ECPUABSTRACT_DEFAULT_STACK);
  setResetEntry(ECPUABSTRACT_DEFAULT_CODE);
  frameRate1=0;  frameRate2=0;
  frameCount=0;
  action=false;
}


/* STATIC */
bool ESwf::recognize(const char *tname)
{
  const char *name=aDefaultStringHelper.removeSpaces2(aDefaultStringHelper.toLower(tname));
  if(!strcmp(name,"flash")) return true; else
  if(!strcmp(name,"flash1")) return true; else
  if(!strcmp(name,"flash2")) return true; else
  if(!strcmp(name,"flash3")) return true; else
  if(!strcmp(name,"flash4")) return true; else
  if(!strcmp(name,"flash5")) return true; else
  if(!strcmp(name,"flash6")) return true; else
  if(!strcmp(name,"flash7")) return true; else
  if(!strcmp(name,"flash8")) return true; else
  if(!strcmp(name,"flash9")) return true; else
  if(!strcmp(name,"shockwave")) return true; else
  if(!strcmp(name,"actionscript")) return true; else
  if(!strcmp(name,"actionscript1")) return true; else
  if(!strcmp(name,"actionscript2")) return true; else
  if(!strcmp(name,"actionscript3")) return true; else
  if(!strcmp(name,"swf")) return true; else
  if(!strcmp(name,"swf1")) return true; else
  if(!strcmp(name,"swf2")) return true; else
  if(!strcmp(name,"swf3")) return true; else
  if(!strcmp(name,"swf4")) return true; else
  if(!strcmp(name,"swf5")) return true; else
  if(!strcmp(name,"swf6")) return true; else
  if(!strcmp(name,"swf7")) return true; else
  if(!strcmp(name,"swf8")) return true; else
  if(!strcmp(name,"swf9")) return true; else
  return false;
}


const char *ESwf::getName()
{
  if(type==E_CPU_SWF1) return "Flash 1";
  if(type==E_CPU_SWF2) return "Flash 2";
  if(type==E_CPU_SWF3) return "Flash 3";
  if(type==E_CPU_SWF4) return "Flash 4";
  if(type==E_CPU_SWF5) return "Flash 5";
  if(type==E_CPU_SWF6) return "Flash 6";
  if(type==E_CPU_SWF7) return "Flash 7";
  if(type==E_CPU_SWF8) return "Flash 8";
  if(type==E_CPU_SWF9) return "Flash 9";
  return "Flash";
}


void ESwf::setKind(const char *tname)
{
  const char *name=aDefaultStringHelper.removeSpaces2(aDefaultStringHelper.toLower(tname));
  type=E_CPU_SWF1;
}


void ESwf::reset()
{
#ifdef DEBUG_EXEC
  setTrace(true);
#endif
  handleHeader();
  breakpoint=0xffffffff;  // since we start at 0, breakpoint's default
  intLock=false;
  pc=0;  // Jump to zero
  clearStackTrace();
  ECPUAbstract::reset();
}


void ESwf::handleHeader()
{
  if(!map) return;
  unsigned int offset=3;
  type=map->read8(offset); offset++;
  unsigned int len=map->read32l(offset);  offset+=4;
  offset+=readRect(&frame,offset);
  //frame.debugPrint();
  frameRate1=map->read8(offset);  offset+=1;
  frameRate2=map->read8(offset);  offset+=1;
  frameCount=map->read16l(offset);  offset+=2;
  DEBUG_OUT<<"frameRate= "<<frameRate1<<"."<<frameRate2<<"\n";
  DEBUG_OUT<<"frameCount= "<<frameCount<<"\n";
  resetEntry=offset;
  DEBUG_OUT<<"resetEntry= "<<resetEntry<<"\n";
}


bool ESwf::setRegister(const char *tname,ULONG value)
{
  const char *name=aDefaultStringHelper.removeSpaces2(aDefaultStringHelper.toLower(tname));
  bool ret=false;
  if(!strcmp(name,"pc"))
  { pc=value; ret=true; } else
  if(!strcmp(name,"sp"))
  { sp=value; ret=true; }
  //free(name);
  return ret;
}


ULONG ESwf::readRegister(const char *tname)
{
  const char *name=aDefaultStringHelper.removeSpaces2(aDefaultStringHelper.toLower(tname));
  if(!strcmp(name,"sp")) { return sp; }
  return 0;
}


void ESwf::wregs()
{
  DEBUG_OUT<<"PC="<<pc<<"\n";
  DEBUG_OUT<<"SP="<<sp<<"\n";
}


void ESwf::execute()
{
  if(action) { executeAction(); return; }
  if(pc==breakpoint) { debugger("Breakpoint!\n"); return; }
  unsigned int temp=map->read16l(pc);
  // NOTE: Should be 0x3ff, there are 10 bits in tag field
  opcode=(temp>>6)&0xff;
  op_len=(temp&0x3f)+2;
  if(op_len==0x41) op_len=map->read32l(pc+2)+7;
  //op_len=eswf_size[opcode];
  cycle=ESwf_time[opcode];
  topcode=opcode;
  unsigned int loc=pc;
#include <cpulib/generated/ESwf_exec.h>
  pc+=op_len;
  cycles+=cycle;
  if(int_pending) handle_int(int_pending);
}


void ESwf::executeAction()
{
  if(pc==breakpoint) { debugger("Breakpoint!\n"); return; }
  opcode=map->read8(pc);
  op_len=EAction_size[opcode];
  cycle=EAction_time[opcode];
  unsigned int topcode=opcode,loc=pc;
#include <cpulib/generated/EAction_exec.h>
  pc+=op_len;
  cycles+=cycle;
  if(int_pending) handle_int(int_pending);
}


ULONG ESwf::disasm(ULONG loc)
{
  if(action) { return disasmAction(loc); }
  unsigned int topcode;
  unsigned int len=0,mode=ESWF_NONE;
  const char *name="???";
  unsigned int temp=map->read16l(loc);
  // NOTE: Should be 0x3ff, there are 10 bits in tag field
  topcode=(temp>>6)&0xff;
  len=(temp&0x3f)+2;
  if(len==0x41) len=map->read32l(loc+2)+7;
  lastSymbol=map->findAnyExactSymbol(loc);
  if(lastSymbol) DEBUG_OUT<<lastSymbol->getName()<<":\n";
  DEBUG_OUT<<"."; DEBUG_OUT<<loc;  DEBUG_OUT<<"  ";
  //len=eswf_size[topcode];
  mode=ESwf_format[topcode];
  name=ESwf_name[topcode];
  if(!len) len=1;  // NOTE: An unimplemented opcode...
  unsigned int t;
  DEBUG_OUT<<topcode<<" "<<len<<"  ";
  //for(t=0;t<len;t++) DEBUG_OUT<<map->read8(loc+t)<<" ";
  //for(t=0;t<(3-len);t++) DEBUG_OUT<<"   ";
  DEBUG_OUT<<name<<" ";
  //DEBUG_OUT<<"Len was "<<len<<"\n";
  len+=printAddrMode(loc,topcode,mode);
  //DEBUG_OUT<<"Len now is "<<len<<"\n";
  DEBUG_OUT<<"\n";
  return len;
}


ULONG ESwf::disasmAction(ULONG loc)
{
  BYTE topcode;
  unsigned int len=0,mode=ESWF_NONE;
  const char *name="???";
  topcode=map->read8(loc);
  lastSymbol=map->findAnyExactSymbol(loc);
  if(lastSymbol) DEBUG_OUT<<lastSymbol->getName()<<":\n";
  DEBUG_OUT<<"."; DEBUG_OUT<<loc;  DEBUG_OUT<<"  ";
  len=EAction_size[topcode];
  mode=EAction_format[topcode];
  name=EAction_name[topcode];
  if(!len) len=1;  // NOTE: An unimplemented opcode...
  unsigned int t;
  for(t=0;t<len;t++) DEBUG_OUT<<(short)map->read8(loc+t)<<" ";
  for(t=0;t<(3-len);t++) DEBUG_OUT<<"   ";
  DEBUG_OUT<<name<<" ";
  //DEBUG_OUT<<"Len was "<<len<<"\n";
  len+=printAddrModeAction(loc,topcode,mode);
  //DEBUG_OUT<<"Len now is "<<len<<"\n";
  DEBUG_OUT<<"\n";
  return len;
}


unsigned int ESwf::printAddrMode(ULONG loc,unsigned int topcode,unsigned int mode)
{
  unsigned int ret=0;
  switch(mode) {
    case ECPUABSTRACT_ADDR_NONE:
      break;
    default:
      debugger("Unknown mode in printAddrMode!");
      break;
  }
  return ret;
}


unsigned int ESwf::printAddrModeAction(ULONG loc,unsigned int topcode,unsigned int mode)
{
  unsigned int ret=0;
  unsigned int num=0;
  switch(mode) {
    case EACTION_ADDR_NONE:
      break;
    case EACTION_ADDR_ONE:
      break;
    case EACTION_ADDR_TWO:
      break;
    case EACTION_ADDR_THREE:
      break;
    case EACTION_ADDR_VAR:
      break;
      num=map->read16l(loc+1);
      ret=num+2;
    default:
      debugger("Unknown mode in printAddrModeAction!");
      break;
  }
  return ret;
}


// returns size of rect in bytes
// TODO: Still not quite right
unsigned int ESwf::readRect(ARect *r,ULONG addr)
{
  unsigned int offset=0;
  unsigned int rawBits=map->read8(addr); offset++;
  unsigned int bits=5+4*((rawBits&0xf8)>>3);
  unsigned int bitsPer=(bits-5)/4;
  unsigned int bytes=(bits+7)/8;
  DEBUG_OUT<<"rawBits="<<rawBits<<"\n";
  DEBUG_OUT<<"bits="<<bits<<"\n";
  DEBUG_OUT<<"bitsPer="<<bitsPer<<"\n";
  DEBUG_OUT<<"bytes="<<bytes<<"\n";
  // Assume 16 bits per for now...
  r->x=map->read16b(addr+offset);  offset+=2;
  r->y=map->read16b(addr+offset);  offset+=2;
  r->width=map->read16b(addr+offset);  offset+=2;
  r->height=map->read16b(addr+offset);  offset+=2;
  return bytes;
}


////////////////////////////////////////////////////////////////////////////////
//  SWF tags
////////////////////////////////////////////////////////////////////////////////


void ESwf::exec_end_tag(ULONG loc,unsigned int opcode)
{
  debugger("end_tag reached");
}


void ESwf::exec_show_frame(ULONG loc,unsigned int opcode)
{
  //debugger("Unimplemented show_frame opcode");
}


void ESwf::exec_define_shape(ULONG loc,unsigned int opcode)
{
  //debugger("Unimplemented define_shape opcode");
}


void ESwf::exec_free_character(ULONG loc,unsigned int opcode)
{
  //debugger("Unimplemented free_character opcode");
}


void ESwf::exec_place_object(ULONG loc,unsigned int opcode)
{
  //debugger("Unimplemented place_object opcode");
}


void ESwf::exec_remove_object(ULONG loc,unsigned int opcode)
{
  //debugger("Unimplemented remove_object opcode");
}


void ESwf::exec_define_bits(ULONG loc,unsigned int opcode)
{
  //debugger("Unimplemented define_bits opcode");
}


void ESwf::exec_define_button(ULONG loc,unsigned int opcode)
{
  //debugger("Unimplemented define_button opcode");
}


void ESwf::exec_jpeg_tables(ULONG loc,unsigned int opcode)
{
  //debugger("Unimplemented jpeg_tables opcode");
}


void ESwf::exec_set_background_color(ULONG loc,unsigned int opcode)
{
  //debugger("Unimplemented set_background_color opcode");
}


void ESwf::exec_define_font(ULONG loc,unsigned int opcode)
{
  //debugger("Unimplemented define_font opcode");
}


void ESwf::exec_define_text(ULONG loc,unsigned int opcode)
{
  //debugger("Unimplemented define_text opcode");
}


void ESwf::exec_do_action(ULONG loc,unsigned int opcode)
{
  action=true;
  op_len=6;
}


void ESwf::exec_define_font_info(ULONG loc,unsigned int opcode)
{
  //debugger("Unimplemented define_font_info opcode");
}


void ESwf::exec_define_sound(ULONG loc,unsigned int opcode)
{
  //debugger("Unimplemented define_sound opcode");
}


void ESwf::exec_start_sound(ULONG loc,unsigned int opcode)
{
  //debugger("Unimplemented start_sound opcode");
}


void ESwf::exec_stop_sound(ULONG loc,unsigned int opcode)
{
  //debugger("Unimplemented stop_sound opcode");
}


void ESwf::exec_define_button_sound(ULONG loc,unsigned int opcode)
{
  //debugger("Unimplemented define_button_sound opcode");
}


void ESwf::exec_sound_stream_head(ULONG loc,unsigned int opcode)
{
  //debugger("Unimplemented sound_stream_head opcode");
}


void ESwf::exec_sound_stream_block(ULONG loc,unsigned int opcode)
{
  //debugger("Unimplemented sound_stream_block opcode");
}


void ESwf::exec_define_bits_lossless(ULONG loc,unsigned int opcode)
{
  //debugger("Unimplemented define_bits_lossless opcode");
}


void ESwf::exec_define_bits_jpeg2(ULONG loc,unsigned int opcode)
{
  //debugger("Unimplemented define_bits_jpeg2 opcode");
}


void ESwf::exec_define_shape2(ULONG loc,unsigned int opcode)
{
  //debugger("Unimplemented define_shape2 opcode");
}


void ESwf::exec_define_bitton_cxform(ULONG loc,unsigned int opcode)
{
  //debugger("Unimplemented define_bitton_cxform opcode");
}


void ESwf::exec_protect(ULONG loc,unsigned int opcode)
{
  //debugger("Unimplemented protect opcode");
}


void ESwf::exec_paths_are_postscript(ULONG loc,unsigned int opcode)
{
  //debugger("Unimplemented paths_are_postscript opcode");
}


void ESwf::exec_place_object2(ULONG loc,unsigned int opcode)
{
  //debugger("Unimplemented place_object2 opcode");
}


void ESwf::exec_remove_object2(ULONG loc,unsigned int opcode)
{
  //debugger("Unimplemented remove_object2 opcode");
}


void ESwf::exec_sync_frame(ULONG loc,unsigned int opcode)
{
  //debugger("Unimplemented sync_frame opcode");
}


void ESwf::exec_free_all(ULONG loc,unsigned int opcode)
{
  //debugger("Unimplemented free_all opcode");
}


void ESwf::exec_define_shape3(ULONG loc,unsigned int opcode)
{
  //debugger("Unimplemented define_shape3 opcode");
}


void ESwf::exec_define_text2(ULONG loc,unsigned int opcode)
{
  //debugger("Unimplemented define_text2 opcode");
}


void ESwf::exec_define_button2(ULONG loc,unsigned int opcode)
{
  //debugger("Unimplemented define_button2 opcode");
}


void ESwf::exec_define_bits_jpeg3(ULONG loc,unsigned int opcode)
{
  //debugger("Unimplemented define_bits_jpeg3 opcode");
}


void ESwf::exec_define_bits_lossless2(ULONG loc,unsigned int opcode)
{
  //debugger("Unimplemented define_bits_lossless2 opcode");
}


void ESwf::exec_define_edit_text(ULONG loc,unsigned int opcode)
{
  //debugger("Unimplemented define_edit_text opcode");
}


void ESwf::exec_define_video(ULONG loc,unsigned int opcode)
{
  //debugger("Unimplemented define_video opcode");
}


void ESwf::exec_define_sprite(ULONG loc,unsigned int opcode)
{
  //debugger("Unimplemented define_sprite opcode");
}


void ESwf::exec_name_character(ULONG loc,unsigned int opcode)
{
  //debugger("Unimplemented name_character opcode");
}


void ESwf::exec_serial_number(ULONG loc,unsigned int opcode)
{
  //debugger("Unimplemented serial_number opcode");
}


void ESwf::exec_define_text_format(ULONG loc,unsigned int opcode)
{
  //debugger("Unimplemented define_text_format opcode");
}


void ESwf::exec_frame_label(ULONG loc,unsigned int opcode)
{
  //debugger("Unimplemented frame_label opcode");
}


void ESwf::exec_sound_stream_head2(ULONG loc,unsigned int opcode)
{
  //debugger("Unimplemented sound_stream_head2 opcode");
}


void ESwf::exec_define_morph_shape(ULONG loc,unsigned int opcode)
{
  //debugger("Unimplemented define_morph_shape opcode");
}


void ESwf::exec_generate_frame(ULONG loc,unsigned int opcode)
{
  //debugger("Unimplemented generate_frame opcode");
}


void ESwf::exec_define_font2(ULONG loc,unsigned int opcode)
{
  //debugger("Unimplemented define_font2 opcode");
}


void ESwf::exec_generator_command(ULONG loc,unsigned int opcode)
{
  //debugger("Unimplemented generator_command opcode");
}


void ESwf::exec_define_command_object(ULONG loc,unsigned int opcode)
{
  //debugger("Unimplemented define_command_object opcode");
}


void ESwf::exec_character_set(ULONG loc,unsigned int opcode)
{
  //debugger("Unimplemented character_set opcode");
}


void ESwf::exec_external_font(ULONG loc,unsigned int opcode)
{
  //debugger("Unimplemented external_font opcode");
}


void ESwf::exec_export(ULONG loc,unsigned int opcode)
{
  //debugger("Unimplemented export opcode");
}


void ESwf::exec_import(ULONG loc,unsigned int opcode)
{
  //debugger("Unimplemented import opcode");
}


void ESwf::exec_protect_debug(ULONG loc,unsigned int opcode)
{
  //debugger("Unimplemented protect_debug opcode");
}


void ESwf::exec_do_init_action(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented do_init_action opcode");
}


void ESwf::exec_define_video_stream(ULONG loc,unsigned int opcode)
{
  //debugger("Unimplemented define_video_stream opcode");
}


void ESwf::exec_video_frame(ULONG loc,unsigned int opcode)
{
  //debugger("Unimplemented video_frame opcode");
}


void ESwf::exec_define_font_info2(ULONG loc,unsigned int opcode)
{
  //debugger("Unimplemented define_font_info2 opcode");
}


void ESwf::exec_illgeal(ULONG loc,unsigned int opcode)
{
  //debugger("Unimplemented illgeal opcode");
}


void ESwf::exec_protect_debug2(ULONG loc,unsigned int opcode)
{
  //debugger("Unimplemented protect_debug2 opcode");
}


void ESwf::exec_script_limits(ULONG loc,unsigned int opcode)
{
  //debugger("Unimplemented script_limits opcode");
}


void ESwf::exec_set_tab_index(ULONG loc,unsigned int opcode)
{
  //debugger("Unimplemented set_tab_index opcode");
}


void ESwf::exec_place_object3(ULONG loc,unsigned int opcode)
{
  //debugger("Unimplemented place_object3 opcode");
}


////////////////////////////////////////////////////////////////////////////////
//  Action opcodes
////////////////////////////////////////////////////////////////////////////////

void ESwf::exec_end(ULONG loc,unsigned int opcode)
{
  action=false;
}


void ESwf::exec_nextframe(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented nextframe opcode");
}


void ESwf::exec_prevframe(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented prevframe opcode");
}


void ESwf::exec_play(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented play opcode");
}


void ESwf::exec_stop(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented stop opcode");
}


void ESwf::exec_togglequality(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented togglequality opcode");
}


void ESwf::exec_stopsounds(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented stopsounds opcode");
}


void ESwf::exec_add(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented add opcode");
}


void ESwf::exec_subtract(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented subtract opcode");
}


void ESwf::exec_multiply(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented multiply opcode");
}


void ESwf::exec_divide(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented divide opcode");
}


void ESwf::exec_equals(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented equals opcode");
}


void ESwf::exec_lessthan(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented lessthan opcode");
}


void ESwf::exec_logicaland(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented logicaland opcode");
}


void ESwf::exec_logicalor(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented logicalor opcode");
}


void ESwf::exec_logicalnot(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented logicalnot opcode");
}


void ESwf::exec_stringeq(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented stringeq opcode");
}


void ESwf::exec_stringlength(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented stringlength opcode");
}


void ESwf::exec_substring(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented substring opcode");
}


void ESwf::exec_pop(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented pop opcode");
}


void ESwf::exec_int(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented int opcode");
}


void ESwf::exec_getvariable(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented getvariable opcode");
}


void ESwf::exec_setvariable(ULONG loc,unsigned int opcode)
{
  //debugger("Unimplemented setvariable opcode");
}


void ESwf::exec_settargetexpression(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented settargetexpression opcode");
}


void ESwf::exec_stringconcat(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented stringconcat opcode");
}


void ESwf::exec_getproperty(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented getproperty opcode");
}


void ESwf::exec_setproperty(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented setproperty opcode");
}


void ESwf::exec_duplicateclip(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented duplicateclip opcode");
}


void ESwf::exec_removeclip(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented removeclip opcode");
}


void ESwf::exec_trace(ULONG loc,unsigned int opcode)
{
  //debugger("Unimplemented trace opcode");
}


void ESwf::exec_startdragmovie(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented startdragmovie opcode");
}


void ESwf::exec_stopdragmovie(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented stopdragmovie opcode");
}


void ESwf::exec_stringlessthan(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented stringlessthan opcode");
}


void ESwf::exec_random(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented random opcode");
}


void ESwf::exec_mblength(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented mblength opcode");
}


void ESwf::exec_ord(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented ord opcode");
}


void ESwf::exec_chr(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented chr opcode");
}


void ESwf::exec_gettimer(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented gettimer opcode");
}


void ESwf::exec_mbsubstring(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented mbsubstring opcode");
}


void ESwf::exec_mbord(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented mbord opcode");
}


void ESwf::exec_mbchr(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented mbchr opcode");
}


void ESwf::exec_delete(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented delete opcode");
}


void ESwf::exec_varequals(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented varequals opcode");
}


void ESwf::exec_callfunction(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented callfunction opcode");
}


void ESwf::exec_return(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented return opcode");
}


void ESwf::exec_modulo(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented modulo opcode");
}


void ESwf::exec_new(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented new opcode");
}


void ESwf::exec_var(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented var opcode");
}


void ESwf::exec_initarray(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented initarray opcode");
}


void ESwf::exec_initobject(ULONG loc,unsigned int opcode)
{
  //debugger("Unimplemented initobject opcode");
}


void ESwf::exec_typeof(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented typeof opcode");
}


void ESwf::exec_targetpath(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented targetpath opcode");
}


void ESwf::exec_enumerate(ULONG loc,unsigned int opcode)
{
  //debugger("Unimplemented enumerate opcode");
}


void ESwf::exec_newadd(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented newadd opcode");
}


void ESwf::exec_newlessthan(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented newlessthan opcode");
}


void ESwf::exec_newequals(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented newequals opcode");
}


void ESwf::exec_dup(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented dup opcode");
}


void ESwf::exec_swap(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented swap opcode");
}


void ESwf::exec_getmember(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented getmember opcode");
}


void ESwf::exec_setmember(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented setmember opcode");
}


void ESwf::exec_increment(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented increment opcode");
}


void ESwf::exec_decrement(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented decrement opcode");
}


void ESwf::exec_callmethod(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented callmethod opcode");
}


void ESwf::exec_bitwiseand(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented bitwiseand opcode");
}


void ESwf::exec_bitwiseor(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented bitwiseor opcode");
}


void ESwf::exec_bitwisexor(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented bitwisexor opcode");
}


void ESwf::exec_shiftleft(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented shiftleft opcode");
}


void ESwf::exec_shiftright(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented shiftright opcode");
}


void ESwf::exec_shiftright2(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented shiftright2 opcode");
}


void ESwf::exec_gotoframe(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented gotoframe opcode");
}


void ESwf::exec_geturl(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented geturl opcode");
}


void ESwf::exec_setregister(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented setregister opcode");
}


void ESwf::exec_constantpool(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented constantpool opcode");
}


void ESwf::exec_waitforframe(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented waitforframe opcode");
}


void ESwf::exec_settarget(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented settarget opcode");
}


void ESwf::exec_gotolabel(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented gotolabel opcode");
}


void ESwf::exec_waitforframeexpression(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented waitforframeexpression opcode");
}


void ESwf::exec_with(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented with opcode");
}


void ESwf::exec_pushdata(ULONG loc,unsigned int opcode)
{
  unsigned int num=map->read16l(loc+1);
  DEBUG_OUT<<num<<" bytes args\n";
  op_len+=num+2;
  //debugger("Unimplemented pushdata opcode");
}


void ESwf::exec_branchalways(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented branchalways opcode");
}


void ESwf::exec_geturl2(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented geturl2 opcode");
}


void ESwf::exec_definefunction(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented definefunction opcode");
}


void ESwf::exec_branchiftrue(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented branchiftrue opcode");
}


void ESwf::exec_callframe(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented callframe opcode");
}



void ESwf::exec_gotoexpression(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented gotoexpression opcode");
}


void ESwf::exec_strict_mode(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented strict_mode opcode");
}


void ESwf::exec_declarefunction2(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented declarefunction2 opcode");
}


void ESwf::exec_try(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented try opcode");
}
