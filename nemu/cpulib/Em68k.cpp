
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


/*

In general there are too many function calls where things could be macros

Look at how flags are done and sign extension, etc.  But, hey, I did it that way
so I could understand what was going on.  In otherwords, first goal is to
correctly work, being fast comes later...

Also, supervisor mode isn't really implemented...

And '020+ addressing modes aren't there, particularly pc relative ones and
ones with scale, etc

*/


//#define DEBUG_EXEC 1

#define CHECK_SAFE 1

#define SHOW_INTS  1

//#define NONSTANDARD_MOVEM 1


#include <elib/EMemMapAbstract.h>
#include <elib/EModule.h>
#include <elib/EHandler.h>

#include <cpulib/Em68k.h>
#include <cpulib/generated/Em68k_tab.h>

#include <portable/nickcpp/ANullStream.h>

#include <portable/nickcpp/AStringHelper.h>


////////////////////////////////////////////////////////////////////////////////
//  E68k Class
////////////////////////////////////////////////////////////////////////////////

Em68k::Em68k()
{
  DEBUG_OUT<<"Em68k created...\n";
  init();
  //w=ww;
  doTrace=false;  doRun=false;
  hasMMU=false;  hasFPU=false;
  int_pending=0;
  type=E_CPU_68000;
  opcode=0;
  reset();
}


Em68k::~Em68k()
{
  DEBUG_OUT<<"Em68k destroyed.\n";
}


void Em68k::init()
{
  ECPUCisc::init();
  bitness=32;
  // Generic features
  doTrace=false;  doRun=false;
  // Set up stack and registers
  intLock=false;
  int_pending=0;
  lsb=false;
  breakpoint=0xffffffff;
  setStack(ECPUABSTRACT_DEFAULT_STACK);
  setFramePtr(ECPUABSTRACT_DEFAULT_STACK);
  setResetStack(ECPUABSTRACT_DEFAULT_STACK);
  setResetEntry(2);
}


// TODO: Add all other registers
bool Em68k::setRegister(const char *tname,ULONG value)
{
  const char *name=aDefaultStringHelper.removeSpaces2(aDefaultStringHelper.toLower(tname));
  bool ret=false;
  if(!strcmp(name,"pc"))
  { pc=value; ret=true; } else
  if(!strcmp(name,"ssp"))
  { ssp=value; ret=true; } else
  if(!strcmp(name,"usp"))
  { sp=value; ret=true; } else
  if(!strcmp(name,"sp"))
  { setStack(value); ret=true; } else
  if(!strcmp(name,"vbr"))
  { vbr=value; ret=true; } else
  if(!strcmp(name,"sr"))
  { sr=value; ret=true; } else
  if(!strcmp(name,"msp"))
  { msp=value; ret=true; } else
  if(!strcmp(name,"sfc"))
  { sfc=value; ret=true; } else
  if(!strcmp(name,"dfc"))
  { dfc=value; ret=true; } else
  if(!strcmp(name,"caar"))
  { caar=value; ret=true; } else
  if(!strcmp(name,"cacr"))
  { cacr=value; ret=true; } else
  if(!strcmp(name,"tc"))
  { tc=value; ret=true; } else
  if(!strcmp(name,"mmusr"))
  { mmusr=value; ret=true; } else
  if(!strcmp(name,"urp"))
  { urp_l=value; ret=true; } else
  if(!strcmp(name,"srp"))
  { srp_l=value; ret=true; } else
  if(!strcmp(name,"drp"))
  { drp_l=value; ret=true; } else
  if(name[0]=='d') {
    unsigned int n=name[1]-'0';
    if(n<8) { d[n]=value; ret=true; }
  } else
  if(name[0]=='a') {
    unsigned int n=name[1]-'0';
    if(n<8) { a[n]=value; ret=true; }
    if(n==7) setStack(value);
  }
  return ret;
}


ULONG Em68k::readRegister(const char *tname)
{
  const char *name=aDefaultStringHelper.removeSpaces2(aDefaultStringHelper.toLower(tname));
  unsigned int ret=0;
  if(name[0]=='d') {
    unsigned int n=name[1]-'0';
    if(n<8) { ret=d[n]; }
  }
  if(name[0]=='a') {
    unsigned int n=name[1]-'0';
    if(n<8) { ret=a[n]; }
  }
  return ret;
}


void Em68k::setReturnReg(ULONG val)
{
  d[0]=val;
}


ULONG Em68k::readReturnReg()
{
  return d[0];
}


ULONG Em68k::readParamReg(unsigned int num)
{
  num&=0x7;
  return d[num];
}


void Em68k::setParamReg(unsigned int num,ULONG val)
{
  num&=0x7;
  d[num]=val;
}


/* STATIC */
bool Em68k::recognize(const char *tname)
{
  char *name=aDefaultStringHelper.removeSpaces2(aDefaultStringHelper.toLower(tname));
  if(!strcmp(name,"010")) return true; else
  if(!strcmp(name,"020")) return true; else
  if(!strcmp(name,"ec030")) return true; else
  if(!strcmp(name,"030")) return true; else
  if(!strcmp(name,"040")) return true; else
  if(!strcmp(name,"lc040")) return true; else
  if(!strcmp(name,"'010")) return true; else
  if(!strcmp(name,"'020")) return true; else
  if(!strcmp(name,"'030")) return true; else
  if(!strcmp(name,"'040")) return true; else
  if(!strcmp(name,"mc68000")) return true; else
  if(!strcmp(name,"m68000")) return true; else
  if(!strcmp(name,"m68008")) return true; else
  if(!strcmp(name,"m68010")) return true; else
  if(!strcmp(name,"m68012")) return true; else
  if(!strcmp(name,"m68020")) return true; else
  if(!strcmp(name,"m68030")) return true; else
  if(!strcmp(name,"m68ec030")) return true; else
  if(!strcmp(name,"m68040")) return true; else
  if(!strcmp(name,"m68lc040")) return true; else
  if(!strcmp(name,"m68060")) return true; else
  if(!strcmp(name,"m68070")) return true; else
  if(!strcmp(name,"m68349")) return true; else
  if(!strcmp(name,"68000")) return true; else
  if(!strcmp(name,"68008")) return true; else
  if(!strcmp(name,"68010")) return true; else
  if(!strcmp(name,"68012")) return true; else
  if(!strcmp(name,"68020")) return true; else
  if(!strcmp(name,"68030")) return true; else
  if(!strcmp(name,"68ec030")) return true; else
  if(!strcmp(name,"68040")) return true; else
  if(!strcmp(name,"68lc040")) return true; else
  if(!strcmp(name,"68060")) return true; else
  if(!strcmp(name,"68070")) return true; else
  if(!strcmp(name,"68349")) return true; else
  if(!strcmp(name,"68328")) return true; else
  if(!strcmp(name,"m68349")) return true; else
  if(!strcmp(name,"m68328")) return true; else
  if(!strcmp(name,"mc68349")) return true; else
  if(!strcmp(name,"mc68328")) return true; else
  if(!strcmp(name,"m68k")) return true; else
  if(!strcmp(name,"68k")) return true; else
  if(!strcmp(name,"coldfire")) return true; else
  if(!strcmp(name,"mcf5307")) return true; else
  if(!strcmp(name,"5307")) return true; else
  if(!strcmp(name,"mcf5206")) return true; else
  if(!strcmp(name,"5206")) return true; else
  if(!strcmp(name,"mcf5202")) return true; else
  if(!strcmp(name,"5202")) return true; else
  if(!strcmp(name,"dragonball")) return true; else
  return false;
}


const char *Em68k::getName()
{
  switch(type) {
    case E_CPU_68000: return "Motorola 68000"; break;
    case E_CPU_68010: return "Motorola 68010"; break;
    case E_CPU_68020: return "Motorola 68020"; break;
    case E_CPU_68030: return "Motorola 68030"; break;
    case E_CPU_68040: return "Motorola 68040"; break;
    case E_CPU_68060: return "Motorola 68060"; break;
    case E_CPU_ColdFire: return "Motorola ColdFire"; break;
    default: return "Motorola 68000 class"; break;
  }
}


void Em68k::setKind(const char *tname)
{
  const char *name=aDefaultStringHelper.removeSpaces2(aDefaultStringHelper.toLower(tname));
  if(!strcmp(name,"010")) type=E_CPU_68010;
  if(!strcmp(name,"020")) type=E_CPU_68020;
  if(!strcmp(name,"ec030")) type=E_CPU_68030;
  if(!strcmp(name,"030")) type=E_CPU_68030;
  if(!strcmp(name,"lc040")) type=E_CPU_68040;
  if(!strcmp(name,"040")) type=E_CPU_68040;
  if(!strcmp(name,"060")) type=E_CPU_68060;
  if(!strcmp(name,"'010")) type=E_CPU_68010;
  if(!strcmp(name,"'020")) type=E_CPU_68020;
  if(!strcmp(name,"'030")) type=E_CPU_68030;
  if(!strcmp(name,"'040")) type=E_CPU_68040;
  if(!strcmp(name,"'060")) type=E_CPU_68060;
  if(!strcmp(name,"68000")) type=E_CPU_68000;
  if(!strcmp(name,"68008")) type=E_CPU_68000;
  if(!strcmp(name,"68010")) type=E_CPU_68010;
  if(!strcmp(name,"68012")) type=E_CPU_68010;
  if(!strcmp(name,"68020")) type=E_CPU_68020;
  if(!strcmp(name,"68030")) type=E_CPU_68030;
  if(!strcmp(name,"68ec030")) type=E_CPU_68030;
  if(!strcmp(name,"68040")) type=E_CPU_68040;
  if(!strcmp(name,"68lc040")) type=E_CPU_68040;
  if(!strcmp(name,"68060")) type=E_CPU_68000;
  if(!strcmp(name,"68070")) type=E_CPU_68060;
  if(!strcmp(name,"68349")) type=E_CPU_68000;
  if(!strcmp(name,"68328")) type=E_CPU_68000;
  if(!strcmp(name,"mc68000")) type=E_CPU_68000;
  if(!strcmp(name,"m68000")) type=E_CPU_68000;
  if(!strcmp(name,"m68008")) type=E_CPU_68000;
  if(!strcmp(name,"m68010")) type=E_CPU_68010;
  if(!strcmp(name,"m68012")) type=E_CPU_68010;
  if(!strcmp(name,"m68020")) type=E_CPU_68020;
  if(!strcmp(name,"m68030")) type=E_CPU_68030;
  if(!strcmp(name,"m68ec030")) type=E_CPU_68030;
  if(!strcmp(name,"m68040")) type=E_CPU_68040;
  if(!strcmp(name,"m68lc040")) type=E_CPU_68040;
  if(!strcmp(name,"m68060")) type=E_CPU_68060;
  if(!strcmp(name,"m68070")) type=E_CPU_68000;
  if(!strcmp(name,"m68349")) type=E_CPU_68000;
  if(!strcmp(name,"m68328")) type=E_CPU_68000;
  if(!strcmp(name,"mc68349")) type=E_CPU_68000;
  if(!strcmp(name,"mc68328")) type=E_CPU_68000;
  if(!strcmp(name,"68k")) type=E_CPU_68000;
  if(!strcmp(name,"m68k")) type=E_CPU_68000;
  if(!strcmp(name,"coldfire")) type=E_CPU_ColdFire;
  if(!strcmp(name,"mcf5307")) type=E_CPU_ColdFire;
  if(!strcmp(name,"5307")) type=E_CPU_ColdFire;
  if(!strcmp(name,"mcf5206")) type=E_CPU_ColdFire;
  if(!strcmp(name,"5206")) type=E_CPU_ColdFire;
  if(!strcmp(name,"mcf5202")) type=E_CPU_ColdFire;
  if(!strcmp(name,"5202")) type=E_CPU_ColdFire;
  if(!strcmp(name,"dragonball")) type=E_CPU_68000;
  switch(type) {
    case E_CPU_68030:
      hasMMU=true;  hasFPU=false;
      break;
    case E_CPU_68040:
      hasMMU=true;  hasFPU=true;
      break;
    default:
      hasMMU=false;  hasFPU=false;
      break;
  }
}


bool Em68k::interp(const char *cmd,const char *a1,const char *a2,const char *a3)
{
  bool ret=false;
  if(!strcmp(cmd,"rm")) { wmregs(); ret=true; }
  if(!strcmp(cmd,"rf")) { wfregs(); ret=true; }
  return ret;
}


void Em68k::handle_int(ULONG value)
{
#ifdef SHOW_INTS
  DEBUG_OUT<<"+";
#endif
  int_pending=0;
  if(!value) { reset(); return; }
  unsigned int table=0;
  if(type>E_CPU_68000) table=vbr;
  ULONG addr=value*4+table;
  // TODO: this needs more work
  goSupervisor();
  // Build exception stack frame...
  push(pc,15,ECPUABSTRACT_SIZE_LONG);
  push(sr,15,ECPUABSTRACT_SIZE_WORD);
  // TODO: handle different stack formats for different cpus
  switch(type) {
    default:
      switch(value) {
        case E68k_INT_BUSERR: case E68k_INT_ADDRESS:
          push(opcode,15,ECPUABSTRACT_SIZE_WORD);
          push(0,15,ECPUABSTRACT_SIZE_LONG);  // TODO: Should be access address!
          push(sfc,15,ECPUABSTRACT_SIZE_WORD);  // TODO: Not right at all...
        default:
          break;
      }
      break;
  }
  unsigned int dest=map->read32b(addr);
  if(!dest) {
    // Found a zero in the vector table, probably not what we wanted so break.
    DEBUG_OUT<<"handle_int("<<value<<"), vector is at "<<addr<<"\n";
    debugger("Zero in vector table!\n");
  }
  else {
    dest-=2;  // TODO: Why is it two off?!?
    jump(dest);
  }
}


// NOTE: value is an int value, not the address in the vector table
// TODO: Check to see if int should be put off until later...
void Em68k::cause_int(ULONG value)
{
  if(value==E68k_INT_RESET) reset();
  else {
    handle_int(value);
  }
}


void Em68k::ints()
{
  //if(w) {
    unsigned int table=0;
    if(type>E_CPU_68000) table=vbr;
    DEBUG_OUT<<"Exception Vector Table is at "<<table<<"\n";
  //}
}


void Em68k::reset()
{
  int t=0;
  lsb=false;
  pc=0;  ssp=0;  sp=0;  msp=0;
  for(t=0;t<8;t++) d[t]=0;
  for(t=0;t<8;t++) a[t]=0;
  for(t=0;t<8;t++) fp[t]=0.0;
  sr=0;
  vbr=0;  sfc=0;  dfc=0;  caar=0;  cacr=0;
  tt0=0;  tt1=0;  dtt0=0;  dtt1=0;
  tc=0;  mmusr=0;  urp_l=0;  urp_h=0;  srp_l=0;  srp_h=0;
  drp_l=0;  drp_h=0;  val=0;  cal=0;  scc=0;  ac=0;
  fpcr=0;  fpsr=0;
  goSupervisor();
  // Because vbr is 0 at this point
  if(map) setStack(map->read32b(0));
  if(map) resetEntry=map->read32b(4);
  if(map) jump(map->read32b(4));
  clearStackTrace();
  ECPUAbstract::reset();
}


void Em68k::wregs()
{
  //unsigned int dig=w->getHexDigits();
  //w->setHexDigits(8);
  DEBUG_OUT<<"PC="<<(ULONG)pc<<" ";
  if(isSSet()) DEBUG_OUT<<"(Supervisor)"; else DEBUG_OUT<<"(User)";
  DEBUG_OUT<<"\n";
  unsigned int t;
  // NOTE: That stuff with '0'+t is to get them to display with only one digit
  for(t=0;t<8;t++) {
    DEBUG_OUT<<"D"<<(char)('0'+t)<<"="<<(ULONG)d[t]<<" ";
    if(t==3) DEBUG_OUT<<"\n";
    //if(t==5) DEBUG_OUT<<"\n";
  }
  DEBUG_OUT<<"\n";
  for(t=0;t<8;t++) {
    DEBUG_OUT<<"A"<<(char)('0'+t)<<"="<<(ULONG)a[t]<<" ";
    if(t==3) DEBUG_OUT<<"\n";
    //if(t==5) DEBUG_OUT<<"\n";
  }
  DEBUG_OUT<<"\n";
  print_sr();
  DEBUG_OUT<<"USP="<<(ULONG)sp<<" "<<"SSP="<<(ULONG)ssp;  // NOTE: no \n here...
  switch(type) {
    case E_CPU_68020:  case E_CPU_68030:  case E_CPU_68040:
      DEBUG_OUT<<" MSP="<<(ULONG)msp<<"\n";
      break;
    default: DEBUG_OUT<<"\n"; break;
  }
  switch(type) {
    case E_CPU_68010:  case E_CPU_68020:
    case E_CPU_68030:  case E_CPU_68040:
      DEBUG_OUT<<"VBR="<<(ULONG)vbr<<"\n";
      break;
    default: break;
  }
  //w->setHexDigits(dig);
}


void Em68k::wmregs()
{
  //unsigned int dig=w->getHexDigits();
  //w->setHexDigits(8);
  switch(type) {
    case E_CPU_68020:  case E_CPU_68030:  case E_CPU_68040:
      DEBUG_OUT<<"CAAR="<<caar<<" CACR="<<cacr<<"\n";
      break;
    default: DEBUG_OUT<<"\n"; break;
  }
  switch(type) {
    case E_CPU_68010:  case E_CPU_68020:
    case E_CPU_68030:  case E_CPU_68040:
      DEBUG_OUT<<"SFC="<<sfc<<" DFC="<<dfc<<"\n";
      break;
    default: break;
  }
  if(hasMMU) {
    DEBUG_OUT<<"TC="<<tc<<" MMUSR="<<mmusr<<"\n";
    DEBUG_OUT<<"URP="<<urp_l<<":"<<urp_h<<"\n";
    DEBUG_OUT<<"SRP="<<srp_l<<":"<<srp_h<<"\n";
    if(type==E_CPU_68020) {
      // TODO: There are a few more that belong here...
      DEBUG_OUT<<"DRP="<<drp_l<<":"<<drp_h<<"\n";
    }
    if(type==E_CPU_68030) {
      DEBUG_OUT<<"TT0="<<tt0<<" TT1="<<tt1<<"\n";
    }
  }
  if(type==E_CPU_68040) {
    DEBUG_OUT<<"ITT0="<<tt0<<" ITT1="<<tt1<<"\n";
    DEBUG_OUT<<"DTT0="<<dtt0<<" DTT1="<<dtt1<<"\n";
  }
  // TODO: '060 and coldfire...
  //w->setHexDigits(dig);
}


void Em68k::wfregs()
{
  if(!hasFPU) return;
  //unsigned int dig=w->getHexDigits();
  //w->setHexDigits(8);
  DEBUG_OUT<<"FPIAR="<<pc<<"\n";
  DEBUG_OUT<<"FPCR="<<fpcr<<"  ";
  DEBUG_OUT<<"FPSR="<<fpsr<<"\n";
  unsigned int t;
  // NOTE: That stuff with '0'+t is to get them to display with only one digit
  for(t=0;t<8;t++) {
    DEBUG_OUT<<"FP"<<(char)('0'+t)<<"="<<fp[t]<<" ";
    if(t==2) DEBUG_OUT<<"\n";
    if(t==5) DEBUG_OUT<<"\n";
  }
  DEBUG_OUT<<"\n";
  //w->setHexDigits(dig);
}


void Em68k::print_sr()
{
  DEBUG_OUT<<"SR="<<(USHORT)sr<<"  (";
  if(sr&E68k_STAT_T1) DEBUG_OUT<<"T";  else DEBUG_OUT<<"t";
  if(sr&E68k_STAT_T0) DEBUG_OUT<<"T";  else DEBUG_OUT<<"t";
  if(sr&E68k_STAT_S) DEBUG_OUT<<"S";  else DEBUG_OUT<<"s";
  if(sr&E68k_STAT_M) DEBUG_OUT<<"M";  else DEBUG_OUT<<"m";
  DEBUG_OUT<<"-";
  if(sr&E68k_STAT_I2) DEBUG_OUT<<"I";  else DEBUG_OUT<<"i";
  if(sr&E68k_STAT_I1) DEBUG_OUT<<"I";  else DEBUG_OUT<<"i";
  if(sr&E68k_STAT_I0) DEBUG_OUT<<"I";  else DEBUG_OUT<<"i";
  DEBUG_OUT<<"---";
  if(sr&E68k_STAT_X) DEBUG_OUT<<"X";  else DEBUG_OUT<<"x";
  if(sr&E68k_STAT_N) DEBUG_OUT<<"N";  else DEBUG_OUT<<"n";
  if(sr&E68k_STAT_Z) DEBUG_OUT<<"Z";  else DEBUG_OUT<<"z";
  if(sr&E68k_STAT_V) DEBUG_OUT<<"V";  else DEBUG_OUT<<"v";
  if(sr&E68k_STAT_C) DEBUG_OUT<<"C";  else DEBUG_OUT<<"c";
  DEBUG_OUT<<")\n";
}


