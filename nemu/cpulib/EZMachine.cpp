
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


//#define DEBUG_DIS_MODES 1

#define DEBUG_EXEC 1

#define CHECK_SAFE 1

#define SHOW_INTS  1


#include <cpulib/EZMachine.h>

#include <elib/EMemMapAbstract.h>
#include <elib/EModule.h>

#include <portable/nickcpp/ANullStream.h>

#include <portable/nickcpp/AStringHelper.h>


#include <cpulib/generated/EZMachine_tab.h>


// The different cpus...
#define E_CPU_NONE     0x00
#define E_CPU_ZMACHINE3   0x03
#define E_CPU_ZMACHINE4   0x04
#define E_CPU_ZMACHINE5   0x05
#define E_CPU_ZMACHINE6   0x06
#define E_CPU_ZMACHINE7   0x07
#define E_CPU_ZMACHINE8   0x08
#define E_CPU_GLULX  0xa


// operand types
#define EZ_LARGE 0
#define EZ_SMALL 1
#define EZ_VAR   2
//#define EZ_NONE  3


////////////////////////////////////////////////////////////////////////////////
//  EZMachine Class
////////////////////////////////////////////////////////////////////////////////

EZMachine::EZMachine()
{
  DEBUG_OUT<<"EZMachine created...\n";
  init();
  type=E_CPU_ZMACHINE3;
  //w=ww;
  reset();
}


EZMachine::~EZMachine()
{
  DEBUG_OUT<<"EZMachine destroyed.\n";
}


void EZMachine::init()
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
  highBase=0; staticBase=0;
  dictLoc=0; objLoc=0; globalLoc=0; abvLoc=0;
  initialPC=0;
  branch=0;  store=0;
}


/* STATIC */
bool EZMachine::recognize(const char *tname)
{
  const char *name=aDefaultStringHelper.removeSpaces2(aDefaultStringHelper.toLower(tname));
  if(!strcmp(name,"zmachine")) return true; else
  return false;
}


const char *EZMachine::getName()
{
  if(type==E_CPU_ZMACHINE3) return "ZMachine 3";
  if(type==E_CPU_ZMACHINE4) return "ZMachine 4";
  if(type==E_CPU_ZMACHINE5) return "ZMachine 5";
  if(type==E_CPU_ZMACHINE6) return "ZMachine 6";
  if(type==E_CPU_ZMACHINE7) return "ZMachine 7";
  if(type==E_CPU_ZMACHINE8) return "ZMachine 8";
  return "ZMachine";
}


void EZMachine::setKind(const char *tname)
{
  const char *name=aDefaultStringHelper.removeSpaces2(aDefaultStringHelper.toLower(tname));
  type=E_CPU_ZMACHINE3;
}


void EZMachine::reset()
{
#ifdef DEBUG_EXEC
  setTrace(true);
#endif
  breakpoint=0xffffffff;  // since we start at 0, breakpoint's default
  intLock=false;
  pc=0;  // Jump to zero
  clearStackTrace();
  handleHeader();
  ECPUAbstract::reset();
}


bool EZMachine::setRegister(const char *tname,ULONG value)
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


ULONG EZMachine::readRegister(const char *tname)
{
  const char *name=aDefaultStringHelper.removeSpaces2(aDefaultStringHelper.toLower(tname));
  if(!strcmp(name,"sp")) { return sp; }
  return 0;
}


void EZMachine::wregs()
{
  DEBUG_OUT<<"PC="<<pc<<"\n";
  DEBUG_OUT<<"SP="<<sp<<"\n";
}


void EZMachine::execute()
{
  if(pc==breakpoint) { debugger("Breakpoint!\n"); return; }
  opcode=map->read8(pc);
  op_len=EZMachine_size[opcode];
  cycle=EZMachine_time[opcode];
  mode=EZMachine_format[opcode];
  store=EZMachine_subformat[opcode];
  branch=EZMachine_flags[opcode];
  topcode=opcode;
  unsigned int loc=pc;
  decodeAddrMode(false,loc,topcode,mode);
  if(mode==EZ_ADDR_SPECIAL) { execSpecial(loc,topcode); }
  else {
#include <cpulib/generated/EZMachine_exec.h>
  }
  pc+=op_len;
  cycles+=cycle;
  if(int_pending) handle_int(int_pending);
}


