
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#define DEBUG_DIS_MODES 1

#define DEBUG_EXEC 1

#define CHECK_SAFE 1

#define SHOW_INTS  1


#include <cpulib/Em6800.h>

#include <elib/EMemMapAbstract.h>
#include <elib/EModule.h>
#include <elib/EHandler.h>

#include <portable/nickcpp/ANullStream.h>

#include <portable/nickcpp/AStringHelper.h>


#include <cpulib/generated/Em6800_tab.h>


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


/*
Notes:

m6800:
8 bits: a,b
16 bits: sp, pc, x
status: hinzvc?

m6805:
8 bits: a,x
16 bits: sp (limited), pc (limited)
status: 111hinzc

m6808
8 bits: a,h,x
16 bits: sp, pc, h:x
status: v11hinzc


*/


////////////////////////////////////////////////////////////////////////////////
//  Em6800 Class
////////////////////////////////////////////////////////////////////////////////

Em6800::Em6800()
{
  DEBUG_OUT<<"Em6800 created...\n";
  init();
  type=E_CPU_6800;
  //w=ww;
  reset();
}


Em6800::~Em6800()
{
  DEBUG_OUT<<"Em6800 destroyed.\n";
}


void Em6800::init()
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
  setStack(ECPUABSTRACT_DEFAULT_STACK>>16);
  setFramePtr(ECPUABSTRACT_DEFAULT_STACK>>16);
  setResetStack(ECPUABSTRACT_DEFAULT_STACK>>16);
  setResetEntry(ECPUABSTRACT_DEFAULT_CODE>>16);
}


/* STATIC */
bool Em6800::recognize(const char *tname)
{
  const char *name=aDefaultStringHelper.removeSpaces2(aDefaultStringHelper.toLower(tname));
  if(!strcmp(name,"6800")) return true; else
  if(!strcmp(name,"6801")) return true; else
  if(!strcmp(name,"6802")) return true; else
  if(!strcmp(name,"6803")) return true; else
  if(!strcmp(name,"6804")) return true; else
  if(!strcmp(name,"6805")) return true; else
  if(!strcmp(name,"hc05")) return true; else
  if(!strcmp(name,"6808")) return true; else
  if(!strcmp(name,"hc08")) return true; else
  if(!strcmp(name,"m6800")) return true; else
  if(!strcmp(name,"m6801")) return true; else
  if(!strcmp(name,"m6802")) return true; else
  if(!strcmp(name,"m6803")) return true; else
  if(!strcmp(name,"m6804")) return true; else
  if(!strcmp(name,"m6805")) return true; else
  if(!strcmp(name,"m6808")) return true; else
  if(!strcmp(name,"6303")) return true; else
  if(!strcmp(name,"hd6303")) return true; else
  return false;
}


const char *Em6800::getName()
{
  return "Motorola 6800";
}


void Em6800::setKind(const char *tname)
{
  type=E_CPU_6800;
}


void Em6800::reset()
{
#ifdef DEBUG_EXEC
  setTrace(true);
#endif
  a=0;  b=0;
  x=0;  y=0;
  sp=0;  u=0;
  p=0;
  dp=0;
  intLock=false;
  if(map) pc=map->read16b(E6800_INT_RESET&map->getAddrMask());
  clearStackTrace();
  ECPUAbstract::reset();
}


// TODO: Finish this
bool Em6800::setRegister(const char *tname,ULONG value)
{
  const char *name=aDefaultStringHelper.removeSpaces2(aDefaultStringHelper.toLower(tname));
  bool ret=false;
  if(!strcmp(name,"pc"))
  { pc=(UINT16)value; ret=true; } else
  if(!strcmp(name,"a"))
  { a=(BYTE)value; ret=true; } else
  if(!strcmp(name,"b"))
  { b=(BYTE)value; ret=true; } else
  if(!strcmp(name,"x"))
  { x=(BYTE)value; ret=true; } else
  if(!strcmp(name,"ix"))
  { x=(BYTE)value; ret=true; } else
  if(!strcmp(name,"y"))
  {  y=(BYTE)value; ret=true; } else
  if(!strcmp(name,"iy"))
  {  y=(BYTE)value; ret=true; } else
  if(!strcmp(name,"p"))
  { p=(BYTE)value; ret=true; } else
  if(!strcmp(name,"s"))
  { sp=(BYTE)value; ret=true; }
  if(!strcmp(name,"u"))
  { u=(BYTE)value; ret=true; }
  //free(name);
  return ret;
}


// TODO: finish this
ULONG Em6800::readRegister(const char *tname)
{
  const char *name=aDefaultStringHelper.removeSpaces2(aDefaultStringHelper.toLower(tname));
  if(name[0]=='a') return a;
  if(name[0]=='b') return b;
  if(name[0]=='d') return readRegD();
  if(name[0]=='x') return x;
  if(name[0]=='y') return y;
  if(name[0]=='s') return sp;
  if(name[0]=='u') return u;
  if(name[0]=='p') return p;
  return 0;
}


void Em6800::wregs()
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


void Em6800::set_flags_nz(int i)
{
  sbits->z=(i==0);
  sbits->n=(i&0x80)!=0;
}


void Em6800::set_flags_nzv(int i)
{
  sbits->z=(i==0);
  sbits->n=(i&0x80)!=0;
  sbits->v=0;
}


void Em6800::set_flags_nzc(int i, int j)
{
  UINT16 val=(UINT16)i-(UINT16)j;
  sbits->z=(val==0);
  sbits->n=(val&0x80)!=0;
  sbits->c=(val<(UINT16)0x100);
}


// TODO: Move pending stuff to cause_int...
void Em6800::handle_int(ULONG value)
{
  int_pending=0;
  bool intDis=false;
  if(intLock) intDis=true;
  if(p&E6800_STAT_I) intDis=true;
  if(intDis) {
    if(value==E6800_INT_IRQ) {
      //if(!(p&E6800_STAT_I)) {
        //DEBUG_OUT<<"IRQ requested...but ints are disabled.\n";
        //debugger("Interrupts are disabled.\n");
        int_pending=value;
#ifdef SHOW_INTS
        DEBUG_OUT<<"x";
#endif
        return;
      //}
      //else {
        //DEBUG_OUT<<"BRK with ints disabled!\n";
        //return;
      //}
    }
  }
#ifdef SHOW_INTS
  DEBUG_OUT<<"+";
#endif
  stack_pushw6800(sp,pc);
  //DEBUG_OUT<<"Pushed "<<pc<<"\n";
  stack_push6800(sp,p);
  p=p|E6800_STAT_I;
  jump(map->read16b(value&map->getAddrMask()));
  //trace=true;
}