void Em68k::setStack(ULONG val)
{
  write_reg(15,val,ECPUABSTRACT_SIZE_LONG);
  if(sr&E68k_STAT_S) ssp=val; else sp=val;
  // TODO: handle M bit here
}


ULONG Em68k::readStack()
{
  return read_reg(15,ECPUABSTRACT_SIZE_LONG);
}


void Em68k::setFramePtr(ULONG val)
{
  write_reg(14,val,ECPUABSTRACT_SIZE_LONG);  // A6
}


ULONG Em68k::readFramePtr()
{
  return read_reg(14,ECPUABSTRACT_SIZE_LONG);  // A6
}


// TODO: This is wrong...needs work...
void Em68k::goSupervisor()
{
  if(!isSSet()) {
    // We were in user mode
    sp=a[7];
  }
  if(type>E_CPU_68010) {
    // 68020 or better
    // TODO: handle M bit here
    a[7]=ssp;
  }
  else {
    // 68000 or 68010
    a[7]=ssp;
  }
  sr|=E68k_STAT_S;
}


// TODO: This is wrong...needs work...
void Em68k::goUser()
{
  if(isSSet()) {
    // We were in supervisor mode
    if(type>E_CPU_68010) {
      // 68020 or better
      // TODO: handle M bit here
      ssp=a[7];
    }
    else {
      // 68000 or 68010
      ssp=a[7];
    }
  }
  else sr-=E68k_STAT_S;
  a[7]=sp;
}


void Em68k::setSR(unsigned int newVal)
{
  if(newVal&E68k_STAT_S) {
    // Switching to supervisor mode
     goSupervisor();
  }
  else {
    // Switching to user mode
    goUser();
  }
}


unsigned int Em68k::getMaskSize68k(unsigned int size)
{
  unsigned int ret=0;
  switch(size) {
    case ECPUABSTRACT_SIZE_BYTE:  ret=0xff;  break;
    case ECPUABSTRACT_SIZE_WORD:  ret=0xffff; break;
    case ECPUABSTRACT_SIZE_LONG:  ret=0xffffffff; break;
    default:
      exec_unimplemented("bad size in getMaskSize!\n");
      break;
  }
  return ret;
}


unsigned int Em68k::getInverseMaskSize68k(unsigned int size)
{
  return 0xffffffff-getMaskSize68k(size);
}


unsigned int Em68k::getTopBitSize68k(unsigned int size)
{
  unsigned int ret=0;
  switch(size) {
    case ECPUABSTRACT_SIZE_BYTE:  ret=0x80;  break;
    case ECPUABSTRACT_SIZE_WORD:  ret=0x8000; break;
    case ECPUABSTRACT_SIZE_LONG:  ret=0x80000000; break;
    default:
      exec_unimplemented("bad size in getTopBitSize!\n");
      break;
  }
  return ret;
}


unsigned int Em68k::getNotTopMaskSize68k(unsigned int size)
{
  return getMaskSize68k(size)-getTopBitSize68k(size);
}


// TODO: set V bit...
unsigned int Em68k::do_lsr_once(unsigned int val,unsigned int size)
{
  unsigned int ret=val>>1;
  ret&=getNotTopMaskSize68k(size);
  setCFlag(val&0x1);  setXFlag(val&0x1);
  return ret;
}


// TODO: set V bit...
// NOTE: This is exactly the same as do_asl_once
unsigned int Em68k::do_lsl_once(unsigned int val,unsigned int size)
{
  unsigned int ret=val<<1;
  setCFlag(val&getTopBitSize68k(size));
  setXFlag(val&getTopBitSize68k(size));
  return ret;
}


// TODO: set V bit...
// TODO: Check the msb stuff...does it work?
unsigned int Em68k::do_asr_once(unsigned int val,unsigned int size)
{
  unsigned int ret=val>>1;
  ret=(ret&getNotTopMaskSize68k(size))|(val&getTopBitSize68k(size));
  setCFlag(val&0x1);
  setXFlag(val&0x1);
  return ret;
}


// TODO: set V bit...
// NOTE: This is exactly the same as do_lsl_once
unsigned int Em68k::do_asl_once(unsigned int val,unsigned int size)
{
  unsigned int ret=val<<1;
  setCFlag(val&getTopBitSize68k(size));
  setXFlag(val&getTopBitSize68k(size));
  return ret;
}


unsigned int Em68k::do_ror_once(unsigned int val,unsigned int size)
{
  unsigned int ret=val>>1;
  setCFlag(val&0x1);
  ret&=getNotTopMaskSize68k(size);
  // NOTE: C has what used to be the low bit
  if(sr&E68k_STAT_C) ret|=getTopBitSize68k(size);
  setVFlag(0);
  return ret;
}


unsigned int Em68k::do_rol_once(unsigned int val,unsigned int size)
{
  unsigned int ret=val<<1;
  setCFlag(val&getTopBitSize68k(size));
  // NOTE: C has what used to be the high bit
  if(sr&E68k_STAT_C) ret|=1;
  setVFlag(0);
  return ret;
}


unsigned int Em68k::do_roxr_once(unsigned int val,unsigned int size)
{
  unsigned int ret=val>>1;
  setCFlag(val&0x1);
  ret&=getNotTopMaskSize68k(size);
  // NOTE: X is set AFTER it is read (unlike C in ROR)
  if(sr&E68k_STAT_X) ret|=getTopBitSize68k(size);
  setXFlag(val&0x1);
  setVFlag(0);
  return ret;
}


unsigned int Em68k::do_roxl_once(unsigned int val,unsigned int size)
{
  unsigned int ret=val<<1;
  setCFlag(val&getTopBitSize68k(size));
  // NOTE: X is set AFTER it is read (unlike C in ROL)
  if(sr&E68k_STAT_X) ret|=1;
  setXFlag(val&getTopBitSize68k(size));
  setVFlag(0);
  return ret;
}


void Em68k::push(unsigned int val,unsigned int reg,unsigned int size)
{
#ifdef DEBUG_EXEC
  DEBUG_OUT<<"Push "<<val<<" on "<<reg<<" size "<<size<<"\n";
#endif
  // Keep it aligned
  if((reg==15)&&(size==ECPUABSTRACT_SIZE_BYTE)) { size=ECPUABSTRACT_SIZE_WORD; }
  ULONG addr=read_reg(reg,ECPUABSTRACT_SIZE_LONG);
  addr-=calc_size_bytes(size,reg);
  switch(size) {
    case ECPUABSTRACT_SIZE_BYTE: map->write8(addr,val&0xff); break;
    case ECPUABSTRACT_SIZE_WORD: map->write16b(addr,val&0xffff); break;
    case ECPUABSTRACT_SIZE_LONG: map->write32b(addr,val); break;
    default: debugger("Bad size in push!\n"); break;
  }
  write_reg(reg,addr,ECPUABSTRACT_SIZE_LONG);
}


// TODO: Size may not be right for reg...
unsigned int Em68k::pull(unsigned int reg,unsigned int size)
{
  unsigned int val=0;
  // Keep it aligned
  if((reg==15)&&(size==ECPUABSTRACT_SIZE_BYTE)) { size=ECPUABSTRACT_SIZE_WORD; }
  ULONG addr=read_reg(reg,ECPUABSTRACT_SIZE_LONG);
  switch(size) {
    case ECPUABSTRACT_SIZE_BYTE: val=map->read8(addr)&0xff; break;
    case ECPUABSTRACT_SIZE_WORD: val=map->read16b(addr)&0xffff; break;
    case ECPUABSTRACT_SIZE_LONG: val=map->read32b(addr); break;
    default: debugger("Bad size in push!\n"); break;
  }
  addr+=calc_size_bytes(size,reg);
  write_reg(reg,addr,ECPUABSTRACT_SIZE_LONG);
#ifdef DEBUG_EXEC
  DEBUG_OUT<<"Pull "<<val<<" from "<<reg<<" size "<<size<<"\n";
#endif
  return val;
}


void Em68k::print_reg(unsigned int reg)
{
  if(reg>15) {
    debugger("bad register number to print!\n");
  }
  if(reg<8) DEBUG_OUT<<"d"; else { DEBUG_OUT<<"a"; reg-=8; }
  DEBUG_OUT<<(char)('0'+reg);
}


void Em68k::print_size(unsigned int size)
{
  switch(size) {
    case ECPUABSTRACT_SIZE_BYTE: DEBUG_OUT<<".b"; break;
    case ECPUABSTRACT_SIZE_WORD: DEBUG_OUT<<".w"; break;
    case ECPUABSTRACT_SIZE_LONG: DEBUG_OUT<<".l"; break;
    default: DEBUG_OUT<<".?"; break;
  }
  DEBUG_OUT<<" ";
}


// NOTE: This takes care of op_len for imm (and only imm)
void Em68k::print_ea_op_len(ULONG loc, unsigned int mode, unsigned int reg, unsigned int size)
{
  unsigned int reg2=0,size2=0,ext_word=0;
  switch(mode) {
    case 0:  // data reg direct
      print_reg(reg);
      break;
    case 1:  // address reg direct
      print_reg(reg+8);
      break;
    case 2:  // address reg indirect
      DEBUG_OUT<<"("; print_reg(reg+8); DEBUG_OUT<<")";
      break;
    case 3:  // address reg indirect, post inc
      DEBUG_OUT<<"("; print_reg(reg+8); DEBUG_OUT<<")+";
      break;
    case 4:  // address reg indirect, pre dec
      DEBUG_OUT<<"-("; print_reg(reg+8); DEBUG_OUT<<")";
      break;
    case 5:  // address reg indirect, disp
      DEBUG_OUT<<"$"<<map->read16b(loc+2)<<"("; print_reg(reg+8); DEBUG_OUT<<")";  op_len+=2;
      break;
    case 6:  // address reg with extension words
      // TODO: !!!!NOT DONE!!!!
      op_len+=2;
      ext_word=map->read16b(loc+2);
      DEBUG_OUT<<"$"<<(ext_word&0xff)<<"("; print_reg(reg+8); DEBUG_OUT<<",";
      reg2=(ext_word>>12)&0xf;  // Already has the right #, so we don't add 8
      size2=(ext_word>>11)&0x1;
      if(reg2>7) { DEBUG_OUT<<"a"; reg2-=8; } else DEBUG_OUT<<"d";
      DEBUG_OUT<<(char)('0'+reg2);
      if(size2)  DEBUG_OUT<<".l"; else DEBUG_OUT<<".w";
      DEBUG_OUT<<")";
      break;
    case 7:  //
      switch(reg) {
        case 0:  // abs short
          DEBUG_OUT<<"$"<<map->read16b(loc+2);  op_len+=2;
          break;
        case 1:  // abs long
          DEBUG_OUT<<"$"<<map->read32b(loc+2);  op_len+=4;
          break;
        case 2:  // pc indirect, disp
          DEBUG_OUT<<"$"<<map->read16b(loc+2)<<"(pc)";  op_len+=2;
          break;
        case 3:  // pc relative with extension words
          // TODO: !!!!NOT DONE!!!!
          op_len+=2;
          ext_word=map->read16b(loc+2);
          DEBUG_OUT<<"$"<<(ext_word&0xff)<<"(pc,";
          reg2=(ext_word>>12)&0x0f;
          if(reg2>7) { DEBUG_OUT<<"a"; reg2-=8; } else DEBUG_OUT<<"d";
          DEBUG_OUT<<(char)('0'+reg2)<<")";
          break;
        case 4:  // immediate
          DEBUG_OUT<<"#$"<<getImm_op_len(loc,size);
        default:
          break;
      }
      break;
    default:
      break;
  }
}


// NOTE: just because it decodes doesn't mean its legal for that operation!
unsigned int Em68k::decode_mode_dir(unsigned int reg,unsigned int mode)
{
  unsigned int dir=0;
  switch(mode) {
    case 0: dir=1; break;
    case 1: dir=1; break;
    case 2: dir=1; break;
    /* mode 3: address register */
    case 3: dir=1; break;  // TODO: may be wrong
    case 4: dir=0; break;
    case 5: dir=0; break;
    case 6: dir=0; break;
    /* mode 7: address register */
    case 7: dir=1; break;  // TODO: may be wrong
  }
  return dir;
}


// NOTE: just because it decodes doesn't mean its legal for that operation!
unsigned int Em68k::decode_mode_size(unsigned int reg,unsigned int mode)
{
  unsigned int size=ECPUABSTRACT_SIZE_OTHER;
  switch(mode) {
    case 0: size=ECPUABSTRACT_SIZE_BYTE; break;
    case 1: size=ECPUABSTRACT_SIZE_WORD; break;
    case 2: size=ECPUABSTRACT_SIZE_LONG; break;
    /* mode 3: address register */
    case 3: size=ECPUABSTRACT_SIZE_WORD; break;
    case 4: size=ECPUABSTRACT_SIZE_BYTE; break;
    case 5: size=ECPUABSTRACT_SIZE_WORD; break;
    case 6: size=ECPUABSTRACT_SIZE_LONG; break;
    /* mode 7: address register */
    case 7: size=ECPUABSTRACT_SIZE_LONG; break;
  }
  return size;
}


// NOTE: just because it decodes doesn't mean its legal for that operation!
unsigned int Em68k::decode_mode_reg(unsigned int reg,unsigned int mode)
{
  unsigned int rreg=reg;
  switch(mode) {
    case 0: case 1: case 2: break;  // Data register
    case 3: rreg+=8; break;  // Address register
    case 4: case 5: case 6: break;  // Data register
    case 7: rreg+=8; break;  // Address register
  }
  return rreg;
}


// NOTE: modifies op_len
unsigned int Em68k::getImm_op_len(ULONG loc, unsigned int size)
{
  unsigned int ret=0;
  switch(size) {
    case ECPUABSTRACT_SIZE_BYTE: op_len+=2; ret=(map->read16b(loc+2))&0xff; break;
    case ECPUABSTRACT_SIZE_WORD: op_len+=2; ret=map->read16b(loc+2); break;
    case ECPUABSTRACT_SIZE_LONG: op_len+=4; ret=map->read32b(loc+2); break;
    default:
      // *debugOut.warning("bad size in getImm_op_len");
      break;
  }
  return ret;
}


// NOTE: This only sign extends to 32 bits, no other size...
int Em68k::sign_extend68k(int val, unsigned int size)
{
  int ret=val;
  switch(size) {
    case ECPUABSTRACT_SIZE_BYTE:
      if(val&0x80) ret=val|0xffffff00;
      break;
    case ECPUABSTRACT_SIZE_WORD:
      if(val&0x8000) ret=val|0xffff0000;
      break;
    case ECPUABSTRACT_SIZE_LONG:
      break;
    default:
      exec_unimplemented("Bad size in sign_extend"); break;
  }
  return ret;
}


// NOTE: see note on setFlagsVC()
void Em68k::setFlagsVCX(ULONG val1, ULONG val2, ULONG val3, unsigned int size)
{
  setFlagsVC(val1,val2,val3,size);
  setXFlag(sr&E68k_STAT_C);
}


// TODO: Does this actually work?  Hard to tell...
// TODO: This may go backward...
// TODO: Doesn't handle movem and the oval thing...
void Em68k::do_movemrm(ULONG loc,unsigned int mode,unsigned int reg,unsigned int size, unsigned int mask)
{
  if(mode==3) { exec_unimplemented("Bad movem mode 3!"); return; }
  ULONG offset=0;
  unsigned int val=0;
  // NOTE: The 2 is there due to the mask
  unsigned int ea=calc_ea_op_len(loc+2,mode,reg,size);
  // NOTE: reg must be address reg
  unsigned int oval=read_reg(reg+8,ECPUABSTRACT_SIZE_LONG);
  for(unsigned int t=0;t<16;t++) {
    if(mask&(1<<t)) {
      val=read_reg(t,size);
      if(size==ECPUABSTRACT_SIZE_WORD)
        map->write16b(ea+offset,val);
      else
        map->write32b(ea+offset,val);
#ifdef DEBUG_EXEC
      DEBUG_OUT<<"movem t="<<t<<" offset="<<offset<<" val="<<val<<"\n";
#endif
      if(mode==4) {
        // pre-dec
        // NOTE: reg must be address reg
        write_reg(reg+8,oval+offset,ECPUABSTRACT_SIZE_LONG);
        if(size==ECPUABSTRACT_SIZE_WORD) offset-=2;
        else offset-=4;
      }
      else {
        // Anything else
        if(size==ECPUABSTRACT_SIZE_WORD) offset+=2;
        else offset+=4;
      }
    }
  }
}


// TODO: Does this actually work?  Hard to tell...
// TODO: This may go backward...
// TODO: Doesn't handle movem and the oval thing...
void Em68k::do_movemmr(ULONG loc,unsigned int mode,unsigned int reg,unsigned int size, unsigned int mask)
{
  if(mode==4) { exec_unimplemented("Bad movem mode 4!"); return; }
  ULONG offset=0;
  unsigned int val=0;
  // NOTE: The 2 is there due to the mask
  unsigned int ea=calc_ea_op_len(loc+2,mode,reg,size);
  // NOTE: reg must be address reg
  unsigned int oval=read_reg(reg+8,ECPUABSTRACT_SIZE_LONG);
  for(unsigned int t=0;t<16;t++) {
    if(mask&(1<<t)) {
      if(size==ECPUABSTRACT_SIZE_WORD) {
        val=map->read16b(ea+offset);
        val=sign_extend68k(val,ECPUABSTRACT_SIZE_WORD);
      }
      else val=map->read32b(ea+offset);
      write_reg(t,val,ECPUABSTRACT_SIZE_LONG);
#ifdef DEBUG_EXEC
      DEBUG_OUT<<"movem t="<<t<<" offset="<<offset<<" val="<<val<<"\n";
#endif
      if(size==ECPUABSTRACT_SIZE_WORD) offset+=2;
      else offset+=4;
      // Post-Inc
      // NOTE: reg must be address reg
      if(mode==3) write_reg(reg+8,oval+offset,ECPUABSTRACT_SIZE_LONG);
    }
  }
}


void Em68k::setCFlag(ULONG val)
{
  sr=sr&(E68k_STAT_ALL-E68k_STAT_C);
  if(val) sr|=E68k_STAT_C;
}


void Em68k::setVFlag(ULONG val)
{
  sr=sr&(E68k_STAT_ALL-E68k_STAT_V);
  if(val) sr|=E68k_STAT_V;
}


void Em68k::setZFlag(ULONG val)
{
  sr=sr&(E68k_STAT_ALL-E68k_STAT_Z);
  if(val) sr|=E68k_STAT_Z;
}


void Em68k::setNFlag(ULONG val)
{
  sr=sr&(E68k_STAT_ALL-E68k_STAT_N);
  if(val) sr|=E68k_STAT_N;
}


void Em68k::setXFlag(ULONG val)
{
  sr=sr&(E68k_STAT_ALL-E68k_STAT_X);
  if(val) sr|=E68k_STAT_X;
}


// TODO: This is totally messed up...
void Em68k::print_movem_maskx(unsigned int mask,unsigned int mode,unsigned int reg)
{
#ifdef NONSTANDARD_MOVEM
  DEBUG_OUT<<"m:#$"<<mask;
#else
  unsigned int last=0;
  unsigned int llast=0;
  unsigned int flast=16;
  bool slashNeeded=false;
  for(unsigned int t=0;t<16;t++) {
    if(mask&(1<<t)) {
      if(slashNeeded) { DEBUG_OUT<<"/"; slashNeeded=false; last=0; }
      if(!last) { print_reg(t); flast=16; }
      else flast=t;
      last++;
      if(t<8) llast++;
      if(last==2) DEBUG_OUT<<"-";
      if(t==15) { if(last>1) print_reg(t); flast=16; }
    }
    if(t==8) {
      if(flast<16) { print_reg(flast); flast=16; }
      if(llast) slashNeeded=true;
      last=0;
    }
    if(!(mask&(1<<t))) {
      if(flast<16) { print_reg(flast); flast=16; }
      last=0;
    }
  }
#endif
}


// TODO: This is a silly way to reverse the bits...
// TODO: This only reverses the low order 16 bits
unsigned int Em68k::reverse_mask(unsigned int mask)
{
  unsigned int nmask=0;
  if(mask&0x0001) nmask|=0x8000;
  if(mask&0x0002) nmask|=0x4000;
  if(mask&0x0004) nmask|=0x2000;
  if(mask&0x0008) nmask|=0x1000;
  if(mask&0x0010) nmask|=0x0800;
  if(mask&0x0020) nmask|=0x0400;
  if(mask&0x0040) nmask|=0x0200;
  if(mask&0x0080) nmask|=0x0100;
  if(mask&0x0100) nmask|=0x0080;
  if(mask&0x0200) nmask|=0x0040;
  if(mask&0x0400) nmask|=0x0020;
  if(mask&0x0800) nmask|=0x0010;
  if(mask&0x1000) nmask|=0x0008;
  if(mask&0x2000) nmask|=0x0004;
  if(mask&0x4000) nmask|=0x0002;
  if(mask&0x8000) nmask|=0x0001;
  return nmask;
}