ULONG EZMachine::disasm(ULONG loc)
{
  BYTE topcode;
  unsigned int len=0;
  const char *name="???";
  topcode=map->read8(loc);
  lastSymbol=map->findAnyExactSymbol(loc);
  if(lastSymbol) DEBUG_OUT<<lastSymbol->getName()<<":\n";
  DEBUG_OUT<<"."; DEBUG_OUT<<loc;  DEBUG_OUT<<"  ";
  len=EZMachine_size[topcode];
  mode=EZMachine_format[topcode];
  name=EZMachine_name[topcode];
  store=EZMachine_subformat[topcode];
  branch=EZMachine_flags[topcode];
  if(!len) len=1;  // NOTE: An unimplemented opcode...
  unsigned int t;
  for(t=0;t<len;t++) DEBUG_OUT<<(short)map->read8(loc+t)<<" ";
  for(t=0;t<(3-len);t++) DEBUG_OUT<<"   ";
  op_len=0;
  DEBUG_OUT<<"@";
  switch(mode) {
    case EZ_ADDR_SPECIAL:
      disSpecial(loc,topcode);
      break;
    case EZ_ADDR_EXTEND:
      disExtended(loc,topcode);
      break;
    default:
      DEBUG_OUT<<name<<" ";
      break;
  }
  //DEBUG_OUT<<"Len was "<<len<<"\n";
  decodeAddrMode(true,loc,topcode,mode);
  len+=op_len;
  //DEBUG_OUT<<"Len now is "<<len<<"\n";
  DEBUG_OUT<<"\n";
  return len;
}


// NOTE: loc points to opcode
void EZMachine::disExtended(ULONG loc,unsigned int topcode)
{
  switch(topcode) {
    default:
      debugger("Unknown opcode in disExtended!");
      break;
  }
}


// NOTE: loc points to opcode
void EZMachine::disSpecial(ULONG loc,unsigned int topcode)
{
  switch(topcode) {
    case 0x1b:  mode=EZ_ADDR_2OP_SMALL_SMALL;
    case 0x3b:  mode=EZ_ADDR_2OP_SMALL_VAR;
    case 0x5b:  mode=EZ_ADDR_2OP_VAR_SMALL;
    case 0x7b:  mode=EZ_ADDR_2OP_VAR_VAR;
    case 0xdb:  mode=EZ_ADDR_VAROP;
      // set_colour
      // if vers 6, then has extra operand
      DEBUG_OUT<<"set_colour ";
      break;
    case 0x8f:  mode=EZ_ADDR_1OP_LARGE;
    case 0x9f:  mode=EZ_ADDR_1OP_SMALL;
    case 0xaf:  mode=EZ_ADDR_1OP_VAR;
      // not/call_1n
      if(type<5) { DEBUG_OUT<<"not "; branch=EZ_NONE; }
      else { DEBUG_OUT<<"call_1n "; branch=EZ_CALL; }
      break;
    case 0xb5:
      // save/illegal
      if(type>4) { DEBUG_OUT<<"illegal "; mode=EZ_ADDR_ILLEGAL; }
      else {
        mode=EZ_ADDR_0OP;
        DEBUG_OUT<<"save ";
        if(type==4) { branch=EZ_NONE;  store=EZ_STORE; }
        else { branch=EZ_BRANCH;  store=EZ_NONE; }
      }
      break;
    case 0xb6:
      // restore/illegal
      if(type>4) { DEBUG_OUT<<"illegal "; mode=EZ_ADDR_ILLEGAL; }
      else {
        mode=EZ_ADDR_0OP;
        DEBUG_OUT<<"restore ";
        if(type==4) { branch=EZ_NONE;  store=EZ_STORE; }
        else { branch=EZ_BRANCH;  store=EZ_NONE; }
      }
      break;
    case 0xb9:  mode=EZ_ADDR_0OP;
      // pop/catch
      if(type<5) {
        DEBUG_OUT<<"pop ";
      }
      else {
        DEBUG_OUT<<"catch ";
        store=EZ_STORE;
      }
      break;
    case 0xbc:  mode=EZ_ADDR_0OP;
      // show_status/illegal
      if(type>3) { DEBUG_OUT<<"illegal "; mode=EZ_ADDR_ILLEGAL; }
      else {
        mode=EZ_ADDR_0OP;
        DEBUG_OUT<<"show_status ";
      }
      break;
    case 0xe0:  mode=EZ_ADDR_VAROP;
      // call/call_vs
      branch=EZ_CALL;
      store=EZ_STORE;
      if(type>3) {
        DEBUG_OUT<<"call_vs ";
      }
      else {
        DEBUG_OUT<<"call ";
      }
      break;
    case 0xe4:  mode=EZ_ADDR_VAROP;
      // sread/aread
      if(type>4) {
        DEBUG_OUT<<"aread ";
        store=EZ_STORE;
      }
      else {
        DEBUG_OUT<<"sread ";
      }
      break;
    case 0xe9:  mode=EZ_ADDR_VAROP;
      // pull
      DEBUG_OUT<<"pull ";
      store=EZ_STORE;
      break;
    case 0xee:  mode=EZ_ADDR_VAROP;
      // erase_line
      DEBUG_OUT<<"erase_line ";
      break;
    case 0xef:  mode=EZ_ADDR_VAROP;
      // set_cursor
      DEBUG_OUT<<"set_cursor ";
      break;
    case 0xf3:  mode=EZ_ADDR_VAROP;
      // output_stream
      DEBUG_OUT<<"output_stream ";
      break;
    default:
      debugger("Unknown opcode in disSpecial!");
      break;
  }
}


