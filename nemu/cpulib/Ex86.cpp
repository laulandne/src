
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


/*

Some notes:

Prefixes don't work.

Flags aren't implemented fully...in particular the parity flag...

Real mode isn't implemented at all...
Neither are true LDT's or GDT's...
Instead everything is pure flat for now and seg regs are hardly used.

NOTE: renamed a couple flags, the S is now N, and the O is now V

VERY IMPORTANT
Reversed the order of the operands from Intel's and used GNU's...and in
some places it might be wrong...needs to be checked.
If you're looking at some x86 documentation remember this!

*/

// STATUS:
// Tiny linux example code runs.
// Tiny 64-bit linux doesn't run.
// Tiny-freebsd doesn't run.
// Tiny darwin x86 hello's and tests's run, but fib and sizes doesn't.
// Tiny darwin 64-bit x86 doesn't run
// Tiny solaris x86 runs except for fib.

//#define DEBUG_DIS_MODES 1

//#define DEBUG_EXEC 1

#define CHECK_SAFE 1

#define SHOW_INTS  1


#include <elib/EMemMapAbstract.h>
#include <elib/EModule.h>
#include <elib/EHandler.h>

#include <cpulib/Ex86.h>

#include <portable/nickcpp/ANullStream.h>

#include <portable/nickcpp/AStringHelper.h>


#include <cpulib/generated/Ex86_tab.h>

#include <cpulib/generated/Ex86_0f_tab.h>


////////////////////////////////////////////////////////////////////////////////
//  Ex86_seg Class
////////////////////////////////////////////////////////////////////////////////

Ex86_seg::Ex86_seg()
{
  reset();
}


void Ex86_seg::reset()
{
  val=0;
  base=0;
  limit=0;
  flags=0;
}


void Ex86_seg::print()
{
  DEBUG_OUT<<"val="<<val<<" base="<<base<<" lim="<<limit<<" flags="<<flags<<"\n";
}


unsigned int Ex86_seg::getSize()
{
  // TODO: for now, always 32 bit
  return Ex86_SIZE_LONG;
}


void Ex86_seg::setBase(unsigned int n)
{
  // TODO: for now, always just set it
  base=n;
}


void Ex86_seg::setVal(unsigned int n)
{
  // TODO: for now, always just set it, later read from LDT, whatever
  val=n;
}


////////////////////////////////////////////////////////////////////////////////
//  Ex86 Class
////////////////////////////////////////////////////////////////////////////////

Ex86::Ex86()
{
  DEBUG_OUT<<"Ex86 created...\n";
  init();
  //w=ww;
  reset();
}


Ex86::~Ex86()
{
  DEBUG_OUT<<"Ex86 destroyed.\n";
}


void Ex86::init()
{
  ECPUCisc::init();
  // Generic features
  doTrace=false;  doRun=false;
  // Set up stack and registers
  intLock=false;
  int_pending=0;
  lsb=true;
  // set everything to a known state...
  eax=0;  ebx=0;  ecx=0;  edx=0;
  esi=0;  edi=0;
  sp=0;  ebp=0;
  pc=0;
  eflags=0;
  cs.reset();  ds.reset();  es.reset();
  ss.reset();  fs.reset();  gs.reset();
  curCodeSeg=&cs;  curDataSeg=&ds;
  curAddrSize=Ex86_SIZE_LONG;  curDataSize=Ex86_SIZE_LONG;
  curNumBytes=4;
  cpl=0;
  unsigned int t=0;
  for(t=0;t<8;t++) fp[t]=0.0;
  fpsw=0;  fpcw=0;  fptw=0;
  for(t=0;t<5;t++) cr[t]=0;
  for(t=0;t<8;t++) dr[t]=0;
  gdtr=0;  idtr=0;  ldtr=0;  tr=0;
  protMode=false;
  v86=false;
  op=0;
  opName=(char *)NULL;
  opFormat=0; opSubFormat=0;  opFlags=0;  topcode=0;
  setStack(ECPUABSTRACT_DEFAULT_STACK);
  setFramePtr(ECPUABSTRACT_DEFAULT_STACK);
  setResetStack(ECPUABSTRACT_DEFAULT_STACK);
  setResetEntry(ECPUABSTRACT_DEFAULT_CODE);
}


/* STATIC */
bool Ex86::recognize(const char *tname)
{
  char *name=aDefaultStringHelper.removeSpaces2(aDefaultStringHelper.toLower(tname));
  //DEBUG_OUT<<"recognize name is "<<name<<"\n";
  if(!strcmp(name,"8086")) return true; else
  if(!strcmp(name,"80x86")) return true; else
  if(!strcmp(name,"x86")) return true; else
  if(!strcmp(name,"8088")) return true; else
  if(!strcmp(name,"80186")) return true; else
  if(!strcmp(name,"80188")) return true; else
  if(!strcmp(name,"80286")) return true; else
  if(!strcmp(name,"80386")) return true; else
  if(!strcmp(name,"80486")) return true; else
  if(!strcmp(name,"286")) return true; else
  if(!strcmp(name,"386")) return true; else
  if(!strcmp(name,"386sx")) return true; else
  if(!strcmp(name,"386ex")) return true; else
  if(!strcmp(name,"486")) return true; else
  if(!strcmp(name,"486sx")) return true; else
  if(!strcmp(name,"486dx")) return true; else
  if(!strcmp(name,"486dx2")) return true; else
  if(!strcmp(name,"486dx4")) return true; else
  if(!strcmp(name,"i286")) return true; else
  if(!strcmp(name,"i386")) return true; else
  if(!strcmp(name,"i386sx")) return true; else
  if(!strcmp(name,"i386ex")) return true; else
  if(!strcmp(name,"i486")) return true; else
  if(!strcmp(name,"i486sx")) return true; else
  if(!strcmp(name,"i486dx")) return true; else
  if(!strcmp(name,"i486dx2")) return true; else
  if(!strcmp(name,"i486dx4")) return true; else
  if(!strcmp(name,"iapx188")) return true; else
  if(!strcmp(name,"iapx186")) return true; else
  if(!strcmp(name,"iapx286")) return true; else
  if(!strcmp(name,"iapx386")) return true; else
  if(!strcmp(name,"iapx486")) return true; else
  if(!strcmp(name,"pentium")) return true; else
  if(!strcmp(name,"pentium2")) return true; else
  if(!strcmp(name,"pentium3")) return true; else
  if(!strcmp(name,"pentium4")) return true; else
  if(!strcmp(name,"pentiumm")) return true; else
  if(!strcmp(name,"pentiumii")) return true; else
  if(!strcmp(name,"pentiumiii")) return true; else
  if(!strcmp(name,"celeron")) return true; else
  if(!strcmp(name,"xenon")) return true; else
  if(!strcmp(name,"586")) return true; else
  if(!strcmp(name,"686")) return true; else
  if(!strcmp(name,"6x86")) return true; else
  if(!strcmp(name,"i586")) return true; else
  if(!strcmp(name,"i686")) return true; else
  if(!strcmp(name,"m1")) return true; else
  if(!strcmp(name,"m2")) return true; else
  if(!strcmp(name,"p5")) return true; else
  if(!strcmp(name,"p54c")) return true; else
  if(!strcmp(name,"p55c")) return true; else
  if(!strcmp(name,"p6")) return true; else
  if(!strcmp(name,"k5")) return true; else
  if(!strcmp(name,"k6")) return true; else
  if(!strcmp(name,"k6-2")) return true; else
  if(!strcmp(name,"k6-3")) return true; else
  if(!strcmp(name,"k7")) return true; else
  if(!strcmp(name,"k8")) return true; else
  if(!strcmp(name,"athlon")) return true; else
  if(!strcmp(name,"duron")) return true; else
  if(!strcmp(name,"sempron")) return true; else
  if(!strcmp(name,"athlon64")) return true; else
  if(!strcmp(name,"amd64")) return true; else
  if(!strcmp(name,"x86_64")) return true; else
  return false;
}


const char *Ex86::getName()
{
  //DEBUG_OUT<<"getName type is "<<type<<"\n";
  switch(type) {
    case 0x086: return "Intel 8086/8088"; break;
    case 0x286: return "Intel 286"; break;
    case 0x386: return "Intel 386"; break;
    case 0x486: return "Intel 486"; break;
    case 0x586: return "Intel Pentium/586"; break;
    case 0x686: return "Intel P6/686"; break;
    case 0x666: return "AMD K6"; break;
    case 0x777: return "AMD Athlon"; break;
    case 0x888: return "AMD Athlon 64"; break;
    default: return "Intel x86 class"; break;
  }
}


void Ex86::setKind(const char *tname)
{
  hasFPU=false;
  const char *name=aDefaultStringHelper.removeSpaces2(aDefaultStringHelper.toLower(tname));
  //DEBUG_OUT<<"setKind name is "<<name<<"\n";
  if(!strcmp(name,"8088")) type=0x086;
  if(!strcmp(name,"8086")) type=0x086;
  if(!strcmp(name,"80x86")) type=0x386;
  if(!strcmp(name,"x86")) type=0x386;
  if(!strcmp(name,"80188")) type=0x086;
  if(!strcmp(name,"80186")) type=0x086;
  if(!strcmp(name,"80286")) type=0x286;
  if(!strcmp(name,"80386")) type=0x386;
  if(!strcmp(name,"80486")) type=0x486;
  if(!strcmp(name,"286")) type=0x286;
  if(!strcmp(name,"386")) type=0x386;
  if(!strcmp(name,"386sx")) type=0x386;
  if(!strcmp(name,"386ex")) type=0x386;
  if(!strcmp(name,"486")) { type=0x486; hasFPU=true; }
  if(!strcmp(name,"486sx")) type=0x486;
  if(!strcmp(name,"486dx")) { type=0x486; hasFPU=true; }
  if(!strcmp(name,"486dx2")) { type=0x486; hasFPU=true; }
  if(!strcmp(name,"486dx4")) { type=0x486; hasFPU=true; }
  if(!strcmp(name,"i286")) type=0x286;
  if(!strcmp(name,"i386")) type=0x386;
  if(!strcmp(name,"i386sx")) type=0x386;
  if(!strcmp(name,"i386ex")) type=0x386;
  if(!strcmp(name,"i486")) { type=0x486; hasFPU=true; }
  if(!strcmp(name,"i486sx")) type=0x486;
  if(!strcmp(name,"i486dx")) { type=0x486; hasFPU=true; }
  if(!strcmp(name,"i486dx2")) { type=0x486; hasFPU=true; }
  if(!strcmp(name,"i486dx4")) { type=0x486; hasFPU=true; }
  if(!strcmp(name,"iapx186")) type=0x286;
  if(!strcmp(name,"iapx188")) type=0x386;
  if(!strcmp(name,"iapx286")) type=0x286;
  if(!strcmp(name,"iapx386")) type=0x386;
  if(!strcmp(name,"iapx486")) type=0x386;
  if(!strcmp(name,"pentium")) { type=0x586; hasFPU=true; }
  if(!strcmp(name,"pentium2")) { type=0x686; hasFPU=true; }
  if(!strcmp(name,"pentiumii")) { type=0x686; hasFPU=true; }
  if(!strcmp(name,"pentium3")) { type=0x686; hasFPU=true; }
  if(!strcmp(name,"pentiumiii")) { type=0x686; hasFPU=true; }
  if(!strcmp(name,"pentium4")) { type=0x686; hasFPU=true; }
  if(!strcmp(name,"pentiumm")) { type=0x686; hasFPU=true; }
  if(!strcmp(name,"celeron")) { type=0x686; hasFPU=true; }
  if(!strcmp(name,"xenon")) { type=0x686; hasFPU=true; }
  if(!strcmp(name,"586")) { type=0x586; hasFPU=true; }
  if(!strcmp(name,"686")) { type=0x686; hasFPU=true; }
  if(!strcmp(name,"6x86")) { type=0x686; hasFPU=true; }
  if(!strcmp(name,"m1")) { type=0x586; hasFPU=true; }
  if(!strcmp(name,"m2")) { type=0x586; hasFPU=true; }
  if(!strcmp(name,"p5")) { type=0x586; hasFPU=true; }
  if(!strcmp(name,"p54c")) { type=0x586; hasFPU=true; }
  if(!strcmp(name,"p55c")) { type=0x586; hasFPU=true; }
  if(!strcmp(name,"p6")) { type=0x686; hasFPU=true; }
  if(!strcmp(name,"k5")) { type=0x586; hasFPU=true; }
  if(!strcmp(name,"k6")) { type=0x666; hasFPU=true; }
  if(!strcmp(name,"k6-2")) { type=0x666; hasFPU=true; }
  if(!strcmp(name,"k6-3")) { type=0x666; hasFPU=true; }
  if(!strcmp(name,"k7")) { type=0x777; hasFPU=true; }
  if(!strcmp(name,"athlon")) { type=0x777; hasFPU=true; }
  if(!strcmp(name,"k8")) { type=0x888; hasFPU=true; }
  if(!strcmp(name,"athlon64")) { type=0x888; hasFPU=true; }
  if(!strcmp(name,"amd64")) { type=0x888; hasFPU=true; }
  if(!strcmp(name,"x86_64")) { type=0x888; hasFPU=true; }
  if(type>0x086) hasMMU=true;
}


