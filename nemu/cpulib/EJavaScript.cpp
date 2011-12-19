
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#define DEBUG_DIS_MODES 1

#define DEBUG_EXEC 1

#define CHECK_SAFE 1

#define SHOW_INTS  1

#include <portable/nickcpp/ANullStream.h>

#include <portable/nickcpp/AStringHelper.h>

#include <cpulib/EJavaScript.h>

#include <elib/EMemMapAbstract.h>
#include <elib/EModule.h>

#include <binarylib/NLd.h>
#include <binarylib/NJavaClassDecoder.h>


#include <cpulib/generated/EJavascript_tab.h>


// The different cpus...
#define E_CPU_NONE     0x00
#define E_CPU_JS       0x02


////////////////////////////////////////////////////////////////////////////////
//  EJavascript Class
////////////////////////////////////////////////////////////////////////////////

EJavascript::EJavascript()
{
  DEBUG_OUT<<"EJavascript created...\n";
  init();
  type=E_CPU_JS;
  //w=ww;
  reset();
}


EJavascript::~EJavascript()
{
  DEBUG_OUT<<"EJavascript destroyed.\n";
}


void EJavascript::init()
{
  ECPUByteCode::init();
  // Generic features
  doTrace=false;  doRun=false;
  // Set up stack and registers
  int_pending=0;
  intLock=false;
  lsb=false;
  setStack(ECPUABSTRACT_DEFAULT_STACK);
  setFramePtr(ECPUABSTRACT_DEFAULT_STACK);
  setResetStack(ECPUABSTRACT_DEFAULT_STACK);
  setResetEntry(ECPUABSTRACT_DEFAULT_CODE);
}


/* STATIC */
bool EJavascript::recognize(const char *tname)
{
  const char *name=aDefaultStringHelper.removeSpaces2(aDefaultStringHelper.toLower(tname));
  if(!strcmp(name,"js")) return true; else
  if(!strcmp(name,"javascript")) return true; else
  return false;
}


const char *EJavascript::getName()
{
  return "Javascript";
}


void EJavascript::setKind(const char *tname)
{
  const char *name=aDefaultStringHelper.removeSpaces2(aDefaultStringHelper.toLower(tname));
  type=E_CPU_JS;
}


void EJavascript::reset()
{
  EMapSymbol *s=NULL;
  if(map) s=map->getFirstModule()->lookupSymbolByNameAndType("main",NJAVA_SYM_CODE);
  if(s) {
    ECPUByteCodeObject o;
    // array of command line args...none for now
    o.type=ECPUABSTRACT_TYPE_ARRAY;
    o.len=0;
    o.val=0;
    o.name=0;
    //pushObj(&o);
    //resetEntry=s->getAddr();
    //jump(resetEntry);
  }
#ifdef DEBUG_EXEC
  setTrace(true);
#endif
  breakpoint=0xffffffff;  // since we start at 0, breakpoint's default
  intLock=false;
  pc=0;  // Jump to zero
  clearStackTrace();
  ECPUAbstract::reset();
}


bool EJavascript::setRegister(const char *tname,ULONG value)
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


ULONG EJavascript::readRegister(const char *tname)
{
  const char *name=aDefaultStringHelper.removeSpaces2(aDefaultStringHelper.toLower(tname));
  if(!strcmp(name,"sp")) { return sp; }
  return 0;
}


void EJavascript::wregs()
{
  DEBUG_OUT<<"PC="<<pc<<"\n";
  DEBUG_OUT<<"SP="<<sp<<"\n";
}


void EJavascript::handle_int(ULONG value)
{
}


void EJavascript::cause_int(ULONG value)
{
  int_pending=0;
  switch(value) {
    default:
      debugger("Illegal interrupt requested!\n");
      break;
  }
}


void EJavascript::ints()
{
  //if(w) {
  //}
}


