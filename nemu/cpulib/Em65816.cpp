
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


/*

Some notes:

Native mode doesn't work at all:
Word size isn't implemented at all, really, or where it is, its untested.
The expanded address space is likewise only half there.

A better direction would be to break away from the 65816/4510 code entirely
and start from "scratch" basing it on the way I did the 68k and x86...
i.e. using size fields for a readReg, etc.

*/


#define DEBUG_DIS_MODES 1

#define DEBUG_EXEC 1

#define CHECK_SAFE 1

#define SHOW_INTS  1

#define CHECK_STACK  1


#include <cpulib/Em65816.h>

#include <elib/EMemMapAbstract.h>
#include <elib/EModule.h>
#include <elib/EHandler.h>

#include <portable/nickcpp/ANullStream.h>

#include <portable/nickcpp/AStringHelper.h>


#include <cpulib/generated/Em65816_tab.h>


#define E_CPU_NONE     0x00
#define E_CPU_65816    0x04
#define E_CPU_65802    0x05
#define E_CPU_65832    0x06


#define M16BIT m16Bit
#define X16BIT x16Bit
#define MSIZEMASK mSizeMask
#define XSIZEMASK xSizeMask
#define MHIGHBIT mHighBit
#define XHIGHBIT xHighBit
#define MHALFBIT mHalfBit
#define XHALFBIT xHalfBit

#ifdef DEBUG_EXEC
#define READ_DATA(q) mapRead8((q&0xffff)+dbr)
#define READW_DATA(q) mapRead16l((q&0xffff)+dbr)
#define WRITE_DATA(q,v) mapWrite8((q&0xffff)+dbr,v)
#define WRITEW_DATA(q,v) mapWrite16l((q&0xffff)+dbr,v)
#define READ_CODE(q) map->read8((q&0xffff)+pbr)
#define READW_CODE(q) map->read16l((q&0xffff)+pbr)
#define WRITE_CODE(q,v) map->write8((q&0xffff)+pbr,v)
#define WRITEW_CODE(q,v) map->write16l((q&0xffff)+pbr,v)
#else
#define READ_DATA(q) (map->read8((q&0xffff)+dbr))
#define READW_DATA(q) (map->read16l((q&0xffff)+dbr))
#define WRITE_DATA(q,v) (map->write8((q&0xffff)+dbr,v))
#define WRITEW_DATA(q,v) (map->write16l((q&0xffff)+dbr,v))
#define READ_CODE(q) (map->read8((q&0xffff)+pbr))
#define READW_CODE(q) (map->read16l((q&0xffff)+pbr))
#define WRITE_CODE(q,v) (map->write8((q&0xffff)+pbr,v))
#define WRITEW_CODE(q,v) (map->write16l((q&0xffff)+pbr,v))
#endif

#define SET_A(q) a=(q)&mSizeMask
#define SET_X(q) x=(q)&xSizeMask
#define SET_Y(q) y=(q)&xSizeMask
#define SET_Z(q)

#define STACK_PUSH(q) stack_push(q)
#define STACK_PUSHW(q) stack_pushw(q)
#define STACK_POP() stack_pop()
#define STACK_POPW() stack_popw()

#define STORE_A(q) { if(!m16Bit) WRITE_DATA(q,a); else WRITEW_DATA(q,a); }
#define STORE_X(q) { if(!x16Bit) WRITE_DATA(q,x); else WRITEW_DATA(q,x); }
#define STORE_Y(q) { if(!x16Bit) WRITE_DATA(q,y); else WRITEW_DATA(q,y); }
#define STORE_Z(q) { if(!m16Bit) WRITE_DATA(q,0); else WRITEW_DATA(q,0); }
#define LOAD_A(q) { if(!m16Bit) SET_A(READ_DATA(q)); else SET_A(READW_DATA(q)); }
#define LOAD_X(q) { if(!x16Bit) SET_X(READ_DATA(q)); else SET_X(READW_DATA(q)); }
#define LOAD_Y(q) { if(!x16Bit) SET_Y(READ_DATA(q)); else SET_Y(READW_DATA(q)); }
#define LOAD_Z(q)

#define READ_DATA_LONG(q) (map->read8(q))
#define READW_DATA_LONG(q) (map->read16l(q))
#define WRITE_DATA_LONG(q,v) (map->write8(q,v))
#define WRITEW_DATA_LONG(q,v) (map->write16l(q,v))
#define STORE_A_LONG(q) { if(!m16Bit) WRITE_DATA_LONG(q,a); else WRITEW_DATA_LONG(q,a); }
#define LOAD_A_LONG(q) { if(!m16Bit) SET_A(READ_DATA_LONG(q)); else SET_A(READW_DATA_LONG(q)); }