void Ex86::reset()
{
#ifdef DEBUG_EXEC
  setTrace(true);
#endif
  unsigned int t;
  // NOTE: a "real" x86 starts up in real mode
  v86=false;  hasMMU=false;  protMode=false;  bitness=16;
  curAddrSize=Ex86_SIZE_SHORT;  curDataSize=Ex86_SIZE_SHORT;  curNumBytes=2;
  if(type>0x086) {
    // 286 or higher
    hasMMU=true; protMode=true;
  }
  if(type>0x286) {
    // 386 or higher
    bitness=32;  curNumBytes=4;
    curAddrSize=Ex86_SIZE_LONG;  curDataSize=Ex86_SIZE_LONG;
  }
  if(type>0x777) {
    // a 64-bit chip or higher
    bitness=64;
  }
  eax=0;  ebx=0;  ecx=0;
  edx=0x580;  // from K6 manual, say we're a K6, later on we'll say different
  esi=0;  edi=0;
  sp=0;  ebp=0;
  pc=0xfffffff0;
  eflags=0x00000000;  // NOTE: I had this 0x00000002, why was that?
  cs.reset();  ds.reset();  es.reset();
  ss.reset();  fs.reset();  gs.reset();
  load_seg(cs,0xf000);
  curCodeSeg=&cs;  curDataSeg=&ds;
  cpl=0;
  for(t=0;t<8;t++) fp[t]=0.0;
  fpsw=0;  fpcw=0x40;  fptw=0x5555;
  for(t=0;t<5;t++) cr[t]=0;
  cr[0]=0x60000010;
  for(t=0;t<8;t++) dr[t]=0;
  dr[7]=0x00000400;
  dr[6]=0xffff0ff0;
  gdtr=0;  idtr=0;  ldtr=0;  tr=0;
  op=0;  opName=(char *)NULL;
  opFormat=0; opSubFormat=0;  opFlags=0;  topcode=0;
  lsb=true;
  hasMMU=true;
  clearStackTrace();
  ECPUAbstract::reset();
}


void Ex86::setBitness(unsigned int val)
{
  switch(val) {
    case 16:
      bitness=16;
      curAddrSize=Ex86_SIZE_SHORT;  curDataSize=Ex86_SIZE_SHORT;  curNumBytes=2;
      break;
    case 32:
      bitness=32;
      curAddrSize=Ex86_SIZE_LONG;  curDataSize=Ex86_SIZE_LONG;  curNumBytes=4;
      break;
    case 64:
      bitness=64;
      // default size is still 32 bits
      curAddrSize=Ex86_SIZE_LONG;  curDataSize=Ex86_SIZE_LONG;  curNumBytes=4;
      break;
    default:
      DEBUG_OUT<<"Illegal value ("<<val<<") for setBitness!\n";
      break;
  }
}


bool Ex86::setRegister(const char *tname,ULONG value)
{
  const char *name=aDefaultStringHelper.removeSpaces2(aDefaultStringHelper.toLower(tname));
  bool ret=false;
  if(!strcmp(name,"pc"))
  { jump(value); ret=true; } else
  if(!strcmp(name,"eip"))
  { pc=value; ret=true; } else
  if(!strcmp(name,"eax"))
  { eax=value; ret=true; } else
  if(!strcmp(name,"ebx"))
  { ebx=value; ret=true; } else
  if(!strcmp(name,"ecx"))
  { ecx=value; ret=true; } else
  if(!strcmp(name,"edx"))
  { edx=value; ret=true; } else
  if(!strcmp(name,"esi"))
  { esi=value; ret=true; } else
  if(!strcmp(name,"edi"))
  { edi=value; ret=true; } else
  if(!strcmp(name,"esp"))
  { sp=value; ret=true; } else
  if(!strcmp(name,"ebp"))
  { ebp=value; ret=true; } else
  if(!strcmp(name,"cs"))
  { load_seg(cs,value); ret=true; } else
  if(!strcmp(name,"ds"))
  { load_seg(ds,value); ret=true; } else
  if(!strcmp(name,"es"))
  { load_seg(es,value); ret=true; } else
  if(!strcmp(name,"ss"))
  { load_seg(ss,value); ret=true; } else
  if(!strcmp(name,"fs"))
  { load_seg(fs,value); ret=true; } else
  if(!strcmp(name,"gs"))
  { load_seg(gs,value); ret=true; }
  return ret;
}


void Ex86::setStack(ULONG val)
{
  if(protMode) sp=val;
  else {
    ss.setBase(getSegPart(val));
    sp=getOffPart(val);
  }
}


ULONG Ex86::readStack()
{
  unsigned int ret=0;
  if(protMode) ret=sp;
  else {
    ret=calcSegVal(ss)+sp;
  }
  return ret;
}


void Ex86::setFramePtr(ULONG val)
{
  if(protMode) ebp=val;
  else {
    ss.setBase(getSegPart(val));
    ebp=getOffPart(val);
  }
}


ULONG Ex86::readFramePtr()
{
  unsigned int ret=0;
  if(protMode) ret=ebp;
  else {
    ret=calcSegVal(ss)+ebp;
  }
  return ret;
}


void Ex86::setReturnReg(ULONG val)
{
  eax=val;
}


ULONG Ex86::readReturnReg()
{
  return eax;
}


ULONG Ex86::readParamReg(unsigned int num)
{
  unsigned int n=num;
  switch(n) {
    case Ex86_REGD_ESP: n=Ex86_REGD_ESI; break;
    case Ex86_REGD_EBP: n=Ex86_REGD_EDI; break;
    default: break;
  }
  return readReg(n);
}


void Ex86::setParamReg(unsigned int num,ULONG val)
{
  unsigned int n=num;
  switch(n) {
    case Ex86_REGD_ESP: n=Ex86_REGD_ESI; break;
    case Ex86_REGD_EBP: n=Ex86_REGD_EDI; break;
    default: break;
  }
  writeReg(n,val);
}


bool Ex86::interp(const char *cmd,const char *a1,const char *a2,const char *a3)
{
  bool ret=false;
  if(!strcmp(cmd,"rm")) { wmregs(); ret=true; }
  if(!strcmp(cmd,"mmu")) { wmregs(); ret=true; }
  if(!strcmp(cmd,"rf")) { wfregs(); ret=true; }
  if(!strcmp(cmd,"fpu")) { wfregs(); ret=true; }
  return ret;
}


void Ex86::wmregs()
{
  //unsigned int dig=w->getHexDigits();
  //w->setHexDigits(8);
  if(v86) DEBUG_OUT<<"(V86 Mode)";
  else {
    if(protMode) DEBUG_OUT<<"(Protected Mode)";
    else DEBUG_OUT<<"(Real Mode)";
  }
  DEBUG_OUT<<"\n";
  DEBUG_OUT<<"cpl= "<<cpl<<"\n";
  DEBUG_OUT<<"cs ";  cs.print();
  DEBUG_OUT<<"ds ";  ds.print();
  DEBUG_OUT<<"es ";  es.print();
  DEBUG_OUT<<"ss ";  ss.print();
  DEBUG_OUT<<"fs ";  fs.print();
  DEBUG_OUT<<"gs ";  gs.print();
  DEBUG_OUT<<"gdtr="<<gdtr<<" ldtr="<<ldtr<<"\n";
  DEBUG_OUT<<"idtr="<<ldtr<<" tr=  "<<tr<<"\n";
  DEBUG_OUT<<"cr0= "<<cr[0]<<" cr1= "<<cr[1]<<"\n";
  DEBUG_OUT<<"cr2= "<<cr[2]<<" cr3= "<<cr[3]<<"\n";
  DEBUG_OUT<<"cr4= "<<cr[4]<<"\n";
  //w->setHexDigits(dig);
}


void Ex86::wfregs()
{
  if(!hasFPU) { DEBUG_OUT<<"No fpu.\n"; return; }
  //unsigned int dig=w->getHexDigits();
  //w->setHexDigits(4);
  DEBUG_OUT<<"fpsw="<<fpsw<<" fpcw="<<fpcw<<" fptw="<<fptw<<"\n";
  //w->setHexDigits(8);
  unsigned int t;
  // NOTE: That stuff with '0'+t is to get them to display with only one digit
  for(t=0;t<8;t++) {
    DEBUG_OUT<<"fp"<<(char)('0'+t)<<"="<<fp[t]<<" ";
    if(t==2) DEBUG_OUT<<"\n";
    if(t==5) DEBUG_OUT<<"\n";
  }
  DEBUG_OUT<<"\n";
  //w->setHexDigits(dig);
}


void Ex86::wregs()
{
  //unsigned int d=w->getHexDigits();
  //w->setHexDigits(8);
  unsigned int theSize=Ex86_SIZE_SHORT;
  if(v86) DEBUG_OUT<<"v86";
  else { if(protMode) DEBUG_OUT<<"protected"; else DEBUG_OUT<<"real"; }
  DEBUG_OUT<<" mode (";
  switch(bitness) {
    case 16: DEBUG_OUT<<"16"; break;
    case 32: DEBUG_OUT<<"32"; theSize=Ex86_SIZE_LONG; break;
    case 64: DEBUG_OUT<<"64"; theSize=Ex86_SIZE_QUAD; break;
    default: DEBUG_OUT<<"unknown"; break;
  }
  DEBUG_OUT<<"-bits)";
  DEBUG_OUT<<"\n";
  DEBUG_OUT<<getRegString(Ex86_REG_EIP,theSize)<<"="<<readReg(Ex86_REG_EIP,theSize)<<"\n";
  DEBUG_OUT<<"eflags="<<eflags<<" (";
  /*
  if(eflags&Ex86_FLAG_ID) DEBUG_OUT<<"ID"; else DEBUG_OUT<<"id";
  if(eflags&Ex86_FLAG_VIP) DEBUG_OUT<<"VIP"; else DEBUG_OUT<<"vip";
  if(eflags&Ex86_FLAG_VIF) DEBUG_OUT<<"VIF"; else DEBUG_OUT<<"vif";
  if(eflags&Ex86_FLAG_AC) DEBUG_OUT<<"AC"; else DEBUG_OUT<<"ac";
  if(eflags&Ex86_FLAG_VM) DEBUG_OUT<<"VM"; else DEBUG_OUT<<"vm";
  */
  if(eflags&Ex86_FLAG_R) DEBUG_OUT<<"R"; else DEBUG_OUT<<"r";
  //if(eflags&Ex86_FLAG_X15) DEBUG_OUT<<"X"; else DEBUG_OUT<<"x";
  /*
  if(eflags&Ex86_FLAG_NT) DEBUG_OUT<<"NT"; else DEBUG_OUT<<"nt";
  if(eflags&Ex86_FLAG_IO1) DEBUG_OUT<<"1"; else DEBUG_OUT<<"-";
  if(eflags&Ex86_FLAG_IO0) DEBUG_OUT<<"0"; else DEBUG_OUT<<"-";
  */
  if(eflags&Ex86_FLAG_V) DEBUG_OUT<<"V"; else DEBUG_OUT<<"v";
  if(eflags&Ex86_FLAG_D) DEBUG_OUT<<"D"; else DEBUG_OUT<<"d";
  if(eflags&Ex86_FLAG_I) DEBUG_OUT<<"I"; else DEBUG_OUT<<"i";
  if(eflags&Ex86_FLAG_T) DEBUG_OUT<<"T"; else DEBUG_OUT<<"t";
  if(eflags&Ex86_FLAG_N) DEBUG_OUT<<"N"; else DEBUG_OUT<<"n";
  if(eflags&Ex86_FLAG_Z) DEBUG_OUT<<"Z"; else DEBUG_OUT<<"z";
  //if(eflags&Ex86_FLAG_X5) DEBUG_OUT<<"X"; else DEBUG_OUT<<"-";
  if(eflags&Ex86_FLAG_A) DEBUG_OUT<<"A"; else DEBUG_OUT<<"a";
  //if(eflags&Ex86_FLAG_X3) DEBUG_OUT<<"X"; else DEBUG_OUT<<"-";
  if(eflags&Ex86_FLAG_P) DEBUG_OUT<<"P"; else DEBUG_OUT<<"p";
  //if(eflags&Ex86_FLAG_X1) DEBUG_OUT<<"X"; else DEBUG_OUT<<"x";
  if(eflags&Ex86_FLAG_C) DEBUG_OUT<<"C"; else DEBUG_OUT<<"c";
  DEBUG_OUT<<")";
  DEBUG_OUT<<"\n";
  // The 8 "general registers"
  for(unsigned int t=0;t<8;t++) {
    DEBUG_OUT<<getRegString(t,theSize)<<"="<<readReg(t,theSize);
    if(t&1) DEBUG_OUT<<"\n"; else DEBUG_OUT<<"  ";
  }
  DEBUG_OUT<<"\n";
  //w->setHexDigits(d);
}


