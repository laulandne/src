
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#define DEBUG_DIS_MODES 1

#define DEBUG_EXEC 1

#define CHECK_SAFE 1

#define SHOW_INTS  1


#include <cpulib/Em6811.h>

#include <elib/EMemMapAbstract.h>
#include <elib/EModule.h>
#include <elib/EHandler.h>

#include <portable/nickcpp/ANullStream.h>

#include <portable/nickcpp/AStringHelper.h>


#include <cpulib/generated/Em6811_tab.h>
#include <cpulib/generated/Em6811x18_tab.h>
#include <cpulib/generated/Em6811x1a_tab.h>
#include <cpulib/generated/Em6811xcd_tab.h>


#define E_CPU_NONE     0x00
#define E_CPU_6800     0x01
#define E_CPU_6809     0x02
#define E_CPU_6811     0x03


#define E6800_REG_D  0x0
#define E6800_REG_X  0x1
#define E6800_REG_Y  0x2
#define E6800_REG_U  0x3
#define E6800_REG_S  0x4
#define E6800_REG_PC 0x5
#define E6800_REG_A  0x8
#define E6800_REG_B  0x9
#define E6800_REG_CC 0xa
#define E6800_REG_DP 0xb


////////////////////////////////////////////////////////////////////////////////
//  Em6811 Class
////////////////////////////////////////////////////////////////////////////////

Em6811::Em6811()
{
  DEBUG_OUT<<"Em6811 created...\n";
  init();
  type=E_CPU_6811;
  //w=ww;
  reset();
}


Em6811::~Em6811()
{
  DEBUG_OUT<<"Em6811 destroyed.\n";
}


void Em6811::init()
{
  ECPU8Bit::init();
  // Generic features
  doTrace=false;  doRun=false;
  dp=0;
  // Set up stack and registers
  sbits=(Em6800StatusBits *)&p;
  int_pending=0;
  intLock=false;
  lsb=false;
  stackBase=0;  stackMask=0xffff;
}


/* STATIC */
bool Em6811::recognize(const char *tname)
{
  const char *name=aDefaultStringHelper.removeSpaces2(aDefaultStringHelper.toLower(tname));
  if(!strcmp(name,"6811")) return true; else
  if(!strcmp(name,"68hc11")) return true; else
  if(!strcmp(name,"mc6811")) return true; else
  if(!strcmp(name,"mc68hc11")) return true; else
  if(!strcmp(name,"6812")) return true; else
  if(!strcmp(name,"68hc12")) return true; else
  if(!strcmp(name,"mc6812")) return true; else
  if(!strcmp(name,"mc68hc12")) return true; else
  return false;
}


const char *Em6811::getName()
{
  return "Motorola 6811";
}


void Em6811::wregs()
{
  DEBUG_OUT<<"PC="<<(unsigned short)pc<<"\n";
  DEBUG_OUT<<"A="<<(short)a<<"  B="<<(short)b<<"  X="<<(short)x<<"  Y="<<(short)y<<"\n";
  DEBUG_OUT<<"S="<<sp<<"  U="<<u<<"  DP="<<dp;
  DEBUG_OUT<<"  P="<<(short)p<<"  (";
  if(p&E6800_STAT_E) DEBUG_OUT<<"E";  else DEBUG_OUT<<"e";
  if(p&E6800_STAT_F) DEBUG_OUT<<"F";  else DEBUG_OUT<<"f";
  if(p&E6800_STAT_H) DEBUG_OUT<<"H";  else DEBUG_OUT<<"h";
  if(p&E6800_STAT_I) DEBUG_OUT<<"I";  else DEBUG_OUT<<"i";
  if(p&E6800_STAT_N) DEBUG_OUT<<"N";  else DEBUG_OUT<<"n";
  if(p&E6800_STAT_Z) DEBUG_OUT<<"Z";  else DEBUG_OUT<<"z";
  if(p&E6800_STAT_V) DEBUG_OUT<<"V";  else DEBUG_OUT<<"v";
  if(p&E6800_STAT_C) DEBUG_OUT<<"C";  else DEBUG_OUT<<"c";
  DEBUG_OUT<<")\n";
}


// TODO: Check and handle any requested ints between instructions
void Em6811::execute()
{
  if(pc==breakpoint) { debugger("Breakpoint!\n"); return; }
  opcode=map->read8(pc);
  op_len=Em6811_size[opcode];
  cycle=Em6811_time[opcode];
  unsigned int topcode=opcode,loc=pc;
#include <cpulib/generated/Em6811_exec.h>
  pc+=op_len;
  cycles+=cycle;
  if(int_pending) handle_int(int_pending);
}