void EJavascript::execute()
{
  //if(!lastUsedAddress) loadInitialClass();
  if(pc==breakpoint) { debugger("Breakpoint!\n"); return; }
  opcode=map->read8(pc);
  op_len=EJavascript_size[opcode];
  cycle=EJavascript_time[opcode];
  unsigned int topcode=opcode,loc=pc;
#include <cpulib/generated/EJavascript_exec.h>
  pc+=op_len;
  cycles+=cycle;
  if(int_pending) handle_int(int_pending);
}


ULONG EJavascript::disasm(ULONG loc)
{
  BYTE topcode;
  unsigned int len=0,mode=EJVM_ADDR_NONE;
  const char *name="???";
  topcode=map->read8(loc);
  lastSymbol=map->findAnyExactSymbol(loc);
  if(lastSymbol) DEBUG_OUT<<lastSymbol->getName()<<":\n";
  DEBUG_OUT<<"";
  DEBUG_OUT<<"."; DEBUG_OUT<<loc;  DEBUG_OUT<<"  ";
  len=EJavascript_size[topcode];
  mode=EJavascript_format[topcode];
  name=EJavascript_name[topcode];
  if(!len) len=1;  // NOTE: An unimplemented opcode...
  unsigned int t;
  for(t=0;t<len;t++) DEBUG_OUT<<(short)map->read8(loc+t)<<" ";
  for(t=0;t<(3-len);t++) DEBUG_OUT<<"   ";
  DEBUG_OUT<<name<<" ";
  //DEBUG_OUT<<"Len was "<<len<<"\n";
  len+=printAddrMode(loc,topcode,mode);
  //DEBUG_OUT<<"Len now is "<<len<<"\n";
  DEBUG_OUT<<"";
  DEBUG_OUT<<"\n";
  return len;
}



unsigned int EJavascript::printAddrMode(ULONG loc,unsigned int topcode,unsigned int mode)
{
  switch(mode) {
    case EJVM_ADDR_NONE:
      break;
    case EJVM_ADDR_ONE:
      break;
    case EJVM_ADDR_TWO:
      break;
    case EJVM_ADDR_IMM8:
      DEBUG_OUT<<(short)map->read8(loc+1);
      break;
    case EJVM_ADDR_IMM16:
      DEBUG_OUT<<map->read16b(loc+1);
      break;
    default:
      debugger("Bad addr mode!\n");
      break;
  }
  return 0;
}


void EJavascript::exec_nop(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented nop opcode");
}


void EJavascript::exec_push(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented push opcode");
}


void EJavascript::exec_popv(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented popv opcode");
}


void EJavascript::exec_enterwith(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented enterwith opcode");
}


void EJavascript::exec_leavewith(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented leavewith opcode");
}


void EJavascript::exec_return(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented return opcode");
}


void EJavascript::exec_goto(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented goto opcode");
}


void EJavascript::exec_ifeq(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented ifeq opcode");
}


void EJavascript::exec_ifne(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented ifne opcode");
}


void EJavascript::exec_js_arguments_str(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented js_arguments_str opcode");
}


void EJavascript::exec_forarg(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented forarg opcode");
}


void EJavascript::exec_forvar(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented forvar opcode");
}


void EJavascript::exec_dup(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented dup opcode");
}


void EJavascript::exec_dup2(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented dup2 opcode");
}


void EJavascript::exec_setconst(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented setconst opcode");
}


void EJavascript::exec_bitor(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented bitor opcode");
}


void EJavascript::exec_bitxor(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented bitxor opcode");
}


void EJavascript::exec_bitand(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented bitand opcode");
}


void EJavascript::exec_eq(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented eq opcode");
}


void EJavascript::exec_ne(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented ne opcode");
}


void EJavascript::exec_lt(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented lt opcode");
}


void EJavascript::exec_le(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented le opcode");
}


void EJavascript::exec_gt(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented gt opcode");
}


void EJavascript::exec_ge(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented ge opcode");
}


void EJavascript::exec_lsh(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented lsh opcode");
}


void EJavascript::exec_rsh(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented rsh opcode");
}


