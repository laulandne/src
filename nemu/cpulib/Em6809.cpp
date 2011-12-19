
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#define DEBUG_DIS_MODES 1

#define DEBUG_EXEC 1

#define CHECK_SAFE 1

#define SHOW_INTS  1


#include <cpulib/Em6809.h>

#include <elib/EMemMapAbstract.h>
#include <elib/EModule.h>
#include <elib/EHandler.h>

#include <portable/nickcpp/ANullStream.h>

#include <portable/nickcpp/AStringHelper.h>


#include <cpulib/generated/Em6809_tab.h>
#include <cpulib/generated/Em6809x10_tab.h>
#include <cpulib/generated/Em6809x11_tab.h>


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
//  Em6809 Class
////////////////////////////////////////////////////////////////////////////////

Em6809::Em6809()
{
  DEBUG_OUT<<"e6809 created...\n";
  init();
  type=E_CPU_6809;
  //w=ww;
  reset();
}


Em6809::~Em6809()
{
  DEBUG_OUT<<"e6809 destroyed.\n";
}


void Em6809::init()
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
bool Em6809::recognize(const char *tname)
{
  const char *name=aDefaultStringHelper.removeSpaces2(aDefaultStringHelper.toLower(tname));
  if(!strcmp(name,"6809")) return true; else
  if(!strcmp(name,"6809e")) return true; else
  if(!strcmp(name,"6809s")) return true; else
  if(!strcmp(name,"m6809")) return true; else
  if(!strcmp(name,"m6809e")) return true; else
  if(!strcmp(name,"m6809s")) return true; else
  if(!strcmp(name,"mc6809")) return true; else
  if(!strcmp(name,"mc6809e")) return true; else
  if(!strcmp(name,"mc6809s")) return true; else
  if(!strcmp(name,"6309")) return true; else
  return false;
}


const char *Em6809::getName()
{
  return "Motorola 6809";
}


void Em6809::wregs()
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
void Em6809::execute()
{
  if(pc==breakpoint) { debugger("Breakpoint!\n"); return; }
  opcode=map->read8(pc);
  op_len=Em6809_size[opcode];
  cycle=Em6809_time[opcode];
  unsigned int topcode=opcode,loc=pc;
#include <cpulib/generated/Em6809_exec.h>
  pc+=op_len;
  cycles+=cycle;
  if(int_pending) handle_int(int_pending);
}


