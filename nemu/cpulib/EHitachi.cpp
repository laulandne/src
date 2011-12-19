
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#define DEBUG_DIS_MODES 1

#define DEBUG_EXEC 1

#define CHECK_SAFE 1

#define SHOW_INTS  1


#include <cpulib/EHitachi.h>

#include <elib/EMemMapAbstract.h>
#include <elib/EModule.h>

#include <portable/nickcpp/ANullStream.h>

#include <portable/nickcpp/AStringHelper.h>


#include <cpulib/generated/EHitachi_tab.h>


// The different cpus...
#define E_CPU_NONE     0x00
#define E_CPU_HITACHI  0x01


////////////////////////////////////////////////////////////////////////////////
//  EHitachi Class
////////////////////////////////////////////////////////////////////////////////

EHitachi::EHitachi()
{
  DEBUG_OUT<<"EHitachi created...\n";
  init();
  type=E_CPU_HITACHI;
  //w=ww;
  reset();
}


EHitachi::~EHitachi()
{
  DEBUG_OUT<<"EHitachi destroyed.\n";
}


void EHitachi::init()
{
  ECPU8Bit::init();
  // Generic features
  doTrace=false;  doRun=false;
  // Set up stack and registers
  int_pending=0;
  intLock=false;
  lsb=true;
  for(unsigned int t=0;t<8;t++) { R[t]=0; }
  sbits=(EHitachiStatusBits *)&ccr;
  setStack(ECPUABSTRACT_DEFAULT_STACK>>16);
  setFramePtr(ECPUABSTRACT_DEFAULT_STACK>>16);
  setResetStack(ECPUABSTRACT_DEFAULT_STACK>>16);
  setResetEntry(ECPUABSTRACT_DEFAULT_CODE>>16);
}


/* STATIC */
bool EHitachi::recognize(const char *tname)
{
  const char *name=aDefaultStringHelper.removeSpaces2(aDefaultStringHelper.toLower(tname));
  if(!strcmp(name,"hitachi")) return true; else
  if(!strcmp(name,"h8")) return true; else
  if(!strcmp(name,"h8300")) return true; else
  if(!strcmp(name,"h8_300")) return true; else
  if(!strcmp(name,"h8_300h")) return true; else
  if(!strcmp(name,"h8s")) return true; else
  if(!strcmp(name,"h8_500")) return true; else
  return false;
}


const char *EHitachi::getName()
{
  return "Hitachi H8";
}


void EHitachi::setKind(const char *tname)
{
  const char *name=aDefaultStringHelper.removeSpaces2(aDefaultStringHelper.toLower(tname));
  type=E_CPU_HITACHI;
}


void EHitachi::reset()
{
#ifdef DEBUG_EXEC
  setTrace(true);
#endif
  breakpoint=0xffff;  // since we start at 0, breakpoint's default
  intLock=false;
  for(unsigned int t=0;t<8;t++) { R[t]=0; }
  sp=0;  // NOTE: R7 is sp!
  pc=0;  // Jump to zero
  ccr=0;
  clearStackTrace();
  ECPUAbstract::reset();
}


bool EHitachi::setRegister(const char *tname,ULONG value)
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


ULONG EHitachi::readRegister(const char *name)
{
  if(!strcmp(name,"sp")) { return sp; }
  return 0;
}


void EHitachi::setStack(ULONG location)
{
  sp=location;
  R[7]=sp;
}


ULONG EHitachi::readStack()
{
  sp=R[7];
  return sp;
}


void EHitachi::wregs()
{
  DEBUG_OUT<<"PC="<<pc<<"\n";
  DEBUG_OUT<<"SP="<<sp<<"\n";
  for(unsigned int t=0;t<8;t++) {
    DEBUG_OUT<<"R"<<t<<"="<<R[t]<<" ";
  }
  DEBUG_OUT<<"\n";
}


void EHitachi::handle_int(ULONG value)
{
}


void EHitachi::cause_int(ULONG value)
{
  int_pending=0;
  switch(value) {
    default:
      debugger("Illegal interrupt requested!\n");
      break;
  }
}


void EHitachi::ints()
{
}


void EHitachi::execute()
{
  R[7]=sp;  // to keep them in sync...
  if(pc==breakpoint) { debugger("Breakpoint!\n"); return; }
  opcode=map->read16l(pc); // always little endian?
  topcode=(opcode>>8)&0xff;
  op_len=EHitachi_size[topcode];
  cycle=EHitachi_time[topcode];
  ULONG loc=pc;
#include <cpulib/generated/EHitachi_exec.h>
  pc+=op_len;
  sp=R[7];  // to keep them in sync...
  cycles+=cycle;
  if(int_pending) handle_int(int_pending);
}


ULONG EHitachi::disasm(ULONG loc)
{
  unsigned int len=0,mode=EHITACHI_NONE;
  const char *name="???";
  opcode=map->read16l(pc); // always little endian?
  unsigned int topcode=(opcode>>8)&0xff;
  lastSymbol=map->findAnyExactSymbol(loc);
  if(lastSymbol) DEBUG_OUT<<lastSymbol->getName()<<":\n";
  DEBUG_OUT<<"";
  DEBUG_OUT<<"."; DEBUG_OUT<<loc;  DEBUG_OUT<<"  ";
  len=EHitachi_size[topcode];
  mode=EHitachi_format[topcode];
  name=EHitachi_name[topcode];
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


unsigned int EHitachi::printAddrMode(ULONG loc,unsigned int topcode,unsigned int mode)
{
  return 0;
}