#define READ_DATA_INDEXED(q) (map->read8((q&0xffff)+indexedOffset))
#define WRITE_DATA_INDEXED(q,v) (map->write8((q&0xffff)+indexedOffset,v))
#define LOAD_A_INDEXED(q) { SET_A(READ_DATA_INDEXED(q)); }
#define STORE_A_INDEXED(q) { WRITE_DATA_INDEXED(q,a); }

#define READV_DATA(q,n) readv((q&0xffff)+dbr,n)
#define READV_DATA_LONG(q,n) readv(q,n)
#define READV_CODE(q,n) readv((q&0xffff)+pbr,n)
#define WRITEV_DATA(q,v) writev((q&0xffff)+dbr,v,n)
#define WRITEV_DATA_LONG(q,v) writev(q,v,n)


// TODO: Some of the data vs code stuff may be wrong...
#define ADDR_ZP()   (UINT16)(READ_CODE(pc+1)+dp)
#define ADDR_ZPX()  (UINT16)((ADDR_ZP()+x)&dpMask)
#define ADDR_ZPY()  (UINT16)((ADDR_ZP()+y)&dpMask)
#define ADDR_ZPZ()  (UINT16)((ADDR_ZP()+z)&dpMask)
#define ADDR_INDX() (UINT16)READW_DATA(ADDR_ZPX())
#define ADDR_INDY() (UINT16)(READW_DATA(ADDR_ZP())+y)
#define ADDR_INDSPY() (UINT16)(READW_DATA(ADDR_ABSB()+s)+y)
#define ADDR_ABS()  (UINT16)READW_CODE(pc+1)
#define ADDR_ABSL()  read24code(pc+1)
#define ADDR_ABSB()  (UINT16)READ_CODE(pc+1)
#define ADDR_ABSX() (UINT16)(ADDR_ABS()+x)
#define ADDR_ABSY() (UINT16)(ADDR_ABS()+y)
#define ADDR_ABSINDX() (UINT16)READW_DATA(ADDR_ABSX())
#define ADDR_ABSLX()  (ADDR_ABSL()+x)
#define ADDR_SR()  (READ_CODE(pc+1)+sp)


// TODO: Some of the data vs code stuff may be wrong...
// TODO: Most of these should vary size depending on which reg they're used with.
#define VAL_IMM8()  READ_CODE(pc+1)
#define VAL_IMM16()  READW_CODE(pc+1)
#define VAL_IMM(n)  READV_CODE(pc+1,n)
#define VAL_ZP(n)   READV_DATA(ADDR_ZP(),n)
#define VAL_ZPX(n)  READV_DATA(ADDR_ZPX(),n)
#define VAL_ZPY(n)  READV_DATA(ADDR_ZPY(),n)
#define VAL_ZPZ(n)  READV_DATA(ADDR_ZPZ(),n)
#define VAL_INDX(n) READV_DATA(ADDR_INDX(),n)
#define VAL_INDY(n) READV_DATA(ADDR_INDY(),n)
#define VAL_INDSPY(n) READV_DATA(ADDR_INDSPY(),n)
#define VAL_ABS(n)  READV_DATA(ADDR_ABS(),n)
#define VAL_ABSL(n)  READV_DATA(ADDR_ABSL(),n)
#define VAL_ABSB(n)  READV_DATA(ADDR_ABSB(),n)
#define VAL_ABSX(n) READV_DATA(ADDR_ABSX(),n)
#define VAL_ABSY(n) READV_DATA(ADDR_ABSY(),n)
#define VAL_ABSINDX(n) READV_DATA(ADDR_ABSINDX(),n)
#define VAL_ABSLX(n)  READV_DATA(ADDR_ABSLX(),n)
#define VAL_SR(n)  READV_DATA(ADDR_SR(),n)

#define ADJ_OP_LEN(n) adj_op_len(n)


////////////////////////////////////////////////////////////////////////////////
//  Em65816 Class
////////////////////////////////////////////////////////////////////////////////


// This will bring in the class' execMain func
#define theClassType Em65816
#define e65816
#include <cpulib/Em65_core.h>
#undef e65816
#undef theClassType


Em65816::Em65816()
{
  DEBUG_OUT<<"Em65816 created...\n";
  init();
  type=E_CPU_65816;
  reset();
}


Em65816::~Em65816()
{
  DEBUG_OUT<<"Em65816 destroyed.\n";
}


void Em65816::init()
{
  ECPU8Bit::init();
  bitness=8;
  // Generic features
  doTrace=false;  doRun=false;
  a=0;  x=0;  y=0;  sp=0;
  statusReg=0;  setStatusReg(0);
  // Specific chips
  dp=0;  dpMask=0xff;
  dbr=0;
  pbr=0;
  // Set up stack and registers
  sbits=(Em65816StatusBits *)&statusReg;
  int_pending=0;
  intLock=false;
  lsb=true;
  stackBase=0x0100;
  stackMask=0x00ff;
  setStack(ECPUABSTRACT_DEFAULT_STACK>>16);
  setFramePtr(ECPUABSTRACT_DEFAULT_STACK>>16);
  setResetStack(ECPUABSTRACT_DEFAULT_STACK>>16);
  setResetEntry(ECPUABSTRACT_DEFAULT_CODE>>16);
}