// TODO: Check to see if ints are disabled, and set
// pending if so and don't call handle_int until later...
void Em6800::cause_int(ULONG value)
{
  int_pending=0;
  switch(value) {
    case E6800_INT_RESET:
      reset();
      break;
    case E6800_INT_NMI:
      //p&=(0xff-E6800_STAT_B);  //reset B flag
      handle_int(value);
      break;
    case E6800_INT_IRQ:
      //p&=(0xff-E6800_STAT_B);  //reset B flag
      handle_int(value);
      break;
    case E6800_INT_FIRQ:
      //p&=(0xff-E6800_STAT_B);  //reset B flag
      handle_int(value);
      break;
    case E6800_INT_SWI:
      //p&=(0xff-E6800_STAT_B);  //reset B flag
      handle_int(value);
      break;
    case E6800_INT_SWI2:
      //p&=(0xff-E6800_STAT_B);  //reset B flag
      handle_int(value);
      break;
    case E6800_INT_SWI3:
      //p&=(0xff-E6800_STAT_B);  //reset B flag
      handle_int(value);
      break;
    default:
      debugger("Illegal interrupt requested!\n");
      break;
  }
}


void Em6800::ints()
{
  //if(w) {
    DEBUG_OUT<<"SWI3 ("<<(unsigned short)E6800_INT_SWI3<<"): "
      <<(unsigned int)map->read16b(E6800_INT_SWI3&map->getAddrMask())<<"\n";
    DEBUG_OUT<<"SWI2 ("<<(unsigned short)E6800_INT_SWI2<<"): "
      <<(unsigned int)map->read16b(E6800_INT_SWI2&map->getAddrMask())<<"\n";
    DEBUG_OUT<<"FIRQ ("<<(unsigned short)E6800_INT_FIRQ<<"): "
      <<(unsigned int)map->read16b(E6800_INT_FIRQ&map->getAddrMask())<<"\n";
    DEBUG_OUT<<"IRQ ("<<(unsigned short)E6800_INT_IRQ<<"): "
      <<(unsigned int)map->read16b(E6800_INT_IRQ&map->getAddrMask())<<"\n";
    DEBUG_OUT<<"SWI ("<<(unsigned short)E6800_INT_SWI<<"): "
      <<(unsigned int)map->read16b(E6800_INT_SWI&map->getAddrMask())<<"\n";
    DEBUG_OUT<<"NMI ("<<(unsigned short)E6800_INT_NMI<<"): "
      <<(unsigned int)map->read16b(E6800_INT_NMI&map->getAddrMask())<<"\n";
    DEBUG_OUT<<"RESET ("<<(unsigned short)E6800_INT_RESET<<"): "
      <<(unsigned int)map->read16b(E6800_INT_RESET&map->getAddrMask())<<"\n";
  //}
}


unsigned int Em6800::readIndReg(unsigned int r)
{
  unsigned int val=0;
  switch(r) {
    case 0:  val=x; break;
    case 1:  val=y; break;
    case 2:  val=u; break;
    case 3:  val=sp; break;
    default: break;
  }
  return val;
}


void Em6800::writeIndReg(unsigned int r,unsigned int val)
{
  switch(r) {
    case 0:  x=val; break;
    case 1:  y=val; break;
    case 2:  u=val; break;
    case 3:  sp=val; break;
    default: break;
  }
}


unsigned int Em6800::readRegD()
{
 unsigned int val=(a&0xff)|((b<<8)&0xff00);
#ifdef DEBUG_EXEC
 DEBUG_OUT<<"readRegD of "<<val<<"\n";
#endif
 return val;
}


void Em6800::writeRegD(unsigned int val)
{
  a=val&0xff;
  b=(val>>8)&0xff;
#ifdef DEBUG_EXEC
 DEBUG_OUT<<"writeRegD of "<<val<<"\n";
#endif
}


void Em6800::writeReg(unsigned int n,unsigned int val)
{
  switch(n) {
    case E6800_REG_D: writeRegD(val); break;
    case E6800_REG_X: x=val; break;
    case E6800_REG_Y: y=val; break;
    case E6800_REG_U: u=val; break;
    case E6800_REG_S: sp=val; break;
    case E6800_REG_PC: pc=val; break;
    case E6800_REG_A: a=val; break;
    case E6800_REG_B: b=val; break;
    case E6800_REG_CC: p=val; break;
    case E6800_REG_DP: dp=val<<8; break;
    default: debugger("bad reg in writeReg");  break;
  }
}


unsigned int Em6800::readReg(unsigned int n)
{
  switch(n) {
    case E6800_REG_D: return readRegD(); break;
    case E6800_REG_X: return x; break;
    case E6800_REG_Y: return y; break;
    case E6800_REG_U: return u; break;
    case E6800_REG_S: return sp; break;
    case E6800_REG_PC: return pc; break;
    case E6800_REG_A: return a; break;
    case E6800_REG_B: return b; break;
    case E6800_REG_CC: return p; break;
    case E6800_REG_DP: return dp>>8; break;
    default: debugger("bad reg in readReg");  break;
  }
  return 0;
}


