
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


//#define SHOW_INTS  1
//#define CHECK_STACK  1


#include <cpulib/Em6502.h>

#include <elib/EMemMapAbstract.h>
#include <elib/EModule.h>
#include <elib/EHandler.h>

#include <portable/nickcpp/ANullStream.h>

#include <portable/nickcpp/AStringHelper.h>


#include <cpulib/generated/Em6502_tab.h>


#define E_CPU_NONE     0x00
#define E_CPU_6502     0x01


#define MSIZEMASK ECPUABSTRACT_MASK_BYTE
#define XSIZEMASK ECPUABSTRACT_MASK_BYTE
#define M16BIT true
#define X16BIT true
#define MHIGHBIT 0x80
#define XHIGHBIT 0x80
#define MHALFBIT 0x40
#define XHALFBIT 0x40

#define READ_DATA(q) (map->read8(q))
#define READW_DATA(q) (map->read16l(q))
#define WRITE_DATA(q,v) (map->write8(q,v))
#define WRITEW_DATA(q,v) (map->write16l(q,v))
#define READ_CODE(q) (map->read8(q))
#define READW_CODE(q) (map->read16l(q))
#define WRITE_CODE(q,v) (map->write8(q,v))
#define WRITEW_CODE(q,v) (map->write16l(q,v))

#define SET_A(q) a=(q)
#define SET_X(q) x=(q)
#define SET_Y(q) y=(q)
#define SET_Z(q)

#define STACK_PUSH(q) stack_push(q)
#define STACK_PUSHW(q) stack_pushw(q)
#define STACK_POP() stack_pop()
#define STACK_POPW() stack_popw()

#define STORE_A(q) WRITE_DATA(q,a)
#define STORE_X(q) WRITE_DATA(q,x)
#define STORE_Y(q) WRITE_DATA(q,y)
#define STORE_Z(q)
#define LOAD_A(q) SET_A(READ_DATA(q))
#define LOAD_X(q) SET_X(READ_DATA(q))
#define LOAD_Y(q) SET_Y(READ_DATA(q))
#define LOAD_Z(q) SET_Z(READ_DATA(q))

#define ADDR_ZP()   (UINT16)(map->read8(pc+1))
#define ADDR_ZPX()  (UINT16)((ADDR_ZP()+x)&0xff)
#define ADDR_ZPY()  (UINT16)((ADDR_ZP()+y)&0xff)
#define ADDR_ZPZ()  (UINT16)((ADDR_ZP()+z)&0xff)
#define ADDR_INDX() (UINT16)map->read16l(ADDR_ZPX())
#define ADDR_INDY() (UINT16)(map->read16l(ADDR_ZP())+y)
#define ADDR_INDSPY() (UINT16)(map->read16l(ADDR_ABSB()+s)+y)
#define ADDR_ABS()  (UINT16)map->read16l(pc+1)
#define ADDR_ABSL()  (UINT16)read24code(pc+1)
#define ADDR_ABSB()  (UINT16)map->read8(pc+1)
#define ADDR_ABSX() (UINT16)(ADDR_ABS()+x)
#define ADDR_ABSY() (UINT16)(ADDR_ABS()+y)
#define ADDR_ABSINDX() (UINT16)map->read16l(ADDR_ABSX())


// TODO: Some of the data vs code stuff may be wrong...
#define VAL_IMM8()  READ_CODE(pc+1)
#define VAL_IMM16()  READW_CODE(pc+1)
#define VAL_IMM(n)  READ_CODE(pc+1)
#define VAL_ZP(n)   (map->read8(ADDR_ZP()))
#define VAL_ZPX(n)  (map->read8(ADDR_ZPX()))
#define VAL_ZPY(n)  (map->read8(ADDR_ZPY()))
#define VAL_ZPZ(n)  (map->read8(ADDR_ZPZ()))
#define VAL_INDX(n) (map->read8(ADDR_INDX()))
#define VAL_INDY(n) (map->read8(ADDR_INDY()))
#define VAL_INDSPY(n) (map->read8(ADDR_INDSPY()))
#define VAL_ABS(n)  (map->read8(ADDR_ABS()))
#define VAL_ABSL(n)  (map->read8(ADDR_ABSL()))
#define VAL_ABSB(n)  (map->read8(ADDR_ABSB()))
#define VAL_ABSX(n) (map->read8(ADDR_ABSX()))
#define VAL_ABSY(n) (map->read8(ADDR_ABSY()))
#define VAL_ABSINDX(n) (map->read8(ADDR_ABSINDX()))