ULONG Em6811::disasm(ULONG loc)
{
  BYTE topcode;
  unsigned int len=0,mode=E6800_NONE;
  const char *name="???";
  topcode=map->read8(loc);
  lastSymbol=map->findAnyExactSymbol(loc);
  if(lastSymbol) DEBUG_OUT<<lastSymbol->getName()<<":\n";
  DEBUG_OUT<<"";
  DEBUG_OUT<<"."; DEBUG_OUT<<loc;  DEBUG_OUT<<"  ";
  len=Em6811_size[topcode];
  mode=Em6811_format[topcode];
  name=Em6811_name[topcode];
  if(!len) len=1;  // NOTE: An unimplemented opcode...
  unsigned int t;
  if(mode==E6800_SPECIAL) {
    loc++;
    switch(topcode) {
      case 0x18:
        // prefix
        topcode=map->read8(loc);
        len=Em6811x18_size[topcode];
        mode=Em6811x18_format[topcode];
        name=Em6811x18_name[topcode];
        break;
      case 0x1a:
        // prefix
        topcode=map->read8(loc);
        len=Em6811x1a_size[topcode];
        mode=Em6811x1a_format[topcode];
        name=Em6811x1a_name[topcode];
        break;
      case 0xcd:
        // prefix
        topcode=map->read8(loc);
        len=Em6811xcd_size[topcode];
        mode=Em6811xcd_format[topcode];
        name=Em6811xcd_name[topcode];
        break;
      default:
        debugger("Bad E6800_SPECIAL!");
        break;
    }
    for(t=0;t<len;t++) DEBUG_OUT<<(short)map->read8(loc+t-1)<<" ";
    for(t=0;t<(4-len);t++) DEBUG_OUT<<"   ";
  }
  else {
    for(t=0;t<len;t++) DEBUG_OUT<<(short)map->read8(loc+t)<<" ";
    for(t=0;t<(3-len);t++) DEBUG_OUT<<"   ";
  }
  DEBUG_OUT<<name<<" ";
  //DEBUG_OUT<<"Len was "<<len<<"\n";
  len+=printAddrMode(loc,topcode,mode);
  //DEBUG_OUT<<"Len now is "<<len<<"\n";
  DEBUG_OUT<<"";
  DEBUG_OUT<<"\n";
  return len;}


void Em6811::exec_special(ULONG loc,unsigned int op)
{
  loc++;
  unsigned int topcode=map->read8(loc);
  opcode=topcode;
  switch(op) {
    case 0x18: // prefix
#include <cpulib/generated/Em6811x18_exec.h>
      op_len=Em6811x18_size[opcode];
      cycle=Em6811x18_time[opcode];
      break;
    case 0x1a: // prefix
#include <cpulib/generated/Em6811x1a_exec.h>
      op_len=Em6811x1a_size[opcode];
      cycle=Em6811x1a_time[opcode];
      break;
    case 0xcd: // prefix
#include <cpulib/generated/Em6811xcd_exec.h>
      op_len=Em6811xcd_size[opcode];
      cycle=Em6811xcd_time[opcode];
      break;
    default:
      debugger("Unimplemented special opcode!");
      break;
  }
#ifdef DEBUG_EXEC
  DEBUG_OUT<<"exec_special op="<<op<<" topcode="<<topcode<<"\n";
#endif
}


void Em6811::exec_aba(ULONG loc,unsigned int op)
{
  debugger("Unimplemented aba opcode!");
}


void Em6811::exec_asrd(ULONG loc,unsigned int op)
{
  debugger("Unimplemented asrd opcode!");
}


void Em6811::exec_brset(ULONG loc,unsigned int op)
{
  debugger("Unimplemented brset opcode!");
}


void Em6811::exec_brclr(ULONG loc,unsigned int op)
{
  debugger("Unimplemented brclr opcode!");
}


void Em6811::exec_bset(ULONG loc,unsigned int op)
{
  debugger("Unimplemented bset opcode!");
}


void Em6811::exec_bclr(ULONG loc,unsigned int op)
{
  debugger("Unimplemented bclr opcode!");
}


void Em6811::exec_fdiv(ULONG loc,unsigned int op)
{
  debugger("Unimplemented fdiv opcode!");
}


void Em6811::exec_idiv(ULONG loc,unsigned int op)
{
  debugger("Unimplemented idiv opcode!");
}


void Em6811::exec_lsrd(ULONG loc,unsigned int op)
{
  debugger("Unimplemented lsrd opcode!");
}


void Em6811::exec_stop(ULONG loc,unsigned int op)
{
  debugger("Unimplemented stop opcode!");
}


void Em6811::exec_tab(ULONG loc,unsigned int op)
{
  debugger("Unimplemented tab opcode!");
}


void Em6811::exec_test(ULONG loc,unsigned int op)
{
  debugger("Unimplemented test opcode!");
}


void Em6811::exec_xgdx(ULONG loc,unsigned int op)
{
  debugger("Unimplemented xgdx opcode!");
}