void EJavascript::exec_ursh(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented ursh opcode");
}


void EJavascript::exec_add(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented add opcode");
}


void EJavascript::exec_sub(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented sub opcode");
}


void EJavascript::exec_mul(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented mul opcode");
}


void EJavascript::exec_div(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented div opcode");
}


void EJavascript::exec_mod(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented mod opcode");
}


void EJavascript::exec_not(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented not opcode");
}


void EJavascript::exec_bitnot(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented bitnot opcode");
}


void EJavascript::exec_neg(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented neg opcode");
}


void EJavascript::exec_js_new_str(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented js_new_str opcode");
}


void EJavascript::exec_delname(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented delname opcode");
}


void EJavascript::exec_delprop(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented delprop opcode");
}


void EJavascript::exec_delelem(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented delelem opcode");
}


void EJavascript::exec_js_typeof_str(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented js_typeof_str opcode");
}


void EJavascript::exec_js_void_str(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented js_void_str opcode");
}


void EJavascript::exec_incname(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented incname opcode");
}


void EJavascript::exec_incprop(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented incprop opcode");
}


void EJavascript::exec_incelem(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented incelem opcode");
}


void EJavascript::exec_decname(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented decname opcode");
}


void EJavascript::exec_decprop(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented decprop opcode");
}


void EJavascript::exec_decelem(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented decelem opcode");
}


void EJavascript::exec_nameinc(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented nameinc opcode");
}


void EJavascript::exec_propinc(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented propinc opcode");
}


void EJavascript::exec_eleminc(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented eleminc opcode");
}


void EJavascript::exec_namedec(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented namedec opcode");
}


void EJavascript::exec_propdec(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented propdec opcode");
}


void EJavascript::exec_elemdec(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented elemdec opcode");
}


void EJavascript::exec_getprop(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented getprop opcode");
}


void EJavascript::exec_setprop(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented setprop opcode");
}


void EJavascript::exec_getelem(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented getelem opcode");
}


void EJavascript::exec_setelem(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented setelem opcode");
}


void EJavascript::exec_pushobj(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented pushobj opcode");
}


void EJavascript::exec_call(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented call opcode");
}


void EJavascript::exec_name(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented name opcode");
}


void EJavascript::exec_number(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented number opcode");
}


void EJavascript::exec_string(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented string opcode");
}


void EJavascript::exec_zero(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented zero opcode");
}


void EJavascript::exec_one(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented one opcode");
}


void EJavascript::exec_js_null_str(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented js_null_str opcode");
}


void EJavascript::exec_js_this_str(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented js_this_str opcode");
}


void EJavascript::exec_js_false_str(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented js_false_str opcode");
}


void EJavascript::exec_js_true_str(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented js_true_str opcode");
}


void EJavascript::exec_or(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented or opcode");
}


void EJavascript::exec_and(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented and opcode");
}


void EJavascript::exec_tableswitch(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented tableswitch opcode");
}


void EJavascript::exec_lookupswitch(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented lookupswitch opcode");
}


void EJavascript::exec_closure(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented closure opcode");
}


void EJavascript::exec_exportall(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented exportall opcode");
}


void EJavascript::exec_exportname(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented exportname opcode");
}


void EJavascript::exec_importall(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented importall opcode");
}


void EJavascript::exec_importprop(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented importprop opcode");
}


void EJavascript::exec_importelem(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented importelem opcode");
}


void EJavascript::exec_object(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented object opcode");
}


void EJavascript::exec_pop(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented pop opcode");
}


void EJavascript::exec_pos(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented pos opcode");
}


void EJavascript::exec_trap(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented trap opcode");
}


void EJavascript::exec_getarg(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented getarg opcode");
}


void EJavascript::exec_setarg(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented setarg opcode");
}


void EJavascript::exec_getvar(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented getvar opcode");
}


