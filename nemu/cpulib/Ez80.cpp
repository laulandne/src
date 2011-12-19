
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#define DEBUG_DIS_MODES 1

#define DEBUG_EXEC 1

#define CHECK_SAFE 1

#define SHOW_INTS  1


#include <cpulib/Ez80.h>

#include <elib/EMemMapAbstract.h>
#include <elib/EModule.h>
#include <elib/EHandler.h>

#include <portable/nickcpp/ANullStream.h>

#include <portable/nickcpp/AStringHelper.h>


#include <cpulib/generated/Ez80_tab.h>
#include <cpulib/generated/Ei8080_tab.h>


// The different cpus...
#define E_CPU_NONE     0x00
#define E_CPU_8080     0x01
#define E_CPU_8085     0x02
#define E_CPU_Z80      0x03
#define E_CPU_Z800     0x04


////////////////////////////////////////////////////////////////////////////////
//  Ez80 Class
////////////////////////////////////////////////////////////////////////////////

Ez80::Ez80()
{
  DEBUG_OUT<<"ez80 created...\n";
  init();
  type=E_CPU_Z80;
  //w=ww;
  reset();
}


Ez80::~Ez80()
{
  DEBUG_OUT<<"ez80 destroyed.\n";
}


void Ez80::init()
{
  ECPU8Bit::init();
  // Generic features
  doTrace=false;  doRun=false;
  // Set up stack and registers
  int_pending=0;
  intLock=false;
  lsb=true;
  sbits=(Ez80StatusBits *)&f;
  intsEnabled=false;
  setStack(0);
  setFramePtr(0);
  setResetStack(0);
  setResetEntry(0);
}


/* STATIC */
bool Ez80::recognize(const char *tname)
{
  const char *name=aDefaultStringHelper.removeSpaces2(aDefaultStringHelper.toLower(tname));
  if(!strcmp(name,"8080")) return true; else
  if(!strcmp(name,"8085")) return true; else
  if(!strcmp(name,"8080a")) return true; else
  if(!strcmp(name,"8085a")) return true; else
  if(!strcmp(name,"i8080")) return true; else
  if(!strcmp(name,"i8085")) return true; else
  if(!strcmp(name,"i8080a")) return true; else
  if(!strcmp(name,"i8085a")) return true; else
  if(!strcmp(name,"hd64180")) return true; else
  if(!strcmp(name,"z80")) return true; else
  if(!strcmp(name,"z80a")) return true; else
  if(!strcmp(name,"z80b")) return true; else
  if(!strcmp(name,"z80c")) return true; else
  if(!strcmp(name,"gbz80")) return true; else
  if(!strcmp(name,"z180")) return true; else
  if(!strcmp(name,"z280")) return true; else
  if(!strcmp(name,"z800")) return true; else
  return false;
}


const char *Ez80::getName()
{
  if(type==E_CPU_8080) return "Intel 8080";
  if(type==E_CPU_8085) return "Intel 8085";
  if(type==E_CPU_Z80) return "Zilog z80";
  if(type==E_CPU_Z800) return "Zilog z800";
  return "Zilog z80";
}


void Ez80::setKind(const char *tname)
{
  const char *name=aDefaultStringHelper.removeSpaces2(aDefaultStringHelper.toLower(tname));
  if(!strcmp(name,"8080")) type=E_CPU_8080;
  if(!strcmp(name,"8085")) type=E_CPU_8085;
  if(!strcmp(name,"8080a")) type=E_CPU_8080;
  if(!strcmp(name,"8085a")) type=E_CPU_8085;
  if(!strcmp(name,"i8080")) type=E_CPU_8080;
  if(!strcmp(name,"i8085")) type=E_CPU_8085;
  if(!strcmp(name,"i8080a")) type=E_CPU_8080;
  if(!strcmp(name,"i8085a")) type=E_CPU_8085;
  if(!strcmp(name,"hd64180")) type=E_CPU_Z80;
  if(!strcmp(name,"z80")) type=E_CPU_Z80;
  if(!strcmp(name,"z80a")) type=E_CPU_Z80;
  if(!strcmp(name,"z80b")) type=E_CPU_Z80;
  if(!strcmp(name,"z80c")) type=E_CPU_Z80;
  if(!strcmp(name,"gbz80")) type=E_CPU_Z80;
  if(!strcmp(name,"z180")) type=E_CPU_Z80;
  if(!strcmp(name,"z280")) type=E_CPU_Z800;
  if(!strcmp(name,"z800")) type=E_CPU_Z800;
}


void Ez80::reset()
{
#ifdef DEBUG_EXEC
  setTrace(true);
#endif
  breakpoint=0xffff;  // since we start at 0, breakpoint's default
  ix=0;  iy=0;  sp=0;
  a=0;  f=0;
  b=0;  c=0;  d=0;  e=0;  h=0;  l=0;
  a1=0;  f1=0;
  b1=0;  c1=0;  d1=0;  e1=0;  h1=0;  l1=0;
  i=0;
  intLock=false;
  pc=0;  // Jump to zero
  clearStackTrace();
  ECPUAbstract::reset();
}


bool Ez80::setRegister(const char *tname,ULONG value)
{
  const char *name=aDefaultStringHelper.removeSpaces2(aDefaultStringHelper.toLower(tname));
  bool ret=false;
  if(!strcmp(name,"pc"))
  { pc=(USHORT)value; ret=true; } else
  if(!strcmp(name,"a"))
  { a=(BYTE)value; ret=true; } else
  if(!strcmp(name,"ix"))
  { ix=(BYTE)value; ret=true; } else
  if(!strcmp(name,"iy"))
  { iy=(BYTE)value; ret=true; } else
  if(!strcmp(name,"f"))
  { f=(BYTE)value; ret=true; } else
  if(!strcmp(name,"s"))
  { sp=(BYTE)value; ret=true; }
  if(!strcmp(name,"sp"))
  { sp=(BYTE)value; ret=true; }
  if(!strcmp(name,"b"))
  { b=(BYTE)value; ret=true; } else
  if(!strcmp(name,"c"))
  { c=(BYTE)value; ret=true; } else
  if(!strcmp(name,"d"))
  { d=(BYTE)value; ret=true; } else
  if(!strcmp(name,"e"))
  { e=(BYTE)value; ret=true; } else
  if(!strcmp(name,"h"))
  { h=(BYTE)value; ret=true; } else
  if(!strcmp(name,"l"))
  { l=(BYTE)value; ret=true; } else
  if(!strcmp(name,"bc"))
  { writeReg(Ez80_REG_BC,value,Ez80_SIZE_WORD); ret=true; } else
  if(!strcmp(name,"de"))
  { writeReg(Ez80_REG_DE,value,Ez80_SIZE_WORD); ret=true; } else
  if(!strcmp(name,"hl"))
  { writeReg(Ez80_REG_HL,value,Ez80_SIZE_WORD); ret=true; }
  //free(name);
  return ret;
}


