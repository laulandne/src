
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


//#define SHOW_INTS  1
//#define CHECK_STACK  1


#include <cpulib/Em65c02.h>

#include <elib/EMemMapAbstract.h>
#include <elib/EModule.h>
#include <elib/EHandler.h>

#include <portable/nickcpp/ANullStream.h>

#include <portable/nickcpp/AStringHelper.h>


#include <cpulib/generated/Em65c02_tab.h>


#define E_CPU_NONE     0x00
#define E_CPU_65C02    0x02


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


///////////////////////////////////////////////////////////////////////////////
//  Em65c02 Class
///////////////////////////////////////////////////////////////////////////////

Em65c02::Em65c02()
{
  DEBUG_OUT<<"Em65c02 created...\n";
  Em6502::init();
  reset();
}


Em65c02::~Em65c02()
{
  DEBUG_OUT<<"Em65c02 destroyed.\n";
}


/* STATIC */
bool Em65c02::recognize(const char *tname)
{
  const char *name=aDefaultStringHelper.removeSpaces2(aDefaultStringHelper.toLower(tname));
  if(!strcmp(name,"65c02")) return true;
  return false;
}


const char *Em65c02::getName()
{
  return "M65c02";
}


void Em65c02::setKind(const char *tname)
{
  const char *name=aDefaultStringHelper.removeSpaces2(aDefaultStringHelper.toLower(tname));
  type=E_CPU_65C02;
}


void Em65c02::reset()
{
  Em6502::reset();
}


void Em65c02::wregs()
{
  Em6502::wregs();
}


ULONG Em65c02::disasm(ULONG loc)
{
  BYTE topcode;
  unsigned int len=0,mode=E6502_NONE;
  const char *name="???";
  topcode=READ_CODE(loc);
  lastSymbol=map->findAnyExactSymbol(loc);
  if(lastSymbol) DEBUG_OUT<<lastSymbol->getName()<<":\n";
  DEBUG_OUT<<"";
  DEBUG_OUT<<"."; DEBUG_OUT<<loc;  DEBUG_OUT<<"  ";
  len=Em65c02_size[topcode];
  mode=Em65c02_format[topcode];
  name=Em65c02_name[topcode];
  if(!len) len=1;  // NOTE: An unimplemented opcode...
  unsigned int t;
  for(t=0;t<len;t++) DEBUG_OUT<<(short)READ_CODE(loc+t)<<" ";
  for(t=0;t<(3-len);t++) DEBUG_OUT<<"   ";
  DEBUG_OUT<<name<<" ";
  disasmAddrMode(loc,mode);
  DEBUG_OUT<<"";
  DEBUG_OUT<<"\n";
  return len;
}


void Em65c02::executeInstead(ULONG opcode2)
{
  opcode=opcode2;
  op_len=Em65c02_size[opcode];
  cycle=Em65c02_time[opcode];
  if(!execMain()) exec_unimplemented(opcode);
  // NOTE: We fall back to the regular execute code here...
}


void Em65c02::execute()
{
  if(pc==breakpoint) { debugger("Breakpoint!\n"); return; }
  opcode=READ_CODE(pc);
  op_len=Em65c02_size[opcode];
  cycle=Em65c02_time[opcode];
  if(!execMain()) exec_unimplemented(opcode);
  pc+=op_len;
  cycles+=cycle;
  if(int_pending) handle_int(int_pending);
}


// This will bring in the class' execMain func
#define theClassType Em65c02
#define e65c02
#include <cpulib/Em65_core.h>
#undef e65c02
#undef theClassType