// NOTE: loc points to opcode
void EZMachine::execSpecial(ULONG loc,unsigned int topcode)
{
  switch(topcode) {
    case 0x1b:  mode=EZ_ADDR_2OP_SMALL_SMALL;
    case 0x3b:  mode=EZ_ADDR_2OP_SMALL_VAR;
    case 0x5b:  mode=EZ_ADDR_2OP_VAR_SMALL;
    case 0x7b:  mode=EZ_ADDR_2OP_VAR_VAR;
    case 0xdb:  mode=EZ_ADDR_VAROP;
      // set_colour
      // if vers 6, then has extra operand
      exec_set_colour(loc,topcode);
      break;
    case 0x8f:  mode=EZ_ADDR_1OP_LARGE;
    case 0x9f:  mode=EZ_ADDR_1OP_SMALL;
    case 0xaf:  mode=EZ_ADDR_1OP_VAR;
      // not/call_1n
      if(type<5) { branch=EZ_NONE; exec_not(loc,topcode); }
      else { branch=EZ_CALL; exec_call_1n(loc,topcode); }
      break;
    case 0xb5:
      // save/illegal
      if(type>4) { mode=EZ_ADDR_ILLEGAL;  exec_illegal(loc,topcode); }
      else {
        mode=EZ_ADDR_0OP;
        if(type==4) { branch=EZ_NONE;  store=EZ_STORE; }
        else { branch=EZ_BRANCH;  store=EZ_NONE; }
        exec_save(loc,topcode);
      }
      break;
    case 0xb6:
      // restore/illegal
      if(type>4) { mode=EZ_ADDR_ILLEGAL;  exec_illegal(loc,topcode); }
      else {
        mode=EZ_ADDR_0OP;
        if(type==4) { branch=EZ_NONE;  store=EZ_STORE; }
        else { branch=EZ_BRANCH;  store=EZ_NONE; }
        exec_restore(loc,topcode);
      }
      break;
    case 0xb9:  mode=EZ_ADDR_0OP;
      // pop/catch
      if(type<5) {
        exec_pop(loc,topcode);
      }
      else {
        store=EZ_STORE;
        exec_catch(loc,topcode);
      }
      break;
    case 0xbc:  mode=EZ_ADDR_0OP;
      // show_status/illegal
      if(type>3) { mode=EZ_ADDR_ILLEGAL;  exec_illegal(loc,topcode); }
      else {
        mode=EZ_ADDR_0OP;
        exec_show_status(loc,topcode);
      }
      break;
    case 0xe0:  mode=EZ_ADDR_VAROP;
      // call/call_vs
      branch=EZ_CALL;
      store=EZ_STORE;
      if(type>3) {
        exec_call_vs(loc,topcode);
      }
      else {
        exec_call(loc,topcode);
      }
      break;
    case 0xe4:  mode=EZ_ADDR_VAROP;
      // sread/aread
      if(type>4) {
        store=EZ_STORE;
        exec_aread(loc,topcode);
      }
      else {
        exec_sread(loc,topcode);
      }
      break;
    case 0xe9:  mode=EZ_ADDR_VAROP;
      // pull
      store=EZ_STORE;
      exec_pull(loc,topcode);
      break;
    case 0xee:  mode=EZ_ADDR_VAROP;
      // erase_line
      exec_erase_line(loc,topcode);
      break;
    case 0xef:  mode=EZ_ADDR_VAROP;
      // set_cursor
      exec_set_cursor(loc,topcode);
      break;
    case 0xf3:  mode=EZ_ADDR_VAROP;
      // output_stream
      exec_output_stream(loc,topcode);
      break;
    default:
      debugger("Unknown opcode in execSpecial!");
      break;
  }
}


