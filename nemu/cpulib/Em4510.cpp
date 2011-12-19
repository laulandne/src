
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


/*

This emulates the "Victor" 4510 chip in the never-released Commodore 65.

I used the preliminary developers manual that came with the machine, and
where things were unclear I made wild guesses.

NOTE: This code doesn't work if an int is 16-bits...

*/


//#define SHOW_INTS  1
#define CHECK_STACK 1


#include <cpulib/Em4510.h>

#include <elib/EMemMapAbstract.h>
#include <elib/EModule.h>
#include <elib/EHandler.h>

#include <portable/nickcpp/ANullStream.h>

#include <portable/nickcpp/AStringHelper.h>


#include <cpulib/generated/Em4510_tab.h>


#define E_CPU_NONE     0x00
#define E_CPU_65x4510  0x03


#define MSIZEMASK ECPUABSTRACT_MASK_BYTE
#define XSIZEMASK ECPUABSTRACT_MASK_BYTE
#define M8BIT true
#define X8BIT true
#define MHIGHBIT 0x80
#define XHIGHBIT 0x80
#define MHALFBIT 0x40
#define XHALFBIT 0x40

#define READ_DATA(q) (map->read8(translate[q&0xffff]))
#define READW_DATA(q) (map->read16l(translate[q&0xffff]))
#define WRITE_DATA(q,v) (map->write8(translate[q&0xffff],v))
#define WRITEW_DATA(q,v) (map->write16l(translate[q&0xffff],v))
#define READ_CODE(q) (map->read8(translate[q&0xffff]))
#define READW_CODE(q) (map->read16l(translate[q&0xffff]))
#define WRITE_CODE(q,v) (map->write8(translate[q&0xffff],v))
#define WRITEW_CODE(q,v) (map->write16l(translate[q&0xffff],v))

#define SET_A(q) a=(q)
#define SET_X(q) x=(q)
#define SET_Y(q) y=(q)
#define SET_Z(q) z=(q)

#define STACK_PUSH(q) stack_push(q)
#define STACK_PUSHW(q) stack_pushw(q)
#define STACK_POP() stack_pop()
#define STACK_POPW() stack_popw()

#define STORE_A(q) WRITE_DATA(q,a)
#define STORE_X(q) WRITE_DATA(q,x)
#define STORE_Y(q) WRITE_DATA(q,y)
#define STORE_Z(q) WRITE_DATA(q,z)
#define LOAD_A(q) SET_A(READ_DATA(q))
#define LOAD_X(q) SET_X(READ_DATA(q))
#define LOAD_Y(q) SET_Y(READ_DATA(q))
#define LOAD_Z(q) SET_Z(READ_DATA(q))


// By default all reads are in code space (like it makes a diference!)
#define ADDR_ZP()   (UINT16)(READ_CODE(pc+1)+dp)
#define ADDR_ZPX()  (UINT16)((ADDR_ZP()+x)&0xff)
#define ADDR_ZPY()  (UINT16)((ADDR_ZP()+y)&0xff)
#define ADDR_ZPZ()  (UINT16)((ADDR_ZP()+z)&0xff)
#define ADDR_INDX() (UINT16)READW_CODE(ADDR_ZPX())
#define ADDR_INDY() (UINT16)(READW_CODE(ADDR_ZP())+y)
#define ADDR_INDZ() (UINT16)(READW_CODE(ADDR_ZP())+z)
#define ADDR_INDYSP() (UINT16)(READW_CODE(ADDR_ABSB()+sp)+y)
#define ADDR_ABS()  (UINT16)READW_CODE(pc+1)
#define ADDR_ABSB()  (UINT16)READ_CODE(pc+1)
#define ADDR_ABSW()  (UINT16)READW_CODE(pc+1)
#define ADDR_ABSX() (UINT16)(ADDR_ABS()+x)
#define ADDR_ABSY() (UINT16)(ADDR_ABS()+y)
#define ADDR_ABSINDX() (UINT16)READW_CODE(ADDR_ABSX())