// TODO: finish this
ULONG Ez80::readRegister(const char *tname)
{
  const char *name=aDefaultStringHelper.removeSpaces2(aDefaultStringHelper.toLower(tname));
  if(!strcmp(name,"a")) { return a; }
  if(!strcmp(name,"s")) { return sp; }
  if(!strcmp(name,"sp")) { return sp; }
  if(!strcmp(name,"f")) { return f; }
  if(!strcmp(name,"ix")) { return ix; }
  if(!strcmp(name,"iy")) { return iy; }
  if(!strcmp(name,"b")) { return b; }
  if(!strcmp(name,"c")) { return c; }
  if(!strcmp(name,"d")) { return d; }
  if(!strcmp(name,"e")) { return e; }
  if(!strcmp(name,"h")) { return h; }
  if(!strcmp(name,"l")) { return l; }
  if(!strcmp(name,"bc")) { return readReg(Ez80_REG_BC,Ez80_SIZE_WORD); }
  if(!strcmp(name,"de")) { return readReg(Ez80_REG_DE,Ez80_SIZE_WORD); }
  if(!strcmp(name,"hl")) { return readReg(Ez80_REG_HL,Ez80_SIZE_WORD); }
  return 0;
}


void Ez80::wregs()
{
  DEBUG_OUT<<"PC="<<(UINT16)pc<<"  "<<"SP="<<sp<<"\n";
  DEBUG_OUT<<"A= "<<(UINT16)a;
  DEBUG_OUT<<"  BC= "<<(UINT16)readReg(Ez80_REG_BC,Ez80_SIZE_WORD);
  DEBUG_OUT<<"  DE= "<<(UINT16)readReg(Ez80_REG_DE,Ez80_SIZE_WORD);
  DEBUG_OUT<<"  HL= "<<(UINT16)readReg(Ez80_REG_HL,Ez80_SIZE_WORD)<<"\n";
  DEBUG_OUT<<"A'="<<(UINT16)a1;
  DEBUG_OUT<<"  BC'="<<(UINT16)readReg1(Ez80_REG_BC1,Ez80_SIZE_WORD);
  DEBUG_OUT<<"  DE'="<<(UINT16)readReg1(Ez80_REG_DE1,Ez80_SIZE_WORD);
  DEBUG_OUT<<"  HL'="<<(UINT16)readReg1(Ez80_REG_HL1,Ez80_SIZE_WORD)<<"\n";
  DEBUG_OUT<<"IX="<<(UINT16)ix<<"  IY="<<(UINT16)iy<<"\n";
  DEBUG_OUT<<"Flags="<<(BYTE)f<<" (";
  if(f&Ez80_STAT_S) DEBUG_OUT<<"S";  else DEBUG_OUT<<"s";
  if(f&Ez80_STAT_Z) DEBUG_OUT<<"Z";  else DEBUG_OUT<<"z";
  DEBUG_OUT<<"-";
  //if(f&Ez80_STAT_E1) DEBUG_OUT<<"E";  else DEBUG_OUT<<"e";
  if(f&Ez80_STAT_H) DEBUG_OUT<<"H";  else DEBUG_OUT<<"h";
  DEBUG_OUT<<"-";
  //if(f&Ez80_STAT_E2) DEBUG_OUT<<"E";  else DEBUG_OUT<<"e";
  if(f&Ez80_STAT_P) DEBUG_OUT<<"P";  else DEBUG_OUT<<"p";
  if(f&Ez80_STAT_N) DEBUG_OUT<<"N";  else DEBUG_OUT<<"n";
  if(f&Ez80_STAT_C) DEBUG_OUT<<"C";  else DEBUG_OUT<<"c";
  DEBUG_OUT<<")  F'="<<(BYTE)f1<<"\n";
}


void Ez80::set_flags_zn(int i)
{
  sbits->z=(i==0);
  sbits->n=(i&0x80)!=0;
}


// TODO: Finish this!
void Ez80::set_flags_szhpn(int i)
{
  //sbits->s=
  sbits->z=(i==0);
  //sbits->h=
  //sbits->p=
  sbits->n=0;
}


// TODO: Finish this!
void Ez80::set_flags_szhpc(int i,int j)
{
  UINT16 val=(UINT16)i-(UINT16)j;
  //sbits->s=
  sbits->z=(val==0);
  //sbits->h=
  //sbits->p=
  sbits->c=(val<(UINT16)0x100);
}


// TODO: Finish this!
void Ez80::set_flags_szp(int i)
{
  //sbits->s=
  sbits->z=(i==0);
  //sbits->p=
}


void Ez80::set_flags_znc(int i, int j)
{
  UINT16 val=(UINT16)i-(UINT16)j;
  sbits->z=(val==0);
  sbits->n=(val&0x80)!=0;
  sbits->c=(val<(UINT16)0x100);
}


// TODO: Move pending stuff to cause_int...
void Ez80::handle_int(ULONG value)
{
  int_pending=0;
  bool intDis=false;
  if(intLock) intDis=true;
  if(intDis) {
    if(value==Ez80_INT_IRQ) {
    }
  }
#ifdef SHOW_INTS
  DEBUG_OUT<<"+";
#endif
  stack_pushw(pc);
  //DEBUG_OUT<<"Pushed "<<pc<<"\n";
  stack_push(f);
  jump(map->read16l(value&map->getAddrMask()));
  //trace=true;
}


void Ez80::cause_int(ULONG value)
{
  if(!intsEnabled) {
    int_pending=value;
    return;
  }
  // TODO: What if we already had a pending one?
  switch(value) {
    case Ez80_INT_RESET:
      reset();
      break;
    case Ez80_INT_NMI:
      handle_int(value);
      break;
    case Ez80_INT_IRQ:
      handle_int(value);
      break;
    default:
      debugger("Illegal interrupt requested!\n");
      break;
  }
}


void Ez80::ints()
{
  //if(w) {
    DEBUG_OUT<<"IRQ ("<<(unsigned short)Ez80_INT_IRQ<<"): "
      <<(unsigned int)map->read16l(Ez80_INT_IRQ&map->getAddrMask())<<"\n";
    DEBUG_OUT<<"NMI ("<<(unsigned short)Ez80_INT_NMI<<"): "
      <<(unsigned int)map->read16l(Ez80_INT_NMI&map->getAddrMask())<<"\n";
    DEBUG_OUT<<"RESET ("<<(unsigned short)Ez80_INT_RESET<<"): "
      <<(unsigned int)map->read16l(Ez80_INT_RESET&map->getAddrMask())<<"\n";
  //}
}