ULONG EZMachine::unpackAddress(unsigned int a)
{
  ULONG ret=0;
  switch(type)
  {
    case 3:
      ret=a*2;
      break;
    case 4:  case 5:
      ret=a*4;
      break;
    case 6:  case 7:
      // TODO: plus 8*offset
      ret=a*4;
      break;
    case 8:
      ret=a*8;
      break;
    default:
      break;
  }
  return ret;
}


// NOTE: loc points to opcode
unsigned int EZMachine::decodeAddrMode(bool print,ULONG loc,unsigned int topcode,unsigned int mode)
{
  unsigned int t=0;
  unsigned int ret=0;
  unsigned int types=0;
  unsigned int type[4];
  op1=0;  op2=0;
  switch(mode) {
    case EZ_ADDR_SPECIAL:
      // ignore it for now, we'll have to decode again later...
      break;
    case EZ_ADDR_ILLEGAL:
#ifdef DEBUG_DIS_MODES
      DEBUG_OUT<<" ; illegal";
#endif // DEBUG_DIS_MODES
      break;
    case EZ_ADDR_0OP:
#ifdef DEBUG_DIS_MODES
      DEBUG_OUT<<" ; 0op";
#endif // DEBUG_DIS_MODES
      break;
    case EZ_ADDR_1OP_LARGE:
      op1=getOperand(print,loc+op_len,EZ_LARGE);
#ifdef DEBUG_DIS_MODES
      DEBUG_OUT<<" ; 1op large";
#endif // DEBUG_DIS_MODES
      break;
    case EZ_ADDR_1OP_SMALL:
      op1=getOperand(print,loc+op_len,EZ_SMALL);
#ifdef DEBUG_DIS_MODES
      DEBUG_OUT<<" ; 1op small";
#endif // DEBUG_DIS_MODES
      break;
    case EZ_ADDR_1OP_VAR:
      op1=getOperand(print,loc+op_len,EZ_VAR);
#ifdef DEBUG_DIS_MODES
      DEBUG_OUT<<" ; 1op var";
#endif // DEBUG_DIS_MODES
      break;
    case EZ_ADDR_2OP_SMALL_SMALL:
      op1=getOperand(print,loc+op_len,EZ_SMALL);
      if(print) DEBUG_OUT<<",";
      op2=getOperand(print,loc+op_len,EZ_SMALL);
#ifdef DEBUG_DIS_MODES
      DEBUG_OUT<<" ; 2op small small";
#endif // DEBUG_DIS_MODES
      break;
    case EZ_ADDR_2OP_SMALL_VAR:
      op1=getOperand(print,loc+op_len,EZ_SMALL);
      if(print) DEBUG_OUT<<",";
      op2=getOperand(print,loc+op_len,EZ_VAR);
#ifdef DEBUG_DIS_MODES
      DEBUG_OUT<<" ; 2op small var";
#endif // DEBUG_DIS_MODES
      break;
    case EZ_ADDR_2OP_VAR_SMALL:
      op1=getOperand(print,loc+op_len,EZ_VAR);
      if(print) DEBUG_OUT<<",";
      op2=getOperand(print,loc+op_len,EZ_SMALL);
#ifdef DEBUG_DIS_MODES
      DEBUG_OUT<<" ; 2op var small";
#endif // DEBUG_DIS_MODES
      break;
    case EZ_ADDR_2OP_VAR_VAR:
      op1=getOperand(print,loc+op_len,EZ_VAR);
      if(print) DEBUG_OUT<<",";
      op2=getOperand(print,loc+op_len,EZ_VAR);
#ifdef DEBUG_DIS_MODES
      DEBUG_OUT<<" ; 2op var var";
#endif // DEBUG_DIS_MODES
      break;
    case EZ_ADDR_VAROP:
      types=map->read8(loc+1); op_len+=2;
      type[0]=(types>>6)&0x3;
      type[1]=(types>>4)&0x3;
      type[2]=(types>>2)&0x3;
      type[3]=types&0x3;
      for(t=0;t<4;t++) {
        if(t) getOperand(print,loc+op_len,type[t]);
        else op1=getOperand(print,loc+op_len,type[t]);
        if(print) { if(t!=3) DEBUG_OUT<<","; }
      }
#ifdef DEBUG_DIS_MODES
      DEBUG_OUT<<" ; varop";
#endif // DEBUG_DIS_MODES
      break;
    default:
      debugger("Unknown mode in printAddrMode!");
      break;
  }
  if(store==EZ_STORE) {
    DEBUG_OUT<<" -> ";
    unsigned int v=map->read8(loc+op_len);  op_len++;
    if(print) printVar(v);
  }
  if(branch==EZ_BRANCH) {
    unsigned int b=getBranch(loc+op_len);
    if(print) DEBUG_OUT<<" "<<b;
  }
  if(branch==EZ_CALL) {
    if(print) DEBUG_OUT<<"  ;"<<unpackAddress(op1);
  }
  return ret;
}