// TODO: Some of the data vs code stuff may be wrong...
#define VAL_IMM8()  READ_CODE(pc+1)
#define VAL_IMM16()  READW_CODE(pc+1)
#define VAL_IMM(n)  READ_CODE(pc+1)
#define VAL_ZP(n)   READ_CODE(ADDR_ZP())
#define VAL_ZPX(n)  READ_CODE(ADDR_ZPX())
#define VAL_ZPY(n)  READ_CODE(ADDR_ZPY())
#define VAL_ZPZ(n)  READ_CODE(ADDR_ZPZ())
#define VAL_INDX(n) READ_CODE(ADDR_INDX())
#define VAL_INDY(n) READ_CODE(ADDR_INDY())
#define VAL_INDZ(n) READ_CODE(ADDR_INDZ())
#define VAL_INDYSP(n) READ_CODE(ADDR_INDYSP())
#define VAL_ABS(n)  READ_CODE(ADDR_ABS())
#define VAL_ABSL(n)  READ_CODE(ADDR_ABSL())
#define VAL_ABSB(n)  READ_CODE(ADDR_ABSB())
#define VAL_ABSW(n)  READW_CODE(ADDR_ABSW())
#define VAL_ABSX(n) READ_CODE(ADDR_ABSX())
#define VAL_ABSY(n) READ_CODE(ADDR_ABSY())
#define VAL_ABSINDX(n) READ_CODE(ADDR_ABSINDX())


#define ADJ_OP_LEN(n)


////////////////////////////////////////////////////////////////////////////////
//  Em4510 Class
////////////////////////////////////////////////////////////////////////////////

Em4510::Em4510()
{
  DEBUG_OUT<<"Em4510 created...\n";
  for(unsigned int t=0;t<64*1024;t++) translate[t]=t;
  Em6502::init();
  type=E_CPU_65x4510;
  //w=ww;
  reset();
}


Em4510::~Em4510()
{
  DEBUG_OUT<<"Em4510 destroyed.\n";
}


/* STATIC */
bool Em4510::recognize(const char *tname)
{
  char *name=aDefaultStringHelper.removeSpaces2(aDefaultStringHelper.toLower(tname));
  if(!strcmp(name,"4510")) return true;
  if(!strcmp(name,"csg4510")) return true;
  if(!strcmp(name,"65x4510")) return true;
  if(!strcmp(name,"c65x4510")) return true;
  if(!strcmp(name,"c65")) return true;
  if(!strcmp(name,"victor")) return true;
  if(!strcmp(name,"65ce02")) return true;
  return false;
}


const char *Em4510::getName()
{
  return "Commodore 4510 (C65 'Victor')";
}


void Em4510::setKind(const char *tname)
{
  const char *name=aDefaultStringHelper.removeSpaces2(aDefaultStringHelper.toLower(tname));
  type=E_CPU_65x4510;
}


void Em4510::reset()
{
  for(unsigned int t=0;t<64*1024;t++) translate[t]=t;
  mapLower=0;  mapUpper=0;  mapBlocks=0;
  /*
  // This is C-65 specific...set up a C64 compatible map
  mapUpper=0x020000;
  mapLower=0x000000;
  mapBlocks=0x20|0x80;
  handle_map();
  */
  Em6502::reset();
  dp=0;
  z=0;
  stackBase=0x0100;
  stackMask=0xffff;
  stackExtend=false;
  setStatusReg(0);
  statusReg|=E6502_STAT_ExtendDisable;
  setStatusBits(E6502_STAT_I);
  sp=0;  // NOTE: Only last 8 bits of it...but remember sp is 16 bits...
  // Because E6502 doesn't handle mem mapping.
  if(map) {
    pc=READW_CODE(E6502_INT_RESET);
    setResetEntry(pc);
  }
  clearStackTrace();
  ECPUAbstract::reset();
}