#define ADJ_OP_LEN(n)


////////////////////////////////////////////////////////////////////////////////
//  Em6502 Class
////////////////////////////////////////////////////////////////////////////////


// This will bring in the class' execMain func
#define theClassType Em6502
#define e6502
#include <cpulib/Em65_core.h>
#undef e6502
#undef theClassType


Em6502::Em6502()
{
  DEBUG_OUT<<"Em6502 created...\n";
  init();
  type=E_CPU_6502;
  //w=ww;
  reset();
}


Em6502::~Em6502()
{
  DEBUG_OUT<<"Em6502 destroyed.\n";
}


void Em6502::init()
{
  ECPU8Bit::init();
  // Generic features
  doTrace=false;  doRun=false;
  // Set up stack and registers
  sbits=(Em6502StatusBits *)&statusReg;
  int_pending=0;
  intLock=false;
  lsb=true;
  stackBase=0x0100;
  stackMask=0x00ff;
  stackCorrection=1;
  setStack(stackBase);
  setFramePtr(stackBase);
  setResetStack(stackBase);
  setResetEntry(0);
}


/* STATIC */
bool Em6502::recognize(const char *tname)
{
  const char *name=aDefaultStringHelper.removeSpaces2(aDefaultStringHelper.toLower(tname));
  if(!strcmp(name,"6502")) return true; else
  if(!strcmp(name,"6502a")) return true; else
  if(!strcmp(name,"6502b")) return true; else
  if(!strcmp(name,"65C102")) return true; else
  if(!strcmp(name,"6502c")) return true; else
  if(!strcmp(name,"6508")) return true; else
  if(!strcmp(name,"6507")) return true; else
  if(!strcmp(name,"6510")) return true; else
  if(!strcmp(name,"7501")) return true; else
  if(!strcmp(name,"7502")) return true; else
  if(!strcmp(name,"7510")) return true; else
  if(!strcmp(name,"8500")) return true; else
  if(!strcmp(name,"8501")) return true; else
  if(!strcmp(name,"8502")) return true; else
  if(!strcmp(name,"8510")) return true; else
  if(!strcmp(name,"6280")) return true; else
  if(!strcmp(name,"2a03")) return true; else
  if(!strcmp(name,"2a07")) return true; else
  if(!strcmp(name,"huc6280")) return true; else
  return false;
}


const char *Em6502::getName()
{
  return "MOS 6502";
}


void Em6502::setKind(const char *name)
{
  type=E_CPU_6502;
}


void Em6502::reset()
{
  a=0; x=0; y=0;
  statusReg=0;  setStatusReg(0);
  sp=0;  // Only last 8 bits of it...
  int_pending=0;
  intLock=false;
  setStatusBits(E6502_STAT_I);
  if(map) {
    pc=READW_CODE(E6502_INT_RESET);
    DEBUG_OUT<<"Reading reset vector and calling setResetEntry("<<pc<<")\n";
    setResetEntry(pc);
  }
  clearStackTrace();
  ECPUAbstract::reset();
}


void Em6502::nmi()
{
  intLock=false;
  if(map) pc=READW_CODE(E6502_INT_NMI);
  clearStackTrace();
  jump(pc);
}


void Em6502::changeMap(EMemMapAbstract *mmap)
{
}


bool Em6502::setRegister(const char *tname,ULONG value)
{
  const char *name=aDefaultStringHelper.removeSpaces2(aDefaultStringHelper.toLower(tname));
  bool ret=false;
  if(!strcmp(name,"pc"))
  { pc=(UINT16)value; ret=true; } else
  if(!strcmp(name,"a"))
  { a=(BYTE)value; ret=true; } else
  if(!strcmp(name,"x"))
  { x=(BYTE)value; ret=true; } else
  if(!strcmp(name,"y"))
  { y=(BYTE)value; ret=true; } else
  if(!strcmp(name,"p"))
  { setStatusReg(value); ret=true; } else
  if(!strcmp(name,"s"))
  { sp=val; ret=true; }
  //free(name);
  return ret;
}


