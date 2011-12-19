
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#define DEBUG_DIS_MODES 1

#define DEBUG_EXEC 1

#define CHECK_SAFE 1

#define SHOW_INTS  1

#define CHECK_STACK  1


#include <cpulib/Em6509.h>

#include <elib/EMemMapAbstract.h>
#include <elib/EModule.h>
#include <elib/EHandler.h>

#include <portable/nickcpp/ANullStream.h>

#include <portable/nickcpp/AStringHelper.h>


#define E_CPU_NONE     0x00
#define E_CPU_6509     0x03


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


///////////////////////////////////////////////////////////////////////////////
//  E6509 Class
///////////////////////////////////////////////////////////////////////////////

/*
// Yuck!  Crude, but it works...
#define E6502_size E6509_size
#define E6502_format E6509_format
#define E6502_subformat E6509_subformat
#define E6502_flags E6509_flags
#define E6502_name E6509_name
#define E6502_time E6509_time
#include <cpulib/generated/E6502_tab.h>


E6509::E6509()
{
  DEBUG_OUT<<"E6509 created...\n";
  E6502::init();
  reset();
}


E6509::~E6509()
{
  DEBUG_OUT<<"E6509 destroyed.\n";
}


bool E6509::recognize(char *tname)
{
  char *name=aDefaultStringHelper.removeSpaces2(aDefaultStringHelper.toLower(tname));
  if(!strcmp(name,"6509"))) return true;
  return false;
}


char *E6509::getName()
{
  return "M6509");
}


void E6509::setKind(char *tname)
{
  char *name=aDefaultStringHelper.removeSpaces2(aDefaultStringHelper.toLower(tname));
  type=E_CPU_6509;
}


void E6509::reset()
{
  E6502::reset();
  dp=0;
  dbr=0xf0000;
  pbr=0xf0000;
  indexedOffset=0;
  m16Bit=false;  x16Bit=false;
  mSizeMask=ECPUABSTRACT_MASK_BYTE; xSizeMask=ECPUABSTRACT_MASK_BYTE;
  mHighBit=0x80;  xHighBit=0x80;
  mHalfBit=0x40;  xHalfBit=0x40;
  if(map) pc=READW_CODE(E6502_INT_RESET);
}


void E6509::wregs()
{
  E6502::wregs();
  DEBUG_OUT<<"indexedOffset="<<indexedOffset<<"\n";
  DEBUG_OUT<<"PBR="<<pbr<<"  DBR="<<dbr<<"\n";
}


void E6509::stack_push(BYTE val)
{
  WRITE_DATA(stackBase+sp,val);
  stack_dec();
}


BYTE E6509::stack_pop()
{
  stack_inc();
  BYTE val=READ_DATA(stackBase+sp);
  return val;
}


void E6509::setMachineSpecificRegister(unsigned int n,unsigned int val)
{
  switch(n) {
    case 0:  // execBank
      pbr=((val&0xff)<<16);
      dbr=((val&0xff)<<16);
      DEBUG_OUT<<"pbr set to "<<pbr<<"\n";
      DEBUG_OUT<<"dbr set to "<<dbr<<"\n";
      break;
    case 1:  // indBank
      indexedOffset=((val&0xff)<<16);
      // DEBUG_OUT<<"indexedOffset set to "<<indexedOffset<<"\n";
      break;
    default:
      debugger("Bad machineSpecificRegister!");
      break;
  }
}


unsigned int E6509::readMachineSpecificRegister(unsigned int n)
{
  unsigned int val=0;
  switch(n) {
    case 0:  // execBank
      val=(pbr>>16)&0xff;
      break;
    case 1:  // indBank
      val=(indexedOffset>>16)&0xff;
      break;
    default:
      debugger("Bad machineSpecificRegister!");
      break;
  }
  return val;
}


// TODO: Move pending stuff to cause_int...
// TODO: doesn't differentiate between native and emulation int vectors...
void E6509::handle_int(ULONG value)
{
  return; // for now...
  int_pending=0;
  bool intDis=false;
  if(intLock) intDis=true;
  if(getStatusReg()&E6502_STAT_I) intDis=true;
  if(intDis) {
    if(value==E6502_INT_IRQ) {
      if(!(getStatusReg()&E6502_STAT_Break)) {
        // DEBUG_OUT<<"EIRQ requested...but ints are disabled.\n";
        //debugger("Interrupts are disabled.\n");
        int_pending=value;
#ifdef SHOW_INTS
        DEBUG_OUT<<"x";
#endif
        return;
      }
      else {
        // DEBUG_OUT<<"BRK with ints disabled!\n";
        //return;
      }
    }
  }
#ifdef SHOW_INTS
  DEBUG_OUT<<"+";
#endif
  STACK_PUSHW(pc);
  // DEBUG_OUT<<"Pushed "<<pc<<"\n";
  STACK_PUSH(getStatusReg());
  setStatusBits(E6502_STAT_I);
  jump(READW_CODE(value&map->getMask()));
  //trace=true;
}


// TODO: Check to see if ints are disabled, and set
// pending if so and don't call handle_int until later...
// TODO: doesn't differentiate between native and emulation int vectors...

ULONG E6509::disasm(ULONG loc)
{
  BYTE topcode;
  unsigned int len=0,mode=E6502_NONE;
  char *name="???";
  topcode=READ_CODE(loc);
  lastSymbol=map->findAnyExactSymbol(loc);
  if(lastSymbol) DEBUG_OUT<<lastSymbol->getName()<<":\n";
  DEBUG_OUT<<"";
  DEBUG_OUT<<".";
  DEBUG_OUT<<pbr;
  DEBUG_OUT<<":";
  DEBUG_OUT<<loc;  DEBUG_OUT<<"  ";
  len=E6509_size[topcode];
  mode=E6509_format[topcode];
  name=E6509_name[topcode];
  if(!len) len=1;  // NOTE: An unimplemented opcode...
  unsigned int t;
  for(t=0;t<len;t++) {
    int n=READ_CODE(loc+t);
    if(n<0x10) DEBUG_OUT<<"0";
    DEBUG_OUT<<n<<" ";
  }
  for(t=0;t<(3-len);t++) DEBUG_OUT<<"   ";
  DEBUG_OUT<<name<<" ";
  disasmAddrMode(loc,mode);
  DEBUG_OUT<<"";
  DEBUG_OUT<<"\n";
  return len;
}


void E6509::executeInstead(ULONG opcode2)
{
  opcode=opcode2;
  op_len=E6509_size[opcode];
  cycle=E6509_time[opcode];
  if(!execMain()) exec_unimplemented(opcode);
  // NOTE: We fall back to the regular execute code here...
}


void E6509::execute()
{
  if(pc==breakpoint) { debugger("Breakpoint!\n"); return; }
  opcode=READ_CODE(pc);
  op_len=E6509_size[opcode];
  cycle=E6509_time[opcode];
  if(!execMain()) exec_unimplemented(opcode);
  pc+=op_len;
  cycles+=cycle;
  if(int_pending) handle_int(int_pending);
}


// This will bring in the class' execMain func
#define theClassType E6509
#define e6509
#include <cpulib/E65_core.h>
#undef e6509
#undef theClassType



unsigned int E6509::readv(ULONG addr,unsigned int size)
{
  return map->read8(addr);
}


void E6509::writev(ULONG addr,unsigned int val,unsigned int size)
{
  map->write8(addr,val);
}


void E6509::adj_op_len(bool n)
{
}

*/