bool Em4510::setRegister(const char *tname,ULONG value)
{
  char *name=aDefaultStringHelper.removeSpaces2(aDefaultStringHelper.toLower(tname));
  bool ret=false;
  if(!strcmp(name,"pc"))
  { pc=(UINT16)value; ret=true; } else
  if(!strcmp(name,"a"))
  { a=(BYTE)value; ret=true; } else
  if(!strcmp(name,"x"))
  { x=(BYTE)value; ret=true; } else
  if(!strcmp(name,"y"))
  { y=(BYTE)value; ret=true; } else
  if(!strcmp(name,"z"))
  { z=(BYTE)value; ret=true; } else
  if(!strcmp(name,"p"))
  { setStatusReg(value); ret=true; } else
  if(!strcmp(name,"s"))
  { sp=val; ret=true; }  // TODO: Doesn't handle extended stack
  //free(name);
  return ret;
}


// TODO: finish this
ULONG Em4510::readRegister(const char *name)
{
  if(name[0]=='a') return a;
  if(name[0]=='x') return x;
  if(name[0]=='y') return y;
  if(name[0]=='z') return z;
  return 0;
}


void Em4510::wregs()
{
  //unsigned int dig=w->getHexDigits();
  //w->setHexDigits(4);
  DEBUG_OUT<<"PC="<<(unsigned short)pc<<"  B="<<dp<<"\n";
  //w->setHexDigits(2);
  DEBUG_OUT<<"A="<<(short)a<<"  X="<<(short)x<<"  Y="<<(short)y<<"  Z="<<(short)z<<"\n";
  DEBUG_OUT<<"SP=";
  //w->setHexDigits(4);
  DEBUG_OUT<<sp;
  DEBUG_OUT<<"  (base="<<stackBase<<"mask="<<stackMask<<")\n";
  //w->setHexDigits(2);
  BYTE p=getStatusReg();
  DEBUG_OUT<<"P="<<(short)p<<"  (";
  if(p&E6502_STAT_N) DEBUG_OUT<<"N";  else DEBUG_OUT<<"n";
  if(p&E6502_STAT_V) DEBUG_OUT<<"V";  else DEBUG_OUT<<"v";
  if(p&E6502_STAT_ExtendDisable) DEBUG_OUT<<"E";  else DEBUG_OUT<<"e";
  if(p&E6502_STAT_Break) DEBUG_OUT<<"B";  else DEBUG_OUT<<"b";
  if(p&E6502_STAT_D) DEBUG_OUT<<"D";  else DEBUG_OUT<<"d";
  if(p&E6502_STAT_I) DEBUG_OUT<<"I";  else DEBUG_OUT<<"i";
  if(p&E6502_STAT_Z) DEBUG_OUT<<"Z";  else DEBUG_OUT<<"z";
  if(p&E6502_STAT_C) DEBUG_OUT<<"C";  else DEBUG_OUT<<"c";
  DEBUG_OUT<<")\n";
  //w->setHexDigits(dig);
  show_map();
}


ULONG Em4510::disasm(ULONG loc)
{
  //unsigned int dig=w->getHexDigits();
  unsigned int len=0,mode=E6502_NONE;
  const char *name="???";
  topcode=READ_CODE(loc);
  // TODO: this doesn't take the bank into account...
  lastSymbol=map->findAnyExactSymbol(loc);
  if(lastSymbol) DEBUG_OUT<<lastSymbol->getName()<<":\n";
  DEBUG_OUT<<"";
  DEBUG_OUT<<".";
  //w->setHexDigits(4);
  DEBUG_OUT<<(loc&0xffff)<<" ";
  //w->setHexDigits(dig);
  DEBUG_OUT<<"("<<translate[loc&0xffff]<<") ";
  len=Em4510_size[topcode];
  mode=Em4510_format[topcode];
  name=Em4510_name[topcode];
  if(!len) len=1;  // NOTE: An unimplemented opcode...
  unsigned int t;
  for(t=0;t<len;t++) DEBUG_OUT<<(short)READ_CODE(loc+t)<<" ";
  for(t=0;t<(3-len);t++) DEBUG_OUT<<"   ";
  DEBUG_OUT<<" "<<name<<" ";
  disasmAddrMode(loc,mode);
  DEBUG_OUT<<"";
  DEBUG_OUT<<"\n";
  //w->setHexDigits(dig);
  return len;
}