/*
void Em68k::print_exec_unimplemented(char *str)
{
  DEBUG_OUT<<"DC.W "<<opcode<<" ; ???";
  if(str) DEBUG_OUT<<"\n; ("<<str<<")";
}
*/


unsigned int Em68k::calc_size_bytes(unsigned int size,unsigned int reg)
{
  unsigned int val=0;
  switch(size) {
    case ECPUABSTRACT_SIZE_BYTE:
      val=1;
      if(reg==15) val=2;  // If its reg A7, make sure its word aligned
      break;
    case ECPUABSTRACT_SIZE_WORD: val=2; break;
    case ECPUABSTRACT_SIZE_LONG: val=4; break;
    default:
      debugger("Illegal size in calc size bytes!\n");
      break;
  }
  return val;
}


// NOTE: This takes care of op_len...but not for immediate!
unsigned int Em68k::calc_ea_op_len(ULONG loc, unsigned int mode, unsigned int reg, unsigned int size)
{
  ULONG addr=0;
  unsigned int reg2=0,size2=0,ext_word=0;
  switch(mode) {
    case 0: case 1:  // register direct
      // *debugOut.warning("calc'd EA is reg!\n");
      break;
    case 2:  // address indirect
      addr=read_reg(reg+8,ECPUABSTRACT_SIZE_LONG);
      break;
    case 3:  // address indirect post inc
      addr=read_reg(reg+8,ECPUABSTRACT_SIZE_LONG);
      write_reg(reg+8,addr+calc_size_bytes(size,reg+8),ECPUABSTRACT_SIZE_LONG);
      break;
    case 4:  // address indirect pre dec
      addr=read_reg(reg+8,ECPUABSTRACT_SIZE_LONG)-calc_size_bytes(size,reg+8);
      write_reg(reg+8,addr,ECPUABSTRACT_SIZE_LONG);
      break;
    case 5:  // address indirect with disp
      addr=read_reg(reg+8,ECPUABSTRACT_SIZE_LONG)
        +sign_extend68k(map->read16b(loc+2),ECPUABSTRACT_SIZE_WORD);
      op_len+=2;
      break;
    case 6:  // address reg with extension words
      // TODO: This doesn't handle the full extension word
      addr=read_reg(reg+8,ECPUABSTRACT_SIZE_LONG);
      ext_word=map->read16b(loc+2);
      addr+=ext_word&0xff;  // 8 bit displacement
      reg2=(ext_word>>12)&0xf;  // Already has the right #, so we don't add 8
      size2=(ext_word>>11)&0x1;
      if(size2) addr+=read_reg(reg2,ECPUABSTRACT_SIZE_LONG);
      else addr+=sign_extend68k(read_reg(reg2,ECPUABSTRACT_SIZE_WORD),ECPUABSTRACT_SIZE_WORD);
      op_len+=2;
      break;
    case 7:
      switch(reg) {
        case 0:  // Short abs
          addr=sign_extend68k(map->read16b(loc+2),ECPUABSTRACT_SIZE_WORD);
          op_len+=2;
          break;
        case 1:  // Long abs
          addr=map->read32b(loc+2);
          op_len+=4;
          break;
        case 2:  // pc indirect w/ disp
          addr=loc+sign_extend68k(map->read16b(loc+2),ECPUABSTRACT_SIZE_WORD)+2;
          op_len+=2;
          break;
        case 3:  // pc with extension words
          // TODO: Not even started...
          debugger("calc'd pc ext not implemented!\n");
          break;
        case 4:  // Immediate
          // *debugOut.warning("calc'd EA is immediate!\n");
          break;
        case 5: case 6: case 7:  // Illegal
          // *debugOut.warning("calc'd EA is illegal!\n");
          break;
      }
      break;
    default:
      debugger("calc'd EA is unimplemented!\n");
      break;
  }
#ifdef DEBUG_EXEC
  DEBUG_OUT<<"Calc'd ea is "<<addr<<"\n";
#endif
  return addr;
}


void Em68k::write_reg(unsigned int reg, unsigned int val, unsigned int size)
{
  if(reg>15) {
    debugger("bad register number to write!\n");
    return ;
  }
  unsigned int tval=0;
  unsigned int omask=0,nmask=0;
  switch(size) {
    case ECPUABSTRACT_SIZE_BYTE: omask=0xffffff00;  nmask=0x000000ff; break;
    case ECPUABSTRACT_SIZE_WORD: omask=0xffff0000;  nmask=0x0000ffff; break;
    case ECPUABSTRACT_SIZE_LONG: omask=0x00000000;  nmask=0xffffffff; break;
    default:  debugger("Bad size in write_reg!\n");  break;
  }
  if(reg<8) {
    tval=d[reg];
    d[reg]=(tval&omask)|(val&nmask);
  }
  else {
    tval=a[reg-8];
    a[reg-8]=(tval&omask)|(val&nmask);
    // If it was A7, update stack regs
    // TODO: Don't use M bit if we don't have it!
    if(reg==15) {
      if(sr&E68k_STAT_S) {
        if(sr&E68k_STAT_M) msp=(tval&omask)|(val&nmask);
        else ssp=(tval&omask)|(val&nmask);
      }
      else sp=(tval&omask)|(val&nmask);
    }
  }
#ifdef DEBUG_EXEC
  DEBUG_OUT<<"Write "<<val<<" to reg "<<reg<<" size "<<size<<"\n";
#endif
}


// NOTE: This version doesn't change op_len
unsigned int Em68k::read_ea(ULONG loc, unsigned int mode, unsigned int reg, unsigned int size)
{
  unsigned int old_op_len=op_len;
  unsigned int ret=read_ea_op_len(loc,mode,reg,size);
  op_len=old_op_len;
  return ret;
}


// NOTE: This version doesn't change op_len
unsigned int Em68k::write_ea(ULONG loc, unsigned int mode, unsigned int reg, unsigned int val, unsigned int size)
{
  unsigned int old_op_len=op_len;
  unsigned int ret=write_ea_op_len(loc,mode,reg,val,size);
  op_len=old_op_len;
  return ret;
}


// NOTE: This uses value in op_len so make sure its valid...
// NOTE: This calls calc_ea_op_len which handles op_len for everything but imm
unsigned int Em68k::write_ea_op_len(ULONG loc, unsigned int mode, unsigned int reg, unsigned int val, unsigned int size)
{
#ifdef DEBUG_EXEC
  //DEBUG_OUT<<"write_ea mode:"<<mode<<" reg:"<<reg<<" size:"<<size<<"\n";
#endif
  switch(mode) {
    case 0: write_reg(reg,val,size);  return reg;  break;
    case 1: write_reg(reg+8,val,size);  return reg+8;  break;
    case 7:
      switch(reg) {
        case 4:  // immediate
          debugger("Illegal immediate write_ea!\n");
          return 0; break;
        case 5:  case 6:  case 7:
          debugger("Illegal ea mode write_ea!\n");
          return 0; break;
        default:  // Any memory addressing mode
          break;
      }
      break;
    default:  // Any memory addressing mode
      break;
  }
  // If we're still here, we're writing to memory...
  ULONG addr=calc_ea_op_len(loc,mode,reg,size);
  switch(size) {
    case ECPUABSTRACT_SIZE_BYTE: map->write8(addr,val&0xff); break;
    case ECPUABSTRACT_SIZE_WORD: map->write16b(addr,val&0xffff); break;
    case ECPUABSTRACT_SIZE_LONG: map->write32b(addr,val); break;
    default: debugger("Bad size in write_ea!\n"); break;
  }
#ifdef DEBUG_EXEC
  DEBUG_OUT<<"Write "<<val<<" to "<<addr<<" size "<<size<<"\n";
#endif
  return addr;
}


unsigned int Em68k::read_reg(unsigned int reg, unsigned int size)
{
  if(reg>15) {
    debugger("bad register number to read!\n");
    return 0;
  }
  unsigned int nmask=0;
  switch(size) {
    case ECPUABSTRACT_SIZE_BYTE: nmask=0x000000ff; break;
    case ECPUABSTRACT_SIZE_WORD: nmask=0x0000ffff; break;
    case ECPUABSTRACT_SIZE_LONG: nmask=0xffffffff; break;
    default:  debugger("Bad size in read_reg!\n");  break;
  }
  if(reg<8) { return d[reg]&nmask; }
  else { return a[reg-8]&nmask; }
#ifdef DEBUG_EXEC
  DEBUG_OUT<<"Read from reg "<<reg<<" size "<<size<<"\n";
#endif
}


// NOTE: This takes care of op_len for immediate (and only imm)
// NOTE: This calls calc_ea_op_len which handles op_len for everything but imm
unsigned int Em68k::read_ea_op_len(ULONG loc, unsigned int mode, unsigned int reg, unsigned int size)
{
#ifdef DEBUG_EXEC
  //DEBUG_OUT<<"read_ea mode:"<<mode<<" reg:"<<reg<<" size:"<<size<<"\n";
#endif
  switch(mode) {
    case 0: return read_reg(reg,size); break;
    case 1: return read_reg(reg+8,size); break;
    case 7:
      switch(reg) {
        case 4:  // immediate
          return getImm_op_len(loc,size);
          break;
        case 5:  case 6:  case 7:
          debugger("Illegal ea mode read_ea!\n");
          break;
        default:  // Any memory addressing mode
          break;
      }
      break;
    default:  // Any memory addressing mode
      break;
  }
  // If we're still here, we're reading from memory...
  ULONG addr=calc_ea_op_len(loc,mode,reg,size);
  unsigned int val=0;
  switch(size) {
    case ECPUABSTRACT_SIZE_BYTE: val=(map->read8(addr)&0xff); break;
    case ECPUABSTRACT_SIZE_WORD: val=(map->read16b(addr)&0xffff); break;
    case ECPUABSTRACT_SIZE_LONG: val=map->read32b(addr); break;
    default: debugger("Bad size in read_ea!\n"); break;
  }
#ifdef DEBUG_EXEC
  DEBUG_OUT<<"Read "<<val<<" from "<<addr<<" size "<<size<<"\n";
#endif
  return val;
}


void Em68k::execute()
{
  // NOTE: We assume the running flag is already set correctly...
  // NOTE: We screw with the trace flag so we don't disasm twice...
  bool oldTrace=trace;
  trace=false;
  decode(pc);
  trace=oldTrace;
  //if(int_pending) handle_int(int_pending);
}


ULONG Em68k::disasm(ULONG loc)
{
  // NOTE: We screw with the running flag so we don't run twice...
  bool oldRunning=running;
  bool oldTrace=trace;
  trace=true;
  running=false;
  decode(loc);
  trace=oldTrace;
  running=oldRunning;
  //DEBUG_OUT<<"(op_len="<<op_len<<")\n";
  return op_len;
}


void Em68k::decode(ULONG loc)
{
  opcode=map->read16b(loc);
  op_len=2;
  topcode=(opcode>>8)&0xff;
  unsigned int mode=0, reg=0, mode2=0;
  unsigned int reg2=0, size=0, dir=0;
  // Predecode as much as possible...
  switch(Em68k_format[topcode]) {
    case E68k_COUNT_SIZE_REG:
      reg=opcode&0x07;  size=(opcode>>6)&0x03;
      reg2=(opcode>>9)&0x07;  dir=(opcode>>8)&0x01;
      switch(size) {
        case 0: size=ECPUABSTRACT_SIZE_BYTE; break;
        case 1: size=ECPUABSTRACT_SIZE_WORD; break;
        case 2: size=ECPUABSTRACT_SIZE_LONG; break;
        default: size=ECPUABSTRACT_SIZE_OTHER; break;
      }
      break;
    case E68k_REG_DATA:
      reg=(opcode>>9)&0x07;  reg2=opcode&0xff;
      break;
    case E68k_EA:
      mode=(opcode>>3)&0x07;  reg=opcode&0x07;
      break;
    case E68k_SIZE_EA:
      mode=(opcode>>3)&0x07;  reg=opcode&0x07;
      size=(opcode>>6)&0x03;
      switch(size) {
        case 0: size=ECPUABSTRACT_SIZE_BYTE; break;
        case 1: size=ECPUABSTRACT_SIZE_WORD; break;
        case 2: size=ECPUABSTRACT_SIZE_LONG; break;
        default: size=ECPUABSTRACT_SIZE_OTHER; break;
      }
      break;
    case E68k_REG_MODE_EA:
      mode=(opcode>>3)&0x07;  reg=opcode&0x07;
      reg2=(opcode>>9)&0x07;  mode2=(opcode>>6)&0x07;
      break;
    case E68k_DATA_SIZE_EA:
      mode=(opcode>>3)&0x07;  reg=opcode&0x07;
      reg2=(opcode>>9)&0x07;  size=(opcode>>6)&0x03;
      switch(size) {
        case 0: size=ECPUABSTRACT_SIZE_BYTE; break;
        case 1: size=ECPUABSTRACT_SIZE_WORD; break;
        case 2: size=ECPUABSTRACT_SIZE_LONG; break;
        default: size=ECPUABSTRACT_SIZE_OTHER; break;
      }
      break;
    case E68k_EA_EA:
      mode=(opcode>>3)&0x07;  reg=opcode&0x07;
      reg2=(opcode>>9)&0x07;  mode2=(opcode>>6)&0x07;
      break;
    case E68k_REG_EA:  case E68k_REG_LEA:
      mode=(opcode>>3)&0x07;  reg=opcode&0x07;  reg2=(opcode>>9)&0x07;
      break;
    case E68k_COND_EA:
      mode=(opcode>>3)&0x07;  reg=opcode&0x07;  mode2=(opcode>>8)&0x0f;
    default:
      break;
  };
  if(trace) {
    lastSymbol=map->findAnyExactSymbol(loc);
    if(lastSymbol) DEBUG_OUT<<lastSymbol->getName()<<":\n";
    DEBUG_OUT<<"."<<loc<<" ";
    DEBUG_OUT<<(USHORT)opcode<<" ";
    // NOTE: we cache op_len if we're tracing, because we WILL change it.
    unsigned int oldOpLen=op_len;
    // This is the master dis switch statement
#include <cpulib/generated/Em68k_dis.h>
    if(running) op_len=oldOpLen;
    //DEBUG_OUT<<" ("<<op_len<<")";
    DEBUG_OUT<<"\n";
  }
  if(running) {
    if(pc==breakpoint) { debugger("Breakpoint!\n"); return; }
    // This is the master exec switch statement
#include <cpulib/generated/Em68k_exec.h>
    pc+=op_len;
  }
}


void Em68k::exec_a_line(ULONG loc, unsigned int opcode)
{
  bool done=false;
  if(!done) { if(io) done=io->systemCall(opcode); }
  if(!done) { if(trapHandler) done=trapHandler->systemCall(opcode); }
  if(!done) {
    exec_unimplemented("a-line instruction");
    //cause_int(E68k_INT_A_LINE);
  }
}


void Em68k::dis_a_line(ULONG loc, unsigned int opcode)
{
  if(io) {
    if(!io->ioDisHelper(loc)) {
      exec_unimplemented("A-Line");
    }
  }
  else {
    exec_unimplemented("A-Line");
  }
}


void Em68k::exec_f_line(ULONG loc, unsigned int opcode)
{
  unsigned int id=(opcode>>9)&0x07;
  unsigned int op=(opcode>>6)&0x07;
  unsigned int data=opcode&0x3f;
  switch(id) {
    case 0:  // MMU, 68851 or built in
      exec_pxxx(loc,op,data);
      break;
    case 1:  // FPU, 68881, 68882 or built in
      exec_fxxx(loc,op,data);
      break;
    case 2:  // '040 (or higher) extension
      exec_040ext(loc,opcode);
      break;
    default:  // Anything else is illegal
      DEBUG_OUT<<"f-line, cp id="<<id<<"\n";
      exec_unimplemented("f-line");
      break;
  }
}


void Em68k::dis_f_line(ULONG loc, unsigned int opcode)
{
  unsigned int id=(opcode>>9)&0x07;
  unsigned int op=(opcode>>6)&0x07;
  unsigned int data=opcode&0x3f;
  switch(id) {
    case 0:  // MMU, 68851 or built in
      dis_pxxx(loc,op,data);
      break;
    case 1:  // FPU, 68881, 68882 or built in
      dis_fxxx(loc,op,data);
      break;
    case 2:  // '040 (or higher) extension
      dis_040ext(loc,opcode);
      break;
    default:  // Anything else is illegal
      exec_unimplemented("F-Line, bad cp id");
      break;
  }
}


void Em68k::exec_040ext(ULONG loc,unsigned int opcode)
{
  if(type<E_CPU_68040) { exec_unimplemented("Bad f-line"); return; }
  if(!(sr&E68k_STAT_S)) {
    cause_int(E68k_INT_PRIVILEGE);
  }
  unsigned int topcode=(opcode>>8)&0xff;
  unsigned int bit=(opcode>>5)&0x01;
  unsigned int cache=(opcode>>6)&0x02;
  unsigned int scope=(opcode>>4)&0x02;
  unsigned int reg=opcode&0x07;
  switch(topcode) {
    case 0xf4:  // CPUSH and CINV
      break;
    default:
      exec_unimplemented("Bad f-line");
      break;
  }
}


void Em68k::dis_040ext(ULONG loc,unsigned int opcode)
{
  unsigned int topcode=(opcode>>8)&0xff;
  unsigned int bit=(opcode>>5)&0x01;
  unsigned int cache=(opcode>>6)&0x02;
  unsigned int scope=(opcode>>4)&0x02;
  unsigned int reg=opcode&0x07;
  switch(topcode) {
    case 0xf4:
      if(bit) DEBUG_OUT<<"cpush"; else DEBUG_OUT<<"cinv";
      switch(scope) {
        case 1: DEBUG_OUT<<"l"; break;
        case 2: DEBUG_OUT<<"p"; break;
        case 3: DEBUG_OUT<<"a"; break;
        default: exec_unimplemented("bad scope!"); break;
      }
      DEBUG_OUT<<" ";
      switch(cache) {
        case 1: DEBUG_OUT<<"data"; break;
        case 2: DEBUG_OUT<<"instruction"; break;
        case 3: DEBUG_OUT<<"data and instruction"; break;
        default: exec_unimplemented("bad cache!"); break;
      }
      if(scope!=3) { DEBUG_OUT<<","; print_reg(reg+8); }
      break;
    default:
      exec_unimplemented("Bad f-line");
      break;
  }
}