// TODO: doesn't handle signed offsets!
unsigned int Em6800::calcEAPostByte(ULONG addr)
{
  unsigned int ea=0;
  unsigned int post=map->read8(addr);
#ifdef DEBUG_EXEC
  DEBUG_OUT<<"postByte is "<<post<<"\n";
#endif
  unsigned int r=(post>>5)&0x3; // NOTE: not all modes use this reg num
  ULONG offset=post&0x1f; // This is the initial 5-bit offset, only used there
  unsigned int mode=0;
  unsigned int d=0;
  if(post&0x80) {
    // postbyte has high bit on, so mode is last 5 bits
    mode=offset;
    switch(mode) {
      case E6800_POST_AUTOINC1:
        ea=readIndReg(r);
        writeIndReg(r,ea+1);
        //debugger("Unimp mode auto inc1 r");
        break;
      case E6800_POST_AUTOINC2:
        ea=readIndReg(r);
        writeIndReg(r,ea+2);
        //debugger("Unimp mode auto inc2 r");
        break;
      case E6800_POST_AUTODEC1:
        ea=readIndReg(r)-1;
        writeIndReg(r,ea);
        //debugger("Unimp mode auto dec1 r");
        break;
      case E6800_POST_AUTODEC2:
        ea=readIndReg(r)-2;
        writeIndReg(r,ea);
        //debugger("Unimp mode auto dec2 r");
        break;
      case E6800_POST_A_OFFSET_R:
        ea=readIndReg(r)+a;
        //debugger("Unimp mode a offset from r");
        break;
      case E6800_POST_B_OFFSET_R:
        ea=readIndReg(r)+b;
        //debugger("Unimp mode b offset from r");
        break;
     case E6800_POST_D_OFFSET_R:
        d=readRegD();
        ea=readIndReg(r)+d;
        //debugger("Unimp mode d offset from r");
        break;
      case E6800_POST_NO_OFFSET_R:
        ea=readIndReg(r);
        //debugger("Unimp mode no offset r");
        break;
      case E6800_POST_8BIT_OFFSET_R:
        ea=map->read8(addr+1)+readIndReg(r);
        //debugger("Unimp mode 8 bit offset r");
        break;
      case E6800_POST_16BIT_OFFSET_R:
        ea=map->read16b(addr+1)+readIndReg(r);
        //debugger("Unimp mode 16 bit offset r");
        break;
      case E6800_POST_8BIT_OFFSET_PC:
        ea=map->read8(addr+1)+pc;
        //debugger("Unimp mode 8 bit offset from pc");
        break;
      case E6800_POST_16BIT_OFFSET_PC:
        ea=map->read16b(addr+1)+pc;
        //debugger("Unimp mode 16 bit offset from pc");
        break;
      case E6800_POST_IND_AUTOINC2:
        offset=readIndReg(r);
        writeIndReg(r,offset+2);
        ea=map->read16b(offset);
        //debugger("Unimp mode indirect auto inc2 r");
        break;
      case E6800_POST_IND_AUTODEC2:
        offset=readIndReg(r)-2;
        writeIndReg(r,offset);
        ea=map->read16b(offset);
        //debugger("Unimp mode indirect auto dec2 r");
        break;
      case E6800_POST_IND_NO_OFFSET_R:
        offset=readIndReg(r);
        ea=map->read16b(offset);
        //debugger("Unimp mode indirect no offset r");
        break;
      case E6800_POST_IND_8BIT_OFFSET_R:
        offset=map->read8(addr+1)+readIndReg(r);
        ea=map->read16b(offset);
        //debugger("Unimp mode indirect 8bit offset r");
        break;
      case E6800_POST_IND_16BIT_OFFSET_R:
        offset=map->read16b(addr+1)+readIndReg(r);
        ea=map->read16b(offset);
        //debugger("Unimp mode indirect 16bit offset r");
        break;
      case E6800_POST_IND_A_OFFSET_R:
        offset=readIndReg(r)+a;
        ea=map->read16b(offset);
        //debugger("Unimp mode indirect a offset r");
        break;
      case E6800_POST_IND_B_OFFSET_R:
        offset=readIndReg(r)+b;
        ea=map->read16b(offset);
        //debugger("Unimp mode indirect b offset r");
        break;
      case E6800_POST_IND_D_OFFSET_R:
        d=readRegD();
        offset=readIndReg(r)+d;
        ea=map->read16b(offset);
        //debugger("Unimp mode indirect d offset r");
        break;
      case E6800_POST_IND_8BIT_OFFSET_PC:
        offset=map->read8(addr+1)+pc;
        ea=map->read16b(offset);
        //debugger("Unimp mode indirect 8 bit offset pc");
        break;
      case E6800_POST_IND_16BIT_OFFSET_PC:
        offset=map->read16b(addr+1)+pc;
        ea=map->read16b(offset);
        //debugger("Unimp mode indirect 16 bit offset pc");
        break;
      case E6800_POST_IND_EXTEND:
        ea=map->read16b(map->read16b(addr+1));
        debugger("Unimp mode indirect extended");
        break;
      default:
        DEBUG_OUT<<"high bit postbyte mode is "<<mode<<"\n";
        debugger("postbyte has high bit on");
        break;
    }
  }
  else {
    // postbyte has high bit off, so its "constant 5-bit offset from r"
    ea=offset+readIndReg(r);
    //debugger("Unimp mode const5 from r");
  }
  return ea;
}


// TODO: Check and handle any requested ints between instructions
void Em6800::execute()
{
  if(pc==breakpoint) { debugger("Breakpoint!\n"); return; }
  opcode=map->read8(pc);
  op_len=Em6800_size[opcode];
  cycle=Em6800_time[opcode];
  topcode=opcode;
  unsigned int loc=pc;
#include <cpulib/generated/Em6800_exec.h>
  pc+=op_len;
  cycles+=cycle;
  if(int_pending) handle_int(int_pending);
}


void Em6800::printReg(unsigned int r)
{
  switch(r) {
    case E6800_REG_D: DEBUG_OUT<<"D"; break;
    case E6800_REG_X: DEBUG_OUT<<"X"; break;
    case E6800_REG_Y: DEBUG_OUT<<"Y"; break;
    case E6800_REG_U: DEBUG_OUT<<"U"; break;
    case E6800_REG_S: DEBUG_OUT<<"S"; break;
    case E6800_REG_PC: DEBUG_OUT<<"PC"; break;
    case E6800_REG_A: DEBUG_OUT<<"A"; break;
    case E6800_REG_B: DEBUG_OUT<<"B"; break;
    case E6800_REG_CC: DEBUG_OUT<<"CC"; break;
    case E6800_REG_DP: DEBUG_OUT<<"DP"; break;
    default:  DEBUG_OUT<<"??REG??";  break;
  }
}


void Em6800::printRegs(unsigned int rs)
{
  unsigned int r1=rs&0xf;
  unsigned int r2=(rs>>4)&0xf;
  printReg(r1); DEBUG_OUT<<",";  printReg(r2);
}


void Em6800::printRegMask(unsigned int rs)
{
  if(rs&0x80) DEBUG_OUT<<"PC ";
  if(rs&0x40) DEBUG_OUT<<"S ";
  if(rs&0x20) DEBUG_OUT<<"Y ";
  if(rs&0x10) DEBUG_OUT<<"X ";
  if(rs&0x08) DEBUG_OUT<<"DP ";
  if(rs&0x04) DEBUG_OUT<<"B ";
  if(rs&0x02) DEBUG_OUT<<"A ";
  if(rs&0x01) DEBUG_OUT<<"CC ";
}


void Em6800::printIndReg(unsigned int r)
{
  switch(r) {
    case 0:  DEBUG_OUT<<"X"; break;
    case 1:  DEBUG_OUT<<"Y"; break;
    case 2:  DEBUG_OUT<<"U"; break;
    case 3:  DEBUG_OUT<<"S"; break;
    default: DEBUG_OUT<<"???"; break;
  }
}