/* STATIC */
bool Em65816::recognize(const char *tname)
{
  const char *name=aDefaultStringHelper.removeSpaces2(aDefaultStringHelper.toLower(tname));
  if(!strcmp(name,"65816")) return true;
  if(!strcmp(name,"65c816")) return true;
  if(!strcmp(name,"65802")) return true;
  if(!strcmp(name,"65c802")) return true;
  if(!strcmp(name,"65832")) return true;
  if(!strcmp(name,"wd65816")) return true;
  if(!strcmp(name,"wd65c816")) return true;
  if(!strcmp(name,"wd65802")) return true;
  if(!strcmp(name,"wd65c802")) return true;
  if(!strcmp(name,"wd65832")) return true;
  if(!strcmp(name,"w65")) return true;
  if(!strcmp(name,"wdc65")) return true;
  return false;
}


const char *Em65816::getName()
{
  if(type==E_CPU_65816) return "WD65816";
  if(type==E_CPU_65802) return "WD65802";
  if(type==E_CPU_65832) return "WD65832";
  return "WD65816";
}


void Em65816::setKind(const char *tname)
{
  const char *name=aDefaultStringHelper.removeSpaces2(aDefaultStringHelper.toLower(tname));
  if(!strcmp(name,"65816")) type=E_CPU_65816;
  if(!strcmp(name,"65c816")) type=E_CPU_65816;
  if(!strcmp(name,"65802")) type=E_CPU_65802;
  if(!strcmp(name,"65c802")) type=E_CPU_65802;
  if(!strcmp(name,"65832")) type=E_CPU_65832;
  if(!strcmp(name,"wd65816")) type=E_CPU_65816;
  if(!strcmp(name,"wd65c816")) type=E_CPU_65816;
  if(!strcmp(name,"wd65802")) type=E_CPU_65802;
  if(!strcmp(name,"wd65c802")) type=E_CPU_65802;
  if(!strcmp(name,"wd65832")) type=E_CPU_65832;
  if(!strcmp(name,"w65")) type=E_CPU_65816;
  if(!strcmp(name,"wdc65")) type=E_CPU_65816;
}


void Em65816::reset()
{
  a=0;  x=0;  y=0;
  bHolder=0;
  statusReg=0;  setStatusReg(0);
  sp=0;  // Only last 8 bits of it...
  dp=0;  dbr=0;  pbr=0;  dpMask=0xff;
  pbrHolder=0;  dbrHolder=0;  dpHolder=0;
  int_pending=0;
  intLock=false;
  setStatusBits(E65816_STAT_I);
  // A real 65816 starts in emulation mode
  //goEmulation();
  goNative();
  if(map) {
    pc=READW_CODE(E65816_INT_RESET);
    setResetEntry(pc);
  }
  clearStackTrace();
  ECPUAbstract::reset();
}


// TODO: check that this is right...
void Em65816::goEmulation()
{
  nativeMode=false;
  m16Bit=false;  x16Bit=false;
  mSizeMask=ECPUABSTRACT_MASK_BYTE;  xSizeMask=ECPUABSTRACT_MASK_BYTE;
  mHighBit=0x80;  xHighBit=0x80;
  mHalfBit=0x40;  xHalfBit=0x40;
  resetStatusBits(E65816_STAT_Break);
  resetStatusBits(E65816_STAT_Extra);
  // TODO: Check, are these actually saved on switch to emu?
  pbrHolder=pbr;  dbrHolder=dbr;  dpHolder=dp;
  pbr=0;  dbr=0;  dp=0;  dpMask=0xff;
  bHolder=(a>>8)&0xff;
  a&=0xff;  x&=0xff;  y&=0xff;
  sp&=0xff;
  bitness=8;
  stackBase=0x0100;
  stackMask=0x00ff;
  DEBUG_OUT<<"65816 emu mode entered.\n";
}


// TODO: obviously there's a lot to do here...
void Em65816::goNative()
{
  nativeMode=true;
  // TODO: does it automatically switch to 16 bit in native or leave it at 8 bit?!?
  m16Bit=true;  x16Bit=true;
  mSizeMask=ECPUABSTRACT_MASK_WORD;  xSizeMask=ECPUABSTRACT_MASK_WORD;
  mHighBit=0x8000;  xHighBit=0x8000;
  mHalfBit=0x4000;  xHalfBit=0x4000;  // TODO: What should these be?!?
  resetStatusBits(E65816_STAT_X);
  resetStatusBits(E65816_STAT_M);
  // TODO: Check, are these actually restored on switch to native?
  pbr=pbrHolder;  dbr=dbrHolder;  dp=dpHolder;  dpMask=0xffff;
  a=(bHolder<<8)|(a&0xff);
  bitness=16;
  stackBase=0x0000;
  stackMask=0xffff;
  sp=100+(sp&0xff);
  DEBUG_OUT<<"Native 65816 mode entered.\n";
}