// TODO: Look up PLOAD's op
// TODO: May have to add op_len to loc when printing ea's
// TODO: 64 bit crp and srp are only 32 bits here...
void Em68k::exec_pxxx(ULONG loc, unsigned int op, unsigned int data)
{
  if(type>E_CPU_68030) {
    exec_unimplemented("Bad F-line");
    return;
  }
  if(hasMMU) {
    if(!(sr&E68k_STAT_S)) {
      cause_int(E68k_INT_PRIVILEGE);
    }
    unsigned int mode=(opcode>>3)&0x07;
    unsigned int reg=opcode&0x07;
    unsigned int preg=0,rw=0,op2=0,ext=0;
    unsigned int mask=0;
    switch(op) {
      case 0x0:  // PFLUSH, PFLUSHR, PMOVE, PTEST, PVALID
        ext=getImm_op_len(loc,ECPUABSTRACT_SIZE_WORD);
        // TODO: do other instructions beside PMOVE!
        preg=(ext>>10)&0x07;
        rw=(ext>>9)&1;
        op2=ext&0xe000;
        switch(op2) {
          case 0x0000: // PMOVE TT regs ('030 only)
            if(type!=E_CPU_68030) exec_unimplemented("Bad PMOVE mode!");
            else {
              if(rw) {
                // reg to EA
                switch(preg) {
                  case 0x2:
                    write_ea_op_len(loc,mode,reg,tt0&0xffff8777,ECPUABSTRACT_SIZE_LONG);
                    break;
                  case 0x3:
                    write_ea_op_len(loc,mode,reg,tt1&0xffff8777,ECPUABSTRACT_SIZE_LONG);
                    break;
                  default: exec_unimplemented("PMOVE from bad preg"); break;
                }
              }
              else {
                // EA to reg
                switch(preg) {
                  case 0x2:
                    tt0=read_ea_op_len(loc,mode,reg,ECPUABSTRACT_SIZE_LONG)&0xffff8777;
                    break;
                  case 0x3:
                    tt1=read_ea_op_len(loc,mode,reg,ECPUABSTRACT_SIZE_LONG)&0xffff8777;
                    break;
                  default: exec_unimplemented("PMOVE to bad preg"); break;
                }
              }
            }
            break;
          case 0x4000: // PMOVE format 1
            if(rw) {
              // reg to EA
              switch(preg) {
                case 0x0:
                  // TODO: Check on '030 bits
                  write_ea_op_len(loc,mode,reg,tc&0x83ffffff,ECPUABSTRACT_SIZE_LONG);
                  break;
                case 0x2:
                  // TODO: This should be 64-bit
                  // TODO: zero out unused bits...
                  write_ea_op_len(loc,mode,reg,srp_l,ECPUABSTRACT_SIZE_LONG);
                  break;
                case 0x3:
                  // TODO: This should be 64-bit
                  // TODO: zero out unused bits...
                  write_ea_op_len(loc,mode,reg,urp_l,ECPUABSTRACT_SIZE_LONG);
                  break;
                default: exec_unimplemented("PMOVE from bad preg"); break;
              }
            }
            else {
              // EA to reg
              switch(preg) {
                case 0x0:
                  // TODO: Check on '030 bits
                  tc=read_ea_op_len(loc,mode,reg,ECPUABSTRACT_SIZE_LONG)&0x83ffffff;
                  break;
                case 0x2:
                  // TODO: This should be 64-bit
                  // TODO: zero out unused bits...
                  srp_l=read_ea_op_len(loc,mode,reg,ECPUABSTRACT_SIZE_LONG);
                  break;
                case 0x3:
                  // TODO: This should be 64-bit
                  // TODO: zero out unused bits...
                  urp_l=read_ea_op_len(loc,mode,reg,ECPUABSTRACT_SIZE_LONG);
                  break;
                default: exec_unimplemented("PMOVE to bad preg"); break;
              }
            }
            break;
          case 0x6000: // PMOVE format 3
            if(rw) {
              // reg to EA
              switch(preg) {
                case 0x0:
                  if(type==E_CPU_68020) mask=0xff87; else mask=0xee47;
                  write_ea_op_len(loc,mode,reg,mmusr&mask,ECPUABSTRACT_SIZE_WORD);
                  break;
                default: exec_unimplemented("PMOVE from bad preg"); break;
              }
            }
            else {
              // EA to reg
              switch(preg) {
                case 0x0:
                  if(type==E_CPU_68020) mask=0xff87; else mask=0xee47;
                  mmusr=read_ea_op_len(loc,mode,reg,ECPUABSTRACT_SIZE_WORD)&mask;
                  break;
                default: exec_unimplemented("PMOVE to bad preg"); break;
              }
            }
            break;
          default: // Assume PMOVE format 2
            exec_unimplemented("PMOVE to/from BAxx reg");
            break;
        }
        break;
      case 0x1:  // PDBcc, PScc, PTRAPcc
        exec_unimplemented("PDBcc/PScc/PTRAPcc"); break;
      case 0x2:  // PBcc
        exec_unimplemented("PBcc"); break;
      case 0x3:  // PBcc
        exec_unimplemented("PBcc"); break;
      case 0x4:  // PSAVE
        exec_unimplemented("PSAVE"); break;
      case 0x5:  // PRESTORE
        exec_unimplemented("PRESTORE"); break;
      case 0x6:
        exec_unimplemented("Illegal MMU op=6"); break;
      case 0x7:
        exec_unimplemented("Illegal MMU op=7"); break;
    }
  }
  else exec_unimplemented("Pxxx instruction");
}


// TODO: Look up PLOAD's op
// TODO: May have to add op_len to loc when printing ea's
void Em68k::dis_pxxx(ULONG loc, unsigned int op, unsigned int data)
{
  unsigned int mode=(opcode>>3)&0x07;
  unsigned int reg=opcode&0x07;
  unsigned int preg=0,rw=0,op2=0,ext=0;
  switch(op) {
    case 0x0:  // PFLUSH, PFLUSHR, PMOVE, PTEST, PVALID
      ext=getImm_op_len(loc,ECPUABSTRACT_SIZE_WORD);
      // TODO: do other instructions beside PMOVE!
      preg=(ext>>10)&0x07;
      rw=(ext>>9)&1;
      op2=ext&0xe000;
      switch(op2) {
        case 0x0000: // PMOVE for TT ('030 only)
          DEBUG_OUT<<"pmove ";
          if(!rw) { print_ea_op_len(loc,mode,reg); DEBUG_OUT<<","; }
          switch(preg) {
            case 0x2: DEBUG_OUT<<"tt0"; break;
            case 0x3: DEBUG_OUT<<"tt1"; break;
            default: DEBUG_OUT<<"???"; break;  // Illegal preg number
          }
          if(rw) { DEBUG_OUT<<","; print_ea_op_len(loc,mode,reg); }
          break;
          break;
        case 0x4000: // PMOVE format 1
          DEBUG_OUT<<"pmove ";
          if(!rw) { print_ea_op_len(loc,mode,reg); DEBUG_OUT<<","; }
          switch(preg) {
            case 0x0: DEBUG_OUT<<"tc"; break;
            case 0x1: DEBUG_OUT<<"drp"; break;
            case 0x2: DEBUG_OUT<<"srp"; break;
            case 0x3: DEBUG_OUT<<"crp"; break;
            case 0x4: DEBUG_OUT<<"cal"; break;
            case 0x5: DEBUG_OUT<<"val"; break;
            case 0x6: DEBUG_OUT<<"scc"; break;
            case 0x7: DEBUG_OUT<<"ac"; break;
            default: DEBUG_OUT<<"???"; break;  // Illegal preg number
          }
          if(rw) { DEBUG_OUT<<","; print_ea_op_len(loc,mode,reg); }
          break;
        case 0x6000: // PMOVE format 3
          DEBUG_OUT<<"pmove ";
          if(!rw) { print_ea_op_len(loc,mode,reg); DEBUG_OUT<<","; }
          switch(preg) {
            case 0x0: DEBUG_OUT<<"psr"; break;
            case 0x1: DEBUG_OUT<<"pcsr"; break;
            default: DEBUG_OUT<<"???"; break;  // Illegal preg number
          }
          if(rw) { DEBUG_OUT<<","; print_ea_op_len(loc,mode,reg); }
          break;
        default: // Assume PMOVE format 2  ('020 only)
          unsigned int num=(ext>>2)&0x07;
          DEBUG_OUT<<"pmove ";
          if(!rw) { print_ea_op_len(loc,mode,reg); DEBUG_OUT<<","; }
          switch(preg) {
            case 0x4: DEBUG_OUT<<"bad"<<((char)'0'+num); break;
            case 0x5: DEBUG_OUT<<"bac"<<((char)'0'+num); break;
            default: DEBUG_OUT<<"???"; break;  // Illegal preg number
          }
          if(rw) { DEBUG_OUT<<","; print_ea_op_len(loc,mode,reg); }
          break;
      }
      break;
    case 0x1:  // PDBcc, PScc, PTRAPcc
      exec_unimplemented("Unimplemented PDBcc/PScc/PTRAPcc");
      break;
    case 0x2:  // PBcc
      exec_unimplemented("Unimplemented PBcc");
      break;
    case 0x3:  // PBcc
      exec_unimplemented("Unimplemented PBcc");
      break;
    case 0x4:  // PSAVE
      DEBUG_OUT<<"psave ";  print_ea_op_len(loc,mode,reg);
      break;
    case 0x5:  // PRESTORE
      DEBUG_OUT<<"prestore ";  print_ea_op_len(loc,mode,reg);
      break;
    case 0x6:
      exec_unimplemented("Illegal MMU instruction op=6");
      break;
    case 0x7:
      exec_unimplemented("Illegal MMU instruction op=7");
      break;
  }
}


void Em68k::exec_fxxx(ULONG loc, unsigned int op, unsigned int data)
{
  if(!hasFPU) { exec_unimplemented("Fxxx instruction"); return; }
  unsigned int ext=getImm_op_len(loc,ECPUABSTRACT_SIZE_WORD);
  unsigned int opdrm=(ext>>13)&0x07;
  unsigned int edr=(ext>>13)&0x07;
  // NOTE: Some opcodes reverse sreg and dreg
  unsigned int sreg=(ext>>10)&0x07;
  unsigned int dreg=(ext>>7)&0x07;
  unsigned int opmode=ext&0x7f;
  unsigned int mode=(op>>3)&0x7;
  unsigned int reg=op&0x7;
  switch(opdrm) {
    case 0:
    case 2:
      // ea to reg
      exec_unimplemented("FMOVE ea to reg");
      break;
    case 3:
      // reg to mem
      exec_unimplemented("FMOVE reg to mem");
      break;
    case 4:
    case 5:
      // fp control reg to or from ea
      if(opmode) exec_unimplemented("opmode should be zero!");
      if(dreg) exec_unimplemented("dreg should be zero!");
      if(opdrm==4) {
        unsigned int val=read_ea_op_len(loc,mode,reg,ECPUABSTRACT_SIZE_LONG);
        switch(sreg) {
          case 4: fpcr=val; break;
          case 2: fpsr=val; break;
          case 1: exec_unimplemented("write to fpiar!"); break;
          default: exec_unimplemented("bad fpcreg!"); break;
        }
      }
      else {
        unsigned int val=0;
        switch(sreg) {
          case 4: val=fpcr; break;
          case 2: val=fpsr; break;
          case 1: val=pc; break;
          default: exec_unimplemented("bad fpcreg!"); break;
        }
        write_ea_op_len(loc,mode,reg,val,ECPUABSTRACT_SIZE_LONG);
      }
      break;
    case 6:
    case 7:
      // fmovem to or from ea
      exec_unimplemented("FMOVEM.X");
      break;
    default:
      DEBUG_OUT<<"FPU: opdrm="<<opdrm<<", opmode="<<opmode<<"\n";
      exec_unimplemented("FPU");
      break;
  }
}


void Em68k::print_fmove(unsigned int opmode)
{
  switch(opmode) {
    case 0x00:  DEBUG_OUT<<"FMOVE";  break;
    case 0x40:  DEBUG_OUT<<"FSMOVE";  break;
    case 0x44:  DEBUG_OUT<<"FDMOVE";  break;
    default:
      DEBUG_OUT<<" ??? (opmode="<<opmode<<")\n";
      break;
  }
}


void Em68k::dis_fxxx(ULONG loc, unsigned int op, unsigned int data)
{
  unsigned int ext=getImm_op_len(loc,ECPUABSTRACT_SIZE_WORD);
  unsigned int opdrm=(ext>>13)&0x07;
  unsigned int edr=(ext>>13)&0x07;
  // NOTE: Some opcodes reverse sreg and dreg
  unsigned int sreg=(ext>>10)&0x07;
  unsigned int dreg=(ext>>7)&0x07;
  unsigned int opmode=ext&0x7f;
  unsigned int mode=(op>>3)&0x7;
  unsigned int reg=op&0x7;
  switch(opdrm) {
    case 0:
    case 2:
      // ea to reg
      print_fmove(opmode);
      DEBUG_OUT<<" (ea to reg)";
      break;
    case 3:
      // reg to mem
      print_fmove(opmode);
      DEBUG_OUT<<" (reg to mem)";
      break;
    case 4:
    case 5:
      // fp control reg to or from ea
      DEBUG_OUT<<"fmove.l ";
      if(opmode) exec_unimplemented("opmode should be zero!");
      if(dreg) exec_unimplemented("dreg should be zero!");
      if(opdrm==4) { print_ea_op_len(loc,mode,reg); DEBUG_OUT<<","; }
      switch(sreg) {
        case 4: DEBUG_OUT<<"FPCR"; break;
        case 2: DEBUG_OUT<<"FPSR"; break;
        case 1: DEBUG_OUT<<"FPIAR"; break;
        default: exec_unimplemented("bad fpcreg!"); break;
      }
      if(opdrm==5) { DEBUG_OUT<<","; print_ea_op_len(loc,mode,reg); }
      break;
    case 6:
    case 7:
      // fmovem to or from ea
      DEBUG_OUT<<"fmovem.x";
      break;
    default:
      DEBUG_OUT<<"FPU: opdrm="<<opdrm<<", opmode="<<opmode<<"\n";
      exec_unimplemented("FPU");
      break;
  }
}


void Em68k::exec_special(ULONG loc, unsigned int opcode)
{
  unsigned int topcode=(opcode>>8)&0xff;
  switch(topcode) {
    case 0x00:  case 0x01:  case 0x02:  case 0x03:
    case 0x04:  case 0x05:  case 0x06:  case 0x07:
    case 0x08:  case 0x09:  case 0x0a:  case 0x0b:
    case 0x0c:  case 0x0d:  case 0x0e:  case 0x0f:
      exec_special_0x(loc,opcode);  break;
    case 0x40: exec_move_sr_ccr(loc,opcode); break;
    case 0x48:  case 0x4c:  exec_movem(loc,opcode);  break;
    case 0x4e:  exec_special_4e(loc,opcode);  break;
    case 0xe0:  case 0xe1:  case 0xe2:  case 0xe3:
    case 0xe4:  case 0xe5:  case 0xe6:  case 0xe7:
    case 0xe8:  case 0xe9:  case 0xea:  case 0xeb:
    case 0xec:  case 0xed:  case 0xee:  case 0xef:
      exec_special_ex(loc,opcode);  break;
    default:  exec_unimplemented("special instruction");  break;
  }
}


void Em68k::dis_special(ULONG loc, unsigned int opcode)
{
  unsigned int topcode=(opcode>>8)&0xff;
  switch(topcode) {
    case 0x00:  case 0x01:  case 0x02:  case 0x03:
    case 0x04:  case 0x05:  case 0x06:  case 0x07:
    case 0x08:  case 0x09:  case 0x0a:  case 0x0b:
    case 0x0c:  case 0x0d:  case 0x0e:  case 0x0f:
      dis_special_0x(loc,opcode);  break;
    case 0x40: dis_move_sr_ccr(loc,opcode); break;
    case 0x48:  case 0x4c:  dis_movem(loc,opcode);  break;
    case 0x4e:  dis_special_4e(loc,opcode);  break;
    case 0xe0:  case 0xe1:  case 0xe2:  case 0xe3:
    case 0xe4:  case 0xe5:  case 0xe6:  case 0xe7:
    case 0xe8:  case 0xe9:  case 0xea:  case 0xeb:
    case 0xec:  case 0xed:  case 0xee:  case 0xef:
      dis_special_ex(loc,opcode);  break;
    default:  exec_unimplemented("special");  break;
  }
}


void Em68k::exec_special_ex(ULONG loc, unsigned int opcode)
{
  // Predecode all the various fields, even if we won't need them
  unsigned int op=(opcode>>3)&0x03;
  unsigned int dir=(opcode>>8)&0x01;
  unsigned int size=(opcode>>6)&0x03;
  unsigned int count=(opcode>>9)&0x07;
  if(size==0x03) {
    // Memory shift/rotate
    switch(count) {
      case 0x00:
        if(dir) exec_aslm(loc,opcode); else exec_asrm(loc,opcode);
        break;
      case 0x01:
        if(dir) exec_lslm(loc,opcode); else exec_lsrm(loc,opcode);
        break;
      case 0x02:
        if(dir) exec_roxlm(loc,opcode); else exec_roxrm(loc,opcode);
        break;
      case 0x03:
        if(dir) exec_rolm(loc,opcode); else exec_rorm(loc,opcode);
        break;
      default:
        exec_unimplemented("ex (bad count)");
        break;
    }
  }
  else {
    // Register shift/rotate
    switch(size) {
      case 0x00: size=ECPUABSTRACT_SIZE_BYTE; break;
      case 0x01: size=ECPUABSTRACT_SIZE_WORD; break;
      case 0x02: size=ECPUABSTRACT_SIZE_LONG; break;
      default: exec_unimplemented("bad size in shift/rotate!"); break;
    }
    switch(op) {
      case 0x00:
        if(dir) exec_aslr(loc,opcode,count,size);
        else exec_asrr(loc,opcode,count,size);
        break;
      case 0x01:
        if(dir) exec_lslr(loc,opcode,count,size);
        else exec_lsrr(loc,opcode,count,size);
        break;
      case 0x02:
        if(dir) exec_roxlr(loc,opcode,count,size);
        else exec_roxrr(loc,opcode,count,size);
        break;
      case 0x03:
        if(dir) exec_rolr(loc,opcode,count,size);
        else exec_rorr(loc,opcode,count,size);
        break;
      default:
        exec_unimplemented("ex (bad op)");
        break;
    }
  }
}


// TODO: For now this only does shift/rotate, not bitfields
void Em68k::dis_special_ex(ULONG loc, unsigned int opcode)
{
  // Predecode all the various fields, even if we won't need them
  unsigned int op=(opcode>>3)&0x03;
  unsigned int dir=(opcode>>8)&0x01;
  unsigned int size=(opcode>>6)&0x03;
  unsigned int count=(opcode>>9)&0x07;
  if(size==0x03) {
    // Memory shift/rotate
    switch(count) {
      case 0x00:
        DEBUG_OUT<<"AS";
        if(dir) dis_xxlm(loc,opcode); else dis_xxrm(loc,opcode);
        break;
      case 0x01:
        DEBUG_OUT<<"LS";
        if(dir) dis_xxlm(loc,opcode); else dis_xxrm(loc,opcode);
        break;
      case 0x02:
        DEBUG_OUT<<"ROX";
        if(dir) dis_xxlm(loc,opcode); else dis_xxrm(loc,opcode);
        break;
      case 0x03:
        DEBUG_OUT<<"RO";
        if(dir) dis_xxlm(loc,opcode); else dis_xxrm(loc,opcode);
        break;
      default:
        exec_unimplemented("ex (bad count)");
        break;
    }
  }
  else {
    // Register shift/rotate
    switch(size) {
      case 0x00: size=ECPUABSTRACT_SIZE_BYTE; break;
      case 0x01: size=ECPUABSTRACT_SIZE_WORD; break;
      case 0x02: size=ECPUABSTRACT_SIZE_LONG; break;
      default: exec_unimplemented("bad size in shift/rotate!"); break;
    }
    switch(op) {
      case 0x00:
        DEBUG_OUT<<"AS";
        if(dir) dis_xxlr(loc,opcode,count,size);
        else dis_xxrr(loc,opcode,count,size);
        break;
      case 0x01:
        DEBUG_OUT<<"LS";
        if(dir) dis_xxlr(loc,opcode,count,size);
        else dis_xxrr(loc,opcode,count,size);
        break;
      case 0x02:
        DEBUG_OUT<<"ROX";
        if(dir) dis_xxlr(loc,opcode,count,size);
        else dis_xxrr(loc,opcode,count,size);
        break;
      case 0x03:
        DEBUG_OUT<<"RO";
        if(dir) dis_xxlr(loc,opcode,count,size);
        else dis_xxrr(loc,opcode,count,size);
        break;
       default:
        exec_unimplemented("ex (bad op)");
        break;
    }
  }
}


void Em68k::exec_move_sr_ccr(ULONG loc,unsigned int opcode)
{
  BYTE topcode=(opcode>>8)&0xff;
  unsigned int mode=(opcode>>3)&0x07;
  unsigned int reg=opcode&0x07;
  switch(topcode) {
    case 0x40:
      if(type>E_CPU_68000) { exec_unimplemented("MOVE SR,<ea>!"); return; }
      write_ea_op_len(loc,mode,reg,sr&0xffff,ECPUABSTRACT_SIZE_WORD);
      break;
    default: exec_unimplemented("MOVE_SR_CCR"); break;
  }
}


void Em68k::dis_move_sr_ccr(ULONG loc,unsigned int opcode)
{
  BYTE topcode=(opcode>>8)&0xff;
  unsigned int mode=(opcode>>3)&0x07;
  unsigned int reg=opcode&0x07;
  switch(topcode) {
    case 0x40:
      if(type>E_CPU_68000) { exec_unimplemented("MOVE SR,<ea>!"); return; }
      DEBUG_OUT<<"move sr,";
      print_ea_op_len(loc,mode,reg,ECPUABSTRACT_SIZE_WORD);
      break;
    default: exec_unimplemented("MOVE_SR_CCR"); break;
  }
}


// NOTE: Special encoding, if bits is >0x8000 we use it as a reg #
void Em68k::exec_bxxx(ULONG loc, unsigned int opcode, unsigned int srcSize, unsigned int type, unsigned int bits)
{
  unsigned int mode=(opcode>>3)&0x07;
  unsigned int reg=opcode&0x07;
  unsigned int size=ECPUABSTRACT_SIZE_BYTE;
  if(bits&0x8000) bits=read_reg(bits-0x8000,size);
  if(!mode) { size=ECPUABSTRACT_SIZE_LONG; bits&=0x1f; } else bits&=0x07;
  unsigned int rmask=1<<bits;
  unsigned int mask=0xffffffff-rmask;
#ifdef DEBUG_EXEC
  DEBUG_OUT<<"Bxxx mask is "<<mask<<"\n";
#endif
  unsigned int val=read_ea_op_len(loc,mode,reg,size);
  unsigned int nval=0;
  switch(type) {
    case 0x0:  /* Nothing to do */  break;  // BTST
    case 0x1:  if(val&rmask) nval=val&mask; else nval=val|rmask; break;  // BCHG
    case 0x2:  nval=val&mask;  break;  // BCLR
    case 0x3:  nval=val|rmask;  break;  // BSET
    default:
      debugger("Illegal Bxxx operation type!\n");  break;
  }
  write_ea(loc+op_len,mode,reg,nval,size);
  if(val-nval) sr=sr&(E68k_STAT_ALL-E68k_STAT_Z);
  else sr=sr|E68k_STAT_Z;
}