unsigned int Ex86::getSegPart(unsigned int vaddr)
{
  unsigned int ret=0;
  if(protMode) ret=vaddr;
  else {
    ret=(vaddr>>16)&0xffff;
  }
  return ret;
}


unsigned int Ex86::getOffPart(unsigned int vaddr)
{
  unsigned int ret=0;
  if(protMode) ret=vaddr;
  else {
    ret=vaddr&0xffff;
  }
  return ret;
}


unsigned int Ex86::calcSegVal(Ex86_seg& seg)
{
  unsigned int ret=0;
  if(protMode) debugger("calcSegVal called in protected mode!");
  else {
    ret=(seg.getBase()<<4)&0xffff0;
  }
  return ret;
}


// TODO: needs a lot of work!
unsigned int Ex86::calcFlat(Ex86_seg& seg,ULONG addr)
{
  if(!protMode) { addr&=0xffff; addr+=(seg.getBase()<<4)&0xffff0; }
  return addr;
}


// TODO: needs a lot of work!
void Ex86::load_seg(Ex86_seg& seg,ULONG val)
{
  // TODO: load base, limit and flags from LDT or GDT or whatever...
  seg.setVal(val);
}


// TODO: obviously not done...
void Ex86::go_real()
{
  protMode=false;  v86=false;
  bitness=16;  // Not quite right
  // TODO: Plenty more here
  // CR0 bit, cpl, d32Bit, b32Bit, etc
  DEBUG_OUT<<"x86 real mode entered.\n";
}


// TODO: obviously not done...
void Ex86::go_protected()
{
  protMode=true;  v86=false;
  bitness=32;  // Not quite right
  // TODO: Plenty more here
  // CR0 bit, cpl, d32Bit, b32Bit, etc
  DEBUG_OUT<<"x86 protected mode entered.\n";
}


// TODO: obviously not done...
void Ex86::go_v86()
{
  protMode=true;  v86=true;
  bitness=16;  // Not quite right
  // TODO: Plenty more here
  // CR0 bit, cpl, d32Bit, b32Bit, etc
  DEBUG_OUT<<"x86 v86 mode entered.\n";
}


void Ex86::handle_int(ULONG value)
{
  // Need to handle these...
}


void Ex86::cause_int(ULONG value)
{
  // Need to handle these...
}


// TODO: may be wrong due to little-endian-ness
unsigned int Ex86::getMaskSizeX86(unsigned int size)
{
  unsigned int ret=0;
  switch(size) {
    case Ex86_SIZE_L:  ret=0xff;  break;
    case Ex86_SIZE_H:  ret=0xff00;  break;
    case Ex86_SIZE_SHORT:  ret=0xffff; break;
    case Ex86_SIZE_LONG:  ret=0xffffffff; break;
    default:
      debugger("bad size in getMaskSize!");
      break;
  }
  return ret;
}


// TODO: may be wrong due to little-endian-ness
unsigned int Ex86::getTopBitSizeX86(unsigned int size)
{
  unsigned int ret=0;
  switch(size) {
    case Ex86_SIZE_L:  ret=0x80;  break;
    case Ex86_SIZE_H:  ret=0x8000;  break;
    case Ex86_SIZE_SHORT:  ret=0x8000; break;
    case Ex86_SIZE_LONG:  ret=0x80000000; break;
    default:
      debugger("bad size in getTopBitSize!");
      break;
  }
  return ret;
}


// TODO: uses sseg, even if you try and use another reg instead of esp...
void Ex86::push(unsigned int val,unsigned int reg,unsigned int size)
{
  // Size is forced to current size...for now...
  size=curDataSize;
  unsigned int stackSize=curDataSize;
  ULONG addr=readReg(reg,stackSize);
  addr-=curNumBytes;
  switch(size) {
    case Ex86_SIZE_L: map->write8(calcFlat(ss,addr),val&0xff); break;
    case Ex86_SIZE_H: map->write8(calcFlat(ss,addr),(val>>8)&0xff); break;
    case Ex86_SIZE_SHORT: map->write16l(calcFlat(ss,addr),val&0xffff); break;
    case Ex86_SIZE_LONG: map->write32l(calcFlat(ss,addr),val); break;
    default: debugger("Bad size in push!"); break;
  }
  writeReg(reg,addr,stackSize);
#ifdef DEBUG_EXEC
  DEBUG_OUT<<"\tPush "<<val<<" on "<<getRegString(reg)<<" size="<<size<<"\n";
  //DEBUG_OUT<<"Push stack: esp="<<sp<<" ebp="<<ebp<<"\n";
#endif
}


// TODO: uses sseg, even if you try and use a different register...
unsigned int Ex86::pull(unsigned int reg,unsigned int size)
{
  // Size is forced to current size...for now...
  size=curDataSize;
  unsigned int val=0;
  unsigned int stackSize=curDataSize;
  ULONG addr=readReg(reg,stackSize);
  switch(size) {
    case Ex86_SIZE_L: val=map->read8(calcFlat(ss,addr))&0xff; break;
    case Ex86_SIZE_H: val=map->read8(calcFlat(ss,addr))&0xff; break;
    case Ex86_SIZE_SHORT: val=map->read16l(calcFlat(ss,addr))&0xffff; break;
    case Ex86_SIZE_LONG: val=map->read32l(calcFlat(ss,addr)); break;
    default: debugger("Bad size in pull"); break;
  }
  addr+=curNumBytes;
  writeReg(reg,addr,stackSize);
#ifdef DEBUG_EXEC
  DEBUG_OUT<<"\tPull "<<val<<" from "<<getRegString(reg)<<" size="<<size<<"\n";
  //DEBUG_OUT<<"Pull stack: esp="<<sp<<" ebp="<<ebp<<"\n";
#endif
  return val;
}


unsigned int Ex86::getImm(ULONG addr,unsigned int size)
{
  switch(size) {
    case Ex86_SIZE_H:
    case Ex86_SIZE_L:
      return map->read8(calcFlat(cs,addr));
      break;
    case Ex86_SIZE_SHORT:
      return map->read16l(calcFlat(cs,addr));
      break;
    case Ex86_SIZE_LONG:
      return map->read32l(calcFlat(cs,addr));
      break;
    default:
      debugger("Bad size in getImm");
      break;
  }
  debugger("Shouldn't get here");
  return 0;
}


ULONG Ex86::readReg(unsigned int n, unsigned int size)
{
  ULONG ret=0;
  if((size==Ex86_SIZE_L)||(size==Ex86_SIZE_H)) {
    switch(n) {
      case Ex86_REGB_AL: ret=eax; break;
      case Ex86_REGB_BL: ret=ebx; break;
      case Ex86_REGB_CL: ret=ecx; break;
      case Ex86_REGB_DL: ret=edx; break;
      case Ex86_REGB_AH: ret=eax; break;
      case Ex86_REGB_BH: ret=ebx; break;
      case Ex86_REGB_CH: ret=ecx; break;
      case Ex86_REGB_DH: ret=edx; break;
      case Ex86_REG_EIP: ret=pc; break;
      default:
        debugger("bad reg in readReg, size L or H!");
        break;
    }
    if(size==Ex86_SIZE_H) ret=ret>>8;
    ret&=0xff;
  }
  else {
    switch(n) {
      case Ex86_REGD_EAX: ret=eax; break;
      case Ex86_REGD_EBX: ret=ebx; break;
      case Ex86_REGD_ECX: ret=ecx; break;
      case Ex86_REGD_EDX: ret=edx; break;
      case Ex86_REGD_ESI: ret=esi; break;
      case Ex86_REGD_EBP: ret=ebp; break;
      case Ex86_REGD_ESP: ret=sp; break;
      case Ex86_REGD_EDI: ret=edi; break;
      case Ex86_REG_EIP: ret=pc; break;
      default:
        debugger("bad reg in readReg, size X or EX!");
        break;
    }
    if(size==Ex86_SIZE_SHORT) ret&=0xffff;
  }
#ifdef DEBUG_EXEC
  DEBUG_OUT<<"\tRead "<<ret<<" from reg "<<getRegString(n,size)<<"\n";
#endif
  return ret;
}


void Ex86::writeReg(unsigned int n, ULONG val, unsigned int size)
{
  if((size==Ex86_SIZE_H)||(size==Ex86_SIZE_L)) {
    val&=0xff;
    switch(n) {
      case Ex86_REGB_AL: eax=(eax&0xffffff00)|val; break;
      case Ex86_REGB_BL: ebx=(ebx&0xffffff00)|val; break;
      case Ex86_REGB_CL: ecx=(ecx&0xffffff00)|val; break;
      case Ex86_REGB_DL: edx=(edx&0xffffff00)|val; break;
      case Ex86_REGB_AH: eax=(eax&0xffff00ff)|(val<<8); break;
      case Ex86_REGB_BH: ebx=(ebx&0xffff00ff)|(val<<8); break;
      case Ex86_REGB_CH: ecx=(ecx&0xffff00ff)|(val<<8); break;
      case Ex86_REGB_DH: edx=(edx&0xffff00ff)|(val<<8); break;
      default:
        debugger("bad reg in writeReg, size L or H!");
        break;
    }
  }
  if(size==Ex86_SIZE_SHORT) {
    val&=0xffff;
    switch(n) {
      case Ex86_REGD_EAX: eax=(eax&0xffff0000)|val; break;
      case Ex86_REGD_EBX: ebx=(ebx&0xffff0000)|val; break;
      case Ex86_REGD_ECX: ecx=(ecx&0xffff0000)|val; break;
      case Ex86_REGD_EDX: edx=(edx&0xffff0000)|val; break;
      case Ex86_REGD_ESI: esi=(esi&0xffff0000)|val; break;
      case Ex86_REGD_EDI: edi=(edi&0xffff0000)|val; break;
      case Ex86_REGD_EBP: ebp=(ebp&0xffff0000)|val; break;
      case Ex86_REGD_ESP: sp=(sp&0xffff0000)|val; break;
      default:
        debugger("bad reg in writeReg, size X!");
        break;
    }
  }
  if(size==Ex86_SIZE_LONG) {
    switch(n) {
      case Ex86_REGD_EAX: eax=val; break;
      case Ex86_REGD_EBX: ebx=val; break;
      case Ex86_REGD_ECX: ecx=val; break;
      case Ex86_REGD_EDX: edx=val; break;
      case Ex86_REGD_ESI: esi=val; break;
      case Ex86_REGD_EDI: edi=val; break;
      case Ex86_REGD_EBP: ebp=val; break;
      case Ex86_REGD_ESP: sp=val; break;
      default:
        debugger("bad reg in writeReg, size EX!");
        break;
    }
  }
#ifdef DEBUG_EXEC
  DEBUG_OUT<<"\tWrite "<<val<<" to reg "<<getRegString(n,size)<<"\n";
#endif
}