void Em65816::setBitness(unsigned int val)
{
  switch(val) {
    case 8:  goEmulation(); break;
    case 16:  goNative(); break;
    default: DEBUG_OUT<<"Illegal value "<<val<<" for setBitness!\n"; break;
  }
}


bool Em65816::setRegister(const char *tname,ULONG value)
{
  const char *name=aDefaultStringHelper.removeSpaces2(aDefaultStringHelper.toLower(tname));
  bool ret=false;
  if(!strcmp(name,"pc"))
  { pc=(UINT16)value; ret=true; } else
  if(!strcmp(name,"a"))
  { a=(UINT16)value; ret=true; } else
  if(!strcmp(name,"x"))
  { x=(UINT16)value; ret=true; } else
  if(!strcmp(name,"y"))
  { y=(UINT16)value; ret=true; } else
  if(!strcmp(name,"p"))
  { setStatusReg(value); ret=true; } else
  if(!strcmp(name,"s"))
  { sp=val; ret=true; }  // TODO: Doesn't handle extended stack
  //free(name);
  return ret;
}


// TODO: finish this
ULONG Em65816::readRegister(const char *tname)
{
  const char *name=aDefaultStringHelper.removeSpaces2(aDefaultStringHelper.toLower(tname));
  if(name[0]=='a') return a;
  if(name[0]=='x') return x;
  if(name[0]=='y') return y;
  return 0;
}


void Em65816::wregs()
{
  //unsigned int dig=w->getHexDigits();
  //w->setHexDigits(4);
  DEBUG_OUT<<"PC="<<(unsigned long)pc<<"  ";
  if(nativeMode) DEBUG_OUT<<"native";  else DEBUG_OUT<<"emulation";
  DEBUG_OUT<<"\n";
  DEBUG_OUT<<"DP="<<dp<<"  ";
  if(m16Bit) DEBUG_OUT<<"m16bit  "; else DEBUG_OUT<<"m8bits  ";
  if(x16Bit) DEBUG_OUT<<"x16bit"; else DEBUG_OUT<<"x8bits";
  DEBUG_OUT<<"\n";
  DEBUG_OUT<<"SP="<<sp;
  DEBUG_OUT<<" (base="<<stackBase<<" mask="<<stackMask<<")\n";
  DEBUG_OUT<<"A=";
  if(m16Bit) { DEBUG_OUT<<(short)a; } else { DEBUG_OUT<<(short)(a&0xff); }
  DEBUG_OUT<<"   ";
  DEBUG_OUT<<"X=";
  if(x16Bit) { DEBUG_OUT<<(short)x; } else { DEBUG_OUT<<(short)(x&0xff); }
  DEBUG_OUT<<"   ";
  DEBUG_OUT<<"Y=";
  if(x16Bit) { DEBUG_OUT<<(short)y; } else { DEBUG_OUT<<(short)(y&0xff); }
  DEBUG_OUT<<"   ";
  if(!m16Bit) DEBUG_OUT<<"   (B="<<(short)bHolder<<")";
  DEBUG_OUT<<"\n";
  //w->setHexDigits(6);
  DEBUG_OUT<<"PBR="<<pbr<<"  "<<"DBR="<<dbr<<"\n";
  //w->setHexDigits(2);
  BYTE p=getStatusReg();
  DEBUG_OUT<<"P="<<(short)p<<" (";
  if(nativeMode) {
    DEBUG_OUT<<"e";  // Because we're in native mode
    if(p&E65816_STAT_N) DEBUG_OUT<<"N";  else DEBUG_OUT<<"n";
    if(p&E65816_STAT_V) DEBUG_OUT<<"V";  else DEBUG_OUT<<"v";
    if(p&E65816_STAT_M) DEBUG_OUT<<"M";  else DEBUG_OUT<<"m";
    if(p&E65816_STAT_X) DEBUG_OUT<<"X";  else DEBUG_OUT<<"x";
    if(p&E65816_STAT_D) DEBUG_OUT<<"D";  else DEBUG_OUT<<"d";
    if(p&E65816_STAT_I) DEBUG_OUT<<"I";  else DEBUG_OUT<<"i";
    if(p&E65816_STAT_Z) DEBUG_OUT<<"Z";  else DEBUG_OUT<<"z";
    if(p&E65816_STAT_C) DEBUG_OUT<<"C";  else DEBUG_OUT<<"c";
  }
  else {
    DEBUG_OUT<<"E";  // Because we're in emulation mode
    if(p&E65816_STAT_N) DEBUG_OUT<<"N";  else DEBUG_OUT<<"n";
    if(p&E65816_STAT_V) DEBUG_OUT<<"V";  else DEBUG_OUT<<"v";
    DEBUG_OUT<<"-";
    //if(p&E65816_STAT_5) DEBUG_OUT<<"5";  else DEBUG_OUT<<"-";
    if(p&E65816_STAT_Break) DEBUG_OUT<<"B";  else DEBUG_OUT<<"b";
    if(p&E65816_STAT_D) DEBUG_OUT<<"D";  else DEBUG_OUT<<"d";
    if(p&E65816_STAT_I) DEBUG_OUT<<"I";  else DEBUG_OUT<<"i";
    if(p&E65816_STAT_Z) DEBUG_OUT<<"Z";  else DEBUG_OUT<<"z";
    if(p&E65816_STAT_C) DEBUG_OUT<<"C";  else DEBUG_OUT<<"c";
  }
  DEBUG_OUT<<")\n";
  //w->setHexDigits(dig);
}