// NOTE: Special encoding, if bits is >0x8000 we use it as a reg #
void Em68k::dis_bxxx(ULONG loc, unsigned int opcode, unsigned int srcSize, unsigned int type, unsigned int bits)
{
  unsigned int mode=(opcode>>3)&0x07;
  unsigned int reg=opcode&0x07;
  unsigned int size=ECPUABSTRACT_SIZE_BYTE;
  switch(type) {
    case 0x0:  DEBUG_OUT<<"btst";  break;
    case 0x1:  DEBUG_OUT<<"bchg";  break;
    case 0x2:  DEBUG_OUT<<"bclr";  break;
    case 0x3:  DEBUG_OUT<<"bset";  break;
    default:
      debugger("Illegal Bxxx operation type!\n");  break;
  }
  DEBUG_OUT<<" ";
  if(bits&0x8000) {
    // bits is in a register
    print_reg(bits-0x8000);
  }
  else {
    // bits is in an immediate
    if(!mode) { size=ECPUABSTRACT_SIZE_LONG; bits&=0x1f; } else bits&=0x07;
    DEBUG_OUT<<"#$"<<bits;
  }
  DEBUG_OUT<<",";
  print_ea_op_len(loc+op_len,mode,reg,size);
}


void Em68k::exec_special_0x(ULONG loc, unsigned int opcode)
{
#ifdef DEBUG_EXEC
  DEBUG_OUT<<"exec_special_0x pre op_len is "<<op_len<<"\n";
#endif
  unsigned int topcode=(opcode>>8)&0xff;
  unsigned int nyb=topcode&0x0f;
  unsigned int size=(opcode>>6)&0x03;
  unsigned int mode=(opcode>>3)&0x07;
  unsigned int reg=opcode&0x07;
  unsigned int type=0;
  switch(size) {
    case 0:  size=ECPUABSTRACT_SIZE_BYTE;  break;
    case 1:  size=ECPUABSTRACT_SIZE_WORD;  break;
    case 2:  size=ECPUABSTRACT_SIZE_LONG;  break;
    default: size=ECPUABSTRACT_SIZE_OTHER; break; // but size field isn't always used...
  }
  switch(nyb) {
    case 0x00:  exec_ori(loc,opcode,mode,reg,size);  break;
    case 0x02:  exec_andi(loc,opcode,mode,reg,size);  break;
    case 0x04:  exec_subi(loc,opcode,mode,reg,size);  break;
    case 0x06:  exec_addi(loc,opcode,mode,reg,size);  break;
    case 0x08:
      type=(opcode>>6)&0x03;
      exec_bxxx(loc,opcode,ECPUABSTRACT_SIZE_BYTE,type,map->read16b(loc+2)&0xff);
      op_len+=2;
      break;
    case 0x0a:  exec_eori(loc,opcode,mode,reg,size);  break;
    case 0x0c:  exec_cmpi(loc,opcode,mode,reg,size);  break;
    case 0x0e:  DEBUG_OUT<<"MOVES ???";  break;  // TODO: Need to do this...
    case 0x01:  case 0x03:  case 0x05:  case 0x07:
    case 0x09:  case 0x0b:  case 0x0d:  case 0x0f:
      // TODO: This won't catch MOVEP
      type=(opcode>>6)&0x03;
      reg=(opcode>>9)&0x07;
      // NOTE: Special encoding of bits, see exec_Bxxx
      exec_bxxx(loc,opcode,ECPUABSTRACT_SIZE_BYTE,type,reg|0x8000);
      break;
    default:  exec_unimplemented("0x instruction");  break;
  }
#ifdef DEBUG_EXEC
  DEBUG_OUT<<"exec_special_0x post op_len is "<<op_len<<"\n";
#endif
}


void Em68k::dis_special_0x(ULONG loc, unsigned int opcode)
{
#ifdef DEBUG_EXEC
  DEBUG_OUT<<"dis_special_0x pre op_len is "<<op_len<<"\n";
#endif
  unsigned int topcode=(opcode>>8)&0xff;
  unsigned int nyb=topcode&0x0f;
  unsigned int size=(opcode>>6)&0x03;
  unsigned int mode=(opcode>>3)&0x07;
  unsigned int reg=opcode&0x07;
  unsigned int type=0;
  switch(size) {
    case 0:  size=ECPUABSTRACT_SIZE_BYTE;  break;
    case 1:  size=ECPUABSTRACT_SIZE_WORD;  break;
    case 2:  size=ECPUABSTRACT_SIZE_LONG;  break;
    default: size=ECPUABSTRACT_SIZE_OTHER; break; // but size field isn't always used...
  }
  switch(nyb) {
    case 0x00:  dis_ori(loc,opcode,mode,reg,size);  break;
    case 0x02:  dis_andi(loc,opcode,mode,reg,size);  break;
    case 0x04:  dis_subi(loc,opcode,mode,reg,size);  break;
    case 0x06:  dis_addi(loc,opcode,mode,reg,size);  break;
    case 0x08:
      type=(opcode>>6)&0x03;
      dis_bxxx(loc,opcode,ECPUABSTRACT_SIZE_BYTE,type,map->read16b(loc+2)&0xff);
      op_len+=2;
      break;
    case 0x0a:  dis_eori(loc,opcode,mode,reg,size);  break;
    case 0x0c:  dis_cmpi(loc,opcode,mode,reg,size);  break;
    case 0x0e:  DEBUG_OUT<<"MOVES ???";  break;  // TODO: Need to do this...
    case 0x01:  case 0x03:  case 0x05:  case 0x07:
    case 0x09:  case 0x0b:  case 0x0d:  case 0x0f:
      // TODO: This won't catch MOVEP
      type=(opcode>>6)&0x03;
      reg=(opcode>>9)&0x07;
      // NOTE: Special encoding of bits, see dis_Bxxx
      dis_bxxx(loc,opcode,ECPUABSTRACT_SIZE_BYTE,type,reg|0x8000);
      break;
    default:  exec_unimplemented("0x");  break;
  }
#ifdef DEBUG_EXEC
  DEBUG_OUT<<"dis_special_0x post op_len is "<<op_len<<"\n";
#endif
}


// TODO: I wonder why I didn't use getImm_op_len in here...
void Em68k::exec_bsr(ULONG loc, unsigned int opcode)
{
  int disp8=opcode&0xff;
  int disp=sign_extend68k(disp8,ECPUABSTRACT_SIZE_BYTE);
  if(!disp8) {
    disp=sign_extend68k(map->read16b(loc+2),ECPUABSTRACT_SIZE_WORD);  op_len+=2;
  }
  if(disp8==0xff) { disp=map->read32b(loc+2);  op_len+=4; }
  int addr=(int)loc+disp+2;
  push(loc+op_len,15,ECPUABSTRACT_SIZE_LONG);
  jump(addr);
}


// TODO: This doesn't check illegal EA modes...
void Em68k::exec_jsr(ULONG loc, unsigned int mode, unsigned int reg)
{
  unsigned int val=calc_ea_op_len(loc,mode,reg,ECPUABSTRACT_SIZE_LONG);
  push(loc+op_len,15,ECPUABSTRACT_SIZE_LONG);
  stackTracePush(loc+op_len);
  jump(val);
}


// TODO: This doesn't check illegal EA modes...
void Em68k::exec_jmp(ULONG loc, unsigned int mode, unsigned int reg)
{
  unsigned int val=calc_ea_op_len(loc,mode,reg,ECPUABSTRACT_SIZE_LONG);
  jump(val);
}


void Em68k::exec_movec(ULONG loc,unsigned int opcode)
{
  if(!(sr&E68k_STAT_S)) {
    //exec_unimplemented("MOVEC in user mode!");
    cause_int(E68k_INT_PRIVILEGE);
  }
  if(type==E_CPU_68000) {
    //exec_unimplemented("This cpu can't MOVEC!");
    cause_int(E68k_INT_ILLEGAL);
  }
  unsigned int extra=map->read16b(loc+2);
  op_len+=2;
  unsigned int reg=(extra>>12)&0x0f;  // NOTE: This can be 0-15 (data or address reg)
  unsigned int dir=opcode&0x01;
  unsigned int creg=extra&0x0fff;
  unsigned int mask=0xffffffff;
  switch(creg) {
    case 0x000:  // SFC ('010 or better)
      if(dir) sfc=read_reg(reg,ECPUABSTRACT_SIZE_LONG)&0x07;
      else write_reg(reg,sfc&0x07,ECPUABSTRACT_SIZE_LONG);
      break;
    case 0x001:  // DFC ('010 or better)
      if(dir) dfc=read_reg(reg,ECPUABSTRACT_SIZE_LONG)&0x07;
      else write_reg(reg,dfc&0x07,ECPUABSTRACT_SIZE_LONG);
      break;
    case 0x002:  // CACR ('020 or better)
      switch(type) {
        case E_CPU_68020:  mask=0x0f;  break;
        case E_CPU_68030:  mask=0x3f1f;  break;
        case E_CPU_68040:  mask=0x80008000;  break;
        case E_CPU_68060:  mask=0xf8e0e000;  break;
        case E_CPU_ColdFire:  mask=0x80008000;  break;
        default: break;
      }
      if(type<E_CPU_68020) exec_unimplemented("MOVEC cacr!");
      else {
        if(dir) cacr=read_reg(reg,ECPUABSTRACT_SIZE_LONG)&mask;
        else write_reg(reg,cacr&mask,ECPUABSTRACT_SIZE_LONG);
      }
      break;
    case 0x003:  // TC ('040)
      if(type<E_CPU_68040) exec_unimplemented("MOVEC tc!");
      else {
        if(dir) tc=read_reg(reg,ECPUABSTRACT_SIZE_LONG)&0xc000;
        else write_reg(reg,tc&0xc000,ECPUABSTRACT_SIZE_LONG);
      }
      break;
    case 0x004:  // ITT0 ('040)
      // NOTE: TT1=ITT1
      if(type<E_CPU_68040) exec_unimplemented("MOVEC itt0!");
      else {
        if(dir) tt0=read_reg(reg,ECPUABSTRACT_SIZE_LONG)&0xffffe364;
        else write_reg(reg,tt0&0xffffe364,ECPUABSTRACT_SIZE_LONG);
      }
      break;
    case 0x005:  // ITT1 ('040)
      // NOTE: TT1=ITT1
      if(type<E_CPU_68040) exec_unimplemented("MOVEC itt1!");
      else {
        if(dir) tt1=read_reg(reg,ECPUABSTRACT_SIZE_LONG)&0xffffe364;
        else write_reg(reg,tt1&0xffffe364,ECPUABSTRACT_SIZE_LONG);
      }
      break;
    case 0x006:  // DTT0 ('040)
      if(type<E_CPU_68040) exec_unimplemented("MOVEC dtt0!");
      else {
        if(dir) dtt0=read_reg(reg,ECPUABSTRACT_SIZE_LONG)&0xffffe364;
        else write_reg(reg,dtt0&0xffffe364,ECPUABSTRACT_SIZE_LONG);
      }
      break;
    case 0x007:  // DTT1 ('040)
      if(type<E_CPU_68040) exec_unimplemented("MOVEC dtt1!");
      else {
        if(dir) dtt1=read_reg(reg,ECPUABSTRACT_SIZE_LONG)&0xffffe364;
        else write_reg(reg,dtt1&0xffffe364,ECPUABSTRACT_SIZE_LONG);
      }
      break;
    case 0x800:  // USP ('010 or better)
      // NOTE: We will always be in super mode, so this ISN'T a7
      if(dir) sp=read_reg(reg,ECPUABSTRACT_SIZE_LONG);
      else write_reg(reg,sp,ECPUABSTRACT_SIZE_LONG);
      break;
    case 0x801:  // VBR ('010 or better)
      if(dir) vbr=read_reg(reg,ECPUABSTRACT_SIZE_LONG);
      else write_reg(reg,vbr,ECPUABSTRACT_SIZE_LONG);
      break;
    case 0x802:  // CAAR ('020 or '030, but not '040)
      if(type<E_CPU_68020) exec_unimplemented("MOVEC caar!"); else
      if(type>E_CPU_68030) exec_unimplemented("MOVEC caar!");
      else {
        if(dir) caar=read_reg(reg,ECPUABSTRACT_SIZE_LONG);
        else write_reg(reg,caar,ECPUABSTRACT_SIZE_LONG);
      }
      break;
    case 0x803:  // MSP ('020 or better)
      if(type<E_CPU_68020) exec_unimplemented("MOVEC msp!");
      else {
        if(dir) {
          val=read_reg(reg,ECPUABSTRACT_SIZE_LONG);
          msp=val;
          if(sr&E68k_STAT_M) write_reg(15,val,ECPUABSTRACT_SIZE_LONG);
        }
        else write_reg(reg,msp,ECPUABSTRACT_SIZE_LONG);
      }
      break;
    case 0x804:  // ISP ('020 or better)
      if(type<E_CPU_68020) exec_unimplemented("MOVEC msp!");
      else {
        // NOTE: ssp=isp
        if(dir) {
          val=read_reg(reg,ECPUABSTRACT_SIZE_LONG);
          ssp=val;
          if(!(sr&E68k_STAT_M)) write_reg(15,val,ECPUABSTRACT_SIZE_LONG);
        }
        else write_reg(reg,ssp,ECPUABSTRACT_SIZE_LONG);
      }
      break;
    case 0x805:  // MMUSR ('040)
      if(type<E_CPU_68040) exec_unimplemented("MOVEC mmusr!");
      else {
        if(dir) mmusr=read_reg(reg,ECPUABSTRACT_SIZE_WORD)&0xfffffff7;
        else write_reg(reg,mmusr&0xfffffff7,ECPUABSTRACT_SIZE_WORD);
      }
      break;
    case 0x806:  // URP ('040, uses urp_l only)
      if(type<E_CPU_68040) exec_unimplemented("MOVEC urp!");
      else {
        if(dir) urp_l=read_reg(reg,ECPUABSTRACT_SIZE_LONG)&0xffffff00;
        else write_reg(reg,urp_l&0xffffff00,ECPUABSTRACT_SIZE_LONG);
      }
      break;
    case 0x807:  // SRP ('040, uses srp_l only)
      if(type<E_CPU_68040) exec_unimplemented("MOVEC srp!");
      else {
        if(dir) srp_l=read_reg(reg,ECPUABSTRACT_SIZE_LONG)&0xffffff00;
        else write_reg(reg,srp_l&0xffffff00,ECPUABSTRACT_SIZE_LONG);
      }
      break;
    default:  exec_unimplemented("?reg? for MOVEC");  break;
  }
}


void Em68k::exec_rts(ULONG loc)
{
  unsigned int val=pull(15,ECPUABSTRACT_SIZE_LONG);
  jump(val);
  stackTracePop(val);
}


void Em68k::exec_special_4e(ULONG loc, unsigned int opcode)
{
  unsigned int reg=0;
  unsigned int mode=0;
  unsigned int size=ECPUABSTRACT_SIZE_OTHER;
  unsigned int val=0;
  unsigned int creg=0,dir=0,extra=0,vector=0,mask=0;
  // Look at last byte of opcode...
  unsigned int bopcode=opcode&0xff;
  switch(bopcode) {
    case 0x40:  case 0x41:  case 0x42:  case 0x43:
    case 0x44:  case 0x45:  case 0x46:  case 0x47:
    case 0x48:  case 0x49:  case 0x4a:  case 0x4b:
    case 0x4c:  case 0x4d:  case 0x4e:  case 0x4f:
      exec_trap(loc,opcode);
      break;
    case 0x50:  case 0x51:  case 0x52:  case 0x53:
    case 0x54:  case 0x55:  case 0x56:  case 0x57:
      exec_link(loc,opcode);
      break;
    case 0x58:  case 0x59:  case 0x5a:  case 0x5b:
    case 0x5c:  case 0x5d:  case 0x5e:  case 0x5f:
      exec_unlk(loc,opcode);
      break;
    case 0x60:  case 0x61:  case 0x62:  case 0x63:
    case 0x64:  case 0x65:  case 0x66:  case 0x67:
      reg=(opcode&0x07)+8;
      //exec_unimplemented("MOVE->USP");
      sp=read_reg(reg,ECPUABSTRACT_SIZE_LONG);
      break;
    case 0x68:  case 0x69:  case 0x6a:  case 0x6b:
    case 0x6c:  case 0x6d:  case 0x6e:  case 0x6f:
      reg=(opcode&0x07)+8;
      //exec_unimplemented("MOVE->USP");
      write_reg(reg,sp,ECPUABSTRACT_SIZE_LONG);
      break;
    case 0x70:
      // NOTE: This is supposed to reset external devices, but NOT the cpu!
      //if(os) os->reset();
      break;
    case 0x71:
      /* NOP */
      // sync the pipeline...
      // wait for all bus ops to complete...
      break;
    case 0x72:  exec_unimplemented("STOP");
      /* DEBUG_OUT<<"STOP #$"<<map->readwb(loc+2); */  op_len+=2;  break;
    case 0x73:  exec_unimplemented("RTE");  break;
    case 0x74:  exec_unimplemented("RTD");
      /* DEBUG_OUT<<"RTD #$"<<map->readwb(loc+2); */  op_len+=2;  break;
    case 0x75:  exec_rts(loc);  break;
    case 0x76:  exec_unimplemented("TRAPV");  break;
    case 0x77:  exec_unimplemented("RTR");  break;
    case 0x7a:  case 0x7b:  // MOVEC
      exec_movec(loc,opcode);
      break;
    default:
      // Look at top 2 bits of last byte of opcode...
      bopcode=opcode&0xc0;
      mode=(opcode>>3)&0x07;  reg=opcode&0x07;
      switch(bopcode) {
        case 0x80:  exec_jsr(loc,mode,reg); break;
        case 0xc0:  exec_jmp(loc,mode,reg); break;
        default:  exec_unimplemented("unknown 4e instuction");  break;
      }
      break;
  }
}