ULONG Ex86::readMem(ULONG ea, unsigned int size)
{
  ULONG ret=0;
  if((size==Ex86_SIZE_H)||(size==Ex86_SIZE_L)) {
    ret=map->read8(ea);
  }
  if(size==Ex86_SIZE_SHORT) {
    ret=map->read16l(ea);
   }
  if(size==Ex86_SIZE_LONG) {
    ret=map->read32l(ea);
  }
#ifdef DEBUG_EXEC
  //DEBUG_OUT<<"\tRead "<<ret<<" from mem "<<ea<<"\n";
#endif
  return ret;
}


void Ex86::writeMem(ULONG ea, ULONG val,unsigned int size)
{
  if((size==Ex86_SIZE_H)||(size==Ex86_SIZE_L)) {
    val&=0xff;
    map->write8(ea,val);
  }
  if(size==Ex86_SIZE_SHORT) {
    val&=0xffff;
    map->write16l(ea,val);
   }
  if(size==Ex86_SIZE_LONG) {
    map->write32l(ea,val);
  }
#ifdef DEBUG_EXEC
  DEBUG_OUT<<"\tWrite "<<val<<" to mem "<<ea<<"\n";
#endif
}


char *Ex86::getRegString(unsigned int num,unsigned int size)
{
  bool done=false;
  if((size==Ex86_SIZE_H)||(size==Ex86_SIZE_L)) {
    done=true;
    switch(num) {
      case Ex86_REGB_AL: return "al"; break;
      case Ex86_REGB_BL: return "bl"; break;
      case Ex86_REGB_CL: return "cl"; break;
      case Ex86_REGB_DL: return "dl"; break;
      case Ex86_REGB_AH: return "ah"; break;
      case Ex86_REGB_BH: return "bh"; break;
      case Ex86_REGB_CH: return "ch"; break;
      case Ex86_REGB_DH: return "dh"; break;
      default:
        debugger("Bad reg num in getRegString, size byte!");
        DEBUG_OUT<<"(bad reg was "<<num<<")\n";
        break;
    }
  }
  if(size==Ex86_SIZE_SHORT) {
    done=true;
    switch(num) {
      case Ex86_REGD_EAX: return "ax"; break;
      case Ex86_REGD_EBX: return "bx"; break;
      case Ex86_REGD_ECX: return "cx"; break;
      case Ex86_REGD_EDX: return "dx"; break;
      case Ex86_REGD_ESI: return "si"; break;
      case Ex86_REGD_EBP: return "bp"; break;
      case Ex86_REGD_ESP: return "sp"; break;
      case Ex86_REGD_EDI: return "di"; break;
      case Ex86_REG_EIP: return "ip"; break;
      default:
        debugger("Bad reg num in getRegString, size short!");
        DEBUG_OUT<<"(bad reg was "<<num<<")\n";
        break;
    }
  }
  if(size==Ex86_SIZE_LONG) {
    done=true;
    switch(num) {
      case Ex86_REGD_EAX: return "eax"; break;
      case Ex86_REGD_EBX: return "ebx"; break;
      case Ex86_REGD_ECX: return "ecx"; break;
      case Ex86_REGD_EDX: return "edx"; break;
      case Ex86_REGD_ESI: return "esi"; break;
      case Ex86_REGD_EBP: return "ebp"; break;
      case Ex86_REGD_ESP: return "esp"; break;
      case Ex86_REGD_EDI: return "edi"; break;
      case Ex86_REG_EIP: return "eip"; break;
      default:
        debugger("Bad reg num in getRegString, size long!");
        DEBUG_OUT<<"(bad reg was "<<num<<")\n";
        break;
    }
  }
  if(size==Ex86_SIZE_QUAD) {
    done=true;
    switch(num) {
      case Ex86_REGD_EAX: return "rax"; break;
      case Ex86_REGD_EBX: return "rbx"; break;
      case Ex86_REGD_ECX: return "rcx"; break;
      case Ex86_REGD_EDX: return "rdx"; break;
      case Ex86_REGD_ESI: return "rsi"; break;
      case Ex86_REGD_EBP: return "rbp"; break;
      case Ex86_REGD_ESP: return "rsp"; break;
      case Ex86_REGD_EDI: return "rdi"; break;
      case Ex86_REG_EIP: return "rip"; break;
      default:
        debugger("Bad reg num in getRegString, size quad!");
        DEBUG_OUT<<"(bad reg was "<<num<<")\n";
        break;
    }
  }
  if(!done) debugger("bad size in getRegString");
  return (char *)NULL;
}


void Ex86::printReg(unsigned int num,unsigned int size)
{
  DEBUG_OUT<<"%"<<getRegString(num,size);
}


// Used to dis push and pop seg instructions (mostly)
void Ex86::printSeg(unsigned int num)
{
  unsigned int snum=(num>>3)&0x07;
  switch(snum) {
    case Ex86_SEG_ES: DEBUG_OUT<<"%es"; break;
    case Ex86_SEG_CS: DEBUG_OUT<<"%cs"; break;
    case Ex86_SEG_SS: DEBUG_OUT<<"%ss"; break;
    case Ex86_SEG_DS: DEBUG_OUT<<"%ds"; break;
    case Ex86_SEG_FS: DEBUG_OUT<<"%fs"; break;
    case Ex86_SEG_GS: DEBUG_OUT<<"%gs"; break;
    default:
      debugger("bad seg in printSeg!");
      break;
  }
}


void Ex86::setFlagsP(ULONG val,unsigned int size)
{
  unsigned int nval=0;
  switch(size) {
    case Ex86_SIZE_L: nval=val&ECPUABSTRACT_MASK_BYTE; break;
    case Ex86_SIZE_H: nval=val&Ex86_MASK_HIGHBYTE; break;
    case Ex86_SIZE_SHORT: nval=val&ECPUABSTRACT_MASK_WORD; break;
    case Ex86_SIZE_LONG: nval=val&ECPUABSTRACT_MASK_LONG; break;
    default:
      debugger("bad size in setFlagsP!");
      break;
  }
  if(popCount(nval)&1) setPFlag(1); else setPFlag(0);
}


void Ex86::setFlagsNZ(ULONG val,unsigned int size)
{
  if(size==Ex86_SIZE_H) debugger("Ex86::setFlagsNZ with Ex86_SIZE_H!");
  else ECPUAbstract::setFlagsNZ(val,size);
}


void Ex86::setFlagsNZvc(ULONG val,unsigned int size)
{
  setFlagsNZ(val,size);
  setVFlag(0);  setCFlag(0);
}


void Ex86::setFlagsVC(ULONG val1,ULONG val2,ULONG val3,unsigned int size)
{
  if(size==Ex86_SIZE_H) debugger("Ex86::setFlagsVC with Ex86_SIZE_H!");
  else ECPUAbstract::setFlagsVC(val1,val2,val3,size);
}


void Ex86::setFlagBits(unsigned int mask)
{
  eflags=eflags|mask;
}


void Ex86::resetFlagBits(unsigned int mask)
{
  eflags=eflags&(Ex86_FLAG_ALL-mask);
}


unsigned int Ex86::getFlagBits(unsigned int mask)
{
  return eflags&mask;
}


void Ex86::setCFlag(ULONG val)
{
  eflags=eflags&(Ex86_FLAG_ALL-Ex86_FLAG_C);
  if(val) eflags|=Ex86_FLAG_C;
}


void Ex86::setPFlag(ULONG val)
{
  eflags=eflags&(Ex86_FLAG_ALL-Ex86_FLAG_P);
  if(val) eflags|=Ex86_FLAG_P;
}


void Ex86::setAFlag(ULONG val)
{
  eflags=eflags&(Ex86_FLAG_ALL-Ex86_FLAG_A);
  if(val) eflags|=Ex86_FLAG_A;
}


void Ex86::setZFlag(ULONG val)
{
  eflags=eflags&(Ex86_FLAG_ALL-Ex86_FLAG_Z);
  if(val) eflags|=Ex86_FLAG_Z;
}


void Ex86::setNFlag(ULONG val)
{
  eflags=eflags&(Ex86_FLAG_ALL-Ex86_FLAG_N);
  if(val) eflags|=Ex86_FLAG_N;
}


void Ex86::setVFlag(ULONG val)
{
  eflags=eflags&(Ex86_FLAG_ALL-Ex86_FLAG_V);
  if(val) eflags|=Ex86_FLAG_V;
}



ULONG Ex86::disasm(ULONG loc)
{
  bool old=doTrace;
  doTrace=true;
  unsigned int oldPC=pc;
  unsigned int oldLen=op_len;
  pc=loc;
  op=map->read8(calcFlat(cs,pc));
  // TODO: this may not be right...
  lastSymbol=map->findAnyExactSymbol(calcFlat(cs,pc));
  if(lastSymbol) DEBUG_OUT<<lastSymbol->getName()<<":\n";
  DEBUG_OUT<<"";
  DEBUG_OUT<<"."<<pc<<"  ";
  opName=Ex86_name[op];
  opFormat=Ex86_format[op];
  opSubFormat=Ex86_subformat[op];
  opFlags=Ex86_flags[op];
  opLen=Ex86_size[op];
  topcode=op; opcode=op;
  decode();
  for(unsigned int t=0;t<op_len;t++) {
    int c=map->read8(pc+t);
    if(c<0x10) DEBUG_OUT<<"0";
    DEBUG_OUT<<c<<" ";
  }
  DEBUG_OUT<<" "<<opName<<" ";
  if(op1.mode) { disOperand(&op1); DEBUG_OUT<<","; }
  if(op2.mode) { disOperand(&op2); }
#ifdef DEBUG_DIS_MODES
  DEBUG_OUT<<"  ; "<<getModeName(op1.mode)<<" ("<<getModeName(opSubFormat)<<")";
  DEBUG_OUT<<" "<<getModeName(op2.mode)<<" ("<<getModeName(opFormat)<<")";
  if((opFormat==Ex86_MODRM)||(opSubFormat==Ex86_MODRM)||(opFormat==Ex86_MODRM8)||(opSubFormat==Ex86_MODRM8)) {
    unsigned int mod=(modrm>>6)&0x3; unsigned int rm=modrm&0x7; unsigned int reg=(modrm>>3)&0x7;
    DEBUG_OUT<<" (mod="<<mod<<" rm="<<rm<<" reg="<<reg<<" sib="<<sib<<")";
  }
#endif // DEBUG_DIS_MODES
  DEBUG_OUT<<"";
  DEBUG_OUT<<"\n";
  doTrace=old;
  pc=oldPC;
  unsigned int ret=op_len;
  op_len=oldLen;
  return ret;
}


void Ex86::execute(void)
{
  if(pc==breakpoint) { debugger("Breakpoint!\n"); return; }
  doRun=true;
  op=map->read8(calcFlat(cs,pc));
  if(pc==breakpoint) { debugger("Breakpoint!\n"); return; }
  opName=Ex86_name[op];
  opFormat=Ex86_format[op];
  opSubFormat=Ex86_subformat[op];
  opFlags=Ex86_flags[op];
  opLen=Ex86_size[op];
  topcode=op; opcode=op;
  unsigned int loc=pc;
  decode();
#include <cpulib/generated/Ex86_exec.h>
  pc=pc+op_len;
  if(int_pending) handle_int(int_pending);
}