void EZMachine::handleHeader()
{
  if(!map) return;
  type=map->read8(0);
  highBase=map->read16b(4);
  initialPC=map->read16b(6);
  dictLoc=map->read16b(8);
  objLoc=map->read16b(0xa);
  globalLoc=map->read16b(0xc);
  staticBase=map->read16b(0xe);
  abvLoc=map->read16b(0x18);
  resetEntry=initialPC;
}


// loc points to branch byte or word
ULONG EZMachine::getBranch(ULONG loc)
{
  unsigned int b=map->read8(loc);  op_len++;
  // if bit 7 is set, then branch on true, else flase
  // if bit 6 is set, then offset is word, not byte
  // offset of 0 means return false from current routine, 1 means true
  if(b&0x40) { b=(b*256)+map->read8(loc+1);  op_len++; }
  return b;
}


void EZMachine::printVar(unsigned int var)
{
  if(var==0) { DEBUG_OUT<<"%sp"; return; }
  if(var<16) { DEBUG_OUT<<"%l["<<var<<"]"; return; }
  DEBUG_OUT<<"%g["<<(var-16)<<"]";
}


unsigned int EZMachine::getVar(unsigned int var)
{
  unsigned int val=0;
  if(var==0) {
    // top of stack
    return val;
  }
  if(var<16) {
    // local var
    return val;
  }
  // global var
  var-=16;
  return val;
}


void EZMachine::setVar(unsigned int var,unsigned int val)
{
  if(var==0) {
    // top of stack
  }
  if(var<16) {
    // local var
  }
  // global var
  var-=16;
}


// NOTE: loc points to operand
// NOTE: returns value if constant, or var number if not
unsigned int EZMachine::getOperand(bool print,ULONG loc,unsigned int type)
{
  unsigned int val=0;
  switch(type) {
    case EZ_NONE:
      if(print) DEBUG_OUT<<"n/a";
      break;
    case EZ_LARGE:
      val=map->read16b(loc); op_len+=2;
      if(print) DEBUG_OUT<<val;
      break;
    case EZ_SMALL:
      val=map->read8(loc); op_len+=1;
      if(print) DEBUG_OUT<<val;
      break;
    case EZ_VAR:
      val=map->read8(loc); op_len+=1;
      if(print) printVar(val);
      break;
    default:
      break;
  }
  return val;
}