unsigned int Em6800::disPostByte(ULONG addr)
{
  unsigned int len=0;
  unsigned int post=map->read8(addr);
  unsigned int r=(post>>5)&0x3; // NOTE: not all modes use this reg num
  ULONG offset=post&0x1f;
  unsigned int mode=0;
  if(post&0x80) {
    // postbyte has high bit on, so mode is last 5 bits
    mode=offset;
    switch(mode) {
      case E6800_POST_AUTOINC1:
        DEBUG_OUT<<",";  printIndReg(r); DEBUG_OUT<<"+";
        break;
      case E6800_POST_AUTOINC2:
        DEBUG_OUT<<",";  printIndReg(r); DEBUG_OUT<<"++";
        break;
      case E6800_POST_AUTODEC1:
        DEBUG_OUT<<",-";  printIndReg(r);
        break;
      case E6800_POST_AUTODEC2:
        DEBUG_OUT<<",--";  printIndReg(r);
        break;
      case E6800_POST_A_OFFSET_R:
        DEBUG_OUT<<"A,";  printIndReg(r);
        break;
      case E6800_POST_B_OFFSET_R:
        DEBUG_OUT<<"B,";  printIndReg(r);
        break;
      case E6800_POST_D_OFFSET_R:
        DEBUG_OUT<<"D,";  printIndReg(r);
        break;
      case E6800_POST_NO_OFFSET_R:
        DEBUG_OUT<<",";  printIndReg(r);
        break;
      case E6800_POST_8BIT_OFFSET_R:
        offset=map->read8(addr+1);
        DEBUG_OUT<<offset<<",";  printIndReg(r);
        len=1;
        break;
      case E6800_POST_16BIT_OFFSET_R:
        offset=map->read16b(addr+1);
        DEBUG_OUT<<offset<<",";  printIndReg(r);
        len=2;
        break;
      case E6800_POST_8BIT_OFFSET_PC:
        offset=map->read8(addr+1);
        DEBUG_OUT<<offset<<",PC";
        len=1;
        break;
      case E6800_POST_16BIT_OFFSET_PC:
        offset=map->read16b(addr+1);
        DEBUG_OUT<<offset<<",PC";
        len=2;
        break;
      case E6800_POST_IND_AUTOINC2:
        DEBUG_OUT<<"[,";  printIndReg(r); DEBUG_OUT<<"++]";
        break;
      case E6800_POST_IND_AUTODEC2:
        DEBUG_OUT<<"[,--";  printIndReg(r); DEBUG_OUT<<"]";
        break;
      case E6800_POST_IND_NO_OFFSET_R:
        DEBUG_OUT<<"[,";  printIndReg(r); DEBUG_OUT<<"]";
        break;
      case E6800_POST_IND_8BIT_OFFSET_R:
        offset=map->read8(addr+1);
        DEBUG_OUT<<"["<<offset<<",";  printIndReg(r); DEBUG_OUT<<"]";
        len=1;
        break;
      case E6800_POST_IND_16BIT_OFFSET_R:
        offset=map->read16b(addr+1);
        DEBUG_OUT<<"["<<offset<<",";  printIndReg(r); DEBUG_OUT<<"]";
        len=2;
        break;
      case E6800_POST_IND_A_OFFSET_R:
        DEBUG_OUT<<"[A,";  printIndReg(r); DEBUG_OUT<<"]";
        break;
      case E6800_POST_IND_B_OFFSET_R:
        DEBUG_OUT<<"[B,";  printIndReg(r); DEBUG_OUT<<"]";
        break;
      case E6800_POST_IND_D_OFFSET_R:
        DEBUG_OUT<<"[D,";  printIndReg(r); DEBUG_OUT<<"]";
        break;
      case E6800_POST_IND_8BIT_OFFSET_PC:
        offset=map->read8(addr+1);
        DEBUG_OUT<<"["<<offset<<",PC]";
        len=1;
        break;
      case E6800_POST_IND_16BIT_OFFSET_PC:
        offset=map->read16b(addr+1);
        DEBUG_OUT<<"["<<offset<<",PC]";
        len=2;
        break;
      case E6800_POST_IND_EXTEND:
        offset=map->read16b(addr+1);
        DEBUG_OUT<<"["<<offset<<"]";
        len=2;
        break;
      default:
        DEBUG_OUT<<"high bit postbyte mode is "<<mode<<"\n";
        debugger("postbyte has high bit on");
        break;
    }
  }
  else {
    // postbyte has high bit off, so its "constant 5-bit offset from r"
    DEBUG_OUT<<"["<<offset<<",";  printIndReg(r); DEBUG_OUT<<"]";
  }
  return len;
}