void EJavascript::exec_setvar(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented setvar opcode");
}


void EJavascript::exec_uint16(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented uint16 opcode");
}


void EJavascript::exec_newinit(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented newinit opcode");
}


void EJavascript::exec_endinit(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented endinit opcode");
}


void EJavascript::exec_initprop(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented initprop opcode");
}


void EJavascript::exec_initelem(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented initelem opcode");
}


void EJavascript::exec_defsharp(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented defsharp opcode");
}


void EJavascript::exec_usesharp(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented usesharp opcode");
}


void EJavascript::exec_incarg(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented incarg opcode");
}


void EJavascript::exec_incvar(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented incvar opcode");
}


void EJavascript::exec_decarg(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented decarg opcode");
}


void EJavascript::exec_decvar(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented decvar opcode");
}


void EJavascript::exec_arginc(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented arginc opcode");
}


void EJavascript::exec_varinc(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented varinc opcode");
}


void EJavascript::exec_argdec(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented argdec opcode");
}


void EJavascript::exec_vardec(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented vardec opcode");
}


void EJavascript::exec_toobject(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented toobject opcode");
}


void EJavascript::exec_forname(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented forname opcode");
}


void EJavascript::exec_forprop(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented forprop opcode");
}


void EJavascript::exec_forelem(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented forelem opcode");
}


void EJavascript::exec_pop2(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented pop2 opcode");
}


void EJavascript::exec_bindname(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented bindname opcode");
}


void EJavascript::exec_setname(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented setname opcode");
}


void EJavascript::exec_throw(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented throw opcode");
}


void EJavascript::exec_js_in_str(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented js_in_str opcode");
}


void EJavascript::exec_js_instanceof_str(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented js_instanceof_str opcode");
}


void EJavascript::exec_debugger(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented debugger opcode");
}


void EJavascript::exec_gosub(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented gosub opcode");
}


void EJavascript::exec_retsub(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented retsub opcode");
}


void EJavascript::exec_exception(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented exception opcode");
}


void EJavascript::exec_setsp(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented setsp opcode");
}


void EJavascript::exec_condswitch(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented condswitch opcode");
}


void EJavascript::exec_case(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented case opcode");
}


void EJavascript::exec_default(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented default opcode");
}


void EJavascript::exec_eval(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented eval opcode");
}


void EJavascript::exec_enumelem(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented enumelem opcode");
}


void EJavascript::exec_js_getter_str(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented js_getter_str opcode");
}


void EJavascript::exec_js_setter_str(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented js_setter_str opcode");
}


void EJavascript::exec_deffun(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented deffun opcode");
}


void EJavascript::exec_defconst(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented defconst opcode");
}


void EJavascript::exec_defvar(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented defvar opcode");
}


void EJavascript::exec_anonfunobj(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented anonfunobj opcode");
}


void EJavascript::exec_namedfunobj(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented namedfunobj opcode");
}


void EJavascript::exec_initcatchvar(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented initcatchvar opcode");
}


void EJavascript::exec_group(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented group opcode");
}


void EJavascript::exec_setcall(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented setcall opcode");
}


void EJavascript::exec_try(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented try opcode");
}


void EJavascript::exec_finally(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented finally opcode");
}


void EJavascript::exec_swap(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented swap opcode");
}


void EJavascript::exec_argsub(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented argsub opcode");
}


void EJavascript::exec_argcnt(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented argcnt opcode");
}


void EJavascript::exec_deflocalfun(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented deflocalfun opcode");
}


void EJavascript::exec_gotox(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented gotox opcode");
}


void EJavascript::exec_ifeqx(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented ifeqx opcode");
}


void EJavascript::exec_ifnex(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented ifnex opcode");
}


void EJavascript::exec_orx(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented orx opcode");
}


void EJavascript::exec_andx(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented andx opcode");
}