void Ez80::printCondCode(unsigned int num)
{
  switch(num) {
    case Ez80_CC_NZ: DEBUG_OUT<<"nz"; break;
    case Ez80_CC_Z: DEBUG_OUT<<"z"; break;
    case Ez80_CC_NC: DEBUG_OUT<<"nc"; break;
    case Ez80_CC_C: DEBUG_OUT<<"c"; break;
    case Ez80_CC_NP: DEBUG_OUT<<"np"; break;
    case Ez80_CC_P: DEBUG_OUT<<"p"; break;
    case Ez80_CC_NS: DEBUG_OUT<<"ns"; break;
    case Ez80_CC_S: DEBUG_OUT<<"s"; break;
    default:
      debugger("printCondCode: Bad cond code!\n");
      break;
  }
}


void Ez80::printReg(unsigned int num,unsigned int size)
{
  DEBUG_OUT<<getRegString(num,size);
}


char *Ez80::getRegString(unsigned int num,unsigned int size)
{
  switch(size) {
    case Ez80_SIZE_BYTE:
      switch(num) {
        // Note: No case for "X" (i.e. num==6)
        case Ez80_REG_B: return (char *)"b"; break;
        case Ez80_REG_C: return (char *)"c"; break;
        case Ez80_REG_D: return (char *)"d"; break;
        case Ez80_REG_E: return (char *)"e"; break;
        case Ez80_REG_H: return (char *)"h"; break;
        case Ez80_REG_L: return (char *)"l"; break;
        case Ez80_REG_A: return (char *)"a"; break;
        default:
          DEBUG_OUT<<"getRegString: bad regb="<<num<<"!\n";
          debugger("getRegString byte: Bad regb num!");
          break;
      }
      break;
    case Ez80_SIZE_WORD:
      switch(num) {
        case Ez80_REG_BC: return (char *)"bc"; break;
        case Ez80_REG_DE: return (char *)"de"; break;
        case Ez80_REG_HL: return (char *)"hl"; break;
        case Ez80_REG_SP: return (char *)"sp"; break;
        default:
          DEBUG_OUT<<"getRegString: bad regw="<<num<<"!\n";
          debugger("getRegString word: Bad regw num!");
          break;
      }
      break;
    default:
      DEBUG_OUT<<"getRegString: bad size="<<size<<"!\n";
      debugger("getRegString: Bad size!\n");
      break;
  }
  return (char *)NULL;
}


unsigned int Ez80::readReg(unsigned int n,unsigned int size)
{
  //DEBUG_OUT<<"readReg("<<n<<","<<size<<")\n";
  switch(size) {
    case Ez80_SIZE_BYTE:
      switch(n) {
        // Note: No case for "X" (i.e. num==6)
        case Ez80_REG_A: return a; break;
        case Ez80_REG_B: return b; break;
        case Ez80_REG_C: return c; break;
        case Ez80_REG_D: return d; break;
        case Ez80_REG_E: return e; break;
        case Ez80_REG_H: return h; break;
        case Ez80_REG_L: return l; break;
        default:
          DEBUG_OUT<<"readReg: bad regb="<<n<<"!\n";
          debugger("readReg byte: Bad reg num!\n");
          break;
      }
      break;
    case Ez80_SIZE_WORD:
      switch(n) {
        case Ez80_REG_BC: return (b<<8)|c; break;
        case Ez80_REG_DE: return (d<<8)|e; break;
        case Ez80_REG_HL: return (h<<8)|l; break;
        case Ez80_REG_SP: return sp; break;
        default:
          DEBUG_OUT<<"readReg: bad regw="<<n<<"!\n";
          debugger("readReg word: Bad reg num!\n");
          break;
      }
      break;
    default:
      DEBUG_OUT<<"readReg: bad size="<<size<<"!\n";
      debugger("readReg: Bad size!\n");
      break;
  }
  return 0;
}


unsigned int Ez80::readReg1(unsigned int n,unsigned int size)
{
  //DEBUG_OUT<<"readReg1("<<n<<","<<size<<")\n";
  switch(size) {
    case Ez80_SIZE_BYTE:
      switch(n) {
        // Note: No case for "X" (i.e. num==6)
        case Ez80_REG_A1: return a1; break;
        case Ez80_REG_B1: return b1; break;
        case Ez80_REG_C1: return c1; break;
        case Ez80_REG_D1: return d1; break;
        case Ez80_REG_E1: return e1; break;
        case Ez80_REG_H1: return h1; break;
        case Ez80_REG_L1: return l1; break;
        default:
          DEBUG_OUT<<"readReg1: bad regb="<<n<<"!\n";
          debugger("readReg1 byte: Bad reg num!\n");
          break;
      }
      break;
    case Ez80_SIZE_WORD:
      switch(n) {
        case Ez80_REG_BC1: return (b1<<8)|c1; break;
        case Ez80_REG_DE1: return (d1<<8)|e1; break;
        case Ez80_REG_HL1: return (h1<<8)|l1; break;
        default:
          DEBUG_OUT<<"readReg1: bad regw="<<n<<"!\n";
          debugger("readReg1 word: Bad reg num!\n");
          break;
      }
      break;
    default:
      DEBUG_OUT<<"readReg1: bad size="<<size<<"!\n";
      debugger("readReg1: Bad size!\n");
      break;
  }
  return 0;
}


// TODO: Check overflow on val by reg size
void Ez80::writeReg(unsigned int n,unsigned int val,unsigned int size)
{
#ifdef DEBUG_EXEC
  //DEBUG_OUT<<"writeReg("<<n<<","<<val<<","<<size<<")\n";
#endif
  BYTE nval=0;
  switch(size) {
    case Ez80_SIZE_BYTE:
      switch(n) {
        // Note: No case for "X" (i.e. num==6)
        case Ez80_REG_A: a=val; break;
        case Ez80_REG_B: b=val; break;
        case Ez80_REG_C: c=val; break;
        case Ez80_REG_D: d=val; break;
        case Ez80_REG_E: e=val; break;
        case Ez80_REG_H: h=val; break;
        case Ez80_REG_L: l=val; break;
        default:
          DEBUG_OUT<<"writeReg: bad regb="<<n<<"!\n";
          debugger("writeReg byte: Bad reg num!\n");
          break;
      }
      break;
    case Ez80_SIZE_WORD:
      switch(n) {
        case Ez80_REG_BC: b=(val>>8)&0xff;  c=val&0xff; break;
        case Ez80_REG_DE: d=(val>>8)&0xff;  e=val&0xff; break;
        case Ez80_REG_HL: h=(val>>8)&0xff;  l=val&0xff; break;
        case Ez80_REG_SP: sp=val; break;
        default:
          DEBUG_OUT<<"writeReg: bad regw="<<n<<"!\n";
          debugger("writeReg word: Bad reg num!\n");
          break;
      }
      break;
    default:
      DEBUG_OUT<<"writeReg: bad size="<<size<<"!\n";
      debugger("writeReg: Bad size!\n");
      break;
  }
}