void Em4510::executeInstead(ULONG opcode2)
{
  opcode=opcode2;
  op_len=Em4510_size[opcode];
  cycle=Em4510_time[opcode];
  if(!cycle) cycle=2;
  if(!execMain()) exec_unimplemented(opcode);
  // NOTE: we fall back to the regular execute code here...
}


void Em4510::execute()
{
  if(pc==breakpoint) { debugger("Breakpoint!\n"); return; }
  opcode=READ_CODE(pc);
  op_len=Em4510_size[opcode];
  cycle=Em4510_time[opcode];
  if(!cycle) cycle=2;
  if(!execMain()) exec_unimplemented(opcode);
  pc+=op_len;
  cycles+=cycle;
  if(int_pending) {
    // Ints can NOT happen after a 1 cycle instruction on the 4510
    if(cycle!=1) handle_int(int_pending);
  }
}


void Em4510::disasmAddrMode(ULONG loc,unsigned int mode)
{
  //unsigned int dig=w->getHexDigits();
  //w->setHexDigits(4);
  switch(mode) {
    case E6502_NONE:
      break;
    case E6502_ZP:
      DEBUG_OUT<<"$"<<(short)READ_CODE(loc+1);
      break;
    case E6502_ZPX:
      DEBUG_OUT<<"$"<<(short)READ_CODE(loc+1)<<",X";
      break;
    case E6502_ZPY:
      DEBUG_OUT<<"$"<<(short)READ_CODE(loc+1)<<",Y";
      break;
    case E6502_ZPZ:
      DEBUG_OUT<<"$"<<(short)READ_CODE(loc+1)<<",Z";
      break;
    case E6502_IMM:
      DEBUG_OUT<<"#"<<(short)READ_CODE(loc+1);
      break;
    case E6502_IMMW:
      DEBUG_OUT<<"#"<<(short)READW_CODE(loc+1);
      break;
    case E6502_ABS:
      DEBUG_OUT<<"$"<<(short)READW_CODE(loc+1);
      break;
    case E6502_ABSW:
      DEBUG_OUT<<"$"<<(short)READW_CODE(loc+1);
      break;
    case E6502_ABSX:
      DEBUG_OUT<<"$"<<(short)READW_CODE(loc+1)<<",X";
      break;
    case E6502_ABSY:
      DEBUG_OUT<<"$"<<(short)READW_CODE(loc+1)<<",Y";
      break;
    case E6502_IND:
      DEBUG_OUT<<"($"<<(short)READ_CODE(loc+1)<<")";
      break;
    case E6502_INDX:
      DEBUG_OUT<<"($"<<(short)READ_CODE(loc+1)<<",X)";
      break;
    case E6502_INDY:
      DEBUG_OUT<<"($"<<(short)READ_CODE(loc+1)<<"),Y";
      break;
    case E6502_INDZ:
      DEBUG_OUT<<"($"<<(short)READ_CODE(loc+1)<<"),Z";
      break;
    case E6502_INDYSP:
      DEBUG_OUT<<"($"<<(short)READ_CODE(loc+1)<<",SP),Y";
      break;
    case E6502_BXX:
      DEBUG_OUT<<"$"<<(short)READ_CODE(loc+1)<<"  ;"<<(UINT16)((SBYTE)READ_CODE(loc+1)+loc+2);
      break;
    case E6502_WREL:
      // TODO: how exactly is the offset added to the program counter?
      // NOTE the 3...the instruction is 3 bytes long...
      DEBUG_OUT<<"$"<<(short)READW_CODE(loc+1)<<"  ;"<<(UINT16)((SINT16)READW_CODE(loc+1)+loc+3);
      break;
    case E6502_ZPBXX:
      // TODO: how exactly is the offset added to the program counter?
      // NOTE the 3...the instruction is 3 bytes long...
      DEBUG_OUT<<"$"<<(short)READ_CODE(loc+1)<<",$"<<(UINT16)((SBYTE)READ_CODE(loc+2)+loc+3);
      //<<READ_CODE(loc+2)<<"  ;";
      //<<(UINT16)((SBYTE)READ_CODE(loc+2)+loc+2);
      break;
    case E6502_JABS:
      DEBUG_OUT<<"$"<<(short)READW_CODE(loc+1);
      break;
    case E6502_JIND:
      DEBUG_OUT<<"($"<<(short)READW_CODE(loc+1)<<")";
      break;
    case E6502_JINDX:
      DEBUG_OUT<<"($"<<(short)READW_CODE(loc+1)<<",X)";
      break;
    default:
      DEBUG_OUT<<"???";
      debugger("Can't Disasm...unknown mode");
      break;
  };
  //w->setHexDigits(dig);
}