void EJavascript::exec_gosubx(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented gosubx opcode");
}


void EJavascript::exec_casex(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented casex opcode");
}


void EJavascript::exec_defaultx(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented defaultx opcode");
}


void EJavascript::exec_tableswitchx(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented tableswitchx opcode");
}


void EJavascript::exec_lookupswitchx(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented lookupswitchx opcode");
}


void EJavascript::exec_backpatch(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented backpatch opcode");
}


void EJavascript::exec_backpatch_pop(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented backpatch_pop opcode");
}


void EJavascript::exec_backpatch_push(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented backpatch_push opcode");
}


void EJavascript::exec_setrval(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented setrval opcode");
}


void EJavascript::exec_retrval(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented retrval opcode");
}


void EJavascript::exec_getgvar(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented getgvar opcode");
}


void EJavascript::exec_setgvar(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented setgvar opcode");
}


void EJavascript::exec_incgvar(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented incgvar opcode");
}


void EJavascript::exec_decgvar(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented decgvar opcode");
}


void EJavascript::exec_gvarinc(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented gvarinc opcode");
}


void EJavascript::exec_gvardec(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented gvardec opcode");
}


void EJavascript::exec_regexp(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented regexp opcode");
}


void EJavascript::exec_defxmlns(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented defxmlns opcode");
}


void EJavascript::exec_anyname(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented anyname opcode");
}


void EJavascript::exec_qnamepart(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented qnamepart opcode");
}


void EJavascript::exec_qnameconst(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented qnameconst opcode");
}


void EJavascript::exec_qname(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented qname opcode");
}


void EJavascript::exec_toattrname(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented toattrname opcode");
}


void EJavascript::exec_toattrval(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented toattrval opcode");
}


void EJavascript::exec_addattrname(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented addattrname opcode");
}


void EJavascript::exec_addattrval(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented addattrval opcode");
}


void EJavascript::exec_bindxmlname(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented bindxmlname opcode");
}


void EJavascript::exec_setxmlname(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented setxmlname opcode");
}


void EJavascript::exec_xmlname(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented xmlname opcode");
}


void EJavascript::exec_descendants(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented descendants opcode");
}


void EJavascript::exec_filter(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented filter opcode");
}


void EJavascript::exec_endfilter(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented endfilter opcode");
}


void EJavascript::exec_toxml(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented toxml opcode");
}


void EJavascript::exec_toxmllist(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented toxmllist opcode");
}


void EJavascript::exec_xmltagexpr(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented xmltagexpr opcode");
}


void EJavascript::exec_xmleltexpr(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented xmleltexpr opcode");
}


void EJavascript::exec_xmlobject(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented xmlobject opcode");
}


void EJavascript::exec_xmlcdata(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented xmlcdata opcode");
}


void EJavascript::exec_xmlcomment(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented xmlcomment opcode");
}


void EJavascript::exec_xmlpi(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented xmlpi opcode");
}


void EJavascript::exec_getmethod(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented getmethod opcode");
}


void EJavascript::exec_getfunns(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented getfunns opcode");
}


void EJavascript::exec_foreach(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented foreach opcode");
}


void EJavascript::exec_deldesc(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented deldesc opcode");
}


void EJavascript::exec_uint24(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented uint24 opcode");
}


void EJavascript::exec_literal(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented literal opcode");
}


void EJavascript::exec_findname(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented findname opcode");
}


void EJavascript::exec_litopx(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented litopx opcode");
}


void EJavascript::exec_startxml(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented startxml opcode");
}


void EJavascript::exec_startxmlexpr(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented startxmlexpr opcode");
}


void EJavascript::exec_setmethod(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented setmethod opcode");
}


void EJavascript::exec_stop(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented stop opcode");
}


void EJavascript::exec_getxprop(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented getxprop opcode");
}


void EJavascript::exec_getxelem(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented getxelem opcode");
}