// TODO: Check overflow on val by reg size
void Ez80::writeReg1(unsigned int n,unsigned int val,unsigned int size)
{
#ifdef DEBUG_EXEC
  //DEBUG_OUT<<"writeReg1("<<n<<","<<val<<","<<size<<")\n";
#endif
  BYTE nval=0;
  switch(size) {
    case Ez80_SIZE_BYTE:
      switch(n) {
        // Note: No case for "X" (i.e. num==6)
        case Ez80_REG_A1: a1=val; break;
        case Ez80_REG_B1: b1=val; break;
        case Ez80_REG_C1: c1=val; break;
        case Ez80_REG_D1: d1=val; break;
        case Ez80_REG_E1: e1=val; break;
        case Ez80_REG_H1: h1=val; break;
        case Ez80_REG_L1: l1=val; break;
        default:
          DEBUG_OUT<<"writeReg1: bad regb="<<n<<"!\n";
          debugger("writeReg1 byte: Bad reg num!\n");
          break;
      }
      break;
    case Ez80_SIZE_WORD:
      switch(n) {
        case Ez80_REG_BC1: b1=(val>>8)&0xff;  c1=val&0xff; break;
        case Ez80_REG_DE1: d1=(val>>8)&0xff;  e1=val&0xff; break;
        case Ez80_REG_HL1: h1=(val>>8)&0xff;  l1=val&0xff; break;
        default:
          DEBUG_OUT<<"writeReg1: bad regw="<<n<<"!\n";
          debugger("writeReg1 word: Bad reg num!\n");
          break;
      }
      break;
    default:
      DEBUG_OUT<<"writeReg1: bad size="<<size<<"!\n";
      debugger("writeReg1: Bad size!\n");
      break;
  }
}


// TODO: Check and handle any requested ints between instructions
void Ez80::execute()
{
  if(pc==breakpoint) { debugger("Breakpoint!\n"); return; }
  opcode=map->read8(pc);
  op_len=Ez80_size[opcode];
  cycle=Ez80_time[opcode];
  unsigned int op=opcode,loc=pc,topcode=opcode;
#include <cpulib/generated/Ez80_exec.h>
  pc+=op_len;
  cycles+=cycle;
  if(int_pending) handle_int(int_pending);
}


ULONG Ez80::disasm(ULONG loc)
{
  unsigned int len=0,mode=Ez80_NONE;
  const char *name="???";
  topcode=map->read8(loc);
  lastSymbol=map->findAnyExactSymbol(loc);
  if(lastSymbol) DEBUG_OUT<<lastSymbol->getName()<<":\n";
  DEBUG_OUT<<"."<<loc<<"  ";
  len=Ez80_size[topcode];
  mode=Ez80_format[topcode];
  if(type==E_CPU_Z80) name=Ez80_name[topcode];
  else name=Ei8080_name[topcode];
  if(!len) len=1;  // NOTE: An unimplemented opcode...
  unsigned int t;
  for(t=0;t<len;t++) DEBUG_OUT<<aDefaultStringHelper.myHex2(map->read8(loc+t))<<" ";
  for(t=0;t<(4-len);t++) DEBUG_OUT<<"   ";
  switch(mode) {
    case Ez80_SPECIAL:
      len=dis_special(loc,topcode,len);
      break;
    default:
      DEBUG_OUT<<name<<" ";
      disasmAddrMode(loc,mode);
      break;
  }
  DEBUG_OUT<<"\n";
  return len;
}