ULONG Em6809::disasm(ULONG loc)
{
  BYTE topcode;
  unsigned int len=0,mode=E6800_NONE;
  const char *name="???";
  topcode=map->read8(loc);
  lastSymbol=map->findAnyExactSymbol(loc);
  if(lastSymbol) DEBUG_OUT<<lastSymbol->getName()<<":\n";
  DEBUG_OUT<<"";
  DEBUG_OUT<<"."; DEBUG_OUT<<loc;  DEBUG_OUT<<"  ";
  len=Em6809_size[topcode];
  mode=Em6809_format[topcode];
  name=Em6809_name[topcode];
  if(!len) len=1;  // NOTE: An unimplemented opcode...
  unsigned int t;
  if(mode==E6800_SPECIAL) {
    loc++;
    switch(topcode) {
      case 0x10:
        // prefix
        topcode=map->read8(loc);
        len=Em6809x10_size[topcode];
        mode=Em6809x10_format[topcode];
        name=Em6809x10_name[topcode];
        break;
      case 0x11:
        // prefix
        topcode=map->read8(loc);
        len=Em6809x11_size[topcode];
        mode=Em6809x11_format[topcode];
        name=Em6809x11_name[topcode];
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
  return len;
}


void Em6809::exec_special(ULONG loc,unsigned int op)
{
  loc++;
  unsigned int topcode=map->read8(loc);
  opcode=topcode;
  switch(op) {
    case 0x10: // prefix
#include <cpulib/generated/Em6809x10_exec.h>
      op_len=Em6809x10_size[opcode];
      cycle=Em6809x10_time[opcode];
      break;
    case 0x11: // prefix
#include <cpulib/generated/Em6809x11_exec.h>
      op_len=Em6809x11_size[opcode];
      cycle=Em6809x11_time[opcode];
      break;
    default:
      debugger("Unimplemented special opcode!");
      break;
  }
#ifdef DEBUG_EXEC
  DEBUG_OUT<<"exec_special op="<<op<<" topcode="<<topcode<<"\n";
#endif
}


void Em6809::exec_andcc(ULONG loc,unsigned int op)
{
  debugger("Unimplemented andcc opcode!");
}


void Em6809::exec_cmpd(ULONG loc,unsigned int op)
{
  debugger("Unimplemented cmpd opcode!");
}


void Em6809::exec_cmpu(ULONG loc,unsigned int op)
{
  debugger("Unimplemented cmpu opcode!");
}


void Em6809::exec_cmps(ULONG loc,unsigned int op)
{
  debugger("Unimplemented cmps opcode!");
}


void Em6809::exec_lbcc(ULONG loc,unsigned int op)
{
  debugger("Unimplemented lbcc opcode!");
}


void Em6809::exec_lbcs(ULONG loc,unsigned int op)
{
  debugger("Unimplemented lbcs opcode!");
}


void Em6809::exec_lbeq(ULONG loc,unsigned int op)
{
  debugger("Unimplemented lbeq opcode!");
}


void Em6809::exec_lbge(ULONG loc,unsigned int op)
{
  debugger("Unimplemented lbge opcode!");
}


void Em6809::exec_lbgt(ULONG loc,unsigned int op)
{
  debugger("Unimplemented lbgt opcode!");
}


void Em6809::exec_lbhi(ULONG loc,unsigned int op)
{
  debugger("Unimplemented lbhi opcode!");
}


void Em6809::exec_lble(ULONG loc,unsigned int op)
{
  debugger("Unimplemented lble opcode!");
}


void Em6809::exec_lbls(ULONG loc,unsigned int op)
{
  debugger("Unimplemented lbls opcode!");
}


void Em6809::exec_lblt(ULONG loc,unsigned int op)
{
  debugger("Unimplemented lblt opcode!");
}


void Em6809::exec_lbmi(ULONG loc,unsigned int op)
{
  debugger("Unimplemented lbmi opcode!");
}


void Em6809::exec_lbne(ULONG loc,unsigned int op)
{
  debugger("Unimplemented lbne opcode!");
}


void Em6809::exec_lbpl(ULONG loc,unsigned int op)
{
  debugger("Unimplemented lbpl opcode!");
}


void Em6809::exec_lbvc(ULONG loc,unsigned int op)
{
  debugger("Unimplemented lbvc opcode!");
}


void Em6809::exec_lbvs(ULONG loc,unsigned int op)
{
  debugger("Unimplemented lbvs opcode!");
}


void Em6809::exec_lbra(ULONG loc,unsigned int op)
{
  debugger("Unimplemented lbra opcode!");
}


void Em6809::exec_lbrn(ULONG loc,unsigned int op)
{
  debugger("Unimplemented lbrn opcode!");
}


void Em6809::exec_lbsr(ULONG loc,unsigned int op)
{
  debugger("Unimplemented lbsr opcode!");
}


void Em6809::exec_ldu(ULONG loc,unsigned int op)
{
  unsigned int val=0,ea=0;
  switch(op) {
    default:
      DEBUG_OUT<<"loc="<<loc<<" op="<<op<<"\n";
      debugger("Unimplemented ldu opcode!");
      break;
  }
  u=val;
  set_flags_nzv(val);
#ifdef DEBUG_EXEC
  DEBUG_OUT<<"LDU with "<<val<<" ea="<<ea<<"\n";
#endif
}


void Em6809::exec_leas(ULONG loc,unsigned int op)
{
  unsigned int ea=calcEAPostByte(loc+1);
#ifdef DEBUG_EXEC
  DEBUG_OUT<<"LEAS sp was "<<sp;
#endif
  sp=ea;
#ifdef DEBUG_EXEC
  DEBUG_OUT<<" now is "<<sp<<"\n";
#endif

}


void Em6809::exec_leau(ULONG loc,unsigned int op)
{
  unsigned int ea=calcEAPostByte(loc+1);
#ifdef DEBUG_EXEC
  DEBUG_OUT<<"LEAU u was "<<u;
#endif
  u=ea;
#ifdef DEBUG_EXEC
  DEBUG_OUT<<" now is "<<u<<"\n";
#endif
}


void Em6809::exec_leax(ULONG loc,unsigned int op)
{
  unsigned int ea=calcEAPostByte(loc+1);
#ifdef DEBUG_EXEC
  DEBUG_OUT<<"LEAX x was "<<x;
#endif
  x=ea;
#ifdef DEBUG_EXEC
  DEBUG_OUT<<" now is "<<x<<"\n";
#endif
}


void Em6809::exec_leay(ULONG loc,unsigned int op)
{
  unsigned int ea=calcEAPostByte(loc+1);
#ifdef DEBUG_EXEC
  DEBUG_OUT<<"LEAY y was "<<y;
#endif
  y=ea;
#ifdef DEBUG_EXEC
  DEBUG_OUT<<" now is "<<y<<"\n";
#endif
}


void Em6809::exec_orcc(ULONG loc,unsigned int op)
{
  debugger("Unimplemented orcc opcode!");
}


void Em6809::exec_pulu(ULONG loc,unsigned int op)
{
  unsigned int rs=map->read8(loc+1);
  if(rs&0x01) p=stack_pop6800(u);
  if(rs&0x02) a=stack_pop6800(u);
  if(rs&0x04) b=stack_pop6800(u);
  if(rs&0x08) dp=stack_pop6800(u)<<8;
  if(rs&0x10) x=stack_popw6800(u);
  if(rs&0x20) y=stack_popw6800(u);
  if(rs&0x40) sp=stack_popw6800(u);
  if(rs&0x80) pc=stack_popw6800(u);
}


void Em6809::exec_pshu(ULONG loc,unsigned int op)
{
  unsigned int rs=map->read8(loc+1);
  if(rs&0x80) stack_pushw6800(u,pc);
  if(rs&0x40) stack_pushw6800(u,sp);
  if(rs&0x20) stack_pushw6800(u,y);
  if(rs&0x10) stack_pushw6800(u,x);
  if(rs&0x08) stack_push6800(u,dp>>8);
  if(rs&0x04) stack_push6800(u,b);
  if(rs&0x02) stack_push6800(u,a);
  if(rs&0x01) stack_push6800(u,p);
}


void Em6809::exec_sex(ULONG loc,unsigned int op)
{
  debugger("Unimplemented sex opcode!");
}


void Em6809::exec_stu(ULONG loc,unsigned int op)
{
  unsigned int val=u,ea=0;
  switch(op) {
    default:
      DEBUG_OUT<<"loc="<<loc<<" op="<<op<<"\n";
      debugger("Unimplemented stu opcode!");
      break;
  }
  map->write16b(ea,val);
  set_flags_nzv(val);
#ifdef DEBUG_EXEC
  DEBUG_OUT<<"STU "<<val<<" at "<<ea<<"\n";
#endif
}


void Em6809::exec_swi1(ULONG loc,unsigned int op)
{
  debugger("Unimplemented swi1 opcode!");
}


void Em6809::exec_swi2(ULONG loc,unsigned int op)
{
  debugger("Unimplemented swi2 opcode!");
}