void Em65816::set_flags_nz(BYTE i)
{
  //if(trace) DEBUG_OUT<<"set_flags_nz ("<<p;
  sbits->z=(i==0);
  sbits->n=(i&0x80)!=0;
  //if(trace) DEBUG_OUT<<"->"<<p<<")\n";
}


void Em65816::set_flags_nzc(BYTE i, BYTE j)
{
  //if(trace) DEBUG_OUT<<"set_flags_nzc ("<<p;
  UINT16 val=(UINT16)i-(UINT16)j;
  sbits->z=(val==0);
  sbits->n=(val&0x80)!=0;
  sbits->c=(val<(UINT16)0x100);
  //if(trace) DEBUG_OUT<<"->"<<p<<")\n";
}


// TODO: This version ignores the sizemask...
void Em65816::set_flags_nz(UINT16 i,UINT16 sizemask)
{
  //if(trace) DEBUG_OUT<<"set_flags_nz ("<<p;
  sbits->z=(i==0);
  sbits->n=(i&0x80)!=0;
  //if(trace) DEBUG_OUT<<"->"<<p<<")\n";
}


// TODO: This version ignores the sizemask...
void Em65816::set_flags_nzc(UINT16 i, UINT16 j,UINT16 sizemask)
{
  //if(trace) DEBUG_OUT<<"set_flags_nzc ("<<p;
  UINT16 val=(UINT16)i-(UINT16)j;
  sbits->z=(val==0);
  sbits->n=(val&0x80)!=0;
  sbits->c=(val<(UINT16)0x100);
  //if(trace) DEBUG_OUT<<"->"<<p<<")\n";
}


// For speed reasons...since we never use the extra parameter
// for the cpus in the this file...but the others might...
//#define set_flags_nzc(x,y,z) set_flags_nzc(x,y)
//#define set_flags_nz(x,y) set_flags_nz(x)


void Em65816::adj_op_len(bool n)
{
  if(n) op_len++;
}