// NOTE: loc points to 1st or 2nd opcode byte (if any)
void Ez80::disasmAddrMode(ULONG loc,unsigned int mode)
{
  //unsigned int dig=w->getHexDigits();
  //w->setHexDigits(4);
  switch(mode) {
      //
    case Ez80_NONE:
      DEBUG_OUT<<"    ;Ez80_NONE";
      break;
      //
    case Ez80_SPECIAL:
      DEBUG_OUT<<"    ;Ez80_SPECIAL";
      break;
      //
    case Ez80_ILLEGAL:
      DEBUG_OUT<<"    ;Ez80_ILLEGAL";
      break;
      //
    case Ez80_CONST:
      DEBUG_OUT<<"$?const?";
      DEBUG_OUT<<"    ;Ez80_CONST";
      break;
      //
    case Ez80_REGB:
      reg=map->read8(loc)&0x7;
      printReg(reg,Ez80_SIZE_BYTE);
      DEBUG_OUT<<"    ;Ez80_REGB";
      break;
      //
    case Ez80_REGB2:
      reg=(map->read8(loc)>>3)&0x7;
      printReg(reg,Ez80_SIZE_BYTE);
      DEBUG_OUT<<"    ;Ez80_REGB2";
      break;
      //
    case Ez80_REGW:
      reg=(map->read8(loc)>>4)&0x3;
      printReg(reg,Ez80_SIZE_WORD);
      DEBUG_OUT<<"    ;Ez80_REGW";
      break;
      //
    case Ez80_A_REGB:
      DEBUG_OUT<<"a,";
      reg=map->read8(loc)&0x7;
      printReg(reg,Ez80_SIZE_BYTE);
      DEBUG_OUT<<"    ;Ez80_REGB";
      break;
      //
    case Ez80_REGB_REGB:
      reg2=(map->read8(loc)>>3)&0x7;
      printReg(reg2,Ez80_SIZE_BYTE);
      DEBUG_OUT<<",";
      reg=map->read8(loc)&0x7;
      printReg(reg,Ez80_SIZE_BYTE);
      DEBUG_OUT<<"    ;Ez80_REGB_REGB";
      break;
      //
    case Ez80_REGW_REGW:
      reg2=(map->read8(loc)>>3)&0x7;
      printReg(reg2,Ez80_SIZE_WORD);
      DEBUG_OUT<<",";
      reg=(map->read8(loc)>>4)&0x3;
      printReg(reg,Ez80_SIZE_WORD);
      DEBUG_OUT<<"    ;Ez80_REGW_REGW";
      break;
      //
    case Ez80_REGB_BYTE:
      reg=(map->read8(loc)>>3)&0x7;
      printReg(reg,Ez80_SIZE_BYTE);
      DEBUG_OUT<<",#$"<<(BYTE)map->read8(loc+1);
      DEBUG_OUT<<"    ;Ez80_REGB_BYTE";
      break;
      //
    case Ez80_REGW_WORD:
      reg=(map->read8(loc)>>4)&0x3;
      printReg(reg,Ez80_SIZE_WORD);
      DEBUG_OUT<<",#$"<<(UINT16)map->read16l(loc+1);
      DEBUG_OUT<<"    ;Ez80_REGW_WORD";
      break;
      //
    case Ez80_INDREGW:
      reg=(map->read8(loc)>>4)&0x3;
      DEBUG_OUT<<"(";
      printReg(reg,Ez80_SIZE_WORD);
      DEBUG_OUT<<")";
      DEBUG_OUT<<"    ;Ez80_INDREGW";
      break;
      //
    case Ez80_INDREGW_BYTE:
      reg=map->read8(loc)&0x3;
      printReg(reg,Ez80_SIZE_BYTE);
      DEBUG_OUT<<",#$"<<(short)map->read8(loc+1);
      DEBUG_OUT<<"    ;Ez80_INDREGW_BYTE";
      break;
      //
    case Ez80_INDBYTE_REGB:
      reg=(map->read8(loc)>>4)&0x3;
      DEBUG_OUT<<"($"<<(short)map->read8(loc+1)<<"),";
      printReg(reg,Ez80_SIZE_BYTE);
      DEBUG_OUT<<"    ;Ez80_INDBYTE_REGB";
      break;
      //
    case Ez80_REGW_INDABS:
      reg=(map->read8(loc)>>4)&0x3;
      printReg(reg,Ez80_SIZE_WORD);
      DEBUG_OUT<<",($"<<map->read16l(loc+1)<<")";
      DEBUG_OUT<<"    ;Ez80_REGW_INDABS";
      break;
      //
    case Ez80_REGW_ABS:
      reg=(map->read8(loc)>>4)&0x3;
      printReg(reg,Ez80_SIZE_WORD);
      DEBUG_OUT<<",$"<<map->read16l(loc+1);
      DEBUG_OUT<<"    ;Ez80_REGW_ABS";
      break;
      //
    case Ez80_INDABS_REGW:
      DEBUG_OUT<<"($"<<map->read16l(loc+1)<<"),";
      reg=(map->read8(loc)>>4)&0x3;
      printReg(reg,Ez80_SIZE_WORD);
      DEBUG_OUT<<"    ;Ez80_INDABS_REGW";
      break;
      //
    case Ez80_INDABS_REGB:
      DEBUG_OUT<<"($"<<map->read16l(loc+1)<<"),";
      reg=(map->read8(loc)>>3)&0x7;
      printReg(reg,Ez80_SIZE_BYTE);
      DEBUG_OUT<<"    ;Ez80_INDABS_REGB";
      break;
      //
    case Ez80_REGB_INDABS:
      reg=(map->read8(loc)>>3)&0x7;
      printReg(reg,Ez80_SIZE_BYTE);
      DEBUG_OUT<<",($"<<map->read16l(loc+1)<<")";
      DEBUG_OUT<<"    ;Ez80_REGB_INDABS";
      break;
      //
    case Ez80_REGB_INDREGW:
      reg=(map->read8(loc)>>3)&0x7;
      printReg(reg,Ez80_SIZE_BYTE);
      DEBUG_OUT<<",(hl)";
      DEBUG_OUT<<"    ;Ez80_REGB_INDREGW";
      break;
      //
    case Ez80_INDREGW_REGB:
      reg=map->read8(loc)&0x7;
      DEBUG_OUT<<"(hl),";
      printReg(reg,Ez80_SIZE_BYTE);
      DEBUG_OUT<<"    ;Ez80_INDREGW_REGB";
      break;
      //
    case Ez80_INDREGW_REGW:
      reg=(map->read8(loc)>>3)&0x7;
      DEBUG_OUT<<"(hl),";
      printReg(reg,Ez80_SIZE_WORD);
      DEBUG_OUT<<"    ;Ez80_INDREGW_REGW";
      break;
      //
    case Ez80_BYTE:
      DEBUG_OUT<<"#$"<<(short)map->read8(loc+1);
      DEBUG_OUT<<"    ;Ez80_BYTE";
      break;
      //
    case Ez80_REGB_INDBYTE:
      reg=(map->read8(loc)>>3)&0x7;
      printReg(reg,Ez80_SIZE_BYTE);
      DEBUG_OUT<<",($"<<(short)map->read8(loc+1)<<")";
      DEBUG_OUT<<"    ;Ez80_INDBYTE";
      break;
      //
    case Ez80_DJ_REL:
    case Ez80_J_REL:
      DEBUG_OUT<<"$"<<(short)map->read8(loc+1)<<"  ;"<<(UINT16)((SBYTE)map->read8(loc+1)+loc+2);
      DEBUG_OUT<<"    ;Ez80_J_REL";
      break;
      //
    case Ez80_ABS:
    case Ez80_JABS:
      DEBUG_OUT<<"$"<<map->read16l(loc+1);
      DEBUG_OUT<<"    ;Ez80_ABS";
      break;
      //
    case Ez80_JCC_REL:
      reg=(map->read8(loc)>>3)&0x3;
      printCondCode(reg);
      DEBUG_OUT<<",$"<<(short)map->read8(loc+1)<<"  ;";
      DEBUG_OUT<<(UINT16)((SBYTE)map->read8(loc+1)+loc+2);
      DEBUG_OUT<<"    ;Ez80_JCC_REL";
      break;
      //
    case Ez80_JCC_ABS:
      reg=(map->read8(loc)>>3)&0x7;
      printCondCode(reg);
      DEBUG_OUT<<",$"<<map->read16l(loc+1);
      DEBUG_OUT<<"    ;Ez80_JCC_ABS";
      break;
      //
    default:
      DEBUG_OUT<<"???";
      DEBUG_OUT<<"    ;Ez80_???";
      debugger("bad address mode in disasmAddrMode!");
      break;
  };
  //w->setHexDigits(dig);
}