char *Ex86::getModeName(unsigned int format)
{
  switch(format) {
    case Ex86_NONE: return "none"; break;
    case Ex86_SPECIAL: return "special"; break;
    case Ex86_AX: return "ax"; break;
    case Ex86_REG: return "reg"; break;
    case Ex86_SEG: return "seg"; break;
    case Ex86_INDREG: return "indreg"; break;
    case Ex86_DISP: return "disp"; break;
    case Ex86_IMM: return "imm"; break;
    case Ex86_BRANCH: return "branch"; break;
    case Ex86_MODRM: return "modrm"; break;
    case Ex86_MODRM8: return "modrm8"; break;
    case Ex86_MODREG: return "modreg"; break;
    case Ex86_MODREG8: return "modreg8"; break;
    default: return "unknown"; break;
  }
}

void Ex86::decode()
{
  op_len=opLen;
  sibPresent=false;  modrmPresent=false;
  preBytes=0;  immBytes=0;  dispBytes=0;
  decodeOperand(opFormat,&op2);
  decodeOperand(opSubFormat,&op1);
  //if(!special) {
    if(preBytes) op_len+=preBytes;
    if(modrmPresent) op_len++;
    if(sibPresent) op_len++;
    if(dispBytes) op_len+=dispBytes;
    if(immBytes) op_len+=immBytes;
  //}
}


void Ex86::decodeOperand(unsigned int format,ECPUOperand *op)
{
  op->init();
  op->mode=format;
  op->size=curDataSize;  // Assume for now...
  EMapSymbol *e=NULL;
  switch(op->mode) {
    case Ex86_NONE:
      break;
    case Ex86_SPECIAL:
      decodeSpecial();
      break;
    case Ex86_AX:
      op->mode=Ex86_REG;  op->reg=0;
      if(opFlags==Ex86_SIZE_BYTE) { op->size=Ex86_SIZE_BYTE; }
      break;
    case Ex86_REG:
      op->reg=opcode&0x7;
      if(opFlags==Ex86_SIZE_BYTE) { op->size=Ex86_SIZE_BYTE; }
      break;
    case Ex86_SEG:
      debugger("special mode in decodeOperand!");
      break;
    case Ex86_INDREG:
      debugger("indreg mode in decodeOperand!");
      break;
    case Ex86_DISP:
      dispBytes=curNumBytes;
      if(opFlags==Ex86_SIZE_BYTE) { op->size=Ex86_SIZE_BYTE; dispBytes=1; }
      op->val=0;
      break;
    case Ex86_IMM:
      immBytes=curNumBytes;
      if(opFlags==Ex86_SIZE_BYTE) { op->size=Ex86_SIZE_BYTE; immBytes=1; }
      op->val=0;
      break;
    case Ex86_BRANCH:
      immBytes=curNumBytes;
      if(opFlags==Ex86_SIZE_BYTE) { op->size=Ex86_SIZE_BYTE; immBytes=1; }
      op->val=0;
      break;
    case Ex86_MODRM:
    case Ex86_MODRM8:
      decodeModrm(op);
      break;
    case Ex86_MODREG:
    case Ex86_MODREG8:
      decodeModreg(op);
      break;
    default:
      DEBUG_OUT<<"mod is "<<op->mode<<"\n";
      debugger("unknown mode in decodeOperand!");
      break;
  }
}


void Ex86::decodeModrm16(ECPUOperand *op)
{
  debugger("decodeModrm16 unimplemented!");
}


void Ex86::decodeModrm(ECPUOperand *op)
{
  modrmPresent=true;
  unsigned int mod=0,rm=0,reg=0;
  modrm=getImm(pc+opLen,Ex86_SIZE_BYTE);
  mod=(modrm>>6)&0x3; rm=modrm&0x7; reg=(modrm>>3)&0x7;
  sib=0;
  op->size=curDataSize;
  if(op->mode==Ex86_MODRM8) op->size=Ex86_SIZE_BYTE;
  if(curDataSize==Ex86_SIZE_SHORT) { decodeModrm16(op); return; }
  switch(mod) {
    case Ex86_MODRM_REGREG:
      op->mode=Ex86_REG;  op->reg=rm;
      break;
    case Ex86_MODRM_INDREG:
      if(rm==4) { sib=getImm(pc+2,Ex86_SIZE_BYTE); sibPresent=true; }
      if(rm!=5) { op->mode=Ex86_INDREG;  op->reg=rm; op->val=0; }
      else {
        op->mode=Ex86_DISP;
        op->val=4; // TODO: placeholder value
        dispBytes=curNumBytes;
      }
      break;
    case Ex86_MODRM_INDREG_DISP8:
      if(rm==4) { sib=getImm(pc+2,Ex86_SIZE_BYTE); sibPresent=true; }
      op->mode=Ex86_INDREG;  op->reg=rm;
      op->val=4; // TODO: placeholder value
      dispBytes=1;
      break;
    case Ex86_MODRM_INDREG_DISP:
      if(rm==4) { sib=getImm(pc+2,Ex86_SIZE_BYTE); sibPresent=true; }
      op->mode=Ex86_INDREG;  op->reg=rm;
      op->val=4; // TODO: placeholder value
      dispBytes=curNumBytes;
      break;
    default:
      DEBUG_OUT<<"mod is "<<mod<<"\n";
      debugger("unknown mod in decodeModrm!");
      break;
  }
  if(sibPresent) {
    unsigned int scale=(sib>>6)&0x3; unsigned int base=sib&0x7; unsigned int index=(sib>>3)&0x7;
    bool worry=false;
    if(scale!=0) worry=true;
    if(index!=4) worry=true;
    if(base!=4) worry=true;
    if(worry) {
      debugger("Unimplemented sib value!");
      DEBUG_OUT<<"Found sib="<<sib<<" scale="<<scale<<" index="<<index<<" base="<<base<<"\n";
    }
  }
}


void Ex86::decodeModreg(ECPUOperand *op)
{
  modrmPresent=true;
  unsigned int mod=0,rm=0,reg=0;
  modrm=getImm(pc+opLen,Ex86_SIZE_BYTE);
  mod=(modrm>>6)&0x3; rm=modrm&0x7; reg=(modrm>>3)&0x7;
  op->size=curDataSize;
  if(op->mode==Ex86_MODREG8) op->size=Ex86_SIZE_BYTE;
  op->mode=Ex86_REG;
  op->reg=reg;
}


void Ex86::decodeSpecial()
{
  unsigned int reg=0;
  switch(opcode) {
    case 0x0f:
      reg=getImm(pc+opLen,Ex86_SIZE_BYTE);
      opName=getSpecialOpName(opcode,reg);
      switch(reg) {
        case 0xb6:
          opFormat=Ex86_MODREG; opSubFormat=Ex86_MODRM;  opFlags=Ex86_SIZE_BYTE;
          break;
        case 0xbe:
          opFormat=Ex86_MODREG; opSubFormat=Ex86_MODRM;  opFlags=Ex86_SIZE_BYTE;
          break;
        default:
          debugger("Bad ext opcode for 0f in decodeSpecial");
          return;
          break;
      }
      opLen=2;
      decodeOperand(opFormat,&op2);
      decodeOperand(opSubFormat,&op1);
      break;
    case 0x48:
      if(bitness==64) {
        //debugger("64-bit prefix found in decodeSpecial!");
        opFormat=Ex86_NONE; opSubFormat=Ex86_NONE;
        opName=getSpecialOpName(opcode,0);
        decodeOperand(opFormat,&op2);
        decodeOperand(opSubFormat,&op1);
      }
      else {
        opFormat=Ex86_REG; opSubFormat=Ex86_NONE;
        opName=getSpecialOpName(opcode,0);
        decodeOperand(opFormat,&op2);
        decodeOperand(opSubFormat,&op1);
      }
      break;
    case 0x80:
      opFormat=Ex86_MODRM8; opSubFormat=Ex86_IMM;  opFlags=Ex86_SIZE_BYTE;
      modrm=getImm(pc+opLen,Ex86_SIZE_BYTE); reg=(modrm>>3)&0x7;
      opName=getSpecialOpName(opcode,reg);
      decodeOperand(opFormat,&op2);
      decodeOperand(opSubFormat,&op1);
      break;
    case 0x81:
      opFormat=Ex86_MODRM; opSubFormat=Ex86_IMM;
      modrm=getImm(pc+opLen,Ex86_SIZE_BYTE); reg=(modrm>>3)&0x7;
      opName=getSpecialOpName(opcode,reg);
      decodeOperand(opFormat,&op2);
      decodeOperand(opSubFormat,&op1);
      break;
    case 0x83:
      opFormat=Ex86_MODRM; opSubFormat=Ex86_IMM;  opFlags=Ex86_SIZE_BYTE;
      modrm=getImm(pc+opLen,Ex86_SIZE_BYTE); reg=(modrm>>3)&0x7;
      opName=getSpecialOpName(opcode,reg);
      decodeOperand(opFormat,&op2);
      decodeOperand(opSubFormat,&op1);
      break;
    case 0xc7:
      opFormat=Ex86_MODRM; opSubFormat=Ex86_IMM;
      modrm=getImm(pc+opLen,Ex86_SIZE_BYTE); reg=(modrm>>3)&0x7;
      opName=getSpecialOpName(opcode,reg);
      decodeOperand(opFormat,&op2);
      decodeOperand(opSubFormat,&op1);
      break;
    case 0xfe:
      opFormat=Ex86_MODRM; opSubFormat=Ex86_NONE;
      modrm=getImm(pc+opLen,Ex86_SIZE_BYTE); reg=(modrm>>3)&0x7;
      opName=getSpecialOpName(opcode,reg);
      decodeOperand(opFormat,&op2);
      decodeOperand(opSubFormat,&op1);
      break;
    case 0xff:
      opFormat=Ex86_MODRM; opSubFormat=Ex86_NONE;
      modrm=getImm(pc+opLen,Ex86_SIZE_BYTE); reg=(modrm>>3)&0x7;
      opName=getSpecialOpName(opcode,reg);
      decodeOperand(opFormat,&op2);
      decodeOperand(opSubFormat,&op1);
      break;
    default:
      DEBUG_OUT<<"opcode is "<<opcode<<"\n";
      debugger("unknown opcode in decodeSpecial!");
      break;
  }
}


char *Ex86::getSpecialOpName(unsigned int op,unsigned int reg)
{
  switch(op) {
    case 0x0f:
      switch(reg) {
        case 0xb6: return "movzx"; break;
        case 0xbe: return "movsx"; break;
        default:
          debugger("Bad reg in 0f getSpecialOpName!");
          DEBUG_OUT<<"(bad reg was "<<reg<<")\n";
          break;
      }
      break;
    case 0x48:
      if(bitness==64) {
        //debugger("64-bit prefix found in getSpcialOpName!");
        return "64-bit prefix";
      }
      else return "dec";
      break;
    case 0x80: case 0x81: case 0x83:
      switch(reg) {
        case 0x0: return "add"; break;
        case 0x1: return "or"; break;
        case 0x2: return "adc"; break;
        case 0x3: return "sbb"; break;
        case 0x4: return "and"; break;
        case 0x5: return "sub"; break;
        case 0x6: return "xor"; break;
        case 0x7: return "cmp"; break;
        default:
          debugger("Bad reg in 80 81 83 getSpecialOpName!");
          DEBUG_OUT<<"(bad reg was "<<reg<<")\n";
          break;
      }
      break;
    case 0xc7:
      switch(reg) {
        case 0x0: return "mov"; break;
        default:
          debugger("Bad reg in c7 getSpecialOpName!");
          DEBUG_OUT<<"(bad reg was "<<reg<<")\n";
          break;
      }
      break;
    case 0xfe:
      switch(reg) {
        default:
          debugger("Bad reg in fe getSpecialOpName!");
          DEBUG_OUT<<"(bad reg was "<<reg<<")\n";
          break;
      }
      break;
    case 0xff:
      switch(reg) {
        case 0x0: return "inc"; break;
        default:
          debugger("Bad reg in ff getSpecialOpName!");
          DEBUG_OUT<<"(bad reg was "<<reg<<")\n";
          break;
      }
      break;
    default:
      debugger("Bad op in getSpecialOpName!");
      DEBUG_OUT<<"(bad op was "<<op<<")\n";
      break;
  }
  return NULL;
}