void Em65816::fixOpLen(unsigned int mode)
{
  switch(mode) {
    case E65816_NONE:
#ifdef DEBUG_DIS_MODES
      DEBUG_OUT<<"none";
#endif // DEBUG_DIS_MODES
      break;
    case E65816_ZP:
#ifdef DEBUG_DIS_MODES
      DEBUG_OUT<<"zp";
#endif // DEBUG_DIS_MODES
      break;
    case E65816_ZPX:
#ifdef DEBUG_DIS_MODES
      DEBUG_OUT<<"zpx";
#endif // DEBUG_DIS_MODES
      break;
    case E65816_ZPY:
#ifdef DEBUG_DIS_MODES
      DEBUG_OUT<<"zpy";
#endif // DEBUG_DIS_MODES
      break;
    case E65816_IMM:
#ifdef DEBUG_DIS_MODES
      DEBUG_OUT<<"imm";
#endif // DEBUG_DIS_MODES
      break;
    case E65816_IMMA:
#ifdef DEBUG_DIS_MODES
      DEBUG_OUT<<"imma";
#endif // DEBUG_DIS_MODES
      adj_op_len(m16Bit);
      break;
    case E65816_IMMX:
#ifdef DEBUG_DIS_MODES
      DEBUG_OUT<<"immx";
#endif // DEBUG_DIS_MODES
      adj_op_len(x16Bit);
      break;
    case E65816_IMMY:
#ifdef DEBUG_DIS_MODES
      DEBUG_OUT<<"immy";
#endif // DEBUG_DIS_MODES
      adj_op_len(x16Bit);
      break;
    case E65816_IMMW:
#ifdef DEBUG_DIS_MODES
      DEBUG_OUT<<"immw";
#endif // DEBUG_DIS_MODES
      break;
    case E65816_ABS:
#ifdef DEBUG_DIS_MODES
      DEBUG_OUT<<"abs";
#endif // DEBUG_DIS_MODES
      break;
    case E65816_ABSL:
#ifdef DEBUG_DIS_MODES
      DEBUG_OUT<<"absl";
#endif // DEBUG_DIS_MODES
      break;
    case E65816_ABSLX:
#ifdef DEBUG_DIS_MODES
      DEBUG_OUT<<"abslx";
#endif // DEBUG_DIS_MODES
      break;
    case E65816_ABSX:
#ifdef DEBUG_DIS_MODES
      DEBUG_OUT<<"absx";
#endif // DEBUG_DIS_MODES
      break;
    case E65816_ABSY:
#ifdef DEBUG_DIS_MODES
      DEBUG_OUT<<"absy";
#endif // DEBUG_DIS_MODES
      break;
    case E65816_IND:
#ifdef DEBUG_DIS_MODES
      DEBUG_OUT<<"ind";
#endif // DEBUG_DIS_MODES
      break;
    case E65816_INDX:
#ifdef DEBUG_DIS_MODES
      DEBUG_OUT<<"indx";
#endif // DEBUG_DIS_MODES
      break;
    case E65816_INDY:
#ifdef DEBUG_DIS_MODES
      DEBUG_OUT<<"indy";
#endif // DEBUG_DIS_MODES
      break;
    case E65816_BXX:
#ifdef DEBUG_DIS_MODES
      DEBUG_OUT<<"bxx";
#endif // DEBUG_DIS_MODES
      break;
    case E65816_JABS:
#ifdef DEBUG_DIS_MODES
      DEBUG_OUT<<"jabs";
#endif // DEBUG_DIS_MODES
      break;
    case E65816_JIND:
#ifdef DEBUG_DIS_MODES
      DEBUG_OUT<<"jind";
#endif // DEBUG_DIS_MODES
      break;
    case E65816_JABSL:
#ifdef DEBUG_DIS_MODES
      DEBUG_OUT<<"jabsl";
#endif // DEBUG_DIS_MODES
      break;
    case E65816_SR:
#ifdef DEBUG_DIS_MODES
      DEBUG_OUT<<"sr";
#endif // DEBUG_DIS_MODES
      break;
    default:
      break;
  };
#ifdef DEBUG_DIS_MODES
  DEBUG_OUT<<"  ";
#endif // DEBUG_DIS_MODES
}


ULONG Em65816::disasm(ULONG loc)
{
  //unsigned int dig=w->getHexDigits();
  const char *name="???";
  unsigned int mode=E65816_NONE;
  topcode=READ_CODE(loc);
  // TODO: this doesn't take the bank into account
  lastSymbol=map->findAnyExactSymbol(loc);
  if(lastSymbol) DEBUG_OUT<<lastSymbol->getName()<<":\n";
  DEBUG_OUT<<"";
  DEBUG_OUT<<".";
  //w->setHexDigits(2);
  DEBUG_OUT<<pbr<<":";
  //w->setHexDigits(4);
  DEBUG_OUT<<loc<<"  ";
  //w->setHexDigits(dig);
  op_len=Em65816_size[topcode];
  mode=Em65816_format[topcode];
  name=Em65816_name[topcode];
  if(!op_len) op_len=1;  // NOTE: An unimplemented opcode...
  unsigned int oldOpLen=op_len;  // TODO: kludge
  fixOpLen(mode);
  unsigned int t;
  for(t=0;t<op_len;t++) {
    int n=READ_CODE(loc+t);
    if(n<0x10) DEBUG_OUT<<"0";
    DEBUG_OUT<<n<<" ";
  }
  for(t=0;t<(4-op_len);t++) DEBUG_OUT<<"   ";  // NOTE: the 3 is now a 4
  DEBUG_OUT<<name<<" ";
  disasmAddrMode(loc,mode);
  DEBUG_OUT<<"";
  DEBUG_OUT<<"\n";
  //w->setHexDigits(dig);
  //op_len=oldOpLen;
  return op_len;
}


void Em65816::executeInstead(ULONG opcode2)
{
  opcode=opcode2;
  op_len=Em65816_size[opcode];
  cycle=Em65816_time[opcode];
  if(!execMain()) exec_unimplemented(opcode);
  // NOTE: we fall back to the regular execute code here...
}


void Em65816::execute()
{
  if(pc==breakpoint) { debugger("Breakpoint!\n"); return; }
  opcode=READ_CODE(pc);
  op_len=Em65816_size[opcode];
  cycle=Em65816_time[opcode];
  if(!execMain()) exec_unimplemented(opcode);
#ifdef DEBUG_EXEC
  DEBUG_OUT<<"  a="<<a<<" x="<<x<<" y="<<y<<" s="<<sp<<"\n";
#endif // DEBUG_EXEC
  pc+=op_len;
  cycles+=cycle;
  if(int_pending) handle_int(int_pending);
}