void Em68k::dis_special_4e(ULONG loc, unsigned int opcode)
{
  unsigned int reg=0;
  unsigned int mode=0;
  unsigned int size=ECPUABSTRACT_SIZE_OTHER;
  unsigned int creg=0,dir=0,extra=0,vector=0,mask=0;
  EMapSymbol *e=(EMapSymbol *)NULL;
  unsigned int ea=0;
  // Look at last byte of opcode...
  unsigned int bopcode=opcode&0xff;
  switch(bopcode) {
    case 0x40:  case 0x41:  case 0x42:  case 0x43:
    case 0x44:  case 0x45:  case 0x46:  case 0x47:
    case 0x48:  case 0x49:  case 0x4a:  case 0x4b:
    case 0x4c:  case 0x4d:  case 0x4e:  case 0x4f:
      vector=opcode&0x0f;
      DEBUG_OUT<<"trap #$"<<vector;
      break;
    case 0x50:  case 0x51:  case 0x52:  case 0x53:
    case 0x54:  case 0x55:  case 0x56:  case 0x57:
      reg=bopcode&0x07;
      DEBUG_OUT<<"link ";
      print_reg(reg+8);
      DEBUG_OUT<<",#$"<<map->read16b(loc+2);
      op_len+=2;
      break;
    case 0x58:  case 0x59:  case 0x5a:  case 0x5b:
    case 0x5c:  case 0x5d:  case 0x5e:  case 0x5f:
      reg=bopcode&0x07;
      DEBUG_OUT<<"unlk ";
      print_reg(reg+8);
      break;
    case 0x60:  case 0x61:  case 0x62:  case 0x63:
    case 0x64:  case 0x65:  case 0x66:  case 0x67:
      reg=opcode&0x07;
      DEBUG_OUT<<"move ";
      print_reg(reg+8);
      DEBUG_OUT<<",usp";
      break;
    case 0x68:  case 0x69:  case 0x6a:  case 0x6b:
    case 0x6c:  case 0x6d:  case 0x6e:  case 0x6f:
      reg=opcode&0x07;
      DEBUG_OUT<<"move usp,";
      print_reg(reg+8);
      break;
    case 0x70:  DEBUG_OUT<<"reset";  break;
    case 0x71:  DEBUG_OUT<<"nop";  break;
    case 0x72:  DEBUG_OUT<<"stop #$"<<map->read16b(loc+2);  op_len+=2;  break;
    case 0x73:  DEBUG_OUT<<"rte";  break;
    case 0x74:  DEBUG_OUT<<"rtd #$"<<map->read16b(loc+2);  op_len+=2;  break;
    case 0x75:  DEBUG_OUT<<"rts";  break;
    case 0x76:  DEBUG_OUT<<"trapv";  break;
    case 0x77:  DEBUG_OUT<<"rtr";  break;
    case 0x7a:  case 0x7b:
      DEBUG_OUT<<"movec ";
      extra=map->read16b(loc+2);
      op_len+=2;
      reg=(extra>>12)&0x0f;  // NOTE: This can be 0-15
      dir=opcode&0x01;
      if(dir) {
        print_reg(reg);
        DEBUG_OUT<<",";
      }
      creg=extra&0x0fff;
      switch(creg) {
        case 0x000:  DEBUG_OUT<<"sfc";  break;   //'010 or better
        case 0x001:  DEBUG_OUT<<"dfc";  break;   //'010 or better
        case 0x002:  DEBUG_OUT<<"cacr";  break;  //'020 or better
        case 0x003:  DEBUG_OUT<<"tc";  break;    //'040
        case 0x004:  DEBUG_OUT<<"itt0";  break;  //'040
        case 0x005:  DEBUG_OUT<<"itt1";  break;  //'040
        case 0x006:  DEBUG_OUT<<"dtt0";  break;  //'040
        case 0x007:  DEBUG_OUT<<"dtt1";  break;  //'040
        case 0x800:  DEBUG_OUT<<"usp";  break;   //'010 or better
        case 0x801:  DEBUG_OUT<<"vbr";  break;   //'010 or better
        case 0x802:  DEBUG_OUT<<"caar";  break;  //'020 or better
        case 0x803:  DEBUG_OUT<<"msp";  break;   //'020 or better
        case 0x804:  DEBUG_OUT<<"isp";  break;   //'020 or better
        case 0x805:  DEBUG_OUT<<"mmusr";  break; //'040
        case 0x806:  DEBUG_OUT<<"urp";  break;   //'040
        case 0x807:  DEBUG_OUT<<"srp";  break;   //'040
        default:  DEBUG_OUT<<"???";  break;
      }
      if(!dir) {
        DEBUG_OUT<<",";
        print_reg(reg);
      }
      break;
    default:
      // Look at top 2 bits of last byte of opcode...
      bopcode=opcode&0xc0;
      mode=(opcode>>3)&0x07;  reg=opcode&0x07;
      switch(bopcode) {
        case 0x80:
          DEBUG_OUT<<"jsr "; // print_ea_op_len(loc,mode,reg,ECPUABSTRACT_SIZE_LONG);
          ea=calc_ea_op_len(loc,mode,reg,ECPUABSTRACT_SIZE_LONG);
          e=map->findAnyExactSymbol(ea);
          if(e) DEBUG_OUT<<e->getName();
          else print_ea_op_len(loc,mode,reg,ECPUABSTRACT_SIZE_LONG);
          break;
        case 0xc0:
          DEBUG_OUT<<"jmp "; // print_ea_op_len(loc,mode,reg,ECPUABSTRACT_SIZE_LONG);
          ea=calc_ea_op_len(loc,mode,reg,ECPUABSTRACT_SIZE_LONG);
          e=map->findAnyExactSymbol(ea);
          if(e) DEBUG_OUT<<e->getName();
          else print_ea_op_len(loc,mode,reg,ECPUABSTRACT_SIZE_LONG);
          break;
        default:  exec_unimplemented("4e");  break;
      }
      break;
  }
}


// NOTE: modifies op_len...
void Em68k::exec_dbcc(ULONG loc, unsigned int opcode)
{
  unsigned int mode=(opcode>>3)&0x07;
  if(mode!=1) { exec_scc(loc,opcode,mode); return; }  // It was really an Scc
  bool willBranch=false;
  unsigned int cond=(opcode>>8)&0x0f;
  bool srC=((sr&E68k_STAT_C)==0) ? false : true;

  bool srZ=((sr&E68k_STAT_Z)==0) ? false : true;
  bool srV=((sr&E68k_STAT_V)==0) ? false : true;

  bool srN=((sr&E68k_STAT_N)==0) ? false : true;
  switch(cond) {
    case 0: willBranch=false; break;  // T
    case 1: willBranch=true; break;  //F, aka DBRA
    case 2: if((!srC)&&(!srZ)) willBranch=true; break;  // HI
    case 3: if(srC||srZ) willBranch=true;  break;  // LS
    case 4: if(!srC) willBranch=true;  break;  // CC
    case 5: if(srC) willBranch=true;  break;  // CS
    case 6: if(!srZ) willBranch=true;  break;  // NE
    case 7: if(srZ) willBranch=true;  break;  // EQ
    case 8: if(!srV) willBranch=true;  break;  // VC
    case 9: if(srV) willBranch=true;  break;  // VS
    case 10: if(!srN) willBranch=true;  break;  // PL
    case 11: if(srN) willBranch=true;  break;  // MI
    case 12: if(((!srN)&&(!srV))||(srN&&srV)) willBranch=true;
      break;  // GE
    case 13: if(((!srN)&&srV)||(srN&&(!srV))) willBranch=true;
      break;  // LT
    case 14: if((srN&&srV&&(!srZ))||((!srN)&&(!srV)&&(!srZ))) willBranch=true;
      break;  // GT
    case 15: if(((!srN)&&srV)||(srN&&(!srV))||(srZ)) willBranch=true;
      break;  // LE
    default: debugger("DBcc illegal cond!\n"); break;
  }
  // Disp is only 16 bits, unlike Bcc
  int disp=sign_extend68k(map->read16b(loc+2),ECPUABSTRACT_SIZE_WORD);
  op_len+=2;
  unsigned int reg=opcode&0x07;
  // NOTE: reversed willBranch...
  if(!willBranch) {
    unsigned int val=read_reg(reg,ECPUABSTRACT_SIZE_WORD);
    write_reg(reg,val-1,ECPUABSTRACT_SIZE_WORD);
    if(val!=0) {
      int addr=(int)loc+disp+2;
      jump(addr);
    }
  }
}


// NOTE: modifies op_len...
void Em68k::dis_dbcc(ULONG loc, unsigned int opcode)
{
  unsigned int mode=(opcode>>3)&0x07;
  if(mode!=1) { dis_scc(loc,opcode,mode); return; }  // It was really an Scc
  DEBUG_OUT<<"d";
  unsigned int cond=(opcode>>8)&0x0f;
  switch(cond) {
    case 0: DEBUG_OUT<<"DBcc ALWAYS true!\n"; break;  // T, PANIC!
    case 1: DEBUG_OUT<<"bra";  break;
    case 2: DEBUG_OUT<<"bhi";  break;
    case 3: DEBUG_OUT<<"bls";  break;
    case 4: DEBUG_OUT<<"bcc";  break;
    case 5: DEBUG_OUT<<"bcs";  break;
    case 6: DEBUG_OUT<<"bne";  break;
    case 7: DEBUG_OUT<<"beq";  break;
    case 8: DEBUG_OUT<<"bvc";  break;
    case 9: DEBUG_OUT<<"bvs";  break;
    case 10: DEBUG_OUT<<"bpl";  break;
    case 11: DEBUG_OUT<<"bmi";  break;
    case 12: DEBUG_OUT<<"bge";  break;
    case 13: DEBUG_OUT<<"blt";  break;
    case 14: DEBUG_OUT<<"bgt";  break;
    case 15: DEBUG_OUT<<"ble";  break;
    default: DEBUG_OUT<<"DBcc illegal cond!\n";
      break;
  }
  // Disp is only 16 bits, unlike Bcc
  int disp=sign_extend68k(map->read16b(loc+2),ECPUABSTRACT_SIZE_WORD);
  op_len+=2;
  unsigned int reg=opcode&0x07;
  DEBUG_OUT<<" ";
  print_reg(reg);
  DEBUG_OUT<<",$"<<disp<<"(pc)";
}


void Em68k::exec_movem(ULONG loc, unsigned int opcode)
{
  unsigned int bop=opcode&0xff;
  if(!(bop&0x80)) { exec_pea(loc,opcode); return; }
  unsigned int size=(opcode>>6)&0x01;
  if(size) size=ECPUABSTRACT_SIZE_LONG; else size=ECPUABSTRACT_SIZE_WORD;
  unsigned int dir=(opcode>>10)&0x01;
  unsigned int mode=(opcode>>3)&0x07;
  unsigned int reg=opcode&0x07;
  unsigned int mask=getImm_op_len(loc,ECPUABSTRACT_SIZE_WORD);
  switch(mode) {
    case 0: exec_swap(loc,opcode); return; break;
    case 1: exec_unimplemented("Bad movem mode 1!"); return; break;
    case 3:  // post-inc
      if(!dir) { exec_unimplemented("Bad movem mode 3!"); return; } break;
    case 4:  // pre-dec
      if(dir) { exec_unimplemented("Bad movem mode 4!"); return; } break;
    default: break;  // Any other mode is legal
  }
  if(dir) {
    // mem to regs
#ifdef DEBUG_EXEC
    DEBUG_OUT<<"MOVEM mem to regs\n";
#endif
    do_movemmr(loc,mode,reg,size,mask);
  }
  else {
    // regs to mem
#ifdef DEBUG_EXEC
    DEBUG_OUT<<"MOVEM regs to mem\n";
#endif
    if(mode==4) mask=reverse_mask(mask);
    do_movemrm(loc,mode,reg,size,mask);
  }
}


void Em68k::dis_movem(ULONG loc, unsigned int opcode)
{
  unsigned int bop=opcode&0xff;
  if(!(bop&0x80)) { dis_pea(loc,opcode); return; }
  unsigned int size=(opcode>>6)&0x01;
  if(size) size=ECPUABSTRACT_SIZE_LONG; else size=ECPUABSTRACT_SIZE_WORD;
  unsigned int dir=(opcode>>10)&0x01;
  unsigned int mode=(opcode>>3)&0x07;
  unsigned int reg=opcode&0x07;
  unsigned int mask=getImm_op_len(loc,ECPUABSTRACT_SIZE_WORD);
  switch(mode) {
    case 0: dis_swap(loc,opcode); return; break;
    case 1: exec_unimplemented("Bad movem mode 1!"); return; break;
    case 3:
      if(!dir) { exec_unimplemented("Bad movem mode 3!"); return; }
      break;
    case 4:
       if(dir) { exec_unimplemented("Bad movem mode 4!"); return; }
       break;
    default: break;  // Any other mode is legal
  }
  DEBUG_OUT<<"movem";
  print_size(size);
  // NOTE: The 2 is there due to the mask
  if(dir) print_ea_op_len(loc+2,mode,reg,size);
  else {
    if(mode==4) mask=reverse_mask(mask);
    print_movem_maskx(mask,mode,reg);
  }
  DEBUG_OUT<<",";
  if(dir) print_movem_maskx(mask,mode,reg);
  else print_ea_op_len(loc+2,mode,reg,size);
}


// WARNING: Doesn't check for illegal modes...
void Em68k::exec_pea(ULONG loc, unsigned int opcode)
{
  unsigned int mode=(opcode>>3)&0x07;
  if(mode==0) { exec_swap(loc,opcode); return; }
  if(mode==1) { debugger("illegal mode==1 for PEA!"); return; }
  unsigned int reg=opcode&0x07;
  unsigned int ea=calc_ea_op_len(loc,mode,reg,ECPUABSTRACT_SIZE_LONG);
  push(ea,15,ECPUABSTRACT_SIZE_LONG);
}


// WARNING: Doesn't check for illegal modes...
void Em68k::dis_pea(ULONG loc, unsigned int opcode)
{
  unsigned int mode=(opcode>>3)&0x07;
  if(mode==0) { dis_swap(loc,opcode); return; }
  if(mode==1) {
    DEBUG_OUT<<"illegal mode==1 for PEA!";
    return;
  }
  unsigned int reg=opcode&0x07;
  DEBUG_OUT<<"pea ";
  print_ea_op_len(loc,mode,reg,ECPUABSTRACT_SIZE_LONG);
}


// TODO: Byte to address reg is illegal, but not caught...
// TODO: Check on MOVEA
void Em68k::exec_move(ULONG loc, unsigned int topcode, unsigned int reg2, unsigned int mode2, unsigned int mode, unsigned int reg)
{
  unsigned int size=0;
  switch((topcode>>4)&0xf) {
    case 1: size=ECPUABSTRACT_SIZE_BYTE; break;
    case 3: size=ECPUABSTRACT_SIZE_WORD; break;
    case 2: size=ECPUABSTRACT_SIZE_LONG; break;
    default:
      size=ECPUABSTRACT_SIZE_OTHER;
      debugger("Illegal size in MOVE!\n");
      break;
  }
  unsigned int val=read_ea_op_len(loc,mode,reg,size);
  write_ea_op_len(loc+op_len-2,mode2,reg2,val,size);
  // If dest was anything but an address register (MOVEA), set flags
  // TODO: Check that this is right for both dirs...
  if(mode2!=1) setFlagsNZvc(val,size);
}


// TODO: Byte to address reg is illegal, but not caught...
// TODO: print MOVEA when appropriate...
void Em68k::dis_move(ULONG loc, unsigned int topcode, unsigned int reg2, unsigned int mode2, unsigned int mode, unsigned int reg)
{
  DEBUG_OUT<<Em68k_name[(opcode>>8)&0xff];
  unsigned int size=0;
  switch((topcode>>4)&0xf) {
    case 1: size=ECPUABSTRACT_SIZE_BYTE; break;
    case 3: size=ECPUABSTRACT_SIZE_WORD; break;
    case 2: size=ECPUABSTRACT_SIZE_LONG; break;
    default:
      size=ECPUABSTRACT_SIZE_OTHER;
      DEBUG_OUT<<"Illegal size in MOVE!\n";
      break;
  }
  print_size(size);
  print_ea_op_len(loc,mode,reg,size);
  DEBUG_OUT<<",";
  print_ea_op_len(loc+op_len-2,mode2,reg2,size);
}


void Em68k::exec_moveq(ULONG loc, unsigned int reg, unsigned int data)
{
  write_reg(reg,data,ECPUABSTRACT_SIZE_LONG);
  setFlagsNZvc(data,ECPUABSTRACT_SIZE_LONG);
}


void Em68k::dis_moveq(ULONG loc, unsigned int reg, unsigned int data)
{
  DEBUG_OUT<<Em68k_name[(opcode>>8)&0xff]<<" #$"<<data<<",";
  print_reg(reg);
}


void Em68k::exec_lea(ULONG loc, unsigned int reg2, unsigned int mode, unsigned int reg)
{
  if(!mode) { exec_ext(loc,opcode); return; }
  unsigned int val=calc_ea_op_len(loc,mode,reg,ECPUABSTRACT_SIZE_LONG);
  write_reg(reg2+8,val,ECPUABSTRACT_SIZE_LONG);
}


void Em68k::dis_lea(ULONG loc, unsigned int reg2, unsigned int mode, unsigned int reg)
{
  if(!mode) { dis_ext(loc,opcode); return; }
  DEBUG_OUT<<Em68k_name[(opcode>>8)&0xff]<<" ";
  print_ea_op_len(loc,mode,reg);
  DEBUG_OUT<<",";
  print_reg(reg2+8);
}


void Em68k::exec_clr(ULONG loc, unsigned int mode, unsigned int reg, unsigned int size)
{
  unsigned int val=0;
  write_ea_op_len(loc,mode,reg,val,size);
  sr=sr&(E68k_STAT_ALL-E68k_STAT_N);
  sr=sr|E68k_STAT_Z;
  sr=sr&(E68k_STAT_ALL-E68k_STAT_V);
  sr=sr&(E68k_STAT_ALL-E68k_STAT_C);
}


void Em68k::dis_clr(ULONG loc, unsigned int mode, unsigned int reg, unsigned int size)
{
  DEBUG_OUT<<Em68k_name[(opcode>>8)&0xff];
  print_size(size);
  print_ea_op_len(loc,mode,reg,size);
}


void Em68k::exec_neg(ULONG loc, unsigned int mode, unsigned int reg, unsigned int size)
{
  unsigned int val=read_ea_op_len(loc,mode,reg,size);
  unsigned int nval=0-val;
  write_ea(loc,mode,reg,nval,size);
  setFlagsNZ(nval,size);
  // TODO: Check, is this right?  (I know it looks weird...but does it work?)
  setFlagsVCX(val,nval,0,size);
}


void Em68k::dis_neg(ULONG loc, unsigned int mode, unsigned int reg, unsigned int size)
{
  DEBUG_OUT<<Em68k_name[(opcode>>8)&0xff];
  print_size(size);
  print_ea_op_len(loc,mode,reg,size);
}


void Em68k::exec_not(ULONG loc, unsigned int mode, unsigned int reg, unsigned int size)
{
  if(size==ECPUABSTRACT_SIZE_OTHER) {
    // It wasn't really a NOT, but was a MOVE <ea>,SR instead
    unsigned int val=read_ea_op_len(loc,mode,reg,ECPUABSTRACT_SIZE_WORD);
    unsigned int mask=0xf71f;
    switch(type) {
      case E_CPU_68000:  case E_CPU_68010:  mask=0xa71f; break;
      default: mask=0xf71f; break;
    }
    setSR(val&mask);
  }
  else {
    unsigned int val=read_ea_op_len(loc,mode,reg,size);
    unsigned int nval=~val;
    write_ea(loc,mode,reg,nval,size);
    setFlagsNZvc(nval,size);
  }
}


void Em68k::dis_not(ULONG loc, unsigned int mode, unsigned int reg, unsigned int size)
{
  if(size==ECPUABSTRACT_SIZE_OTHER) {
    // It wasn't really a NOT, but was a MOVE <ea>,SR instead
    DEBUG_OUT<<"move ";
    print_ea_op_len(loc,mode,reg,ECPUABSTRACT_SIZE_WORD);
    DEBUG_OUT<<",sr";
  }
  else {
    DEBUG_OUT<<Em68k_name[(opcode>>8)&0xff];
    print_size(size);
    print_ea_op_len(loc,mode,reg,size);
  }
}


void Em68k::exec_tst(ULONG loc, unsigned int mode, unsigned int reg, unsigned int size)
{
  unsigned int val=read_ea_op_len(loc,mode,reg,size);
  unsigned int nval=0-val;  // order right?
  // results are thrown out...
  setFlagsNZvc(nval,size);
}


void Em68k::dis_tst(ULONG loc, unsigned int mode, unsigned int reg, unsigned int size)
{
  DEBUG_OUT<<Em68k_name[(opcode>>8)&0xff];
  print_size(size);
  print_ea_op_len(loc,mode,reg,size);
}


void Em68k::exec_ext(ULONG loc, unsigned int opcode)
{
  unsigned int mode=(opcode>>6)&0x7;
  unsigned int reg=opcode&0x07;
  unsigned int size=0;
  unsigned int sign=0;
  unsigned int mask=0;
  switch(mode) {
    case 2:
	    // byte to word
	    size=ECPUABSTRACT_SIZE_WORD;
	    sign=0x0000ff00;
	    mask=0xffff00ff;
	    break;
    case 3:
	    // word to long
	    size=ECPUABSTRACT_SIZE_LONG;
	    sign=0xffff0000;
	    mask=ECPUABSTRACT_MASK_WORD;
	    break;
    case 7:
	    // byte to long
	    size=ECPUABSTRACT_SIZE_LONG;
	    sign=0xffffff00;
	    mask=ECPUABSTRACT_MASK_BYTE;
	    break;
    default:
      debugger("Illegal mode for EXT!\n");
			return;
      break;
  }
  unsigned int val=read_reg(reg,size);
  if(!(val&ECPUABSTRACT_TOP_BYTE)) sign=0;
  unsigned int nval=(val&mask)|sign;
  write_reg(reg,nval,size);
  setFlagsNZvc(nval,size);
}


void Em68k::dis_ext(ULONG loc, unsigned int opcode)
{
  DEBUG_OUT<<"ext";
  unsigned int mode=(opcode>>6)&0x7;
  unsigned int reg=opcode&0x07;
  switch(mode) {
    case 2:  DEBUG_OUT<<".w"; break;
    case 3:  DEBUG_OUT<<".l"; break;
    case 7:  DEBUG_OUT<<"b.l"; break;
    default:
      DEBUG_OUT<<"Illegal mode for EXT!\n";
			return;
      break;
  }
  DEBUG_OUT<<" ";
  print_reg(reg);
}




void Em68k::exec_exg(ULONG loc, unsigned int opcode)
{
  unsigned int mode=(opcode>>3)&0x1f;
  unsigned int reg1=(opcode>>9)&0x07;
  unsigned int reg2=opcode&0x07;
  switch(mode) {
    case 8: break;  // data registers
    case 9: reg1+=8;  reg2+=8; break;  // address registers
    case 11: reg2+=8; break;  // a data and an address register
    default:
      exec_unimplemented("Illegal mode for EXG");
      break;
  }
  unsigned int val=read_reg(reg1,ECPUABSTRACT_SIZE_LONG);
  write_reg(reg1,read_reg(reg2,ECPUABSTRACT_SIZE_LONG),ECPUABSTRACT_SIZE_LONG);
  write_reg(reg2,val,ECPUABSTRACT_SIZE_LONG);
}