unsigned int Ez80::dis_special(ULONG loc,BYTE topcode,unsigned int len)
{
  BYTE mode=Ez80_NONE;
  BYTE nextcode=map->read8(loc+1);
  bool more=false;
  switch(topcode) {
    case 0x08:
      DEBUG_OUT<<"ex af,af'";
      break;
    case 0x19:
      DEBUG_OUT<<"add hl,";
      reg=(topcode>>4)&0x3;
      printReg(reg,Ez80_SIZE_WORD);
      break;
    case 0x32:
      DEBUG_OUT<<"ld (";
      DEBUG_OUT<<map->read16l(loc+1);
      DEBUG_OUT<<"),a";
      break;
    case 0xc7: case 0xcf: case 0xd7: case 0xdf: case 0xe7: case 0xef: case 0xf7: case 0xff:
      reg=(topcode>>3)&0x7;  // not a reg, but a const
      DEBUG_OUT<<"rst $";
      switch(reg) {
        case 0: DEBUG_OUT<<"0000"; break;
        case 1: DEBUG_OUT<<"0008"; break;
        case 2: DEBUG_OUT<<"0010"; break;
        case 3: DEBUG_OUT<<"0018"; break;
        case 4: DEBUG_OUT<<"0020"; break;
        case 5: DEBUG_OUT<<"0028"; break;
        case 6: DEBUG_OUT<<"0030"; break;
        case 7: DEBUG_OUT<<"0038"; break;
        default: DEBUG_OUT<<"???"; break;
      }
      break;
    case 0xcb:
      switch(nextcode) {
        case 0x12:
          DEBUG_OUT<<"rl";  mode=Ez80_REGB;  more=true;
          break;
        case 0x23:
          DEBUG_OUT<<"sla";  mode=Ez80_REGB;  more=true;
          break;
        case 0x39:
          DEBUG_OUT<<"srl";  mode=Ez80_REGB;  more=true;
          break;
        case 0x57:
          DEBUG_OUT<<"bit 2,";  mode=Ez80_REGB;  more=true;
          break;
        default:
          DEBUG_OUT<<Ez80_name[topcode]<<" ?special_0xcb/0x"<<(short)nextcode<<"?\n";
          debugger("Unimplemented opcode on dis_special!");
          break;
      }
      break;
    case 0xd3:
      DEBUG_OUT<<"out (";
      DEBUG_OUT<<map->read8(loc+1);
      DEBUG_OUT<<"),a";
      break;
    case 0xed:
      switch(nextcode) {
        case 0x4b:  case 0x5b:
          DEBUG_OUT<<"ld ";  mode=Ez80_REGW_ABS;  len+=2;  more=true;
          break;
        case 0x53:
          DEBUG_OUT<<"ld ";  mode=Ez80_INDABS_REGW;  len+=2;  more=true;
          break;
        case 0x51: case 0x59: case 0x79:
          reg=(nextcode>>3)&0x7;
          DEBUG_OUT<<"out (bc),";
          printReg(reg,Ez80_SIZE_BYTE);
          break;
        case 0x78:
          reg=(nextcode>>3)&0x7;
          DEBUG_OUT<<"in ";
          printReg(reg,Ez80_SIZE_BYTE);
          DEBUG_OUT<<",(bc)";
          break;
        default:
          DEBUG_OUT<<Ez80_name[topcode]<<" ?special_0xed/0x"<<(short)nextcode<<"?\n";
          debugger("Unimplemented opcode on dis_special!");
          break;
      }
      break;
    default:
      DEBUG_OUT<<Ez80_name[topcode]<<" ?special_0x"<<(short)topcode<<"?\n";
      debugger("Unimplemented opcode on dis_special!");
      break;
  }
  if(more) {
    disasmAddrMode(loc+1,mode);
  }
  return len;
}


void Ez80::doOutByte(ULONG addr,BYTE val)
{
  map->getIOHandler()->io_out8(addr,val);
}


BYTE Ez80::doInByte(ULONG addr)
{
  return map->getIOHandler()->io_in8(addr);
}


void Ez80::exec_adc(ULONG loc,unsigned int op)
{
  debugger("Unimplemented adc opcode!");
}


void Ez80::exec_add(ULONG loc,unsigned int op)
{
  debugger("Unimplemented add opcode!");
}


void Ez80::exec_and(ULONG loc,unsigned int op)
{
  switch(op) {
    case 0xa4: // and h
      a=a&h;
      set_flags_szp(a);
      break;
    case 0xe6: // and #const8
      a=a&map->read8(loc+1);
      set_flags_szp(a);
      break;
    default:
      debugger("Unimplemented and opcode!");
      break;
  }
}


void Ez80::exec_call(ULONG loc,unsigned int op)
{
  switch(op) {
    case 0xcd:
      ea=map->read16l(loc+1);
      break;
    default:
      debugger("Unimplemented call opcode!");
      break;
  }
  pc+=op_len;
  pushAddr(pc);
  jump(ea);
}


void Ez80::exec_ccf(ULONG loc,unsigned int op)
{
  debugger("Unimplemented ccf opcode!");
}


void Ez80::exec_cp(ULONG loc,unsigned int op)
{
  switch(op) {
    case 0xfe:
      // cp #00
      val=map->read8(loc+1);
      set_flags_szhpc(a,val);
      sbits->n=1;
      break;
    default:
      debugger("Unimplemented sbc opcode!");
      break;
  }
}


// TODO: Is this right?!?
void Ez80::exec_cpl(ULONG loc,unsigned int op)
{
  a=~a;
  sbits->h=1;
  sbits->n=1;
}


void Ez80::exec_daa(ULONG loc,unsigned int op)
{
  debugger("Unimplemented daa opcode!");
}


void Ez80::exec_dec(ULONG loc,unsigned int op)
{
  switch(op) {
    case 0x3d: // dec a
      a--;
      set_flags_szhpn(a);
      break;
    case 0x05: // dec b
      b--;
      set_flags_szhpn(b);
      break;
    case 0x0d: // dec c
      c--;
      set_flags_szhpn(c);
      break;
    case 0x15: // dec d
      d--;
      set_flags_szhpn(d);
      break;
    case 0x1d: // dec e
      e--;
      set_flags_szhpn(e);
      break;
    case 0x25: // dec h
      h--;
      set_flags_szhpn(h);
      break;
    case 0x2d: // dec l
      l--;
      set_flags_szhpn(l);
      break;
    case 0x0b: // dec bc
      val=readReg(Ez80_REG_BC,Ez80_SIZE_WORD)-1;
      writeReg(Ez80_REG_BC,val,Ez80_SIZE_WORD);
      break;
    case 0x1b: // dec de
      val=readReg(Ez80_REG_DE,Ez80_SIZE_WORD)-1;
      writeReg(Ez80_REG_DE,val,Ez80_SIZE_WORD);
      break;
    case 0x2b: // dec hl
      val=readReg(Ez80_REG_HL,Ez80_SIZE_WORD)-1;
      writeReg(Ez80_REG_HL,val,Ez80_SIZE_WORD);
      break;
    case 0x3b: // dec sp
      sp--;
      break;
    default:
      debugger("Unimplemented dec opcode!");
      break;
  }
}


void Ez80::exec_di(ULONG loc,unsigned int op)
{
  intsEnabled=false;
}


void Ez80::exec_djnz(ULONG loc,unsigned int op)
{
  debugger("Unimplemented djnz opcode!");
}


void Ez80::exec_ei(ULONG loc,unsigned int op)
{
  intsEnabled=true;
}


void Ez80::exec_ex(ULONG loc,unsigned int op)
{
  BYTE t;
  t=a; a=a1; a1=t;
  t=f; f=f1; f1=t;
}