// TODO: finish this
ULONG Em6502::readRegister(const char *tname)
{
  const char *name=aDefaultStringHelper.removeSpaces2(aDefaultStringHelper.toLower(tname));
  if(name[0]=='a') return a;
  if(name[0]=='x') return x;
  if(name[0]=='y') return y;
  return 0;
}


void Em6502::wregs()
{
  //unsigned int dig=//w->getHexDigits();
  //w->setHexDigits(4);
  DEBUG_OUT<<"PC="<<(unsigned short)pc<<"\n";
  //w->setHexDigits(2);
  DEBUG_OUT<<"A="<<(short)a<<"  X="<<(short)x<<"  Y="<<(short)y<<"\n";
  //w->setHexDigits(4);
  DEBUG_OUT<<"SP="<<sp;
  DEBUG_OUT<<"  (base="<<stackBase<<" mask="<<stackMask<<")\n";
  //w->setHexDigits(2);
  BYTE p=getStatusReg();
  DEBUG_OUT<<"P="<<(short)p<<"  (";
  if(p&E6502_STAT_N) DEBUG_OUT<<"N";  else DEBUG_OUT<<"n";
  if(p&E6502_STAT_V) DEBUG_OUT<<"V";  else DEBUG_OUT<<"v";
  DEBUG_OUT<<"-";
  if(p&E6502_STAT_Break) DEBUG_OUT<<"B";  else DEBUG_OUT<<"b";
  if(p&E6502_STAT_D) DEBUG_OUT<<"D";  else DEBUG_OUT<<"d";
  if(p&E6502_STAT_I) DEBUG_OUT<<"I";  else DEBUG_OUT<<"i";
  if(p&E6502_STAT_Z) DEBUG_OUT<<"Z";  else DEBUG_OUT<<"z";
  if(p&E6502_STAT_C) DEBUG_OUT<<"C";  else DEBUG_OUT<<"c";
  DEBUG_OUT<<")\n";
  //w->setHexDigits(dig);
}


void Em6502::set_flags_nz(BYTE i)
{
  //if(trace) DEBUG_OUT<<"set_flags_nz ("<<p;
  sbits->z=(i==0);
  sbits->n=(i&0x80)!=0;
  //if(trace) DEBUG_OUT<<"->"<<p<<")\n";
}


void Em6502::set_flags_nzc(BYTE i, BYTE j)
{
  //if(trace) DEBUG_OUT<<"set_flags_nzc ("<<p;
  UINT16 val=(UINT16)i-(UINT16)j;
  sbits->z=(val==0);
  sbits->n=(val&0x80)!=0;
  sbits->c=(val<(UINT16)0x100);
  //if(trace) DEBUG_OUT<<"->"<<p<<")\n";
}


// TODO: This version ignores the sizemask...
void Em6502::set_flags_nz(UINT16 i,UINT16 sizemask)
{
  //if(trace) DEBUG_OUT<<"set_flags_nz ("<<p;
  sbits->z=(i==0);
  sbits->n=(i&0x80)!=0;
  //if(trace) DEBUG_OUT<<"->"<<p<<")\n";
}


// TODO: This version ignores the sizemask...
void Em6502::set_flags_nzc(UINT16 i, UINT16 j,UINT16 sizemask)
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
#define set_flags_nzc(x,y,z) set_flags_nzc(x,y)
#define set_flags_nz(x,y) set_flags_nz(x)


