
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


//#define DEBUG_DIS_MODES 1

#define DEBUG_EXEC 1

#define CHECK_SAFE 1

#define SHOW_INTS  1


#include <cpulib/EGlulx.h>

#include <elib/EMemMapAbstract.h>
#include <elib/EModule.h>

#include <portable/nickcpp/ANullStream.h>

#include <portable/nickcpp/AStringHelper.h>


#include <cpulib/generated/EGlulx_tab.h>


////////////////////////////////////////////////////////////////////////////////
//  EGLULX Class
////////////////////////////////////////////////////////////////////////////////

/*
EGLULX::EGLULX()
{
  DEBUG_OUT<<"EGLULX created...\n";
  init();
  type=E_CPU_GLULX;
  //w=ww;
  reset();
}


EGLULX::~EGLULX()
{
  DEBUG_OUT<<"EGLULX destroyed.\n";
}


void EGLULX::init()
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
}


bool EGLULX::recognize(char *tname)
{
  char *name=aDefaultStringHelper.removeSpaces2(aDefaultStringHelper.toLower(tname));
  if(!strcmp(name,"glulx")) return true; else
  return false;
}


char *EGLULX::getName()
{
  return "glulx");
}


void EGLULX::setKind(char *tname)
{
  char *name=aDefaultStringHelper.removeSpaces2(aDefaultStringHelper.toLower(tname));
  type=E_CPU_GLULX;
}


void EGLULX::reset()
{
#ifdef DEBUG_EXEC
  setTrace(true);
#endif
  breakpoint=0xffffffff;  // since we start at 0, breakpoint's default
  intLock=false;
  pc=0;  // Jump to zero
  clearStackTrace();
  if(resetEntry) jump(resetEntry);
}


bool EGLULX::setRegister(char *tname,ULONG value)
{
  char *name=aDefaultStringHelper.removeSpaces2(aDefaultStringHelper.toLower(tname));
  bool ret=false;
  if(!strcmp(name,"pc")))
  { pc=value; ret=true; } else
  if(!strcmp(name,"sp")))
  { sp=value; ret=true; }
  //free(name);
  return ret;
}


ULONG EGLULX::readRegister(char *name)
{
  if(!strcmp(name,"sp"))) { return sp; }
  return 0;
}


void EGLULX::wregs()
{
  DEBUG_OUT<<"PC="<<pc<<"\n";
  DEBUG_OUT<<"SP="<<sp<<"\n";
}


void EGLULX::handle_int(ULONG value)
{
}


void EGLULX::cause_int(ULONG value)
{
  int_pending=0;
  switch(value) {
    default:
      debugger("Illegal interrupt requested!\n");
      break;
  }
}


void EGLULX::ints()
{
  //if(w) {
  //}
}


void EGLULX::execute()
{
  if(pc==breakpoint) { debugger("Breakpoint!\n"); return; }
  opcode=map->read8(pc);
  op_len=EGLULX_size[opcode];
  cycle=EGLULX_time[opcode];
  unsigned int topcode=opcode,loc=pc;
#include <cpulib/generated/EGLULX_exec.h>
  pc+=op_len;
  cycles+=cycle;
  if(int_pending) handle_int(int_pending);
}


ULONG EGLULX::disasm(ULONG loc)
{
  BYTE topcode;
  unsigned int len=0,mode=EZ_ADDR_NONE;
  char *name="???";
  topcode=map->read8(loc);
  lastSymbol=map->findAnyExactSymbol(loc);
  if(lastSymbol) DEBUG_OUT<<lastSymbol->getName()<<":\n";
  DEBUG_OUT<<"."; DEBUG_OUT<<loc;  DEBUG_OUT<<"  ";
  len=EGLULX_size[topcode];
  mode=EGLULX_format[topcode];
  name=EGLULX_name[topcode];
  if(!len) len=1;  // NOTE: An unimplemented opcode...
  unsigned int t;
  for(t=0;t<len;t++) DEBUG_OUT<<map->read8(loc+t)<<" ";
  for(t=0;t<(3-len);t++) DEBUG_OUT<<"   ";
  DEBUG_OUT<<name<<" ";
  //DEBUG_OUT<<"Len was "<<len<<"\n";
  len+=printAddrMode(loc,topcode,mode);
  //DEBUG_OUT<<"Len now is "<<len<<"\n";
  DEBUG_OUT<<"\n";
  return len;
}


unsigned int EGLULX::printAddrMode(ULONG loc,unsigned int topcode,unsigned int mode)
{
  return 0;
}


void EGLULX::exec_nop(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented nop opcode");
}


void EGLULX::exec_add(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented add opcode");
}


void EGLULX::exec_sub(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented sub opcode");
}


void EGLULX::exec_mul(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented mul opcode");
}


void EGLULX::exec_div(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented div opcode");
}


void EGLULX::exec_mod(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented mod opcode");
}


void EGLULX::exec_neg(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented neg opcode");
}

void EGLULX::exec_bitand(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented bitand opcode");
}


void EGLULX::exec_bitor(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented bitor opcode");
}


void EGLULX::exec_bitxor(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented bitxor opcode");
}


void EGLULX::exec_bitnot(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented bitnot opcode");
}


void EGLULX::exec_shiftl(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented shiftl opcode");
}


void EGLULX::exec_sshiftr(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented sshiftr opcode");
}


void EGLULX::exec_ushiftr(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented ushiftr opcode");
}


void EGLULX::exec_jump(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented jump opcode");
}


void EGLULX::exec_jz(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented jz opcode");
}


void EGLULX::exec_jnz(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented jnz opcode");
}


void EGLULX::exec_jeq(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented jeq opcode");
}


void EGLULX::exec_jne(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented jne opcode");
}


void EGLULX::exec_jlt(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented jlt opcode");
}


void EGLULX::exec_jge(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented jge opcode");
}


void EGLULX::exec_jgt(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented jgt opcode");
}


void EGLULX::exec_jle(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented jle opcode");
}


void EGLULX::exec_jltu(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented jltu opcode");
}


void EGLULX::exec_jgeu(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented jgeu opcode");
}


void EGLULX::exec_jgtu(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented jgtu opcode");
}


void EGLULX::exec_jleu(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented jleu opcode");
}



void EGLULX::exec_call(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented call opcode");
}


void EGLULX::exec_return(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented return opcode");
}


void EGLULX::exec_catch(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented catch opcode");
}


void EGLULX::exec_throw(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented throw opcode");
}


void EGLULX::exec_tailcall(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented tailcall opcode");
}


void EGLULX::exec_copy(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented copy opcode");
}


void EGLULX::exec_copys(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented copys opcode");
}


void EGLULX::exec_copyb(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented copyb opcode");
}


void EGLULX::exec_sexs(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented sexs opcode");
}


void EGLULX::exec_sexb(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented sexb opcode");
}



void EGLULX::exec_aload(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented aload opcode");
}


void EGLULX::exec_aloads(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented aloads opcode");
}


void EGLULX::exec_aloadb(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented aloadb opcode");
}


void EGLULX::exec_aloadbit(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented aloadbit opcode");
}


void EGLULX::exec_astore(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented astore opcode");
}


void EGLULX::exec_astores(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented astores opcode");
}


void EGLULX::exec_astoreb(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented astoreb opcode");
}


void EGLULX::exec_astorebit(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented astorebit opcode");
}


void EGLULX::exec_stkcount(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented stkcount opcode");
}


void EGLULX::exec_stkpeek(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented stkpeek opcode");
}


void EGLULX::exec_stkswap(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented stkswap opcode");
}


void EGLULX::exec_stkroll(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented stkroll opcode");
}


void EGLULX::exec_stkcopy(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented stkcopy opcode");
}



void EGLULX::exec_streamchar(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented streamchar opcode");
}


void EGLULX::exec_streamnum(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented streamnum opcode");
}


void EGLULX::exec_streamstr(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented streamstr opcode");
}


void EGLULX::exec_gestalt(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented gestalt opcode");
}


void EGLULX::exec_debugtrap(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented debugtrap opcode");
}


void EGLULX::exec_getmemsize(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented getmemsize opcode");
}


void EGLULX::exec_setmemsize(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented setmemsize opcode");
}


void EGLULX::exec_jumpabs(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented jumpabs opcode");
}


void EGLULX::exec_random(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented random opcode");
}


void EGLULX::exec_setrandom(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented setrandom opcode");
}


void EGLULX::exec_quit(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented quit opcode");
}


void EGLULX::exec_verify(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented verify opcode");
}


void EGLULX::exec_restart(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented restart opcode");
}


void EGLULX::exec_save(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented save opcode");
}


void EGLULX::exec_restore(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented restore opcode");
}


void EGLULX::exec_saveundo(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented saveundo opcode");
}


void EGLULX::exec_restoreundo(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented restoreundo opcode");
}


void EGLULX::exec_protect(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented protect opcode");
}

void EGLULX::exec_glk(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented glk opcode");
}


void EGLULX::exec_getstringtbl(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented getstringtbl opcode");
}


void EGLULX::exec_setstringtbl(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented setstringtbl opcode");
}


void EGLULX::exec_getiosys(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented getiosys opcode");
}


void EGLULX::exec_setiosys(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented setiosys opcode");
}


void EGLULX::exec_linearsearch(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented linearsearch opcode");
}


void EGLULX::exec_binarysearch(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented binarysearch opcode");
}


void EGLULX::exec_linkedsearch(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented linkedsearch opcode");
}

void EGLULX::exec_callf(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented callf opcode");
}


void EGLULX::exec_callfi(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented callfi opcode");
}


void EGLULX::exec_callfii(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented callfii opcode");
}


void EGLULX::exec_callfiii(ULONG loc,unsigned int opcode)
{
  debugger("Unimplemented callfiii opcode");
}

*/