void Ez80::exec_exx(ULONG loc,unsigned int op)
{
  BYTE t;
  t=b; b=b1; b1=t;
  t=c; c=c1; c1=t;
  t=d; d=d1; d1=t;
  t=e; e=e1; e1=t;
  t=h; h=h1; h1=t;
  t=l; l=l1; l1=t;
}


void Ez80::exec_halt(ULONG loc,unsigned int op)
{
  debugger("Unimplemented halt opcode!");
}


void Ez80::exec_in(ULONG loc,unsigned int op)
{
  debugger("Unimplemented in opcode!");
}


void Ez80::exec_inc(ULONG loc,unsigned int op)
{
  switch(op) {
    case 0x3c: // inc a
      a++;
      set_flags_szhpn(a);
      break;
    case 0x04: // inc b
      b++;
      set_flags_szhpn(b);
      break;
    case 0x0c: // inc c
      c++;
      set_flags_szhpn(c);
      break;
    case 0x14: // inc d
      d++;
      set_flags_szhpn(d);
      break;
    case 0x1c: // inc e
      e++;
      set_flags_szhpn(e);
      break;
    case 0x24: // inc h
      h++;
      set_flags_szhpn(h);
      break;
    case 0x2c: // inc l
      l++;
      set_flags_szhpn(l);
      break;
    case 0x03: // inc bc
      val=readReg(Ez80_REG_BC,Ez80_SIZE_WORD)+1;
      writeReg(Ez80_REG_BC,val,Ez80_SIZE_WORD);
      break;
    case 0x13: // inc de
      val=readReg(Ez80_REG_DE,Ez80_SIZE_WORD)+1;
      writeReg(Ez80_REG_DE,val,Ez80_SIZE_WORD);
      break;
    case 0x23: // inc hl
      val=readReg(Ez80_REG_HL,Ez80_SIZE_WORD)+1;
      writeReg(Ez80_REG_HL,val,Ez80_SIZE_WORD);
      break;
    case 0x33: // inc sp
      sp++;
      break;
    default:
      debugger("Unimplemented inc opcode!");
      break;
  }
}


void Ez80::exec_jp(ULONG loc,unsigned int op)
{
  ea=map->read16l(loc+1);
#ifdef DEBUG_EXEC
  DEBUG_OUT<<"jp ea="<<ea<<"\n";
#endif
  jump(ea);
}


void Ez80::exec_jr(ULONG loc,unsigned int op)
{
  bool doJump=false;
  ea=(UINT16)((SBYTE)map->read8(loc+1)+loc+2);
  switch(op) {
    case 0x20: // jr nz
      if(!sbits->z) doJump=true;
      break;
    case 0x28: // jr z
      if(sbits->z) doJump=true;
      break;
    default:
      debugger("Unimplemented jr opcode!");
      break;
  }
#ifdef DEBUG_EXEC
  DEBUG_OUT<<"jr op="<<op<<" cc="<<doJump<<" to "<<ea<<"\n";
#endif
  if(doJump) jump(ea);
}


// TODO: Doesn't handle signed offsets!
void Ez80::exec_ld(ULONG loc,unsigned int op)
{
  switch(op) {
    case 0x06: case 0x0e: case 0x16:
    case 0x1e: case 0x26: case 0x2e: case 0x3e:
      // ld r,const8
      val=map->read8(loc+1);
      reg=(op>>3)&0x7;
      writeReg(reg,val,Ez80_SIZE_BYTE);
      break;
    case 0x01: case 0x11: case 0x21: case 0x31:
      // ld rr,const16
      val=map->read16l(loc+1);
      reg=(op>>4)&0x3;
      writeReg(reg,val,Ez80_SIZE_WORD);
      break;
    case 0x7e: case 0x46: case 0x4e: case 0x56:
    case 0x5e: case 0x66: case 0x6e:
      // ld r,(hl)
      reg=(op>>3)&0x7;
      ea=readReg(Ez80_REG_HL,Ez80_SIZE_WORD);
      val=map->read8(ea);
      writeReg(reg,val,Ez80_SIZE_BYTE);
      break;
    case 0x7f: case 0x78: case 0x79: case 0x7a: case 0x7b: case 0x7c: case 0x7d:
    case 0x4f: case 0x48: case 0x49: case 0x4a: case 0x4b: case 0x4c: case 0x4d:
    case 0x5f: case 0x58: case 0x59: case 0x5a: case 0x5b: case 0x5c: case 0x5d:
    case 0x6f: case 0x68: case 0x69: case 0x6a: case 0x6b: case 0x6c: case 0x6d:
    case 0x47: case 0x40: case 0x41: case 0x42: case 0x43: case 0x44: case 0x45:
    case 0x57: case 0x50: case 0x51: case 0x52: case 0x53: case 0x54: case 0x55:
    case 0x67: case 0x60: case 0x61: case 0x62: case 0x63: case 0x64: case 0x65:
      // ld r,r
      reg=(op>>3)&0x7;
      ea=op&0x7; // actually not an ea, but a 2nd register
      val=readReg(ea,Ez80_SIZE_BYTE);
      writeReg(reg,val,Ez80_SIZE_BYTE);
      break;
    case 0x02:
      // ld (bc),a
      ea=readReg(Ez80_REG_BC,Ez80_SIZE_WORD);
      map->write8(ea,a);
      break;
    case 0x12:
      // ld (de),a
      ea=readReg(Ez80_REG_DE,Ez80_SIZE_WORD);
      map->write8(ea,a);
      break;
    case 0x36:
      // ld (hl),const8
      ea=readReg(Ez80_REG_HL,Ez80_SIZE_WORD);
      val=map->read8(loc+1);
      map->write8(ea,val);
      break;
    case 0x77: case 0x70: case 0x71: case 0x72: case 0x73: case 0x74: case 0x75:
      // ld (hl),r
      reg=op&0x7;
      val=readReg(reg,Ez80_SIZE_BYTE);
      ea=readReg(Ez80_REG_HL,Ez80_SIZE_WORD);
      map->write8(ea,val);
      break;
    case 0xdd:
      // ld r,(ix+const8)
      // ld (ix+const8),const8
      // ld (ix+const8),r
      // ld (const16),ix
      // ld ix,const16
      // ld ix,(const16)
      // ld sp,ix
      // ldd
      debugger("Unimplemented ld opcode!");
      break;
    case 0xfd:
      // ld r,(iy+const8)
      // ld (iy+const8),const8
      // ld (iy+const8),r
      // ld (const16),iy
      // ld iy,const16
      // ld iy,(const16)
      // ld sp,iy
      debugger("Unimplemented ld opcode!");
      break;
    case 0xed:
      // ld (const16),rr
      // ld a,I
      // ld I,a
      // ld a,R
      // ld R,a
      // lddr
      // ldi
      // ldir
      debugger("Unimplemented ld opcode!");
      break;
    case 0x3a:
      // ld a,(const16)
      ea=map->read16l(loc+1);
      val=map->read8(ea);
      a=val;
      break;
    case 0x0a:
      // ld a,(bc)
      ea=readReg(Ez80_REG_BC,Ez80_SIZE_WORD);
      val=map->read8(ea);
      a=val;
      break;
    case 0x32:
      // ld (const16),a
      ea=map->read16l(loc+1);
      map->write8(ea,a);
      break;
    case 0x22:
      // ld (const16),hl
      ea=map->read16l(loc+1);
      val=readReg(Ez80_REG_HL,Ez80_SIZE_WORD);
      map->write8(ea,val);
      break;
    case 0x1a:
      // ld a,(de)
      ea=readReg(Ez80_REG_DE,Ez80_SIZE_WORD);
      val=map->read8(ea);
      a=val;
      break;
    case 0x2a:
      // ld hl,(const16)
      ea=map->read16l(loc+1);
      val=map->read16l(ea);
      writeReg(Ez80_REG_HL,val,Ez80_SIZE_WORD);
      break;
    case 0xf9:
      // ld sp,hl
      val=readReg(Ez80_REG_HL,Ez80_SIZE_WORD);
      sp=val;
      break;
    default:
      debugger("Unimplemented ld opcode!");
      break;
  }
#ifdef DEBUG_EXEC
  DEBUG_OUT<<"ld op="<<op<<" reg="<<reg<<" ea="<<ea<<" val="<<val<<"\n";
#endif
}