ULONG Em6800::disasm(ULONG loc)
{
  unsigned int len=0,mode=E6800_NONE;
  const char *name="???";
  topcode=map->read8(loc);
  lastSymbol=map->findAnyExactSymbol(loc);
  if(lastSymbol) DEBUG_OUT<<lastSymbol->getName()<<":\n";
  DEBUG_OUT<<"";
  DEBUG_OUT<<"."; DEBUG_OUT<<loc;  DEBUG_OUT<<"  ";
  len=Em6800_size[topcode];
  mode=Em6800_format[topcode];
  name=Em6800_name[topcode];
  if(!len) len=1;  // NOTE: An unimplemented opcode...
  unsigned int t;
  if(mode==E6800_SPECIAL) {
    loc++;
    switch(topcode) {
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


unsigned int Em6800::printAddrMode(ULONG loc,unsigned int topcode,unsigned int mode)
{
  unsigned int len=0;
  switch(mode) {
    case E6800_NONE:
#ifdef DEBUG_DIS_MODES
      DEBUG_OUT<<"  ;E6800_NONE";
#endif
      break;
    case E6800_DP:
      DEBUG_OUT<<"<$"<<(short)map->read8(loc+1);
#ifdef DEBUG_DIS_MODES
      DEBUG_OUT<<"  ;E6800_DP";
#endif
      break;
    case E6800_IMM8:
      DEBUG_OUT<<"#"<<(short)map->read8(loc+1);
#ifdef DEBUG_DIS_MODES
      DEBUG_OUT<<"  ;E6800_IMM8";
#endif
      break;
    case E6800_IMM16:
      DEBUG_OUT<<"#"<<(short)map->read16b(loc+1);
#ifdef DEBUG_DIS_MODES
      DEBUG_OUT<<"  ;E6800_IMM16";
#endif
      break;
    case E6800_ABS:
      DEBUG_OUT<<"$"<<(short)map->read16b(loc+1);
#ifdef DEBUG_DIS_MODES
      DEBUG_OUT<<"  ;E6800_ABS";
#endif
      break;
    case E6800_REL:
      DEBUG_OUT<<"$"<<(short)map->read8(loc+1)<<"  ;"<<(UINT16)((SBYTE)map->read8(loc+1)+loc+2);
#ifdef DEBUG_DIS_MODES
      DEBUG_OUT<<"  ;E6800_REL";
#endif
      break;
    case E6800_REGS:
      printRegs(map->read8(loc+1));
      len=1;
#ifdef DEBUG_DIS_MODES
      DEBUG_OUT<<"  ;E6800_REGS";
#endif
      break;
    case E6800_REGMASK:
      printRegMask(map->read8(loc+1));
      len=1;
#ifdef DEBUG_DIS_MODES
      DEBUG_OUT<<"  ;E6800_REGMASK";
#endif
      break;
    case E6800_INDEXED:
      len=disPostByte(loc+1);
#ifdef DEBUG_DIS_MODES
      DEBUG_OUT<<"  ;E6800_INDEXED";
#endif
      break;
    case E6800_EXTEND:
      DEBUG_OUT<<"$"<<map->read16b(loc+1);
#ifdef DEBUG_DIS_MODES
      DEBUG_OUT<<"  ;E6800_EXTEND";
#endif
      break;
    case E6800_SPECIAL:
      DEBUG_OUT<<"?E6800_SPECIAL?";
      len=disPostByte(loc+1);
#ifdef DEBUG_DIS_MODES
      DEBUG_OUT<<"  ;E6800_SPECIAL";
#endif
      break;
    default:
      DEBUG_OUT<<"???";
      break;
  }
  return len;
}


void Em6800::stack_push6800(UINT16 &s,BYTE val)
{
  map->write8(s,val);
  s--;
#ifdef DEBUG_EXEC
  DEBUG_OUT<<"stack_push6800("<<s<<","<<(short)val<<")\n";
#endif
}


// NOTE: Big endian!
void Em6800::stack_pushw6800(UINT16 &s,UINT16 val)
{
  BYTE high=(BYTE)((val>>8)&0xff);
  BYTE low=(BYTE)(val&0xff);
  stack_push6800(s,high);
  stack_push6800(s,low);
}


BYTE Em6800::stack_pop6800(UINT16 &s)
{
  s++;
  BYTE val=map->read8(s);
#ifdef DEBUG_EXEC
  DEBUG_OUT<<"stack_pop6800("<<s<<")="<<(short)val<<"\n";
#endif
  return val;
}


// NOTE: Big endian!
UINT16 Em6800::stack_popw6800(UINT16 &s)
{
  BYTE high=stack_pop6800(s);
  BYTE low=stack_pop6800(s);
  unsigned int val=low+(high<<8);
  return val;
}


void Em6800::exec_special(ULONG loc,unsigned int op)
{
  debugger("Unimplemented special opcode!");
}


void Em6800::exec_abx(ULONG loc,unsigned int op)
{
  x=x+a;
}


void Em6800::exec_adca(ULONG loc,unsigned int op)
{
  debugger("Unimplemented adca opcode!");
}


void Em6800::exec_adcb(ULONG loc,unsigned int op)
{
  debugger("Unimplemented adcb opcode!");
}


void Em6800::exec_adda(ULONG loc,unsigned int op)
{
  debugger("Unimplemented adda opcode!");
}


void Em6800::exec_addb(ULONG loc,unsigned int op)
{
  debugger("Unimplemented addb opcode!");
}


void Em6800::exec_addd(ULONG loc,unsigned int op)
{
  unsigned int val=map->read16b(calcEAPostByte(loc+1));
  writeRegD(readRegD()+val);
}


void Em6800::exec_anda(ULONG loc,unsigned int op)
{
  debugger("Unimplemented anda opcode!");
}


void Em6800::exec_andb(ULONG loc,unsigned int op)
{
  debugger("Unimplemented andb opcode!");
}


void Em6800::exec_asl(ULONG loc,unsigned int op)
{
  debugger("Unimplemented asl opcode!");
}


void Em6800::exec_asla(ULONG loc,unsigned int op)
{
  debugger("Unimplemented asla opcode!");
}


void Em6800::exec_aslb(ULONG loc,unsigned int op)
{
  debugger("Unimplemented aslb opcode!");
}


void Em6800::exec_asr(ULONG loc,unsigned int op)
{
  debugger("Unimplemented asr opcode!");
}


void Em6800::exec_asra(ULONG loc,unsigned int op)
{
  debugger("Unimplemented asra opcode!");
}


void Em6800::exec_asrb(ULONG loc,unsigned int op)
{
  debugger("Unimplemented asrb opcode!");
}


void Em6800::exec_bcc(ULONG loc,unsigned int op)
{
  bool doJump=false;
  ULONG addr=(UINT16)((SBYTE)map->read8(loc+1)+loc+2);
  if(!sbits->c) doJump=true;
  if(doJump) jump(addr);
}


void Em6800::exec_bcs(ULONG loc,unsigned int op)
{
  bool doJump=false;
  ULONG addr=(UINT16)((SBYTE)map->read8(loc+1)+loc+2);
  if(sbits->c) doJump=true;
  if(doJump) jump(addr);
}


void Em6800::exec_beq(ULONG loc,unsigned int op)
{
  bool doJump=false;
  ULONG addr=(UINT16)((SBYTE)map->read8(loc+1)+loc+2);
  if(sbits->z) doJump=true;
  if(doJump) jump(addr);
}


void Em6800::exec_bge(ULONG loc,unsigned int op)
{
  bool doJump=false;
  ULONG addr=(UINT16)((SBYTE)map->read8(loc+1)+loc+2);
  if((sbits->n)&&(!sbits->v)) doJump=true;
  if((sbits->v)&&(!sbits->n)) doJump=true;
  if(doJump) jump(addr);
}


void Em6800::exec_bgt(ULONG loc,unsigned int op)
{
  bool doJump=false;
  ULONG addr=(UINT16)((SBYTE)map->read8(loc+1)+loc+2);
  if(!sbits->z) {
    if((sbits->n)&&(!sbits->v)) doJump=true;
    if((sbits->v)&&(!sbits->n)) doJump=true;
  }
  if(doJump) jump(addr);
}


void Em6800::exec_bhi(ULONG loc,unsigned int op)
{
  bool doJump=false;
  ULONG addr=(UINT16)((SBYTE)map->read8(loc+1)+loc+2);
  if((!sbits->c)&&(!sbits->z)) doJump=true;
  if(doJump) jump(addr);
}


void Em6800::exec_bita(ULONG loc,unsigned int op)
{
  debugger("Unimplemented bita opcode!");
}


void Em6800::exec_bitb(ULONG loc,unsigned int op)
{
  debugger("Unimplemented bitb opcode!");
}


void Em6800::exec_ble(ULONG loc,unsigned int op)
{
  bool doJump=false;
  ULONG addr=(UINT16)((SBYTE)map->read8(loc+1)+loc+2);
  if(sbits->z) doJump=true;
  if((sbits->n)&&(!sbits->v)) doJump=true;
  if((sbits->v)&&(!sbits->n)) doJump=true;
  if(doJump) jump(addr);
}


void Em6800::exec_bls(ULONG loc,unsigned int op)
{
  bool doJump=false;
  ULONG addr=(UINT16)((SBYTE)map->read8(loc+1)+loc+2);
  if((sbits->c)||(sbits->z)) doJump=true;
  if(doJump) jump(addr);
}


void Em6800::exec_blt(ULONG loc,unsigned int op)
{
  bool doJump=false;
  ULONG addr=(UINT16)((SBYTE)map->read8(loc+1)+loc+2);
  if((sbits->n)&&(!sbits->v)) doJump=true;
  if((sbits->v)&&(!sbits->n)) doJump=true;
  if(doJump) jump(addr);
}


void Em6800::exec_bmi(ULONG loc,unsigned int op)
{
  bool doJump=false;
  ULONG addr=(UINT16)((SBYTE)map->read8(loc+1)+loc+2);
  if(sbits->n) doJump=true;
  if(doJump) jump(addr);
}


void Em6800::exec_bne(ULONG loc,unsigned int op)
{
  bool doJump=false;
  ULONG addr=(UINT16)((SBYTE)map->read8(loc+1)+loc+2);
  if(!sbits->z) doJump=true;
  if(doJump) jump(addr);
}


void Em6800::exec_bpl(ULONG loc,unsigned int op)
{
  bool doJump=false;
  ULONG addr=(UINT16)((SBYTE)map->read8(loc+1)+loc+2);
  if(!sbits->n) doJump=true;
  if(doJump) jump(addr);
}


void Em6800::exec_bra(ULONG loc,unsigned int op)
{
  bool doJump=false;
  ULONG addr=(UINT16)((SBYTE)map->read8(loc+1)+loc+2);
  doJump=true;
  if(doJump) jump(addr);
}


void Em6800::exec_brn(ULONG loc,unsigned int op)
{
  bool doJump=false;
  ULONG addr=(UINT16)((SBYTE)map->read8(loc+1)+loc+2);
  doJump=false;
  if(doJump) jump(addr);
}


void Em6800::exec_bsr(ULONG loc,unsigned int op)
{
  debugger("Unimplemented bsr opcode!");
}


void Em6800::exec_bvc(ULONG loc,unsigned int op)
{
  bool doJump=false;
  ULONG addr=(UINT16)((SBYTE)map->read8(loc+1)+loc+2);
  if(!sbits->v) doJump=true;
  if(doJump) jump(addr);
}


void Em6800::exec_bvs(ULONG loc,unsigned int op)
{
  bool doJump=false;
  ULONG addr=(UINT16)((SBYTE)map->read8(loc+1)+loc+2);
  if(sbits->v) doJump=true;
  if(doJump) jump(addr);
}


void Em6800::exec_clr(ULONG loc,unsigned int op)
{
  unsigned int ea=0;
  switch(op) {
    case 0x7f:  // CLR $const16
      ea=map->read16b(loc+1);
      break;
    default:
      DEBUG_OUT<<"loc="<<loc<<" op="<<op<<"\n";
      debugger("Unimplemented clr opcode!");
      break;
  }
  map->write8(ea,0);
  set_flags_nzv(val);
#ifdef DEBUG_EXEC
  DEBUG_OUT<<"CLR at "<<ea<<"\n";
#endif
}


void Em6800::exec_clra(ULONG loc,unsigned int op)
{
  a=0;
}


void Em6800::exec_clrb(ULONG loc,unsigned int op)
{
  b=0;
}


void Em6800::exec_cmpa(ULONG loc,unsigned int op)
{
  debugger("Unimplemented cmpa opcode!");
}


void Em6800::exec_cmpb(ULONG loc,unsigned int op)
{
  debugger("Unimplemented cmpb opcode!");
}


void Em6800::exec_cmpx(ULONG loc,unsigned int op)
{
  debugger("Unimplemented cmpx opcode!");
}


void Em6800::exec_cmpy(ULONG loc,unsigned int op)
{
  debugger("Unimplemented cmpy opcode!");
}


void Em6800::exec_com(ULONG loc,unsigned int op)
{
  debugger("Unimplemented com opcode!");
}


void Em6800::exec_coma(ULONG loc,unsigned int op)
{
  debugger("Unimplemented coma opcode!");
}


void Em6800::exec_comb(ULONG loc,unsigned int op)
{
  debugger("Unimplemented comb opcode!");
}


void Em6800::exec_cpx(ULONG loc,unsigned int op)
{
  debugger("Unimplemented cpx opcode!");
}


void Em6800::exec_cwai(ULONG loc,unsigned int op)
{
  debugger("Unimplemented cwai opcode!");
}


void Em6800::exec_daa(ULONG loc,unsigned int op)
{
  debugger("Unimplemented daa opcode!");
}


void Em6800::exec_dec(ULONG loc,unsigned int op)
{
  debugger("Unimplemented dec opcode!");
}


void Em6800::exec_deca(ULONG loc,unsigned int op)
{
  a--;
  set_flags_nzv(a);
}


void Em6800::exec_decb(ULONG loc,unsigned int op)
{
  b--;
  set_flags_nzv(b);
}


void Em6800::exec_eora(ULONG loc,unsigned int op)
{
  debugger("Unimplemented eora opcode!");
}


void Em6800::exec_eorb(ULONG loc,unsigned int op)
{
  debugger("Unimplemented eorb opcode!");
}


void Em6800::exec_exg(ULONG loc,unsigned int op)
{
  debugger("Unimplemented exg opcode!");
}


void Em6800::exec_inc(ULONG loc,unsigned int op)
{
  debugger("Unimplemented inc opcode!");
}


void Em6800::exec_inca(ULONG loc,unsigned int op)
{
  a++;
  set_flags_nzv(a);
}


void Em6800::exec_incb(ULONG loc,unsigned int op)
{
  b++;
  set_flags_nzv(b);
}


void Em6800::exec_jmp(ULONG loc,unsigned int op)
{
  ea=map->read16b(loc+1);
  jump(ea);
}


void Em6800::exec_jsr(ULONG loc,unsigned int op)
{
  ea=map->read16b(loc+1);
  stack_pushw(loc+3);
  jump(ea);
}


void Em6800::exec_lda(ULONG loc,unsigned int op)
{
  unsigned int val=0,ea=0;
  switch(op) {
    default:
      debugger("Unimplemented lda opcode!");
      break;
  }
  a=val;
  set_flags_nzv(val);
#ifdef DEBUG_EXEC
  DEBUG_OUT<<"LDA with "<<val<<" ea="<<ea<<"\n";
#endif
}


void Em6800::exec_ldb(ULONG loc,unsigned int op)
{
  unsigned int val=0,ea=0;
  switch(op) {
    case 0xc6:  // LDB #const8
      val=map->read8(loc+1);
      break;
    default:
      debugger("Unimplemented ldb opcode!");
      break;
  }
  b=val;
  set_flags_nzv(val);
#ifdef DEBUG_EXEC
  DEBUG_OUT<<"LDB with "<<val<<" ea="<<ea<<"\n";
#endif
}


void Em6800::exec_ldd(ULONG loc,unsigned int op)
{
  unsigned int val=0,ea=0;
  switch(op) {
    case 0xec: // LDD indexed
      ea=calcEAPostByte(loc+1);
      val=map->read16b(ea);
      break;
    default:
      debugger("Unimplemented ldd opcode!");
      break;
  }
  writeRegD(val);
  set_flags_nzv(val);
#ifdef DEBUG_EXEC
  DEBUG_OUT<<"LDD with "<<val<<" ea="<<ea<<"\n";
#endif
}


void Em6800::exec_lds(ULONG loc,unsigned int op)
{
  unsigned int val=0,ea=0;
  switch(op) {
    case 0xce:
      // lds #const16
      val=map->read16b(loc+1);
      break;
    default:
      DEBUG_OUT<<"loc="<<loc<<" op="<<op<<"\n";
      debugger("Unimplemented lds opcode!");
      break;
  }
  sp=val;
  set_flags_nzv(val);
#ifdef DEBUG_EXEC
  DEBUG_OUT<<"LDS with "<<val<<" ea="<<ea<<"\n";
#endif
}


void Em6800::exec_ldx(ULONG loc,unsigned int op)
{
  unsigned int val=0,ea=0;
  switch(op) {
    case 0x8e:  // LDX #const16
      val=map->read16b(loc+1);
      break;
    default:
      DEBUG_OUT<<"loc="<<loc<<" op="<<op<<"\n";
      debugger("Unimplemented ldx opcode!");
      break;
  }
  x=val;
  set_flags_nzv(val);
#ifdef DEBUG_EXEC
  DEBUG_OUT<<"LDX with "<<val<<" ea="<<ea<<"\n";
#endif
}


void Em6800::exec_ldy(ULONG loc,unsigned int op)
{
  unsigned int val=0,ea=0;
  switch(op) {
    case 0x8e:  // LDY #const16
      val=map->read16b(loc+1);
      break;
    default:
      DEBUG_OUT<<"loc="<<loc<<" op="<<op<<"\n";
      debugger("Unimplemented ldy opcode!");
      break;
  }
  y=val;
  set_flags_nzv(val);
#ifdef DEBUG_EXEC
  DEBUG_OUT<<"LDY with "<<val<<" ea="<<ea<<"\n";
#endif
}


void Em6800::exec_lsr(ULONG loc,unsigned int op)
{
  debugger("Unimplemented lsr opcode!");
}


void Em6800::exec_lsra(ULONG loc,unsigned int op)
{
  debugger("Unimplemented lsra opcode!");
}


void Em6800::exec_lsrb(ULONG loc,unsigned int op)
{
  debugger("Unimplemented lsrb opcode!");
}


void Em6800::exec_mul(ULONG loc,unsigned int op)
{
  debugger("Unimplemented mul opcode!");
}


void Em6800::exec_neg(ULONG loc,unsigned int op)
{
  debugger("Unimplemented neg opcode!");
}


void Em6800::exec_nega(ULONG loc,unsigned int op)
{
  debugger("Unimplemented nega opcode!");
}


void Em6800::exec_negb(ULONG loc,unsigned int op)
{
  debugger("Unimplemented negb opcode!");
}


void Em6800::exec_nop(ULONG loc,unsigned int op)
{
}


void Em6800::exec_ora(ULONG loc,unsigned int op)
{
  debugger("Unimplemented ora opcode!");
}


void Em6800::exec_orb(ULONG loc,unsigned int op)
{
  debugger("Unimplemented orb opcode!");
}


void Em6800::exec_pshs(ULONG loc,unsigned int op)
{
  unsigned int rs=map->read8(loc+1);
  if(rs&0x80) stack_pushw6800(sp,pc);
  if(rs&0x40) stack_pushw6800(sp,u);
  if(rs&0x20) stack_pushw6800(sp,y);
  if(rs&0x10) stack_pushw6800(sp,x);
  if(rs&0x08) stack_push6800(sp,dp>>8);
  if(rs&0x04) stack_push6800(sp,b);
  if(rs&0x02) stack_push6800(sp,a);
  if(rs&0x01) stack_push6800(sp,p);
}


void Em6800::exec_puls(ULONG loc,unsigned int op)
{
  unsigned int rs=map->read8(loc+1);
  if(rs&0x01) p=stack_pop6800(sp);
  if(rs&0x02) a=stack_pop6800(sp);
  if(rs&0x04) b=stack_pop6800(sp);
  if(rs&0x08) dp=stack_pop6800(sp)<<8;
  if(rs&0x10) x=stack_popw6800(sp);
  if(rs&0x20) y=stack_popw6800(sp);
  if(rs&0x40) u=stack_popw6800(sp);
  if(rs&0x80) pc=stack_popw6800(sp);
}


void Em6800::exec_rol(ULONG loc,unsigned int op)
{
  debugger("Unimplemented rol opcode!");
}


void Em6800::exec_rola(ULONG loc,unsigned int op)
{
  debugger("Unimplemented rola opcode!");
}


void Em6800::exec_rolb(ULONG loc,unsigned int op)
{
  debugger("Unimplemented rolb opcode!");
}


void Em6800::exec_ror(ULONG loc,unsigned int op)
{
  debugger("Unimplemented ror opcode!");
}


void Em6800::exec_rora(ULONG loc,unsigned int op)
{
  debugger("Unimplemented rora opcode!");
}


void Em6800::exec_rorb(ULONG loc,unsigned int op)
{
  debugger("Unimplemented rorb opcode!");
}


void Em6800::exec_rti(ULONG loc,unsigned int op)
{
  debugger("Unimplemented rti opcode!");
}


void Em6800::exec_rts(ULONG loc,unsigned int op)
{
  debugger("Unimplemented rts opcode!");
}


void Em6800::exec_sbca(ULONG loc,unsigned int op)
{
  debugger("Unimplemented scbca opcode!");
}


void Em6800::exec_sbcb(ULONG loc,unsigned int op)
{
  debugger("Unimplemented sbcb opcode!");
}


void Em6800::exec_sta(ULONG loc,unsigned int op)
{
  unsigned int val=a,ea=0;
  switch(op) {
    default:
      DEBUG_OUT<<"loc="<<loc<<" op="<<op<<"\n";
      debugger("Unimplemented sta opcode!");
      break;
  }
  map->write8(ea,val);
  set_flags_nzv(val);
#ifdef DEBUG_EXEC
  DEBUG_OUT<<"STA "<<val<<" at "<<ea<<"\n";
#endif
}


void Em6800::exec_stb(ULONG loc,unsigned int op)
{
  unsigned int val=b,ea=0;
  switch(op) {
    case 0xf7:  // STB $const16
      ea=map->read16b(loc+1);
      break;
    case 0xe7: // STB indexed
      ea=calcEAPostByte(loc+1);
      break;
    default:
      DEBUG_OUT<<"loc="<<loc<<" op="<<op<<"\n";
      debugger("Unimplemented stb opcode!");
      break;
  }
  map->write8(ea,val);
  set_flags_nzv(val);
#ifdef DEBUG_EXEC
  DEBUG_OUT<<"STB "<<val<<" at "<<ea<<"\n";
#endif
}


void Em6800::exec_std(ULONG loc,unsigned int op)
{
  unsigned int val=readRegD(),ea=0;
  switch(op) {
    case 0xed: // STD indexed
      ea=calcEAPostByte(loc+1);
      break;
    default:
      DEBUG_OUT<<"loc="<<loc<<" op="<<op<<"\n";
      debugger("Unimplemented std opcode!");
      break;
  }
  map->write16b(ea,val);
  set_flags_nzv(val);
#ifdef DEBUG_EXEC
  DEBUG_OUT<<"STD "<<val<<" at "<<ea<<"\n";
#endif
}


void Em6800::exec_sts(ULONG loc,unsigned int op)
{
  unsigned int val=sp,ea=0;
  switch(op) {
    default:
      DEBUG_OUT<<"loc="<<loc<<" op="<<op<<"\n";
      debugger("Unimplemented sts opcode!");
      break;
  }
  map->write16b(ea,val);
  set_flags_nzv(val);
#ifdef DEBUG_EXEC
  DEBUG_OUT<<"STS "<<val<<" at "<<ea<<"\n";
#endif
  debugger("Unimplemented sts opcode!");
}


void Em6800::exec_stx(ULONG loc,unsigned int op)
{
  unsigned int val=x,ea=0;
  switch(op) {
    case 0xaf:  // STX INDEXED
      ea=calcEAPostByte(loc+1);
      break;
    case 0xbf:  // STX $const16
      ea=map->read16b(loc+1);
      break;
    default:
      DEBUG_OUT<<"loc="<<loc<<" op="<<op<<"\n";
      debugger("Unimplemented stx opcode!");
      break;
  }
  map->write16b(ea,val);
  set_flags_nzv(val);
#ifdef DEBUG_EXEC
  DEBUG_OUT<<"STX "<<val<<" at "<<ea<<"\n";
#endif
}


void Em6800::exec_sty(ULONG loc,unsigned int op)
{
  unsigned int val=y,ea=0;
  switch(op) {
    default:
      DEBUG_OUT<<"loc="<<loc<<" op="<<op<<"\n";
      debugger("Unimplemented sty opcode!");
      break;
  }
  map->write16b(ea,val);
  set_flags_nzv(val);
#ifdef DEBUG_EXEC
  DEBUG_OUT<<"STY "<<val<<" at "<<ea<<"\n";
#endif
}


void Em6800::exec_suba(ULONG loc,unsigned int op)
{
  debugger("Unimplemented suba opcode!");
}


void Em6800::exec_subb(ULONG loc,unsigned int op)
{
  debugger("Unimplemented subb opcode!");
}


void Em6800::exec_subd(ULONG loc,unsigned int op)
{
  unsigned int v=map->read16b(loc+1);
  unsigned int val=readRegD()-v;
  writeRegD(val);
  set_flags_nzv(val);
#ifdef DEBUG_EXEC
  DEBUG_OUT<<"SUBD "<<v<<"\n";
#endif
}


void Em6800::exec_swi(ULONG loc,unsigned int op)
{
  debugger("Unimplemented swi opcode!");
}


void Em6800::exec_tfr(ULONG loc,unsigned int op)
{
  unsigned int r1=((map->read8(loc+1))>>4)&0xf;
  unsigned int r2=(map->read8(loc+1))&0xf;
  // NOTE: Doesn't catch mismatched reg sizes!
  writeReg(r2,readReg(r1));
}


void Em6800::exec_tst(ULONG loc,unsigned int op)
{
  debugger("Unimplemented tst opcode!");
}


void Em6800::exec_tsta(ULONG loc,unsigned int op)
{
  debugger("Unimplemented tsta opcode!");
}


void Em6800::exec_tstb(ULONG loc,unsigned int op)
{
  debugger("Unimplemented tstb opcode!");
}


void Em6800::exec_wai(ULONG loc,unsigned int op)
{
  debugger("Unimplemented wai opcode!");
}


void Em6800::exec_tpa(ULONG loc,unsigned int op)
{
  debugger("Unimplemented tpa opcode!");
}


void Em6800::exec_tap(ULONG loc,unsigned int op)
{
  debugger("Unimplemented tap opcode!");
}


void Em6800::exec_tsx(ULONG loc,unsigned int op)
{
  debugger("Unimplemented tsx opcode!");
}


void Em6800::exec_txs(ULONG loc,unsigned int op)
{
  debugger("Unimplemented txs opcode!");
}


void Em6800::exec_tba(ULONG loc,unsigned int op)
{
  debugger("Unimplemented tba opcode!");
}


void Em6800::exec_pula(ULONG loc,unsigned int op)
{
  debugger("Unimplemented pula opcode!");
}


void Em6800::exec_pulb(ULONG loc,unsigned int op)
{
  debugger("Unimplemented pulb opcode!");
}


void Em6800::exec_psha(ULONG loc,unsigned int op)
{
  debugger("Unimplemented psha opcode!");
}


void Em6800::exec_pshb(ULONG loc,unsigned int op)
{
  debugger("Unimplemented pshb opcode!");
}


void Em6800::exec_pulx(ULONG loc,unsigned int op)
{
  debugger("Unimplemented pulx opcode!");
}


void Em6800::exec_pshx(ULONG loc,unsigned int op)
{
  debugger("Unimplemented pshx opcode!");
}


void Em6800::exec_sba(ULONG loc,unsigned int op)
{
  debugger("Unimplemented sba opcode!");
}


void Em6800::exec_sev(ULONG loc,unsigned int op)
{
  debugger("Unimplemented sev opcode!");
}


void Em6800::exec_sei(ULONG loc,unsigned int op)
{
  debugger("Unimplemented sei opcode!");
}


void Em6800::exec_sec(ULONG loc,unsigned int op)
{
  debugger("Unimplemented sec opcode!");
}


void Em6800::exec_inx(ULONG loc,unsigned int op)
{
  debugger("Unimplemented inx opcode!");
}

void Em6800::exec_ins(ULONG loc,unsigned int op)
{
  debugger("Unimplemented ins opcode!");
}


void Em6800::exec_cba(ULONG loc,unsigned int op)
{
  debugger("Unimplemented cba opcode!");
}


void Em6800::exec_clv(ULONG loc,unsigned int op)
{
  debugger("Unimplemented clv opcode!");
}


void Em6800::exec_clc(ULONG loc,unsigned int op)
{
  debugger("Unimplemented clc opcode!");
}


void Em6800::exec_cec(ULONG loc,unsigned int op)
{
  debugger("Unimplemented cec opcode!");
}


void Em6800::exec_cli(ULONG loc,unsigned int op)
{
  debugger("Unimplemented cli opcode!");
}


void Em6800::exec_des(ULONG loc,unsigned int op)
{
  debugger("Unimplemented des opcode!");
}


void Em6800::exec_dex(ULONG loc,unsigned int op)
{
  debugger("Unimplemented dex opcode!");
}


void Em6800::exec_asld(ULONG loc,unsigned int op)
{
  debugger("Unimplemented asrd opcode!");
}