void Em4510::show_map()
{
  //unsigned int dig=w->getHexDigits();
  DEBUG_OUT<<"MAP:\n";
  //w->setHexDigits(6);
  DEBUG_OUT<<"mapUpper="<<mapUpper<<"\n";
  DEBUG_OUT<<"mapLower="<<mapLower<<"\n";
  DEBUG_OUT<<"mapBlocks="<<(short)mapBlocks<<"  (";
  if(mapBlocks&0x01) DEBUG_OUT<<"01"; else DEBUG_OUT<<"--";
  if(mapBlocks&0x02) DEBUG_OUT<<"23"; else DEBUG_OUT<<"--";
  if(mapBlocks&0x04) DEBUG_OUT<<"45"; else DEBUG_OUT<<"--";
  if(mapBlocks&0x08) DEBUG_OUT<<"67"; else DEBUG_OUT<<"--";
  if(mapBlocks&0x10) DEBUG_OUT<<"89"; else DEBUG_OUT<<"--";
  if(mapBlocks&0x20) DEBUG_OUT<<"ab"; else DEBUG_OUT<<"--";
  if(mapBlocks&0x40) DEBUG_OUT<<"cd"; else DEBUG_OUT<<"--";
  if(mapBlocks&0x80) DEBUG_OUT<<"ef"; else DEBUG_OUT<<"--";
  DEBUG_OUT<<")\n";
  //w->setHexDigits(dig);
}


void Em4510::handle_map()
{
  //DEBUG_OUT<<"***intLock turned on***\n";
  //intLock=true;
  //mapUpper=((y<<8)&0xff00)|(((z&0x0f)<<16)&0xff0000);
  //mapLower=((a<<8)&0xff00)|(((x&0x0f)<<16)&0xff0000);
  //mapBlocks=(z&0xf0)|((x>>4)&0x0f);
  // show what's about to happen...
  show_map();
  // done showing what's happening, now do the actual work...
  unsigned int t=0;
  // TODO: Should the for loop go one past?  Am I missing the last byte?
  if(mapBlocks&0x80) {
    // block from 0xe000-0xffff
    for(t=0xe000;t<0xffff;t++) translate[t]=mapUpper+t;
  }
  else { for(t=0xe000;t<0xffff;t++) translate[t]=t; }
  if(mapBlocks&0x40) {
    // block from 0xc000-0xdfff
    for(t=0xc000;t<0xdfff;t++) translate[t]=mapUpper+t;
  }
  else { for(t=0xc000;t<0xdfff;t++) translate[t]=t; }
  if(mapBlocks&0x20) {
    // block from 0xa000-0xbfff
    for(t=0xa000;t<0xbfff;t++) translate[t]=mapUpper+t;
  }
  else { for(t=0xa000;t<0xbfff;t++) translate[t]=t; }
  if(mapBlocks&0x10) {
    // block from 0x8000-0x9fff
    for(t=0x8000;t<0x9fff;t++) translate[t]=mapUpper+t;
  }
  else { for(t=0x8000;t<0x9fff;t++) translate[t]=t; }
  if(mapBlocks&0x08) {
    // block from 0x6000-0x7fff
    for(t=0x6000;t<0x7fff;t++) translate[t]=mapLower+t;
  }
  else { for(t=0x6000;t<0x7fff;t++) translate[t]=t; }
  if(mapBlocks&0x04) {
    // block from 0x4000-0x5fff
    for(t=0x4000;t<0x5fff;t++) translate[t]=mapLower+t;
  }
  else { for(t=0x4000;t<0x5fff;t++) translate[t]=t; }
  if(mapBlocks&0x02) {
    // block from 0x2000-0x3fff
    for(t=0x2000;t<0x3fff;t++) translate[t]=mapLower+t;
  }
  else { for(t=0x2000;t<0x3fff;t++) translate[t]=t; }
  if(mapBlocks&0x01) {
    // block from 0x0000-0x1fff
    for(t=0x0000;t<0x1fff;t++) translate[t]=mapLower+t;
  }
  else { for(t=0x0000;t<0x1fff;t++) translate[t]=t; }
  //debugger("MAP!\n");
}