void Ez80::exec_nop(ULONG loc,unsigned int op)
{
}


void Ez80::exec_or(ULONG loc,unsigned int op)
{
  switch(op) {
    case 0xb5: // or l
      a=a|l;
      set_flags_szp(a);
      break;
    default:
      debugger("Unimplemented or opcode!");
      break;
  }
}


void Ez80::exec_out(ULONG loc,unsigned int op)
{
  switch(op) {
    case 0xd3: // out (#),a
      ea=map->read8(loc+1);
      doOutByte(ea,a);
      break;
    default:
      debugger("Unimplemented out opcode!");
      break;
  }
}


void Ez80::exec_pop(ULONG loc,unsigned int op)
{
  debugger("Unimplemented pop opcode!");
}


void Ez80::exec_push(ULONG loc,unsigned int op)
{
  debugger("Unimplemented push opcode!");
}


void Ez80::exec_ret(ULONG loc,unsigned int op)
{
  ea=pullAddr();
  jump(ea);
}


void Ez80::exec_rla(ULONG loc,unsigned int op)
{
  debugger("Unimplemented rla opcode!");
}


void Ez80::exec_rlca(ULONG loc,unsigned int op)
{
  debugger("Unimplemented rlca opcode!");
}


void Ez80::exec_rra(ULONG loc,unsigned int op)
{
  debugger("Unimplemented rra opcode!");
}


void Ez80::exec_rrca(ULONG loc,unsigned int op)
{
  debugger("Unimplemented rrca opcode!");
}


void Ez80::exec_rst(ULONG loc,unsigned int op)
{
  val=(op>>3)&0x7,ea=0;
  switch(val) {
    case 0: ea=0x0000; break;
    case 1: ea=0x0008; break;
    case 2: ea=0x0010; break;
    case 3: ea=0x0018; break;
    case 4: ea=0x0020; break;
    case 5: ea=0x0028; break;
    case 6: ea=0x0030; break;
    case 7: ea=0x0038; break;
  }
  pushAddr(pc);
  jump(ea);
}


void Ez80::exec_sbc(ULONG loc,unsigned int op)
{
  switch(op) {
    case 0x99:
      // sbc a,r
      reg=map->read8(loc)&0x7;
      val=readReg(reg,Ez80_SIZE_BYTE);
      val=a-(val+sbits->c);
      a=val;
      set_flags_szhpn(a);
      break;
    default:
      debugger("Unimplemented sbc opcode!");
      break;
  }
}


void Ez80::exec_scf(ULONG loc,unsigned int op)
{
  debugger("Unimplemented scf opcode!");
}


void Ez80::exec_special(ULONG loc,unsigned int op)
{
  BYTE mode=Ez80_NONE;
  BYTE nextcode=map->read8(loc+1);
  switch(op) {
    case 0xed:
      switch(nextcode) {
        case 0x51: case 0x59: case 0x79:
          // out (c),r
          reg=(nextcode>>3)&0x7;
          val=readReg(reg,Ez80_SIZE_BYTE);
          ea=readReg(Ez80_REG_BC,Ez80_SIZE_WORD);
          doOutByte(ea,val);
          break;
        case 0x53:
          // ld ($xxxx),wreg
          reg=(map->read8(loc)>>4)&0x3;
          ea=map->read16l(loc+1);
          val=readReg(reg,Ez80_SIZE_WORD);
          map->write16l(ea,val);
          break;
        case 0x5b:
          // ld wreg,$xxxx
          reg=(map->read8(loc)>>4)&0x3;
          ea=map->read16l(loc+1);
          val=map->read16l(ea);
          writeReg(reg,val,Ez80_SIZE_WORD);
          break;
        case 0x78:
          // in r,(c)
          reg=(nextcode>>3)&0x7;
          ea=readReg(Ez80_REG_BC,Ez80_SIZE_WORD);
          val=doInByte(ea);
          writeReg(reg,val,Ez80_SIZE_BYTE);
          break;
        default:
          debugger("Unimplemented 0xed special opcode!");
          break;
      }
      break;
    case 0xcb:
      switch(nextcode) {
        case 0x57:
          // bit 2,a
          val=a&0x2;
          if(val) sbits->z=1; else sbits->z=0;
          break;
        default:
          debugger("Unimplemented 0xcb special opcode!");
          break;
      }
      break;
    default:
      debugger("Unimplemented special opcode!");
      break;
  }
#ifdef DEBUG_EXEC
  DEBUG_OUT<<"special op="<<op<<" nextcode="<<(short)nextcode<<" reg="<<reg<<" ea="<<ea<<" val="<<val<<"\n";
#endif
}


void Ez80::exec_sub(ULONG loc,unsigned int op)
{
  debugger("Unimplemented sub opcode!");
}


void Ez80::exec_xor(ULONG loc,unsigned int op)
{
  switch(op) {
    case 0xae:  // xor (hl)
      ea=readReg(Ez80_REG_HL,Ez80_SIZE_WORD);
      val=a^map->read8(ea);
      map->write8(ea,val);
      set_flags_szp(val);
      break;
    case 0xaf:  // xor a
      a=a^a;
      set_flags_szp(a);
      break;
    default:
      debugger("Unimplemented xor opcode!");
      break;
  }
}