// NOTE: loc points to operand
void EZMachine::setOperand(ULONG loc,unsigned int type,unsigned int val)
{
  unsigned int vnum=0;
  switch(type) {
    case EZ_LARGE:
      // Doesn't have any effect?
      vnum=map->read16b(loc); op_len+=2;
      break;
    case EZ_SMALL:
      // Doesn't have any effect?
      vnum=map->read8(loc); op_len+=1;
      break;
    case EZ_VAR:
      vnum=map->read8(loc); op_len+=1;
      setVar(vnum,val);
      break;
    default:
      break;
  }
}


void EZMachine::doCall(ULONG loc,unsigned int opcode,unsigned int op1)
{
  ULONG a=unpackAddress(op1);
  pushAddr(pc);
  unsigned int numLocals=map->read8(a);
  DEBUG_OUT<<"Jumping to "<<a+1<<" routine has "<<numLocals<<" locals\n";
  jump(a+1);
}


void EZMachine::exec_je(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented je opcode");
}


void EZMachine::exec_jl(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented jl opcode");
}


void EZMachine::exec_jg(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented jg opcode");
}


void EZMachine::exec_dec_chk(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented dec_chk opcode");
}


void EZMachine::exec_inc_chk(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented inc_chk opcode");
}


void EZMachine::exec_jin(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented jin opcode");
}


void EZMachine::exec_test(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented test opcode");
}


void EZMachine::exec_or(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented or opcode");
}


void EZMachine::exec_and(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented and opcode");
}


void EZMachine::exec_test_attr(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented test_attr opcode");
}


void EZMachine::exec_set_attr(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented set_attr opcode");
}


void EZMachine::exec_clear_attr(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented clear_attr opcode");
}


void EZMachine::exec_store(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented store opcode");
}


void EZMachine::exec_insert_obj(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented insert_obj opcode");
}


void EZMachine::exec_loadw(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented loadw opcode");
}


void EZMachine::exec_loadb(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented loadb opcode");
}


void EZMachine::exec_get_prop(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented get_prop opcode");
}


void EZMachine::exec_get_prop_addr(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented get_prop_addr opcode");
}


void EZMachine::exec_get_next_prop(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented get_next_prop opcode");
}


void EZMachine::exec_add(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented add opcode");
}


void EZMachine::exec_sub(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented sub opcode");
}


void EZMachine::exec_mul(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented mul opcode");
}


void EZMachine::exec_div(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented div opcode");
}


void EZMachine::exec_mod(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented mod opcode");
}


void EZMachine::exec_call_vs(ULONG loc,unsigned int opcode)
{
  unsigned int op1=0;
  // TODO: decode!
  doCall(loc,opcode,op1);
}


void EZMachine::exec_call_1n(ULONG loc,unsigned int opcode)
{
  unsigned int op1=0;
  // TODO: decode!
  doCall(loc,opcode,op1);
}


void EZMachine::exec_call_1s(ULONG loc,unsigned int opcode)
{
  unsigned int op1=0;
  // TODO: decode!
  doCall(loc,opcode,op1);
}


void EZMachine::exec_call_2s(ULONG loc,unsigned int opcode)
{
  unsigned int op1=0;
  // TODO: decode!
  doCall(loc,opcode,op1);
}


void EZMachine::exec_call_2n(ULONG loc,unsigned int opcode)
{
  unsigned int op1=0;
  // TODO: decode!
  doCall(loc,opcode,op1);
}


void EZMachine::exec_set_colour(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented set_colour opcode");
}


void EZMachine::exec_throw(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented throw opcode");
}


void EZMachine::exec_catch(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented catch opcode");
}


void EZMachine::exec_jz(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented jz opcode");
}


void EZMachine::exec_get_sibling(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented get_sibling opcode");
}


void EZMachine::exec_get_child(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented get_child opcode");
}