// TODO: Move pending stuff to cause_int...
void Em6502::handle_int(ULONG value)
{
  //DEBUG_OUT<<"Em6502::handle_int...\n";
#ifdef SHOW_INTS
  DEBUG_OUT<<"-";
#endif // SHOW_INTS
  int_pending=0;
  bool intDis=false;
  if(intLock) {
    intDis=true;
    DEBUG_OUT<<"6502 int, but intLock set!\n";
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
  stack_pushw(pc);
  //DEBUG_OUT<<"Pushed "<<pc<<"\n";
  stack_push(getStatusReg());
  setStatusBits(E6502_STAT_I);
  jump(READW_CODE(value));
  //trace=true;
}


// TODO: Check to see if ints are disabled, and set
// pending if so and don't call handle_int until later...
void Em6502::cause_int(ULONG value)
{
  int_pending=0;
  switch(value) {
    case E6502_INT_RESET:
      reset();
      break;
    case E6502_INT_NMI:
      resetStatusBits(E6502_STAT_Break);
      handle_int(value);
      break;
    case E6502_INT_IRQ:
      resetStatusBits(E6502_STAT_Break);
      handle_int(value);
      break;
    default:
      debugger("Illegal interrupt requested!\n");
      break;
  }
}


void Em6502::ints()
{
  DEBUG_OUT<<"IRQ ("<<(unsigned short)E6502_INT_IRQ<<"): "
    <<(unsigned int)READW_CODE(E6502_INT_IRQ)<<"\n";
  DEBUG_OUT<<"NMI ("<<(unsigned short)E6502_INT_NMI<<"): "
    <<(unsigned int)READW_CODE(E6502_INT_NMI)<<"\n";
  DEBUG_OUT<<"RESET ("<<(unsigned short)E6502_INT_RESET<<"): "
    <<(unsigned int)READW_CODE(E6502_INT_RESET)<<"\n";
}


void Em6502::executeInstead(ULONG opcode2)
{
  opcode=opcode2;
  op_len=Em6502_size[opcode];
  cycle=Em6502_time[opcode];
  if(!execMain()) exec_unimplemented(opcode);
  // NOTE: we fall back to the regular execute code here...
}


void Em6502::execute()
{
  if(pc==breakpoint) { debugger("Breakpoint!\n"); return; }
  opcode=READ_CODE(pc);
  op_len=Em6502_size[opcode];
  cycle=Em6502_time[opcode];
  if(!execMain()) exec_unimplemented(opcode);
  pc+=op_len;
  cycles+=cycle;
  if(int_pending) handle_int(int_pending);
}


ULONG Em6502::disasm(ULONG loc)
{
  unsigned int len=0,mode=E6502_NONE;
  const char *name="???";
  topcode=READ_CODE(loc);
  lastSymbol=map->findAnyExactSymbol(loc);
  if(lastSymbol) DEBUG_OUT<<lastSymbol->getName()<<":\n";
  DEBUG_OUT<<"."; DEBUG_OUT<<loc;  DEBUG_OUT<<"  ";
  len=Em6502_size[topcode];
  mode=Em6502_format[topcode];
  name=Em6502_name[topcode];
  if(!len) len=1;  // NOTE: An unimplemented opcode...
  unsigned int t;
  for(t=0;t<len;t++) DEBUG_OUT<<aDefaultStringHelper.myHex2(READ_CODE(loc+t))<<" ";
  for(t=0;t<(3-len);t++) DEBUG_OUT<<"   ";
  DEBUG_OUT<<name<<" ";
  disasmAddrMode(loc,mode);
  DEBUG_OUT<<"";
  DEBUG_OUT<<"\n";
  return len;
}


void Em6502::disasmAddrMode(ULONG loc,unsigned int mode)
{
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
    case E6502_IMM:
      DEBUG_OUT<<"#"<<(short)READ_CODE(loc+1);
      break;
    case E6502_ABS:
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
    case E6502_BXX:
      DEBUG_OUT<<"$"<<(short)READ_CODE(loc+1)<<"  ;"<<(UINT16)((SBYTE)READ_CODE(loc+1)+loc+2);
      break;
    case E6502_JABS:
      DEBUG_OUT<<"$"<<(short)READW_CODE(loc+1);
      break;
    case E6502_JIND:
      DEBUG_OUT<<"($"<<(short)READW_CODE(loc+1)<<")";
      break;
    default:
      DEBUG_OUT<<"???";
      break;
  };
}


void Em6502::handle_brk()
{
  //DEBUG_OUT<<"Em6502::handle_brk...\n";
  bool ret=false;
  if(io) ret=io->systemCall();
  if(!ret) {
    //DEBUG_OUT<<"(going to call handle_int since systemCall returned false)\n";
    pc++;
    setStatusBits(E6502_STAT_Break);
    handle_int(E6502_INT_IRQ);
  }
}


void Em6502::setStatusReg(unsigned int newValue)
{
  statusReg=newValue;
}


BYTE Em6502::getStatusReg()
{
  return statusReg;
}


void Em6502::setStatusBits(BYTE mask)
{
  BYTE oldStatus=statusReg;
  statusReg=oldStatus|mask;
}


void Em6502::resetStatusBits(BYTE mask)
{
  BYTE oldStatus=statusReg;
  statusReg=oldStatus&(0xff-mask);
}