void Em65816::disasmAddrMode(ULONG loc,unsigned int mode)
{
  //unsigned int dig=w->getHexDigits();
  //w->setHexDigits(4);
  switch(mode) {
    case E65816_NONE:
      break;
    case E65816_ZP:
      DEBUG_OUT<<"$"<<(short)READ_CODE(loc+1);
      break;
    case E65816_ZPX:
      DEBUG_OUT<<"$"<<(short)READ_CODE(loc+1)<<",X";
      break;
    case E65816_ZPY:
      DEBUG_OUT<<"$"<<(short)READ_CODE(loc+1)<<",Y";
      break;
    case E65816_IMM:
      DEBUG_OUT<<"#"<<(short)READ_CODE(loc+1);
      break;
    case E65816_IMMA:
      // NOTE: dis won't show extra byte
      if(m16Bit) { DEBUG_OUT<<"#"<<(USHORT)READW_CODE(loc+1); }
      else DEBUG_OUT<<"#"<<(short)READ_CODE(loc+1);
      break;
    case E65816_IMMX:
      // NOTE: dis won't show extra byte
      if(x16Bit) { DEBUG_OUT<<"#"<<(USHORT)READW_CODE(loc+1); }
      else DEBUG_OUT<<"#"<<(short)READ_CODE(loc+1);
      break;
    case E65816_IMMY:
      // NOTE: dis won't show extra byte
      if(x16Bit) { DEBUG_OUT<<"#"<<(USHORT)READW_CODE(loc+1);}
      else DEBUG_OUT<<"#"<<(short)READ_CODE(loc+1);
      break;
    case E65816_IMMW:
      DEBUG_OUT<<"#"<<(USHORT)READW_CODE(loc+1);
      break;
    case E65816_ABS:
      DEBUG_OUT<<"$"<<(USHORT)READW_CODE(loc+1);
      break;
    case E65816_ABSL:
      DEBUG_OUT<<"$"<<(ULONG)read24code(loc+1);
      break;
    case E65816_ABSLX:
      DEBUG_OUT<<"$"<<(ULONG)read24code(loc+1)<<",X";
      break;
    case E65816_ABSX:
      DEBUG_OUT<<"$"<<(USHORT)READW_CODE(loc+1)<<",X";
      break;
    case E65816_ABSY:
      DEBUG_OUT<<"$"<<(USHORT)READW_CODE(loc+1)<<",Y";
      break;
    case E65816_IND:
      DEBUG_OUT<<"($"<<(short)READ_CODE(loc+1)<<")";
      break;
    case E65816_INDX:
      DEBUG_OUT<<"($"<<(short)READ_CODE(loc+1)<<",X)";
      break;
    case E65816_INDY:
      DEBUG_OUT<<"($"<<(short)READ_CODE(loc+1)<<"),Y";
      break;
    case E65816_BXX:
      DEBUG_OUT<<"$"<<(short)READ_CODE(loc+1)<<" ;"<<(UINT16)((SBYTE)READ_CODE(loc+1)+loc+2);
      break;
    case E65816_JABS:
      DEBUG_OUT<<"$"<<(USHORT)READW_CODE(loc+1);
      break;
    case E65816_JIND:
      DEBUG_OUT<<"($"<<(USHORT)READW_CODE(loc+1)<<")";
      break;
    case E65816_JABSL:
      //w->setHexDigits(6);
      DEBUG_OUT<<"$"<<(ULONG)read24code(loc+1);
      break;
    case E65816_SR:
      DEBUG_OUT<<"#"<<(short)READ_CODE(loc+1)<<",S";
      break;
    default:
      DEBUG_OUT<<"???";
      break;
  };
  //w->setHexDigits(dig);
}


// TODO: doesn't differentiate between native and emulation int vectors...
void Em65816::handle_brk()
{
  bool ret=false;
  if(io) ret=io->systemCall();
  if(!ret) {
    debugger("BRK!");
    /*
    pc++;
    if(nativeMode) {
      handle_int(E65816_INT_NBRK);
    }
    else {
      setStatusBits(E65816_STAT_Break);
      handle_int(E65816_INT_EIRQ);
    }
    */
  }
}