void Ex86::execSpecialOp(unsigned int loc,unsigned int op,unsigned int reg)
{
  switch(op) {
    case 0x0f:
      switch(reg) {
        case 0xb6: exec_movzx(loc,op); break;
        case 0xbe: exec_movsx(loc,op); break;
        default:
          debugger("Bad reg in 0f execSpecialOp!");
          DEBUG_OUT<<"(bad reg was "<<reg<<")\n";
          break;
      }
      break;
    case 0x48:
      if(bitness==64) {
        //debugger("64-bit prefix found in execSpcialOp!");
  exec_nop(loc,op);
      }
      else exec_dec(loc,op);
      break;
    case 0x80: case 0x81: case 0x83:
      switch(reg) {
        case 0x0: exec_add(loc,op); break;
        case 0x1: exec_or(loc,op); break;
        case 0x2: exec_adc(loc,op); break;
        case 0x3: exec_sbb(loc,op); break;
        case 0x4: exec_and(loc,op); break;
        case 0x5: exec_sub(loc,op); break;
        case 0x6: exec_xor(loc,op); break;
        case 0x7: exec_cmp(loc,op); break;
        default:
          debugger("Bad reg in 80 81 83 execSpecialOp!");
          DEBUG_OUT<<"(bad reg was "<<reg<<")\n";
          break;
      }
      break;
    case 0xc7:
      switch(reg) {
        case 0x0: exec_mov(loc,op); break;
        default:
          debugger("Bad reg in c7 execSpecialOp!");
          DEBUG_OUT<<"(bad reg was "<<reg<<")\n";
          break;
      }
      break;
    case 0xfe:
      switch(reg) {
        default:
          debugger("Bad reg in fe execSpecialOp!");
          DEBUG_OUT<<"(bad reg was "<<reg<<")\n";
          break;
      }
      break;
    case 0xff:
      switch(reg) {
        case 0x0: exec_inc(loc,op); break;
        default:
          debugger("Bad reg in ff execSpecialOp!");
          DEBUG_OUT<<"(bad reg was "<<reg<<")\n";
          break;
      }
      break;
    default:
      debugger("Bad op in execSpecialOp!");
      DEBUG_OUT<<"(bad op was "<<op<<")\n";
      break;
  }
}


void Ex86::exec_special(ULONG loc,unsigned int opcode)
{
  unsigned int reg=0;
  switch(opcode) {
    case 0x0f:
      reg=getImm(pc+1,Ex86_SIZE_BYTE);
      execSpecialOp(loc,opcode,reg);
      break;
    case 0x48:
      execSpecialOp(loc,opcode,0);
      break;
    case 0x80:  case 0x81:  case 0x83:
      modrm=getImm(pc+opLen,Ex86_SIZE_BYTE); reg=(modrm>>3)&0x7;
      execSpecialOp(loc,opcode,reg);
      break;
    case 0xc7:
      modrm=getImm(pc+opLen,Ex86_SIZE_BYTE); reg=(modrm>>3)&0x7;
      execSpecialOp(loc,opcode,reg);
      break;
    case 0xfe:
      modrm=getImm(pc+opLen,Ex86_SIZE_BYTE); reg=(modrm>>3)&0x7;
      execSpecialOp(loc,opcode,reg);
      break;
    case 0xff:
      modrm=getImm(pc+opLen,Ex86_SIZE_BYTE); reg=(modrm>>3)&0x7;
      execSpecialOp(loc,opcode,reg);
      break;
    default:
      DEBUG_OUT<<"bad opcode is "<<opcode<<"\n";
      debugger("unknown opcode in exec_special!");
      break;
  }
}


ULONG Ex86::getOpDisp(ECPUOperand *op)
{
  if(!dispBytes) return 0;
  ULONG offset=opLen;
  if(modrmPresent) offset++;
  if(sibPresent) offset++;
  //if(immBytes) offset+=immBytes;
  unsigned int size=curDataSize;
  ULONG ret=0;
  if(dispBytes==1) {
    size=Ex86_SIZE_BYTE;
    ret=sign_ext(readMem(pc+offset,size),8);
  }
  else ret=readMem(pc+offset,size);
#ifdef DEBUG_EXEC
  //DEBUG_OUT<<"\tgetOpDisp: "<<ret<<"\n";
#endif // DEBUG_EXEC
  return ret;
}


ULONG Ex86::getOpImm(ECPUOperand *op)
{
  if(!immBytes) return 0;
  ULONG offset=opLen;
  if(modrmPresent) offset++;
  if(sibPresent) offset++;
  if(dispBytes) offset+=dispBytes;
  unsigned int size=curDataSize;
  ULONG ret=0;
  if(immBytes==1) {
    size=Ex86_SIZE_BYTE;
    ret=sign_ext(readMem(pc+offset,size),8);
  }
  else ret=readMem(pc+offset,size);
#ifdef DEBUG_EXEC
  //DEBUG_OUT<<"\tgetOpImm: "<<ret<<"\n";
#endif // DEBUG_EXEC
  return ret;
}


void Ex86::disOperand(ECPUOperand *op)
{
  EMapSymbol *e=NULL;
  unsigned int ea=0;
  switch(op->mode) {
    case Ex86_NONE:
      break;
    case Ex86_SPECIAL:
      debugger("special mode in disOperand!");
      break;
    case Ex86_AX:
      debugger("ax mode in disOperand!");
      break;
    case Ex86_REG:
      printReg(op->reg,op->size);
      break;
    case Ex86_SEG:
      debugger("seg mode in disOperand!");
      break;
    case Ex86_INDREG:
      op->val=getOpDisp(op);
      if(op->val) DEBUG_OUT<<op->val;
      DEBUG_OUT<<"(";
      //printReg(op->reg,op->size);
      printReg(op->reg,curDataSize);
      DEBUG_OUT<<")";
      break;
    case Ex86_DISP:
      op->val=getOpDisp(op);
      DEBUG_OUT<<"($"<<op->val<<")";
      break;
    case Ex86_IMM:
      op->val=getOpImm(op);
      DEBUG_OUT<<"$"<<op->val;
      e=map->findAnyExactSymbol(op->val);
      if(e) {
        DEBUG_OUT<<" ("<<e->getName();
        DEBUG_OUT<<")";
      }
      break;
    case Ex86_BRANCH:
      op->val=getOpImm(op);
      DEBUG_OUT<<"$"<<op->val;
      ea=op->val+pc+op_len;
      e=map->findAnyNearestPrevSymbol(ea);
      if(e) {
        DEBUG_OUT<<" ("<<e->getName();
        if(e->addr!=ea) {
          DEBUG_OUT<<"+0x"<<(ea-(e->addr));
        }
        DEBUG_OUT<<")";
      }
      break;
    case Ex86_MODRM:
    case Ex86_MODRM8:
      debugger("modrm mode in disOperand!");
      break;
    case Ex86_MODREG:
    case Ex86_MODREG8:
      debugger("modreg mode in disOperand!");
      break;
    default:
      DEBUG_OUT<<"bad mod is "<<op->mode<<"\n";
      debugger("unknown mode in disOperand!");
      break;
  }
}


void Ex86::writeOperand(ECPUOperand *op,ULONG val)
{
  switch(op->mode) {
    case Ex86_NONE:
      break;
    case Ex86_SPECIAL:
      debugger("special mode in writeOperand!");
      break;
    case Ex86_AX:
      debugger("ax mode in writeOperand!");
      break;
    case Ex86_REG:
      writeReg(op->reg,val,op->size);
      break;
    case Ex86_SEG:
      debugger("seg mode in writeOperand!");
      break;
    case Ex86_INDREG:
      op->val=getOpDisp(op);
      //op->ea=readReg(op->reg,op->size)+op->val;
      op->ea=readReg(op->reg,curDataSize)+op->val;
      writeMem(op->ea,val,op->size);
      break;
    case Ex86_DISP:
      op->ea=getOpDisp(op);
      writeMem(op->ea,val,op->size);
      break;
    case Ex86_IMM:
      debugger("imm mode in writeOperand!");
      break;
    case Ex86_BRANCH:
      //op->val=getOpDisp(op);
      //writeMem(op->val,val,op->size);
      debugger("branch mode in writeOperand!");
      break;
    case Ex86_MODRM:
    case Ex86_MODRM8:
      debugger("modrm mode in writeOperand!");
      break;
    case Ex86_MODREG:
    case Ex86_MODREG8:
      debugger("modreg mode in writeOperand!");
      break;
    default:
      DEBUG_OUT<<"bad mod is "<<op->mode<<"\n";
      debugger("unknown mode in writeOperand!");
      break;
  }
}


ULONG Ex86::readOperand(ECPUOperand *op)
{
  switch(op->mode) {
    case Ex86_NONE:
      break;
    case Ex86_SPECIAL:
      debugger("special mode in readOperand!");
      break;
    case Ex86_AX:
      debugger("ax mode in readOperand!");
      break;
    case Ex86_REG:
      return readReg(op->reg,op->size);
      break;
    case Ex86_SEG:
      debugger("seg mode in readOperand!");
      break;
    case Ex86_INDREG:
      op->val=getOpDisp(op);
      //op->ea=readReg(op->reg,op->size)+op->val;
      op->ea=readReg(op->reg,curDataSize)+op->val;
      return readMem(op->ea,op->size);
      break;
    case Ex86_DISP:
      op->ea=getOpDisp(op);
      return readMem(op->ea,op->size);
      break;
    case Ex86_IMM:
      op->val=getOpImm(op);
      return op->val;
      break;
    case Ex86_BRANCH:
      op->val=getOpImm(op);
      return op->val;
      break;
    case Ex86_MODRM:
    case Ex86_MODRM8:
      debugger("modrm mode in readOperand!");
      break;
    case Ex86_MODREG:
    case Ex86_MODREG8:
      debugger("modreg mode in readOperand!");
      break;
    default:
      DEBUG_OUT<<"bad mod is "<<op->mode<<"\n";
      debugger("unknown mode in readOperand!");
      break;
  }
  return 0;
}


void Ex86::exec_plusseg(ULONG loc,unsigned int opcode)
{
  debugger("exec_plusseg not implemented!");
}


// Prefix
void Ex86::exec_lock(ULONG loc,unsigned int opcode)
{
  exec_unimplemented(loc,opcode);
}


// Prefix
void Ex86::exec_repnz(ULONG loc,unsigned int opcode)
{
  exec_unimplemented(loc,opcode);
}


// Prefix
void Ex86::exec_repz(ULONG loc,unsigned int opcode)
{
  exec_unimplemented(loc,opcode);
}


// Prefix
void Ex86::exec_es(ULONG loc,unsigned int opcode)
{
  exec_unimplemented(loc,opcode);
}


// Prefix
void Ex86::exec_cs(ULONG loc,unsigned int opcode)
{
  exec_unimplemented(loc,opcode);
}


// Prefix
void Ex86::exec_ss(ULONG loc,unsigned int opcode)
{
  exec_unimplemented(loc,opcode);
}


// Prefix
void Ex86::exec_ds(ULONG loc,unsigned int opcode)
{
  exec_unimplemented(loc,opcode);
}


// Prefix
void Ex86::exec_fs(ULONG loc,unsigned int opcode)
{
  exec_unimplemented(loc,opcode);
}


// Prefix
void Ex86::exec_gs(ULONG loc,unsigned int opcode)
{
  exec_unimplemented(loc,opcode);
}


// Prefix
void Ex86::exec_op_size(ULONG loc,unsigned int opcode)
{
  exec_unimplemented(loc,opcode);
}