void Em68k::dis_exg(ULONG loc, unsigned int opcode)
{
  DEBUG_OUT<<Em68k_name[(opcode>>8)&0xff];
  unsigned int mode=(opcode>>3)&0x1f;
  unsigned int reg1=(opcode>>9)&0x07;
  unsigned int reg2=opcode&0x07;
  switch(mode) {
    case 8: break;  // data registers
    case 9: reg1+=8;  reg2+=8; break;  // address registers
    case 11: reg2+=8; break;  // a data and an address register
    default:
      exec_unimplemented("Illegal mode for EXG");
      break;
  }
  print_reg(reg1);
  DEBUG_OUT<<",";
  print_reg(reg2);
}


void Em68k::exec_scc(ULONG loc, unsigned int opcode, unsigned int mode)
{
  unsigned int reg=opcode&0x07;
  if(mode==0x07) { exec_trapcc(loc,opcode,reg); return; }
  bool willSet=false;
  unsigned int cond=(opcode>>8)&0x0f;
  bool srC=sr&E68k_STAT_C,srZ=sr&E68k_STAT_Z;
  bool srV=sr&E68k_STAT_V,srN=sr&E68k_STAT_N;
  switch(cond) {
    case 0: willSet=false; break;  // T
    case 1: willSet=true; break;  //F
    case 2: if((!srC)&&(!srZ)) willSet=true; break;  // HI
    case 3: if(srC||srZ) willSet=true;  break;  // LS
    case 4: if(!srC) willSet=true;  break;  // CC
    case 5: if(srC) willSet=true;  break;  // CS
    case 6: if(!srZ) willSet=true;  break;  // NE
    case 7: if(srZ) willSet=true;  break;  // EQ
    case 8: if(!srV) willSet=true;  break;  // VC
    case 9: if(srV) willSet=true;  break;  // VS
    case 10: if(!srN) willSet=true;  break;  // PL
    case 11: if(srN) willSet=true;  break;  // MI
    case 12: if(((!srN)&&(!srV))||(srN&&srV)) willSet=true;
      break;  // GE
    case 13: if(((!srN)&&srV)||(srN&&(!srV))) willSet=true;
      break;  // LT
    case 14: if((srN&&srV&&(!srZ))||((!srN)&&(!srV)&&(!srZ))) willSet=true;
      break;  // GT
    case 15: if(((!srN)&&srV)||(srN&&(!srV))||(srZ)) willSet=true;
      break;  // LE
    default: debugger("Scc illegal cond!\n"); break;
  }
  unsigned int val=0;
  if(willSet) val=1;
  write_ea_op_len(loc,mode,reg,val,ECPUABSTRACT_SIZE_BYTE);
}


void Em68k::dis_scc(ULONG loc, unsigned int opcode, unsigned int mode)
{
  unsigned int reg=opcode&0x07;
  if(mode==0x07) { dis_trapcc(loc,opcode,reg); return; }
  unsigned int cond=(opcode>>8)&0x0f;
  DEBUG_OUT<<"S";
  switch(cond) {
    case 0: DEBUG_OUT<<"T";  break;
    case 1: DEBUG_OUT<<"F";  break;
    case 2: DEBUG_OUT<<"HI";  break;
    case 3: DEBUG_OUT<<"LS";  break;
    case 4: DEBUG_OUT<<"CC";  break;
    case 5: DEBUG_OUT<<"CS";  break;
    case 6: DEBUG_OUT<<"NE";  break;
    case 7: DEBUG_OUT<<"EQ";  break;
    case 8: DEBUG_OUT<<"VC";  break;
    case 9: DEBUG_OUT<<"VS";  break;
    case 10: DEBUG_OUT<<"PL";  break;
    case 11: DEBUG_OUT<<"MI";  break;
    case 12: DEBUG_OUT<<"GE";  break;
    case 13: DEBUG_OUT<<"LT";  break;
    case 14: DEBUG_OUT<<"GT";  break;
    case 15: DEBUG_OUT<<"LE";  break;
    default: DEBUG_OUT<<"Scc illegal cond!\n"; break;
  }
  DEBUG_OUT<<" ";
  print_ea_op_len(loc,mode,reg,ECPUABSTRACT_SIZE_BYTE);
}


// TODO: reg is really opmode
void Em68k::exec_trapcc(ULONG loc, unsigned int opcode, unsigned int reg)
{
  exec_unimplemented("TRAPcc instruction");
}


// TODO: reg is really opmode
void Em68k::dis_trapcc(ULONG loc, unsigned int opcode, unsigned int reg)
{
  exec_unimplemented("TRAPcc");
}


// TODO: I wonder why I didn't use getImm_op_len here...
void Em68k::exec_bcc(ULONG loc, unsigned int opcode)
{
  bool willBranch=false;
  unsigned int cond=(opcode>>8)&0x0f;
  bool srC=sr&E68k_STAT_C,srZ=sr&E68k_STAT_Z;
  bool srV=sr&E68k_STAT_V,srN=sr&E68k_STAT_N;
#ifdef DEBUG_EXEC
  DEBUG_OUT<<"Bcc: ";  //print_sr();
  DEBUG_OUT<<"\n";
#endif
  switch(cond) {
    case 0: willBranch=true; break;  //T, aka BRA
    case 1: exec_bsr(loc,opcode); return; break;  // F, really BSR
    case 2: if((!srC)&&(!srZ)) willBranch=true; break;  // HI
    case 3: if(srC||srZ) willBranch=true;  break;  // LS
    case 4: if(!srC) willBranch=true;  break;  // CC
    case 5: if(srC) willBranch=true;  break;  // CS
    case 6: if(!srZ) willBranch=true;  break;  // NE
    case 7: if(srZ) willBranch=true;  break;  // EQ
    case 8: if(!srV) willBranch=true;  break;  // VC
    case 9: if(srV) willBranch=true;  break;  // VS
    case 10: if(!srN) willBranch=true;  break;  // PL
    case 11: if(srN) willBranch=true;  break;  // MI
    case 12: if(((!srN)&&(!srV))||(srN&&srV)) willBranch=true;
      break;  // GE
    case 13: if(((!srN)&&srV)||(srN&&(!srV))) willBranch=true;
      break;  // LT
    case 14: if((srN&&srV&&(!srZ))||((!srN)&&(!srV)&&(!srZ))) willBranch=true;
      break;  // GT
    case 15: if(((!srN)&&srV)||(srN&&(!srV))||(srZ)) willBranch=true;
      break;  // LE
    default: debugger("Bcc illegal cond!\n"); break;
  }
  int disp8=opcode&0xff;
  int disp=sign_extend68k(disp8,ECPUABSTRACT_SIZE_BYTE);
  if(!disp8) {
    disp=sign_extend68k(map->read16b(loc+2),ECPUABSTRACT_SIZE_WORD);  op_len+=2;
  }
  if(disp8==0xff) { disp=map->read32b(loc+2);  op_len+=4; }
  if(willBranch) {
    int addr=(int)loc+disp+2;
    jump(addr);
  }
}


// TODO: I wonder why I didn't use getImm_op_len here...
void Em68k::dis_bcc(ULONG loc, unsigned int opcode)
{
  unsigned int cond=(opcode>>8)&0x0f;
  switch(cond) {
    case 0: DEBUG_OUT<<"bra";  break;
    case 1: DEBUG_OUT<<"bsr";  break;  // F, really BSR
    case 2: DEBUG_OUT<<"bhi";  break;
    case 3: DEBUG_OUT<<"bls";  break;
    case 4: DEBUG_OUT<<"bcc";  break;
    case 5: DEBUG_OUT<<"bcs";  break;
    case 6: DEBUG_OUT<<"bne";  break;
    case 7: DEBUG_OUT<<"beq";  break;
    case 8: DEBUG_OUT<<"bvc";  break;
    case 9: DEBUG_OUT<<"bvs";  break;
    case 10: DEBUG_OUT<<"bpl";  break;
    case 11: DEBUG_OUT<<"bmi";  break;
    case 12: DEBUG_OUT<<"bge";  break;
    case 13: DEBUG_OUT<<"blt";  break;
    case 14: DEBUG_OUT<<"bgt";  break;
    case 15: DEBUG_OUT<<"ble";  break;
    default: DEBUG_OUT<<"Bcc illegal cond!\n"; break;
  }
  DEBUG_OUT<<" ";
  int disp8=opcode&0xff;
  int disp=sign_extend68k(disp8,ECPUABSTRACT_SIZE_BYTE);
  if(!disp8) {
    disp=sign_extend68k(map->read16b(loc+2),ECPUABSTRACT_SIZE_WORD);  op_len+=2;
  }
  if(disp8==0xff) { disp=map->read32b(loc+2);  op_len+=4; }
  unsigned int actual=disp+loc;
  EMapSymbol *e=map->findAnyExactSymbol(actual+op_len);
  if(e) DEBUG_OUT<<e->getName();
  else DEBUG_OUT<<"$"<<disp<<"(pc)  ;"<<actual+op_len;
}


// TODO: This will miss DIV and SBCD
void Em68k::exec_or(ULONG loc, unsigned int reg2, unsigned int mode2, unsigned int mode, unsigned int reg)
{
#ifdef CHECK_SAFE
  if(reg>7) {
    debugger("illegal register for OR!\n");
    return ;
  }
#endif
  unsigned int dir=decode_mode_dir(reg2,mode2);
  unsigned int size=decode_mode_size(reg2,mode2);
  reg2=decode_mode_reg(reg2,mode2);
  unsigned int val=read_ea_op_len(loc,mode,reg,size);
  unsigned int nval=read_reg(reg2,size)|val;  // Order isn't important
  if(dir) write_reg(reg2,nval,size);
  else write_ea(loc,mode,reg,nval,size);
  setFlagsNZvc(nval,size);
}


// TODO: This will miss DIV and SBCD
void Em68k::dis_or(ULONG loc, unsigned int reg2, unsigned int mode2, unsigned int mode, unsigned int reg)
{
  DEBUG_OUT<<Em68k_name[(opcode>>8)&0xff];
  unsigned int dir=decode_mode_dir(reg2,mode2);
  unsigned int size=decode_mode_size(reg2,mode2);
  reg2=decode_mode_reg(reg2,mode2);
  print_size(size);
  if(dir) print_ea_op_len(loc,mode,reg,size);
  else print_reg(reg2);
  DEBUG_OUT<<",";
  if(dir) print_reg(reg2);
  else print_ea_op_len(loc,mode,reg,size);
}


void Em68k::exec_ori(ULONG loc, unsigned int opcode, unsigned int mode, unsigned int reg, unsigned int size)
{
  if((opcode&0xff)==0x3c) {
    // It was really an ORI to the CCR
    unsigned int tsr=sr&0xffff0000;
    sr=((getImm_op_len(loc,size)&0xffff)|sr)&0xffff;
    sr|=tsr;
    return;
  }
  if((opcode&0xff)==0x7c) {
    // It was really an ORI to the SR
    if(sr&E68k_STAT_S) {
      sr=getImm_op_len(loc,size)|sr;
    }
    else exec_unimplemented("ORI to SR in user mode!");
    return;
  }
  unsigned int val=read_ea_op_len(loc,mode,reg,size);
  unsigned int nval=getImm_op_len(loc,size)|val;  // Order isn't important
  write_ea(loc,mode,reg,nval,size);
  setFlagsNZvc(nval,size);
}


void Em68k::dis_ori(ULONG loc, unsigned int opcode, unsigned int mode, unsigned int reg, unsigned int size)
{
  DEBUG_OUT<<Em68k_name[(opcode>>8)&0xff]<<" #$"<<getImm_op_len(loc,size)<<",";
  if((opcode&0xff)==0x3c) {
    // It was really an ORI to the CCR
    DEBUG_OUT<<"CCR"; return;
  }
  if((opcode&0xff)==0x7c) {
    // It was really an ORI to the SR
    DEBUG_OUT<<"SR"; return;
  }
  print_ea_op_len(loc,mode,reg,size);
}


void Em68k::exec_eor(ULONG loc, unsigned int reg2, unsigned int mode2, unsigned int mode, unsigned int reg)
{
#ifdef CHECK_SAFE
  if(reg>7) {
    debugger("illegal register for EOR!\n");
    return ;
  }
#endif
  unsigned int dir=decode_mode_dir(reg2,mode2);
  unsigned int size=decode_mode_size(reg2,mode2);
  reg2=decode_mode_reg(reg2,mode2);
  unsigned int val=read_ea_op_len(loc,mode,reg,size);
  unsigned int nval=read_reg(reg2,size)^val;  // Order isn't important
  // NOTE: all dir is for is to specify where the results go
  if(dir) write_reg(reg2,nval,size);
  else write_ea(loc,mode,reg,nval,size);
  setFlagsNZvc(nval,size);
}


void Em68k::dis_eor(ULONG loc, unsigned int reg2, unsigned int mode2, unsigned int mode, unsigned int reg)
{
  DEBUG_OUT<<Em68k_name[(opcode>>8)&0xff];
  unsigned int dir=decode_mode_dir(reg2,mode2);
  unsigned int size=decode_mode_size(reg2,mode2);
  reg2=decode_mode_reg(reg2,mode2);
  print_size(size);
  if(dir) print_ea_op_len(loc,mode,reg,size);
  else print_reg(reg2);
  DEBUG_OUT<<",";
  if(dir) print_reg(reg2);
  else print_ea_op_len(loc,mode,reg,size);
}


void Em68k::exec_eori(ULONG loc, unsigned int opcode, unsigned int mode, unsigned int reg, unsigned int size)
{
  if((opcode&0xff)==0x3c) {
    unsigned int tsr=sr&0xffff0000;
    sr=((getImm_op_len(loc,size)&0xffff)^sr)&0xffff;
    sr|=tsr;
    return;
  }
  if((opcode&0xff)==0x7c) {
    // It was really an EORI to the SR
    if(sr&E68k_STAT_S) {
      sr=getImm_op_len(loc,size)^sr;
    }
    else exec_unimplemented("EORI to SR in user mode!");
    return;
  }
  unsigned int nval=getImm_op_len(loc,size)^val;  // Order isn't important
  unsigned int val=read_ea_op_len(loc,mode,reg,size);
  write_ea(loc,mode,reg,nval,size);
  setFlagsNZvc(nval,size);
}


void Em68k::dis_eori(ULONG loc, unsigned int opcode, unsigned int mode, unsigned int reg, unsigned int size)
{
  DEBUG_OUT<<Em68k_name[(opcode>>8)&0xff]<<" #$"<<getImm_op_len(loc,size)<<",";
  if((opcode&0xff)==0x3c) {
    // It was really an EORI to the CCR
    DEBUG_OUT<<"CCR"; return;
  }
  if((opcode&0xff)==0x7c) {
    // It was really an EORI to the SR
    DEBUG_OUT<<"SR"; return;
  }
  print_ea_op_len(loc,mode,reg,size);
}


void Em68k::exec_and(ULONG loc, unsigned int reg2, unsigned int mode2, unsigned int mode, unsigned int reg)
{
#ifdef CHECK_SAFE
  if(reg>7) {
    debugger("illegal register for AND!\n");
    return ;
  }
#endif
  unsigned int dir=decode_mode_dir(reg2,mode2);
  unsigned int size=decode_mode_size(reg2,mode2);
  reg2=decode_mode_reg(reg2,mode2);
  unsigned int val=read_ea_op_len(loc,mode,reg,size);
  unsigned int nval=read_reg(reg2,size)&val;  // Order isn't important
  // NOTE: all dir is for is to specify where the results go
  if(dir) write_reg(reg2,nval,size);
  else write_ea(loc,mode,reg,nval,size);
  setFlagsNZvc(nval,size);
}


void Em68k::dis_and(ULONG loc, unsigned int reg2, unsigned int mode2, unsigned int mode, unsigned int reg)
{
  DEBUG_OUT<<Em68k_name[(opcode>>8)&0xff];
  unsigned int dir=decode_mode_dir(reg2,mode2);
  unsigned int size=decode_mode_size(reg2,mode2);
  reg2=decode_mode_reg(reg2,mode2);
  print_size(size);
  if(dir) print_ea_op_len(loc,mode,reg,size);
  else print_reg(reg2);
  DEBUG_OUT<<",";
  if(dir) print_reg(reg2);
  else print_ea_op_len(loc,mode,reg,size);
}


void Em68k::exec_andi(ULONG loc, unsigned int opcode, unsigned int mode, unsigned int reg, unsigned int size)
{
  if((opcode&0xff)==0x3c) {
    // It was really an ANDI to the CCR
    unsigned int tsr=sr&0xffff0000;
    sr=((getImm_op_len(loc,size)&0xffff)&sr)&0xffff;
    sr|=tsr;
    return;
  }
  if((opcode&0xff)==0x7c) {
    // It was really an ANDI to the SR
    if(sr&E68k_STAT_S) {
      sr=getImm_op_len(loc,size)&sr;
    }
    else exec_unimplemented("ANDI to SR in user mode!");
    return;
  }
  unsigned int val=read_ea_op_len(loc,mode,reg,size);
  unsigned int nval=getImm_op_len(loc,size)&val;  // Order isn't important
  write_ea(loc,mode,reg,nval,size);
  setFlagsNZvc(nval,size);
}


void Em68k::dis_andi(ULONG loc, unsigned int opcode, unsigned int mode, unsigned int reg, unsigned int size)
{
  DEBUG_OUT<<Em68k_name[(opcode>>8)&0xff]<<" #$"<<getImm_op_len(loc,size)<<",";
  if((opcode&0xff)==0x3c) {
    // It was really an ANDI to the CCR
    DEBUG_OUT<<"CCR"; return;
  }
  if((opcode&0xff)==0x7c) {
    // It was really an ANDI to the SR
    DEBUG_OUT<<"SR"; return;
  }
  print_ea_op_len(loc,mode,reg,size);
}



// TODO: This doesn't catch SUBX
// TODO: This doesn't catch SUBA
void Em68k::exec_sub(ULONG loc, unsigned int reg2, unsigned int mode2, unsigned int mode, unsigned int reg)
{
  unsigned int dir=decode_mode_dir(reg2,mode2);
  unsigned int size=decode_mode_size(reg2,mode2);
  reg2=decode_mode_reg(reg2,mode2);
  unsigned int rreg=read_reg(reg2,size);
  unsigned int val=read_ea_op_len(loc,mode,reg,size);
  unsigned int nval=0;
  // NOTE: order is VERY important
  if(dir) {
    nval=rreg-val;
    write_reg(reg2,nval,size);
  }
  else {
    nval=val-rreg;
    write_ea(loc,mode,reg,nval,size);
  }
  setFlagsNZ(nval,size);
  // TODO: Check, is this right?  (I know it looks weird...but does it work?)
  setFlagsVC(rreg,nval,val,size);  // Notice how rreg and val can reverse
}


// TODO: This doesn't catch SUBX
// TODO: This doesn't catch SUBA
void Em68k::dis_sub(ULONG loc, unsigned int reg2, unsigned int mode2, unsigned int mode, unsigned int reg)
{
  DEBUG_OUT<<Em68k_name[(opcode>>8)&0xff];
  unsigned int dir=decode_mode_dir(reg2,mode2);
  unsigned int size=decode_mode_size(reg2,mode2);
  reg2=decode_mode_reg(reg2,mode2);
  print_size(size);
  if(dir) {
    print_ea_op_len(loc,mode,reg,size);
    DEBUG_OUT<<",";
    print_reg(reg2);
  }
  else {
    print_reg(reg2);
    DEBUG_OUT<<",";
    print_ea_op_len(loc,mode,reg,size);
  }
}


// TODO: This will miss MUL, ABCD, EXG and ADDX
// TODO: This misses ADDA which should be handled differently
void Em68k::exec_add(ULONG loc, unsigned int reg2, unsigned int mode2, unsigned int mode, unsigned int reg)
{
  unsigned int dir=decode_mode_dir(reg2,mode2);
  unsigned int size=decode_mode_size(reg2,mode2);
  reg2=decode_mode_reg(reg2,mode2);
  unsigned int rreg=read_reg(reg2,size);
  unsigned int val=read_ea_op_len(loc,mode,reg,size);
  unsigned int nval=rreg+val;  // Order isn't important
  // NOTE: all dir is for is to specify where the results go
  if(dir) write_reg(reg2,nval,size);
  else write_ea(loc,mode,reg,nval,size);
  setFlagsNZ(nval,size);
  setFlagsVC(val,rreg,nval,size);
}


// TODO: This will miss MUL, ABCD, EXG and ADDX
// TODO: This misses ADDA which should be handled differently
void Em68k::dis_add(ULONG loc, unsigned int reg2, unsigned int mode2, unsigned int mode, unsigned int reg)
{
  DEBUG_OUT<<Em68k_name[(opcode>>8)&0xff];
  unsigned int dir=decode_mode_dir(reg2,mode2);
  unsigned int size=decode_mode_size(reg2,mode2);
  reg2=decode_mode_reg(reg2,mode2);
  print_size(size);
  if(dir) {
    print_ea_op_len(loc,mode,reg,size);
    DEBUG_OUT<<",";
    print_reg(reg2);
  }
  else {
    print_reg(reg2);
    DEBUG_OUT<<",";
    print_ea_op_len(loc,mode,reg,size);
  }
}