// TODO: Move pending stuff to cause_int...
// TODO: doesn't differentiate between native and emulation int vectors...
void Em65816::handle_int(ULONG value)
{
  return; // for now...
  int_pending=0;
  bool intDis=false;
  if(intLock) intDis=true;
  if(getStatusReg()&E65816_STAT_I) intDis=true;
  if(intDis) {
    if(value==E65816_INT_EIRQ) {
      if(!(getStatusReg()&E65816_STAT_Break)) {
        //DEBUG_OUT<<"EIRQ requested...but ints are disabled.\n";
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
  setStatusBits(E65816_STAT_I);
  jump(READW_CODE(value&map->getAddrMask()));
  //trace=true;
}


// TODO: Check to see if ints are disabled, and set
// pending if so and don't call handle_int until later...
// TODO: doesn't differentiate between native and emulation int vectors...
void Em65816::cause_int(ULONG value)
{
  int_pending=0;
  switch(value) {
    case E65816_INT_RESET:
      reset();
      break;
    case E65816_INT_ENMI:
      resetStatusBits(E65816_STAT_Break);
      handle_int(value);
      break;
    case E65816_INT_EIRQ:
      resetStatusBits(E65816_STAT_Break);
      handle_int(value);
      break;
    default:
      debugger("Illegal interrupt requested!\n");
      break;
  }
}


// TODO: This doesn't handle the 16-bit stack at all...
void Em65816::changeMap(EMemMapAbstract *mmap)
{
}


unsigned int Em65816::read24code(ULONG addr)
{
  unsigned int val=READW_CODE(addr);
  BYTE highByte=READ_CODE(addr+2);
  val|=(highByte<<16)&0xff0000;
  val&=0xffffff;
  return val;
}


unsigned int Em65816::read24data(ULONG addr)
{
  unsigned int val=READW_DATA(addr);
  BYTE highByte=READ_DATA(addr+2);
  val|=(highByte<<16)&0xff0000;
  val&=0xffffff;
  return val;
}


void Em65816::setStatusBits(BYTE mask)
{
  BYTE oldStatus=getStatusReg();
  setStatusReg(oldStatus|mask);
}


void Em65816::resetStatusBits(BYTE mask)
{
  BYTE oldStatus=getStatusReg();
  setStatusReg(oldStatus&(0xff-mask));
}


void Em65816::setStatusReg(unsigned int newValue)
{
  // TODO: if in emu mode, don't let extra or b bit change
  // TODO: if in native, if m or x changed, change regs to match...
  statusReg=newValue;
  if(nativeMode) {
    if(statusReg&E65816_STAT_M) {
      m16Bit=false; mSizeMask=ECPUABSTRACT_MASK_BYTE;
      mHighBit=0x80; mHalfBit=0x40;
    }
    else {
      m16Bit=true; mSizeMask=ECPUABSTRACT_MASK_WORD;
      mHighBit=0x8000; mHalfBit=0x4000;
    }
    if(statusReg&E65816_STAT_X) {
      x16Bit=false; xSizeMask=ECPUABSTRACT_MASK_BYTE;
      xHighBit=0x80; xHalfBit=0x40;
    }
    else {
      x16Bit=true; xSizeMask=ECPUABSTRACT_MASK_WORD;
      xHighBit=0x8000; xHalfBit=0x4000;
    }
  }
}


BYTE Em65816::getStatusReg()
{
  return statusReg;
}


void Em65816::stack_push(BYTE val)
{
  WRITE_DATA(stackBase+sp,val);
  stack_dec();
}


BYTE Em65816::stack_pop()
{
  stack_inc();
  BYTE val=READ_DATA(stackBase+sp);
  return val;
}


// TODO: doesn't differentiate between native and emulation int vectors...
void Em65816::ints()
{
  DEBUG_OUT<<"EIRQ ("<<(unsigned long)E65816_INT_EIRQ<<"): "
    <<(unsigned int)READW_CODE(E65816_INT_EIRQ)<<"\n";
  DEBUG_OUT<<"ENMI ("<<(unsigned long)E65816_INT_ENMI<<"): "
    <<(unsigned int)READW_CODE(E65816_INT_ENMI)<<"\n";
  DEBUG_OUT<<"RESET ("<<(unsigned long)E65816_INT_RESET<<"): "
    <<(unsigned int)READW_CODE(E65816_INT_RESET)<<"\n";
}


// TODO: doesn't differentiate between native and emulation int vectors...
void Em65816::nmi()
{
  intLock=false;
  if(map) pc=READW_CODE(E65816_INT_ENMI);
  clearStackTrace();
  jump(pc);
}


unsigned int Em65816::readv(ULONG addr,unsigned int size)
{
  if(!size) return map->read16l(addr);
  return map->read8(addr);
}


void Em65816::writev(ULONG addr,unsigned int val,unsigned int size)
{
  if(!size) map->write16l(addr,val);
  else map->write8(addr,val);
}


unsigned int Em65816::mapRead8(ULONG addr,bool echo)
{
  unsigned int val=map->read8(addr);
  if(echo) DEBUG_OUT<<"  read8 "<<val<<" from "<<addr<<"\n";
  return val;
}


unsigned int Em65816::mapRead16l(ULONG addr,bool echo)
{
  unsigned int val=map->read16l(addr);
  if(echo) DEBUG_OUT<<"  read16 "<<val<<" from "<<addr<<"\n";
  return val;
}


void Em65816::mapWrite8(ULONG addr,unsigned int val,bool echo)
{
  if(echo) DEBUG_OUT<<"  write8 "<<val<<" to "<<addr<<"\n";
  map->write8(addr,val);
}


void Em65816::mapWrite16l(ULONG addr,unsigned int val,bool echo)
{
  if(echo) DEBUG_OUT<<"  write16 "<<val<<" to "<<addr<<"\n";
  map->write16l(addr,val);
}