void EZMachine::exec_get_parent(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented get_parent opcode");
}


void EZMachine::exec_get_prop_len(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented get_prop_len opcode");
}


void EZMachine::exec_inc(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented inc opcode");
}


void EZMachine::exec_dec(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented dec opcode");
}


void EZMachine::exec_print_addr(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented print_addr opcode");
}



void EZMachine::exec_remove_obj(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented remove_obj opcode");
}


void EZMachine::exec_print_obj(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented print_obj opcode");
}


void EZMachine::exec_ret(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented ret opcode");
}


void EZMachine::exec_jump(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented jump opcode");
}


void EZMachine::exec_print_paddr(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented print_paddr opcode");
}


void EZMachine::exec_load(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented load opcode");
}


void EZMachine::exec_not(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented not opcode");
}


void EZMachine::exec_rtrue(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented rtrue opcode");
}


void EZMachine::exec_rfalse(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented rfalse opcode");
}


void EZMachine::exec_print(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented print opcode");
}


void EZMachine::exec_print_ret(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented print_ret opcode");
}


void EZMachine::exec_nop(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented nop opcode");
}


void EZMachine::exec_save(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented save opcode");
}


void EZMachine::exec_restore(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented restore opcode");
}


void EZMachine::exec_restart(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented restart opcode");
}


void EZMachine::exec_ret_popped(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented ret_popped opcode");
}


void EZMachine::exec_pop(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented pop opcode");
}


void EZMachine::exec_quit(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented quit opcode");
}


void EZMachine::exec_new_line(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented new_line opcode");
}


void EZMachine::exec_show_status(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented show_status opcode");
}


void EZMachine::exec_verify(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented verify opcode");
}


void EZMachine::exec_extended(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented extended opcode");
}


void EZMachine::exec_piracy(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented piracy opcode");
}


void EZMachine::exec_call(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented call opcode");
}


void EZMachine::exec_storew(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented storew opcode");
}


void EZMachine::exec_storeb(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented storeb opcode");
}


void EZMachine::exec_put_prop(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented put_prop opcode");
}


void EZMachine::exec_aread(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented aread opcode");
}


void EZMachine::exec_sread(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented sread opcode");
}


void EZMachine::exec_print_char(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented print_char opcode");
}


void EZMachine::exec_print_num(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented print_num opcode");
}


void EZMachine::exec_random(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented random opcode");
}


void EZMachine::exec_push(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented push opcode");
}


void EZMachine::exec_pull(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented pull opcode");
}


void EZMachine::exec_split_window(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented split_window opcode");
}


void EZMachine::exec_set_window(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented set_window opcode");
}


void EZMachine::exec_call_vs2(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented call_vs2 opcode");
}


void EZMachine::exec_erase_window(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented erase_window opcode");
}


void EZMachine::exec_erase_line(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented erase_line opcode");
}


void EZMachine::exec_set_cursor(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented set_cursor opcode");
}


void EZMachine::exec_get_cursor(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented get_cursor opcode");
}


void EZMachine::exec_set_text_style(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented set_text_style opcode");
}


void EZMachine::exec_buffer_mode(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented buffer_mode opcode");
}


void EZMachine::exec_output_stream(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented output_stream opcode");
}


void EZMachine::exec_input_stream(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented input_stream opcode");
}


void EZMachine::exec_sound_effect(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented sound_effect opcode");
}


void EZMachine::exec_read_char(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented read_char opcode");
}


void EZMachine::exec_scan_table(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented scan_table opcode");
}


void EZMachine::exec_call_vn(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented call_vn opcode");
}


void EZMachine::exec_call_vn2(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented call_vn2 opcode");
}


void EZMachine::exec_tokenise(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented tokenise opcode");
}


void EZMachine::exec_encode_text(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented encode_text opcode");
}


void EZMachine::exec_copy_table(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented copy_table opcode");
}


void EZMachine::exec_print_table(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented print_table opcode");
}


void EZMachine::exec_check_arg_count(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented check_arg_count opcode");
}