void Em4510::changeMap(EMemMapAbstract *mmap)
{
}


// This will bring in the class' execMain func
#define theClassType Em4510
#define e65x4510
#include <cpulib/Em65_core.h>
#undef e65x4510
#undef theClassType


// TODO: Move pending stuff to cause_int...
void Em4510::handle_int(ULONG value)
{
  int_pending=0;
  bool intDis=false;
  if(intLock) {
    intDis=true;
    debugger("6502 int, but intLock set!");
  }
  if(getStatusReg()&E6502_STAT_I) intDis=true;
  if(intDis) {
    if(value==E6502_INT_IRQ) {
      if(!(getStatusReg()&E6502_STAT_Break)) {
        //DEBUG_OUT<<"IRQ requested...but ints are disabled.\n";
        //debugger("Interrupts are disabled.\n");
        int_pending=value;
#ifdef SHOW_INTS
        DEBUG_OUT<<"x";
#endif
        return;
      }
      else {
        //DEBUG_OUT<<"BRK with ints disabled!\n";
        //return;
      }
    }
  }
#ifdef SHOW_INTS
  DEBUG_OUT<<"+";
#endif
  STACK_PUSHW(pc);
  //DEBUG_OUT<<"Pushed "<<pc<<"\n";
  STACK_PUSH(getStatusReg());
  setStatusBits(E6502_STAT_I);
  jump(READW_DATA(value));
  //trace=true;
}


void Em4510::setStatusBits(BYTE mask)
{
  BYTE oldStatus=statusReg;
  if(mask&E6502_STAT_ExtendDisable) mask-=E6502_STAT_ExtendDisable;
  statusReg=oldStatus|mask;
  //
  if(stackExtend) {
    if(statusReg&E6502_STAT_ExtendDisable)
      debugger("\nsSB: Stack Extend bit mismatch!\n");
  }
  else {
    if(!(statusReg&E6502_STAT_ExtendDisable))
      debugger("\nsSB: Stack Extend bit mismatch!\n");
  }
}


void Em4510::resetStatusBits(BYTE mask)
{
  BYTE oldStatus=statusReg;
  if(mask&E6502_STAT_ExtendDisable) mask-=E6502_STAT_ExtendDisable;
  statusReg=oldStatus&(0xff-mask);
  //
  if(stackExtend) {
    if(statusReg&E6502_STAT_ExtendDisable)
      debugger("\nrSB: Stack Extend bit mismatch!\n");
  }
  else {
    if(!(statusReg&E6502_STAT_ExtendDisable))
      debugger("\nrSB: Stack Extend bit mismatch!\n");
  }
}


void Em4510::setStatusReg(unsigned int newValue)
{
  if(newValue&E6502_STAT_ExtendDisable) newValue-=E6502_STAT_ExtendDisable;
  statusReg=newValue;
  if(!stackExtend) statusReg|=E6502_STAT_ExtendDisable;
  //
  if(stackExtend) {
    if(statusReg&E6502_STAT_ExtendDisable)
      debugger("\nsSR: Stack Extend bit mismatch!\n");
  }
  else {
    if(!(statusReg&E6502_STAT_ExtendDisable))
      debugger("\nsSR: Stack Extend bit mismatch!\n");
  }
}