// Prefix
void Ex86::exec_addr_size(ULONG loc,unsigned int opcode)
{
  exec_unimplemented(loc,opcode);
}


// NOTE: add and adc are the same for now...
void Ex86::exec_add(ULONG loc,unsigned int opcode)
{
  ULONG val1=readOperand(&op1);
  ULONG val2=readOperand(&op2);
  ULONG nval=val2+val1;
  writeOperand(&op2,nval);
  setFlagsNZ(nval,op2.size);
  setFlagsVC(val1,nval,val2,op2.size);
#ifdef DEBUG_EXEC
  DEBUG_OUT<<"\tADD "<<val1<<" + "<<val2<<" = "<<nval<<"\n";
#endif
}


void Ex86::exec_and(ULONG loc,unsigned int opcode)
{
  ULONG val1=readOperand(&op1);
  ULONG val2=readOperand(&op2);
  ULONG nval=val2&val1;
  writeOperand(&op2,nval);
  setFlagsNZ(nval,op2.size);
  setFlagsVC(val1,nval,val2,op2.size);
#ifdef DEBUG_EXEC
  DEBUG_OUT<<"\tAND "<<val1<<" + "<<val2<<" = "<<nval<<"\n";
#endif
}


void Ex86::exec_push(ULONG loc,unsigned int opcode)
{
  ULONG val=readOperand(&op2);
  push(val,Ex86_REGD_ESP,op2.size);
  // NOTE: x86 does NOT set any flags on a PUSH
}


void Ex86::exec_pop(ULONG loc,unsigned int opcode)
{
  ULONG val=pull(Ex86_REGD_ESP,op2.size);
  writeOperand(&op2,val);
}


void Ex86::exec_or(ULONG loc,unsigned int opcode)
{
  ULONG val1=readOperand(&op1);
  ULONG val2=readOperand(&op2);
  ULONG nval=val2|val1;
  writeOperand(&op2,nval);
  setFlagsNZ(nval,op2.size);
  setFlagsVC(val1,nval,val2,op2.size);
#ifdef DEBUG_EXEC
  DEBUG_OUT<<"\tOR "<<val1<<" + "<<val2<<" = "<<nval<<"\n";
#endif
}


// NOTE: add and adc are the same for now...
void Ex86::exec_adc(ULONG loc,unsigned int opcode)
{
  ULONG val1=readOperand(&op1);
  ULONG val2=readOperand(&op2);
  ULONG nval=val2&val1;
  writeOperand(&op2,nval);
  setFlagsNZ(nval,op2.size);
  setFlagsVC(val1,nval,val2,op2.size);
#ifdef DEBUG_EXEC
  DEBUG_OUT<<"\tADC "<<val1<<" + "<<val2<<" = "<<nval<<"\n";
#endif
}


void Ex86::exec_daa(ULONG loc,unsigned int opcode)
{
  exec_unimplemented(loc,opcode);
}


// NOTE: sub and sbb are the same for now...
void Ex86::exec_sub(ULONG loc,unsigned int opcode)
{
  ULONG val1=readOperand(&op1);
  ULONG val2=readOperand(&op2);
  ULONG nval=val2-val1;
  writeOperand(&op2,nval);
  setFlagsNZ(nval,op2.size);
  setFlagsVC(val1,nval,val2,op2.size);
#ifdef DEBUG_EXEC
  DEBUG_OUT<<"\tSUB "<<val1<<" + "<<val2<<" = "<<nval<<"\n";
#endif
}


// NOTE: sub and sbb are the same for now...
void Ex86::exec_sbb(ULONG loc,unsigned int opcode)
{
  ULONG val1=readOperand(&op1);
  ULONG val2=readOperand(&op2);
  ULONG nval=val2-val1;
  writeOperand(&op2,nval);
  setFlagsNZ(nval,op2.size);
  setFlagsVC(val1,nval,val2,op2.size);
#ifdef DEBUG_EXEC
  DEBUG_OUT<<"\tSBB "<<val1<<" + "<<val2<<" = "<<nval<<"\n";
#endif
}


void Ex86::exec_das(ULONG loc,unsigned int opcode)
{
  exec_unimplemented(loc,opcode);
}


void Ex86::exec_xor(ULONG loc,unsigned int opcode)
{
  ULONG val1=readOperand(&op1);
  ULONG val2=readOperand(&op2);
  ULONG nval=val2^val1;
  writeOperand(&op2,nval);
  setFlagsNZ(nval,op2.size);
  setFlagsVC(val1,nval,val2,op2.size);
#ifdef DEBUG_EXEC
  DEBUG_OUT<<"\tXOR "<<val1<<" + "<<val2<<" = "<<nval<<"\n";
#endif

}


void Ex86::exec_aaa(ULONG loc,unsigned int opcode)
{
  exec_unimplemented(loc,opcode);
}


// TODO: check order!
void Ex86::exec_cmp(ULONG loc,unsigned int opcode)
{
  ULONG val1=readOperand(&op1);
  ULONG val2=readOperand(&op2);
  ULONG nval=val2-val1;
  //writeOperand(&op2,nval);
  setFlagsNZ(nval,op2.size);
  setFlagsVC(val1,nval,val2,op2.size);
#ifdef DEBUG_EXEC
  DEBUG_OUT<<"\tCMP "<<val1<<" + "<<val2<<" = "<<nval<<"\n";
#endif
}


void Ex86::exec_aas(ULONG loc,unsigned int opcode)
{
  exec_unimplemented(loc,opcode);
}


void Ex86::exec_inc(ULONG loc,unsigned int opcode)
{
  ULONG val2=readOperand(&op2);
  ULONG nval=val2+1;
  writeOperand(&op2,nval);
  setFlagsNZ(nval,op2.size);
  setFlagsVC(1,nval,val2,op2.size);
#ifdef DEBUG_EXEC
  DEBUG_OUT<<"\tINC "<<val2<<" = "<<nval<<"\n";
#endif
}


void Ex86::exec_dec(ULONG loc,unsigned int opcode)
{
  ULONG val2=readOperand(&op2);
  ULONG nval=val2-1;
  writeOperand(&op2,nval);
  setFlagsNZ(nval,op2.size);
  setFlagsVC((ULONG)-1,nval,val2,op2.size);
#ifdef DEBUG_EXEC
  DEBUG_OUT<<"\tDEC "<<val2<<" = "<<nval<<"\n";
#endif
}


void Ex86::exec_pusha(ULONG loc,unsigned int opcode)
{
  exec_unimplemented(loc,opcode);
}


void Ex86::exec_popa(ULONG loc,unsigned int opcode)
{
  exec_unimplemented(loc,opcode);
}


void Ex86::exec_bound(ULONG loc,unsigned int opcode)
{
  exec_unimplemented(loc,opcode);
}


void Ex86::exec_arpl(ULONG loc,unsigned int opcode)
{
  exec_unimplemented(loc,opcode);
}


void Ex86::exec_imul(ULONG loc,unsigned int opcode)
{
  exec_unimplemented(loc,opcode);
}


void Ex86::exec_jmp(ULONG loc,unsigned int opcode)
{
  ULONG ea=readOperand(&op2)+pc+op_len;
  jump(ea);
}


void Ex86::exec_jo(ULONG loc,unsigned int opcode)
{
  ULONG ea=readOperand(&op2)+pc+op_len;
  if(eflags&Ex86_FLAG_V) {
    jump(ea);
  }
}


void Ex86::exec_jno(ULONG loc,unsigned int opcode)
{
  ULONG ea=readOperand(&op2)+pc+op_len;
  if(!(eflags&Ex86_FLAG_V)) {
    jump(ea);
  }
}


void Ex86::exec_jc(ULONG loc,unsigned int opcode)
{
  ULONG ea=readOperand(&op2)+pc+op_len;
  if(eflags&Ex86_FLAG_C) {
    jump(ea);
  }
}


void Ex86::exec_jnc(ULONG loc,unsigned int opcode)
{
  ULONG ea=readOperand(&op2)+pc+op_len;
  if(!(eflags&Ex86_FLAG_C)) {
    jump(ea);
  }
}


void Ex86::exec_jz(ULONG loc,unsigned int opcode)
{
  ULONG ea=readOperand(&op2)+pc+op_len;
  if(eflags&Ex86_FLAG_Z) {
    jump(ea);
  }
}


void Ex86::exec_jnz(ULONG loc,unsigned int opcode)
{
  ULONG ea=readOperand(&op2)+pc+op_len;
  if(!(eflags&Ex86_FLAG_Z)) {
    jump(ea);
  }
}


void Ex86::exec_jbe(ULONG loc,unsigned int opcode)
{
  ULONG ea=readOperand(&op2)+pc+op_len;
  if((eflags&Ex86_FLAG_Z)||(eflags&Ex86_FLAG_C)) {
    jump(ea);
  }
}


void Ex86::exec_jnbe(ULONG loc,unsigned int opcode)
{
  ULONG ea=readOperand(&op2)+pc+op_len;
  if(!((eflags&Ex86_FLAG_Z)||(eflags&Ex86_FLAG_C))) {
    jump(ea);
  }
}


void Ex86::exec_js(ULONG loc,unsigned int opcode)
{
  ULONG ea=readOperand(&op2)+pc+op_len;
  if(eflags&Ex86_FLAG_N) {
    jump(ea);
  }
}


void Ex86::exec_jns(ULONG loc,unsigned int opcode)
{
  ULONG ea=readOperand(&op2)+pc+op_len;
  if(!(eflags&Ex86_FLAG_N)) {
    jump(ea);
  }
}


void Ex86::exec_jpo(ULONG loc,unsigned int opcode)
{
  ULONG ea=readOperand(&op2)+pc+op_len;
  exec_unimplemented(loc,opcode);
}


void Ex86::exec_jpe(ULONG loc,unsigned int opcode)
{
  ULONG ea=readOperand(&op2)+pc+op_len;
  exec_unimplemented(loc,opcode);
}


void Ex86::exec_jnge(ULONG loc,unsigned int opcode)
{
  ULONG ea=readOperand(&op2)+pc+op_len;
  if(((eflags&Ex86_FLAG_N)!=(eflags&Ex86_FLAG_V))) {
    jump(ea);
  }
}


void Ex86::exec_jge(ULONG loc,unsigned int opcode)
{
  ULONG ea=readOperand(&op2)+pc+op_len;
  exec_unimplemented(loc,opcode);
}


void Ex86::exec_jng(ULONG loc,unsigned int opcode)
{
  ULONG ea=readOperand(&op2)+pc+op_len;
  if((eflags&Ex86_FLAG_Z)||((eflags&Ex86_FLAG_N)!=(eflags&Ex86_FLAG_V))) {
    jump(ea);
  }
}


void Ex86::exec_jg(ULONG loc,unsigned int opcode)
{
  ULONG ea=readOperand(&op2)+pc+op_len;
  if((!(eflags&Ex86_FLAG_Z))&&((eflags&Ex86_FLAG_N)==(eflags&Ex86_FLAG_V))) {
    jump(ea);
  }
}


void Ex86::exec_test(ULONG loc,unsigned int opcode)
{
  ULONG val1=readOperand(&op1);
  ULONG val2=readOperand(&op2);
  ULONG nval=val2&val1;
  setFlagsNZ(nval,op2.size);
  setCFlag(0);
  setVFlag(0);
#ifdef DEBUG_EXEC
  DEBUG_OUT<<"\tTEST "<<val1<<" + "<<val2<<" = "<<nval<<"\n";
#endif
}


void Ex86::exec_xchg(ULONG loc,unsigned int opcode)
{
  ULONG val1=readOperand(&op1);
  ULONG val2=readOperand(&op2);
  writeOperand(&op1,val2);
  writeOperand(&op2,val1);
#ifdef DEBUG_EXEC
  DEBUG_OUT<<"\tXCHG "<<val1<<" and "<<val2<<"\n";
#endif
}