// TODO: This misses EOR encoded with a different mode2 field
// TODO: This misses CMPA which should be handled differently
void Em68k::exec_cmp(ULONG loc, unsigned int reg2, unsigned int mode2, unsigned int mode, unsigned int reg)
{
  unsigned int dir=decode_mode_dir(reg2,mode2);
  unsigned int size=decode_mode_size(reg2,mode2);
  reg2=decode_mode_reg(reg2,mode2);
  unsigned int val=read_ea_op_len(loc,mode,reg,size);
  unsigned int nval=0;
  unsigned int rreg=read_reg(reg2,size);
  // NOTE: order is very important...CMP is read as subtraction
  if(dir) nval=rreg-val; else nval=val-rreg;
  // results are thrown out...
  setFlagsNZ(nval,size);
  // TODO: Check, is this right?  (I know it looks weird...but does it work?)
  setFlagsVC(rreg,nval,val,size);
#ifdef DEBUG_EXEC
  DEBUG_OUT<<"CMP dir="<<dir<<" nval="<<nval<<"\n";
  DEBUG_OUT<<"val was "<<val<<" rreg was "<<rreg<<"\n";
#endif
}


// TODO: This misses EOR encoded with a different mode2 field
// TODO: This misses CMPA which should be handled differently
void Em68k::dis_cmp(ULONG loc, unsigned int reg2, unsigned int mode2, unsigned int mode, unsigned int reg)
{
  DEBUG_OUT<<Em68k_name[(opcode>>8)&0xff];
  unsigned int dir=decode_mode_dir(reg2,mode2);
  unsigned int size=decode_mode_size(reg2,mode2);
  reg2=decode_mode_reg(reg2,mode2);
  print_size(size);
  if(dir) {
    print_ea_op_len(loc,mode,reg,size);
    DEBUG_OUT<<",";
    print_reg(reg2);
  }
  else {
    print_reg(reg2);
    DEBUG_OUT<<",";
    print_ea_op_len(loc,mode,reg,size);
  }
}


void Em68k::exec_subi(ULONG loc, unsigned int opcode, unsigned int mode, unsigned int reg, unsigned int size)
{
  unsigned int val=read_ea_op_len(loc,mode,reg,size);
  unsigned int imm=getImm_op_len(loc,size);
  unsigned int nval=val-imm;
  write_ea(loc,mode,reg,nval,size);
  setFlagsNZ(nval,size);
  // TODO: Check, is this right?  (I know it looks weird...but does it work?)
  setFlagsVC(val,nval,imm,size);
}


void Em68k::dis_subi(ULONG loc, unsigned int opcode, unsigned int mode, unsigned int reg, unsigned int size)
{
  DEBUG_OUT<<"subi";
  print_size(size);
  DEBUG_OUT<<"#$"<<getImm_op_len(loc,size)<<",";
  print_ea_op_len(loc,mode,reg,size);
}


void Em68k::exec_addi(ULONG loc, unsigned int opcode, unsigned int mode, unsigned int reg, unsigned int size)
{
  unsigned int val=read_ea_op_len(loc,mode,reg,size);
  unsigned int imm=getImm_op_len(loc,size);
  unsigned int nval=imm+val;  // Order isn't important
  write_ea(loc,mode,reg,nval,size);
  setFlagsNZ(nval,size);
  setFlagsVC(val,imm,nval,size);
}


void Em68k::dis_addi(ULONG loc, unsigned int opcode, unsigned int mode, unsigned int reg, unsigned int size)
{
  DEBUG_OUT<<"addi";
  print_size(size);
  DEBUG_OUT<<"#$"<<getImm_op_len(loc,size)<<",";
  print_ea_op_len(loc,mode,reg,size);
}


void Em68k::exec_cmpi(ULONG loc, unsigned int opcode, unsigned int mode, unsigned int reg, unsigned int size)
{
  unsigned int val=read_ea_op_len(loc,mode,reg,size);
  unsigned int imm=getImm_op_len(loc,size);
  unsigned int nval=val-imm;
  // results are thrown out...
  setFlagsNZ(nval,size);
  // TODO: Check, is this right?  (I know it looks weird...but does it work?)
  setFlagsVC(val,nval,imm,size);
}


void Em68k::dis_cmpi(ULONG loc, unsigned int opcode, unsigned int mode, unsigned int reg, unsigned int size)
{
  DEBUG_OUT<<"cmpi";
  print_size(size);
  DEBUG_OUT<<"#$"<<getImm_op_len(loc,size)<<",";
  print_ea_op_len(loc,mode,reg,size);
}


// TODO: reg2 should actually be called "data"
void Em68k::exec_addq(ULONG loc, unsigned int mode, unsigned int reg, unsigned int reg2, unsigned int size)
{
  if(size==ECPUABSTRACT_SIZE_OTHER) { exec_dbcc(loc,opcode); return; }
  if(!reg2) reg2=8;  // NOTE: 0 really means 8...
  unsigned int val=read_ea_op_len(loc,mode,reg,size);
  unsigned int nval=val+reg2;  // Order isn't important
  write_ea(loc,mode,reg,nval,size);
  setFlagsNZ(nval,size);
  setFlagsVCX(val,reg2,nval,size);  // Order isn't important
}


// TODO: reg2 should actually be called "data"
void Em68k::dis_addq(ULONG loc, unsigned int mode, unsigned int reg, unsigned int reg2, unsigned int size)
{
  if(size==ECPUABSTRACT_SIZE_OTHER) { dis_dbcc(loc,opcode); return; }
  DEBUG_OUT<<Em68k_name[(opcode>>8)&0xff];
  print_size(size);
  if(!reg2) reg2=8;  // NOTE: 0 really means 8...
  DEBUG_OUT<<"#$"<<reg2<<",";
  print_ea_op_len(loc,mode,reg,size);
}


// TODO: reg2 should actually be called "data"
void Em68k::exec_subq(ULONG loc, unsigned int mode, unsigned int reg, unsigned int reg2, unsigned int size)
{
  if(size==ECPUABSTRACT_SIZE_OTHER) { exec_dbcc(loc,opcode); return; }
  if(!reg2) reg2=8;  // NOTE: 0 really means 8...
  unsigned int val=read_ea_op_len(loc,mode,reg,size);
  unsigned int nval=val-reg2;
  write_ea(loc,mode,reg,nval,size);
  setFlagsNZ(nval,size);
  // NOTE: see note in exec_SUB(), notice that val here isn't the same as there
  setFlagsVCX(val,nval,reg2,size);
}


// TODO: reg2 should actually be called "data"
void Em68k::dis_subq(ULONG loc, unsigned int mode, unsigned int reg, unsigned int reg2, unsigned int size)
{
  if(size==ECPUABSTRACT_SIZE_OTHER) { dis_dbcc(loc,opcode); return; }
  DEBUG_OUT<<Em68k_name[(opcode>>8)&0xff];
  print_size(size);
  if(!reg2) reg2=8;  // NOTE: 0 really means 8...
  DEBUG_OUT<<"#$"<<reg2<<",";
  print_ea_op_len(loc,mode,reg,size);
}


// TODO: Test this sucker! (Does it even decode correctly?)
void Em68k::exec_lsrm(ULONG loc, unsigned int opcode)
{
  unsigned int mode=(opcode>>3)&0x07;
  unsigned int reg=opcode&0x07;
  unsigned int nval=read_ea_op_len(loc,mode,reg,ECPUABSTRACT_SIZE_WORD);
  nval=do_lsr_once(nval,ECPUABSTRACT_SIZE_WORD);
  write_ea(loc,mode,reg,nval,ECPUABSTRACT_SIZE_WORD);
}


// TODO: Test this sucker! (Does it even decode correctly?)
void Em68k::exec_lslm(ULONG loc, unsigned int  opcode)
{
  unsigned int mode=(opcode>>3)&0x07;
  unsigned int reg=opcode&0x07;
  unsigned int nval=read_ea_op_len(loc,mode,reg,ECPUABSTRACT_SIZE_WORD);
  nval=do_lsl_once(nval,ECPUABSTRACT_SIZE_WORD);
  write_ea(loc,mode,reg,nval,ECPUABSTRACT_SIZE_WORD);
}


// TODO: Test this sucker! (Does it even decode correctly?)
void Em68k::exec_asrm(ULONG loc, unsigned int  opcode)
{
  unsigned int mode=(opcode>>3)&0x07;
  unsigned int reg=opcode&0x07;
  unsigned int nval=read_ea_op_len(loc,mode,reg,ECPUABSTRACT_SIZE_WORD);
  nval=do_asr_once(nval,ECPUABSTRACT_SIZE_WORD);
  write_ea(loc,mode,reg,nval,ECPUABSTRACT_SIZE_WORD);
}


// TODO: Test this sucker! (Does it even decode correctly?)
void Em68k::exec_aslm(ULONG loc, unsigned int  opcode)
{
  unsigned int mode=(opcode>>3)&0x07;
  unsigned int reg=opcode&0x07;
  unsigned int nval=read_ea_op_len(loc,mode,reg,ECPUABSTRACT_SIZE_WORD);
  nval=do_asl_once(nval,ECPUABSTRACT_SIZE_WORD);
  write_ea(loc,mode,reg,nval,ECPUABSTRACT_SIZE_WORD);
}


// TODO: Test this sucker! (Does it even decode correctly?)
void Em68k::exec_roxrm(ULONG loc, unsigned int opcode)
{
  unsigned int mode=(opcode>>3)&0x07;
  unsigned int reg=opcode&0x07;
  unsigned int nval=read_ea_op_len(loc,mode,reg,ECPUABSTRACT_SIZE_WORD);
  nval=do_roxr_once(nval,ECPUABSTRACT_SIZE_WORD);
  write_ea(loc,mode,reg,nval,ECPUABSTRACT_SIZE_WORD);
}


// TODO: Test this sucker! (Does it even decode correctly?)
// TODO: Test this looping "once" stuff..
void Em68k::exec_roxlm(ULONG loc, unsigned int  opcode)
{
  unsigned int mode=(opcode>>3)&0x07;
  unsigned int reg=opcode&0x07;
  unsigned int nval=read_ea_op_len(loc,mode,reg,ECPUABSTRACT_SIZE_WORD);
  nval=do_roxl_once(nval,ECPUABSTRACT_SIZE_WORD);
  write_ea(loc,mode,reg,nval,ECPUABSTRACT_SIZE_WORD);
}


// TODO: Test this sucker! (Does it even decode correctly?)
void Em68k::exec_rorm(ULONG loc, unsigned int opcode)
{
  unsigned int mode=(opcode>>3)&0x07;
  unsigned int reg=opcode&0x07;
  unsigned int nval=read_ea_op_len(loc,mode,reg,ECPUABSTRACT_SIZE_WORD);
  nval=do_ror_once(nval,ECPUABSTRACT_SIZE_WORD);
  write_ea(loc,mode,reg,nval,ECPUABSTRACT_SIZE_WORD);
}


// TODO: Test this sucker! (Does it even decode correctly?)
void Em68k::exec_rolm(ULONG loc, unsigned int  opcode)
{
  unsigned int mode=(opcode>>3)&0x07;
  unsigned int reg=opcode&0x07;
  unsigned int nval=read_ea_op_len(loc,mode,reg,ECPUABSTRACT_SIZE_WORD);
  nval=do_rol_once(nval,ECPUABSTRACT_SIZE_WORD);
  write_ea(loc,mode,reg,nval,ECPUABSTRACT_SIZE_WORD);
}


// TODO: Test this sucker! (Does it even decode correctly?)
void Em68k::dis_xxrm(ULONG loc, unsigned int  opcode)
{
  DEBUG_OUT<<"R";
  unsigned int mode=(opcode>>3)&0x07;
  unsigned int reg=opcode&0x07;
  print_size(ECPUABSTRACT_SIZE_WORD);
  print_ea_op_len(loc,mode,reg,ECPUABSTRACT_SIZE_WORD);
}


// TODO: Test this sucker! (Does it even decode correctly?)
void Em68k::dis_xxlm(ULONG loc, unsigned int  opcode)
{
  DEBUG_OUT<<"L";
  unsigned int mode=(opcode>>3)&0x07;
  unsigned int reg=opcode&0x07;
  print_size(ECPUABSTRACT_SIZE_WORD);
  print_ea_op_len(loc,mode,reg,ECPUABSTRACT_SIZE_WORD);
}


// TODO: Test this looping "once" stuff...
void Em68k::exec_lsrr(ULONG loc, unsigned int opcode, unsigned int count, unsigned int size)
{
  unsigned int ir=(opcode>>5)&0x01;
  unsigned int reg=opcode&0x07;
  if(ir) count=read_reg(count,ECPUABSTRACT_SIZE_BYTE)&0x3f;
  unsigned int nval=read_reg(reg,size);
  for(unsigned int t=0;t<count;t++)
    nval=do_lsr_once(nval,size);
  write_reg(reg,nval,size);
}


// TODO: Test this looping "once" stuff...
void Em68k::exec_lslr(ULONG loc, unsigned int  opcode, unsigned int count, unsigned int size)
{
  unsigned int ir=(opcode>>5)&0x01;
  unsigned int reg=opcode&0x07;
  if(ir) count=read_reg(count,ECPUABSTRACT_SIZE_BYTE)&0x3f;
  unsigned int nval=read_reg(reg,size);
  for(unsigned int t=0;t<count;t++)
    nval=do_lsl_once(nval,size);
  write_reg(reg,nval,size);
}


// TODO: Test this looping "once" stuff...
void Em68k::exec_asrr(ULONG loc, unsigned int  opcode, unsigned int count, unsigned int size)
{
  unsigned int ir=(opcode>>5)&0x01;
  unsigned int reg=opcode&0x07;
  if(ir) count=read_reg(count,ECPUABSTRACT_SIZE_BYTE)&0x3f;
  unsigned int nval=read_reg(reg,size);
  for(unsigned int t=0;t<count;t++)
    nval=do_asr_once(nval,size);
  write_reg(reg,nval,size);
}


// TODO: Test this looping "once" stuff...
void Em68k::exec_aslr(ULONG loc, unsigned int  opcode, unsigned int count, unsigned int size)
{
  unsigned int ir=(opcode>>5)&0x01;
  unsigned int reg=opcode&0x07;
  if(ir) count=read_reg(count,ECPUABSTRACT_SIZE_BYTE)&0x3f;
  unsigned int nval=read_reg(reg,size);
  for(unsigned int t=0;t<count;t++)
    nval=do_asl_once(nval,size);
  write_reg(reg,nval,size);
}


// TODO: Test this sucker! (Does it even decode correctly?)
// TODO: Test this looping "once" stuff...
void Em68k::exec_roxrr(ULONG loc, unsigned int  opcode, unsigned int count, unsigned int size)
{
  unsigned int ir=(opcode>>5)&0x01;
  unsigned int reg=opcode&0x07;
  if(ir) count=read_reg(count,ECPUABSTRACT_SIZE_BYTE)&0x3f;
  unsigned int nval=read_reg(reg,size);
  for(unsigned int t=0;t<count;t++)
    nval=do_roxr_once(nval,size);
  write_reg(reg,nval,size);
}


// TODO: Test this sucker! (Does it even decode correctly?)
// TODO: Test this looping "once" stuff...
void Em68k::exec_roxlr(ULONG loc, unsigned int  opcode, unsigned int count, unsigned int size)
{
  unsigned int ir=(opcode>>5)&0x01;
  unsigned int reg=opcode&0x07;
  if(ir) count=read_reg(count,ECPUABSTRACT_SIZE_BYTE)&0x3f;
  unsigned int nval=read_reg(reg,size);
  for(unsigned int t=0;t<count;t++)
    nval=do_roxl_once(nval,size);
  write_reg(reg,nval,size);
}


// TODO: Test this looping "once" stuff...
void Em68k::exec_rorr(ULONG loc, unsigned int  opcode, unsigned int count, unsigned int size)
{
  unsigned int ir=(opcode>>5)&0x01;
  unsigned int reg=opcode&0x07;
  if(ir) count=read_reg(count,ECPUABSTRACT_SIZE_BYTE)&0x3f;
  unsigned int nval=read_reg(reg,size);
  for(unsigned int t=0;t<count;t++)
    nval=do_ror_once(nval,size);
  write_reg(reg,nval,size);
}


// TODO: Test this looping "once" stuff...
void Em68k::exec_rolr(ULONG loc, unsigned int  opcode, unsigned int count, unsigned int size)
{
  unsigned int ir=(opcode>>5)&0x01;
  unsigned int reg=opcode&0x07;
  if(ir) count=read_reg(count,ECPUABSTRACT_SIZE_BYTE)&0x3f;
  unsigned int nval=read_reg(reg,size);
  for(unsigned int t=0;t<count;t++)
    nval=do_rol_once(nval,size);
  write_reg(reg,nval,size);
}


void Em68k::dis_xxlr(ULONG loc, unsigned int  opcode, unsigned int count, unsigned int size)
{
  DEBUG_OUT<<"L";
  unsigned int ir=(opcode>>5)&0x01;
  unsigned int reg=opcode&0x07;
  print_size(size);
  if(ir) print_reg(count);
  else {
    if(!count) count=8;
    DEBUG_OUT<<"#$"<<count;
  }
  DEBUG_OUT<<",";
  print_reg(reg);
}


void Em68k::dis_xxrr(ULONG loc, unsigned int  opcode, unsigned int count, unsigned int size)
{
  DEBUG_OUT<<"R";
  unsigned int ir=(opcode>>5)&0x01;
  unsigned int reg=opcode&0x07;
  print_size(size);
  if(ir) print_reg(count);
  else {
    if(!count) count=8;
    DEBUG_OUT<<"#$"<<count;
  }
  DEBUG_OUT<<",";
  print_reg(reg);
}


// TODO: misses EXT, maybe more...
void Em68k::exec_swap(ULONG loc, unsigned int opcode)
{
  unsigned int chk=opcode&0xfff8;
  if(chk!=0x4840) {
    exec_unimplemented("Not a swap!");
    return;
  }
  unsigned int reg=opcode&0x07;
  unsigned int val=read_reg(reg,ECPUABSTRACT_SIZE_LONG);
  unsigned int top=(val>>16)&0xffff;
  unsigned int bottom=val&0xffff;
  val=top|((bottom<<16)&0xffff0000);
  write_reg(reg,val,ECPUABSTRACT_SIZE_LONG);
  // TODO: Set flags...
}


// TODO: misses EXT, maybe more...
void Em68k::dis_swap(ULONG loc, unsigned int opcode)
{
  unsigned int chk=opcode&0xfff8;
  if(chk!=0x4840) {
    exec_unimplemented("Not a swap!");
    return;
  }
  DEBUG_OUT<<"swap ";
  unsigned int reg=opcode&0x07;
  print_reg(reg);
}


// NOTE: This is only the WORD sized version...
// TODO: shouldn't I test for the other one?
void Em68k::exec_link(ULONG loc, unsigned int opcode)
{
  unsigned int reg=(opcode&0x07)+8;
#ifdef DEBUG_EXEC
  DEBUG_OUT<<"LINK sp="<<a[7]<<" a["<<(reg-8)<<"]="<<a[reg-8]<<"\n";
#endif
  unsigned int val=read_reg(reg,ECPUABSTRACT_SIZE_LONG);
  push(val,15,ECPUABSTRACT_SIZE_LONG);
  // Note: this is signed...
  int disp=sign_extend68k(getImm_op_len(loc,ECPUABSTRACT_SIZE_WORD),ECPUABSTRACT_SIZE_WORD);
  unsigned int st=read_reg(15,ECPUABSTRACT_SIZE_LONG);
  write_reg(reg,st,ECPUABSTRACT_SIZE_LONG);
  write_reg(15,st+disp,ECPUABSTRACT_SIZE_LONG);
  //debugger();
}


void Em68k::exec_unlk(ULONG loc, unsigned int opcode)
{
  unsigned int reg=(opcode&0x07)+8;
  unsigned int st=read_reg(reg,ECPUABSTRACT_SIZE_LONG);
  write_reg(15,st,ECPUABSTRACT_SIZE_LONG);
  unsigned int val=pull(15,ECPUABSTRACT_SIZE_LONG);
  write_reg(reg,val,ECPUABSTRACT_SIZE_LONG);
#ifdef DEBUG_EXEC
  DEBUG_OUT<<"UNLK sp="<<a[7]<<" a["<<(reg-8)<<"]="<<a[reg-8]<<"\n";
#endif
  //debugger();
}


void Em68k::exec_trap(ULONG loc, unsigned int opcode)
{
  bool done=false;
  unsigned int vector=opcode&0xf;
  if(!done) {
    if(vector==15) {
      if(io) done=io->systemCall(vector);
    }
  }
  if(!done) {
    if(trapHandler) done=trapHandler->systemCall(vector);
  }
  if(!done) {
    // TODO: call cause_int with the correct int num...
    exec_unimplemented("TRAP");
  }
}