BYTE Em4510::getStatusReg()
{
  BYTE val=statusReg;
  //
  if(stackExtend) {
    if(val&E6502_STAT_ExtendDisable)
      debugger("\ngSR: Stack Extend bit mismatch!\n");
  }
  else {
    if(!(val&E6502_STAT_ExtendDisable))
      debugger("\ngSR: Stack Extend bit mismatch!\n");
  }
  return val;
}


bool Em4510::interp(const char *cmd,const char *a1,const char *a2,const char *a3)
{
  bool ret=false;
  //if(!strcmp(cmd,"rm")) { show_map(); ret=true; }
  return ret;
}


void Em4510::stack_push(BYTE val)
{
  WRITE_DATA(stackBase+sp,val);
  stack_dec();
}


// NOTE: Little endian!
void Em4510::stack_pushw(UINT16 val)
{
  BYTE low=(BYTE)(val&0xff);
  BYTE high=(BYTE)((val>>8)&0xff);
  stack_push(high);
  stack_push(low);
}


BYTE Em4510::stack_pop()
{
  stack_inc();
  BYTE val=READ_DATA(stackBase+sp);
  return val;
}


// NOTE: Little endian!
UINT16 Em4510::stack_popw()
{
  BYTE low=stack_pop();
  BYTE high=stack_pop();
  unsigned int val=low+(high<<8);
  return val;
}


void Em4510::stack_inc()
{
  unsigned int v=(sp+1);
#ifdef CHECK_STACK
  if(v>stackMask) {
    DEBUG_OUT<<"stack underflow\n";
  }
#endif
  sp=v&stackMask;
}


void Em4510::stack_dec()
{
  unsigned int v=(sp-1);
#ifdef CHECK_STACK
  if(v>stackMask) {
    DEBUG_OUT<<"stack overflow\n";
  }
#endif
  sp=v&stackMask;
}


void Em4510::ints()
{
  //if(w) {
    DEBUG_OUT<<"IRQ ("<<(unsigned short)E6502_INT_IRQ<<"): "
      <<(unsigned int)READW_CODE(E6502_INT_IRQ)<<"\n";
    DEBUG_OUT<<"NMI ("<<(unsigned short)E6502_INT_NMI<<"): "
      <<(unsigned int)READW_CODE(E6502_INT_NMI)<<"\n";
    DEBUG_OUT<<"RESET ("<<(unsigned short)E6502_INT_RESET<<"): "
      <<(unsigned int)READW_CODE(E6502_INT_RESET)<<"\n";
  //}
}


void Em4510::nmi()
{
  intLock=false;
  if(map) pc=READW_CODE(E6502_INT_NMI);
  clearStackTrace();
  jump(pc);
}


void Em4510::set_flags_w_nz(UINT16 i)
{
  //if(trace) DEBUG_OUT<<"set_flags_nz ("<<p;
  sbits->z=(i==0);
  sbits->n=(i&0x8000)!=0;
  //if(trace) DEBUG_OUT<<"->"<<p<<")\n";
}


// TODO: this doesn't work at all since I'm using an
// 8 to 16 bit trick here...
void Em4510::set_flags_w_nzc(UINT16 i, UINT16 j)
{
  //if(trace) DEBUG_OUT<<"set_flags_nzc ("<<p;
  UINT16 val=(UINT16)i-(UINT16)j;
  sbits->z=(val==0);
  sbits->n=(val&0x8000)!=0;
  sbits->c=(val<(UINT16)0x100);
  //if(trace) DEBUG_OUT<<"->"<<p<<")\n";
}


void Em4510::handle_brk()
{
  bool ret=false;
  if(io) ret=io->systemCall();
  if(!ret) {
    debugger("BRK!");
    /*
    pc++;
    setStatusBits(E6502_STAT_Break);
    handle_int(E6502_INT_IRQ);
    */
  }
}