void Ex86::exec_mov(ULONG loc,unsigned int opcode)
{
  ULONG val1=readOperand(&op1);
  writeOperand(&op2,val1);
#ifdef DEBUG_EXEC
  DEBUG_OUT<<"\tMOV "<<val1<<" to ???\n";
#endif
  // NOTE: x86 does NOT set any flags on a mov
}


void Ex86::exec_lea(ULONG loc,unsigned int opcode)
{
  readOperand(&op1);
  ULONG val1=op1.ea;
  writeOperand(&op2,val1);
#ifdef DEBUG_EXEC
  DEBUG_OUT<<"\tLEA "<<val1<<" to "<<op2.reg<<"\n";
#endif
}


void Ex86::exec_nop(ULONG loc,unsigned int opcode)
{
  //exec_unimplemented(loc,opcode);
}


void Ex86::exec_cbw(ULONG loc,unsigned int opcode)
{
  exec_unimplemented(loc,opcode);
}


void Ex86::exec_cwd(ULONG loc,unsigned int opcode)
{
  exec_unimplemented(loc,opcode);
}


void Ex86::exec_call(ULONG loc,unsigned int opcode)
{
  ULONG ea=readOperand(&op2)+pc+op_len;
  jsr(ea);
}


void Ex86::exec_wait(ULONG loc,unsigned int opcode)
{
  exec_unimplemented(loc,opcode);
}


void Ex86::exec_pushf(ULONG loc,unsigned int opcode)
{
  exec_unimplemented(loc,opcode);
}


void Ex86::exec_popf(ULONG loc,unsigned int opcode)
{
  exec_unimplemented(loc,opcode);
}


void Ex86::exec_sahf(ULONG loc,unsigned int opcode)
{
  exec_unimplemented(loc,opcode);
}


void Ex86::exec_lahf(ULONG loc,unsigned int opcode)
{
  exec_unimplemented(loc,opcode);
}


void Ex86::exec_movsb(ULONG loc,unsigned int opcode)
{
  exec_unimplemented(loc,opcode);
}


void Ex86::exec_movsw(ULONG loc,unsigned int opcode)
{
  exec_unimplemented(loc,opcode);
}


void Ex86::exec_cmpsb(ULONG loc,unsigned int opcode)
{
  exec_unimplemented(loc,opcode);
}


void Ex86::exec_cmpsw(ULONG loc,unsigned int opcode)
{
  exec_unimplemented(loc,opcode);
}


void Ex86::exec_stosb(ULONG loc,unsigned int opcode)
{
  exec_unimplemented(loc,opcode);
}


void Ex86::exec_stosw(ULONG loc,unsigned int opcode)
{
  exec_unimplemented(loc,opcode);
}


void Ex86::exec_lodsb(ULONG loc,unsigned int opcode)
{
  exec_unimplemented(loc,opcode);
}


void Ex86::exec_lodsw(ULONG loc,unsigned int opcode)
{
  exec_unimplemented(loc,opcode);
}


void Ex86::exec_scasb(ULONG loc,unsigned int opcode)
{
  exec_unimplemented(loc,opcode);
}


void Ex86::exec_scasw(ULONG loc,unsigned int opcode)
{
  exec_unimplemented(loc,opcode);
}


void Ex86::exec_ret(ULONG loc,unsigned int opcode)
{
  rts();
}


void Ex86::exec_les(ULONG loc,unsigned int opcode)
{
  exec_unimplemented(loc,opcode);
}


void Ex86::exec_lds(ULONG loc,unsigned int opcode)
{
  exec_unimplemented(loc,opcode);
}


void Ex86::exec_enter(ULONG loc,unsigned int opcode)
{
  exec_unimplemented(loc,opcode);
}


void Ex86::exec_leave(ULONG loc,unsigned int opcode)
{
  ULONG val=readReg(Ex86_REGD_EBP,curDataSize);
  writeReg(Ex86_REGD_ESP,val,curDataSize);
  ULONG nval=pull(Ex86_REGD_ESP,curDataSize);
  writeReg(Ex86_REGD_EBP,nval,curDataSize);
#ifdef DEBUG_EXEC
  DEBUG_OUT<<"\tleave: val="<<val<<" nval="<<nval<<"\n";
#endif
}



void Ex86::exec_int(ULONG loc,unsigned int opcode)
{
  bool ret=false;
  if(io) {
    unsigned int vector=readOperand(&op2);
    ret=io->systemCall(vector);
  }
  if(!ret) exec_unimplemented(loc,opcode);
}


void Ex86::exec_into(ULONG loc,unsigned int opcode)
{
  exec_unimplemented(loc,opcode);
}


void Ex86::exec_iret(ULONG loc,unsigned int opcode)
{
  exec_unimplemented(loc,opcode);
}


void Ex86::exec_xlat(ULONG loc,unsigned int opcode)
{
  exec_unimplemented(loc,opcode);
}


void Ex86::exec_loopnz(ULONG loc,unsigned int opcode)
{
  exec_unimplemented(loc,opcode);
}


void Ex86::exec_loopz(ULONG loc,unsigned int opcode)
{
  exec_unimplemented(loc,opcode);
}


void Ex86::exec_loop(ULONG loc,unsigned int opcode)
{
  exec_unimplemented(loc,opcode);
}


void Ex86::exec_jcxz(ULONG loc,unsigned int opcode)
{
  exec_unimplemented(loc,opcode);
}


void Ex86::exec_in(ULONG loc,unsigned int opcode)
{
  exec_unimplemented(loc,opcode);
}


void Ex86::exec_insb(ULONG loc,unsigned int opcode)
{
  exec_unimplemented(loc,opcode);
}


void Ex86::exec_insw(ULONG loc,unsigned int opcode)
{
  exec_unimplemented(loc,opcode);
}


void Ex86::exec_outsb(ULONG loc,unsigned int opcode)
{
  exec_unimplemented(loc,opcode);
}


void Ex86::exec_outsw(ULONG loc,unsigned int opcode)
{
  exec_unimplemented(loc,opcode);
}


void Ex86::exec_out(ULONG loc,unsigned int opcode)
{
  exec_unimplemented(loc,opcode);
}


void Ex86::exec_hlt(ULONG loc,unsigned int opcode)
{
  exec_unimplemented(loc,opcode);
}


void Ex86::exec_cmc(ULONG loc,unsigned int opcode)
{
  exec_unimplemented(loc,opcode);
}


void Ex86::exec_clc(ULONG loc,unsigned int opcode)
{
  resetFlagBits(Ex86_FLAG_C);
}


void Ex86::exec_stc(ULONG loc,unsigned int opcode)
{
  setFlagBits(Ex86_FLAG_C);
}


void Ex86::exec_cli(ULONG loc,unsigned int opcode)
{
  resetFlagBits(Ex86_FLAG_I);
}


void Ex86::exec_sti(ULONG loc,unsigned int opcode)
{
  setFlagBits(Ex86_FLAG_I);
}


void Ex86::exec_cld(ULONG loc,unsigned int opcode)
{
  resetFlagBits(Ex86_FLAG_D);
}


void Ex86::exec_std(ULONG loc,unsigned int opcode)
{
  setFlagBits(Ex86_FLAG_D);
}


void Ex86::exec_lar(ULONG loc,unsigned int opcode)
{
  exec_unimplemented(loc,opcode);
}


void Ex86::exec_lsl(ULONG loc,unsigned int opcode)
{
  exec_unimplemented(loc,opcode);
}


void Ex86::exec_clts(ULONG loc,unsigned int opcode)
{
  exec_unimplemented(loc,opcode);
}


void Ex86::exec_invd(ULONG loc,unsigned int opcode)
{
  exec_unimplemented(loc,opcode);
}


void Ex86::exec_wbinvd(ULONG loc,unsigned int opcode)
{
  exec_unimplemented(loc,opcode);
}


void Ex86::exec_setc(ULONG loc,unsigned int opcode)
{
  exec_unimplemented(loc,opcode);
}


void Ex86::exec_setnc(ULONG loc,unsigned int opcode)
{
  exec_unimplemented(loc,opcode);
}


void Ex86::exec_setz(ULONG loc,unsigned int opcode)
{
  exec_unimplemented(loc,opcode);
}


void Ex86::exec_setnz(ULONG loc,unsigned int opcode)
{
  exec_unimplemented(loc,opcode);
}


void Ex86::exec_setbe(ULONG loc,unsigned int opcode)
{
  exec_unimplemented(loc,opcode);
}


void Ex86::exec_setnbe(ULONG loc,unsigned int opcode)
{
  exec_unimplemented(loc,opcode);
}


void Ex86::exec_sets(ULONG loc,unsigned int opcode)
{
  exec_unimplemented(loc,opcode);
}


void Ex86::exec_setns(ULONG loc,unsigned int opcode)
{
  exec_unimplemented(loc,opcode);
}


void Ex86::exec_setpe(ULONG loc,unsigned int opcode)
{
  exec_unimplemented(loc,opcode);
}


void Ex86::exec_setpo(ULONG loc,unsigned int opcode)
{
  exec_unimplemented(loc,opcode);
}


void Ex86::exec_setnge(ULONG loc,unsigned int opcode)
{
  exec_unimplemented(loc,opcode);
}


void Ex86::exec_setge(ULONG loc,unsigned int opcode)
{
  exec_unimplemented(loc,opcode);
}


void Ex86::exec_setng(ULONG loc,unsigned int opcode)
{
  exec_unimplemented(loc,opcode);
}


void Ex86::exec_setg(ULONG loc,unsigned int opcode)
{
  exec_unimplemented(loc,opcode);
}


void Ex86::exec_bt(ULONG loc,unsigned int opcode)
{
  exec_unimplemented(loc,opcode);
}


void Ex86::exec_bts(ULONG loc,unsigned int opcode)
{
  exec_unimplemented(loc,opcode);
}


void Ex86::exec_btr(ULONG loc,unsigned int opcode)
{
  exec_unimplemented(loc,opcode);
}


void Ex86::exec_btc(ULONG loc,unsigned int opcode)
{
  exec_unimplemented(loc,opcode);
}


void Ex86::exec_bsf(ULONG loc,unsigned int opcode)
{
  exec_unimplemented(loc,opcode);
}


void Ex86::exec_bsr(ULONG loc,unsigned int opcode)
{
  exec_unimplemented(loc,opcode);
}


void Ex86::exec_shld(ULONG loc,unsigned int opcode)
{
  exec_unimplemented(loc,opcode);
}


void Ex86::exec_shrd(ULONG loc,unsigned int opcode)
{
  exec_unimplemented(loc,opcode);
}


void Ex86::exec_cmpxchg(ULONG loc,unsigned int opcode)
{
  exec_unimplemented(loc,opcode);
}


void Ex86::exec_lss(ULONG loc,unsigned int opcode)
{
  exec_unimplemented(loc,opcode);
}


void Ex86::exec_lfs(ULONG loc,unsigned int opcode)
{
  exec_unimplemented(loc,opcode);
}


void Ex86::exec_lgs(ULONG loc,unsigned int opcode)
{
  exec_unimplemented(loc,opcode);
}


// TODO: only handles byte
void Ex86::exec_movzx(ULONG loc,unsigned int opcode)
{
  ULONG val1=readOperand(&op1);
  val1=val1&0xff;
  writeOperand(&op2,val1);
#ifdef DEBUG_EXEC
  DEBUG_OUT<<"\tMOVZX "<<val1<<" to ???\n";
#endif
  // NOTE: x86 does NOT set any flags on a mov
}


// TODO: only handles byte
void Ex86::exec_movsx(ULONG loc,unsigned int opcode)
{
  ULONG val1=readOperand(&op1);
  val1=sign_ext(val1,8);
  writeOperand(&op2,val1);
#ifdef DEBUG_EXEC
  DEBUG_OUT<<"\tMOVSX "<<val1<<" to ???\n";
#endif
  // NOTE: x86 does NOT set any flags on a mov
}



void Ex86::exec_xadd(ULONG loc,unsigned int opcode)
{
  exec_unimplemented(loc,opcode);
}



void Ex86::exec_bswap(ULONG loc,unsigned int opcode)
{
  exec_unimplemented(loc,opcode);
}



