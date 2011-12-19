
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


// STATUS:
// Tiny linux example code runs.
// Tiny solaris sparc not built yet.

//#define DEBUG_EXEC 1

#define CHECK_SAFE 1

#define SHOW_INTS  1


#include <cpulib/ESparc.h>

#include <elib/EMemMapAbstract.h>
#include <elib/EModule.h>
#include <elib/EHandler.h>

#include <portable/nickcpp/ANullStream.h>

#include <portable/nickcpp/AStringHelper.h>


#define ESPARC_REG_GLOBAL 0
#define ESPARC_REG_OUTPUT 8
#define ESPARC_REG_LOCAL  16
#define ESPARC_REG_INPUT  24


#define ESPARC_FLOAT_SINGLE 0
#define ESPARC_FLOAT_DOUBLE 1
#define ESPARC_FLOAT_QUAD   2


ESparc::ESparc()
{
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"ESparc created...\n";
#endif // DEBUG_VERBOSE
  init();
  //w=ww;
  reset();
}


ESparc::~ESparc()
{
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"ESparc destroyed.\n";
#endif // DEBUG_VERBOSE
}


void ESparc::init()
{
  ECPURisc::init();
  bitness=32;
  branchesAreDelayed=true;
  r0IsZero=true;
  doTrace=false;  doRun=false;
  lsb=false;
  setStack(ECPUABSTRACT_DEFAULT_STACK);
  setFramePtr(ECPUABSTRACT_DEFAULT_STACK);
  setResetStack(ECPUABSTRACT_DEFAULT_STACK);
  setResetEntry(ECPUABSTRACT_DEFAULT_CODE);
}


/* STATIC */
bool ESparc::recognize(const char *tname)
{
  const char *name=aDefaultStringHelper.removeSpaces2(aDefaultStringHelper.toLower(tname));
  if(!strcmp(name,"sun4")) return true; else
  if(!strcmp(name,"sun4c")) return true; else
  if(!strcmp(name,"sun4m")) return true; else
  if(!strcmp(name,"sun4u")) return true; else
  if(!strcmp(name,"sun4v")) return true; else
  if(!strcmp(name,"sparc")) return true; else
  if(!strcmp(name,"sparc1")) return true; else
  if(!strcmp(name,"sparc2")) return true; else
  if(!strcmp(name,"sparc3")) return true; else
  if(!strcmp(name,"sparc4")) return true; else
  if(!strcmp(name,"sparc5")) return true; else
  if(!strcmp(name,"sparc6")) return true; else
  if(!strcmp(name,"sparc7")) return true; else
  if(!strcmp(name,"sparcv7")) return true; else
  if(!strcmp(name,"sparc8")) return true; else
  if(!strcmp(name,"sparcv8")) return true; else
  if(!strcmp(name,"sparc9")) return true; else
  if(!strcmp(name,"sparcv9")) return true; else
  if(!strcmp(name,"sparc32")) return true; else
  if(!strcmp(name,"sparc32plus")) return true; else
  if(!strcmp(name,"sparc64")) return true; else
  if(!strcmp(name,"microsparc")) return true; else
  if(!strcmp(name,"microsparc2")) return true; else
  if(!strcmp(name,"sparclite")) return true; else
  if(!strcmp(name,"hypersparc")) return true; else
  if(!strcmp(name,"supersparc")) return true; else
  if(!strcmp(name,"utlrasparc")) return true; else
  if(!strcmp(name,"utlrasparc1")) return true; else
  if(!strcmp(name,"utlrasparc2")) return true; else
  if(!strcmp(name,"utlrasparc3")) return true; else
  if(!strcmp(name,"cypress")) return true; else
  return false;
}


const char *ESparc::getName()
{
  return "SunSparc";
}


void ESparc::setKind(const char *tname)
{
  const char *name=aDefaultStringHelper.removeSpaces2(aDefaultStringHelper.toLower(tname));
  bitness=32;
  if(!strcmp(name,"sun4u")) bitness=64;
  if(!strcmp(name,"sun4v")) bitness=64;
  if(!strcmp(name,"sparc9")) bitness=64;
  if(!strcmp(name,"sparcv9")) bitness=64;
  if(!strcmp(name,"sparc64")) bitness=64;
  if(!strcmp(name,"utlrasparc")) bitness=64;
  if(!strcmp(name,"utlrasparc1")) bitness=64;
  if(!strcmp(name,"utlrasparc2")) bitness=64;
  if(!strcmp(name,"utlrasparc3")) bitness=64;
}


void ESparc::reset()
{
  unsigned int t;
  pc=0;
  npc=pc+4;
  CWP=NUM_WINDOWS-1;
  for(t=0;t<8;t++) { G[t]=0; }
  for(t=0;t<32;t++) { F[t]=0.0; }
  for(t=0;t<NUM_REG;t++) { R[t]=0; }
  for(t=0;t<NUM_WINDOWS;t++) { dirty[t]=0; }
  CC.N=0; CC.Z=0; CC.V=0; CC.C=0;
  y=0;
  wim=0;
  tbr=0;
  rs1=0;  rs2=0;  rd=0;  rd=0;  simm13=0;
  ibit=0;  imm22=0;  disp30=0;
  anull=false;
  hasFPU=true;
  hasMMU=true;
  lsb=false; // But we can swtich...right?
  // TODO: where does a sparc normally go on reset?
  clearStackTrace();
  ECPUAbstract::reset();
}


bool ESparc::setRegister(const char *tname,ULONG value)
{
  const char *name=aDefaultStringHelper.removeSpaces2(aDefaultStringHelper.toLower(tname));
  bool ret=false;
  if(!strcmp(name,"pc"))
  { pc=value; ret=true; } else
  if(!strcmp(name,"sp"))
  { setStack(value); ret=true; } else
  if(!strcmp(name,"npc"))
  { npc=value; ret=true; } else
  if(!strcmp(name,"cwp"))
  { CWP=value; ret=true; } else
  if(name[0]=='g') {
    unsigned int n=name[1]-'0';
    writeReg(ESPARC_REG_GLOBAL+n,value);
    ret=true;
  } else
  if(name[0]=='o') {
    unsigned int n=name[1]-'0';
    writeReg(ESPARC_REG_OUTPUT+n,value);
    ret=true;
  } else
  if(name[0]=='l') {
    unsigned int n=name[1]-'0';
    writeReg(ESPARC_REG_LOCAL+n,value);
    ret=true;
  } else
  if(name[0]=='i') {
    unsigned int n=name[1]-'0';
    writeReg(ESPARC_REG_INPUT+n,value);
    ret=true;
  } else
  ret=false;
  return ret;
}


void ESparc::wregs()
{
  //unsigned int d=w->getHexDigits();
  //w->setHexDigits(8);
  DEBUG_OUT<<"PC= "<<pc<<" nPC="<<npc<<"\n";
  //DEBUG_OUT<<"Y="<<y<<"\n";
  DEBUG_OUT<<"CWP="<<CWP<<" WIM="<<wim<<"\n";
  DEBUG_OUT<<"TBR="<<tbr<<"\n";
  printRegWindow();
  DEBUG_OUT<<"CC=";
  if(CC.N) DEBUG_OUT<<"N";  else DEBUG_OUT<<"n";
  if(CC.Z) DEBUG_OUT<<"Z";  else DEBUG_OUT<<"z";
  if(CC.V) DEBUG_OUT<<"V";  else DEBUG_OUT<<"v";
  if(CC.C) DEBUG_OUT<<"C";  else DEBUG_OUT<<"c";
  DEBUG_OUT<<"\n";
  //w->setHexDigits(d);
}


void ESparc::setStack(ULONG val)
{
  writeReg(ESPARC_REG_OUTPUT+6,val);  // O6==SP
}


void ESparc::setFramePtr(ULONG val)
{
  writeReg(ESPARC_REG_INPUT+6,val);  // I6==FP
}


ULONG ESparc::readStack()
{
  return readReg(ESPARC_REG_OUTPUT+6);  // O6==SP
}


ULONG ESparc::readFramePtr()
{
  return readReg(ESPARC_REG_INPUT+6);  // I6==FP
}


// TODO: This in the called func only, its different (%O0) for the caller
void ESparc::setReturnReg(ULONG val)
{
  writeReg(ESPARC_REG_INPUT+0,val);  // I0
}


// TODO: This in the called func only, its different (%O0) for the caller
ULONG ESparc::readReturnReg()
{
  return readReg(ESPARC_REG_INPUT+0);  // I0
}


ULONG ESparc::readParamReg(unsigned int num)
{
  num&=0x7;
  return readReg(ESPARC_REG_INPUT+num);
}


void ESparc::setParamReg(unsigned int num,ULONG val)
{
  num&=0x7;
  writeReg(ESPARC_REG_INPUT+num,val);
}


bool ESparc::interp(const char *cmd,const char *a1,const char *a2,const char *a3)
{
  bool ret=false;
  if(!strcmp(cmd,"rm")) { wmregs(); ret=true; }
  if(!strcmp(cmd,"rf")) { wfregs(); ret=true; }
  return ret;
}


void ESparc::wmregs()
{
  //unsigned int dig=w->getHexDigits();
  //w->setHexDigits(8);
  // What goes here?
  //w->setHexDigits(dig);
}


void ESparc::wfregs()
{
  if(!hasFPU) return;
  //unsigned int dig=w->getHexDigits();
  //w->setHexDigits(1);
  //w->dec();
  unsigned int t;
  // NOTE: That stuff with '0'+t is to get them to display with only one digit
  for(t=0;t<32;t++) {
    DEBUG_OUT<<"F";
    if(t<10) DEBUG_OUT<<"0";
    DEBUG_OUT<<t<<"="<<F[t]<<" ";
    if(!((t+1)%3)) { DEBUG_OUT<<"\n"; }
  }
  DEBUG_OUT<<"\n";
  //w->hex();
  //w->setHexDigits(dig);
}


void ESparc::handle_int(ULONG value)
{
  // Need to handle these...
}


void ESparc::cause_int(ULONG value)
{
  // Need to handle these...
}


ULONG ESparc::disasm(ULONG loc)
{
  bool old=doTrace;
  doTrace=true;
  unsigned int oldPC=pc;
  pc=loc;
  ir=read32mem(pc);
  lastSymbol=map->findAnyExactSymbol(pc);
  if(lastSymbol) DEBUG_OUT<<lastSymbol->getName()<<":\n";
  DEBUG_OUT<<"";
  DEBUG_OUT<<"."<<pc<<" ";
  //unsigned int d=w->getHexDigits();
  //w->setHexDigits(8);
  DEBUG_OUT<<ir<<" ";
  //w->setHexDigits(d);
  op=ir;
  unsigned int top=(op>>30)&0x3;
  switch(top) {
    case 0: disOp0(); break;
    case 1: disOp1(); break;
    case 2: disOp2(); break;
    case 3: disOp3(); break;
    default:
      DEBUG_OUT<<"top=="<<top<<" ";
      dis_illegal("disasm");
      break;
  }
  doTrace=old;
  pc=oldPC;
  DEBUG_OUT<<"";
  DEBUG_OUT<<"\n";
  return 4;
}


void ESparc::execute(void)
{
  if(pc==breakpoint) { debugger("Breakpoint!\n"); return; }
  doRun=true;
  ir=read32mem(pc);
  op=ir;
  op_len=4;
  tookBranch=false;
  unsigned int top=(op>>30)&0x3;
  switch(top) {
    case 0: execOp0(); break;
    case 1: execOp1(); break;
    case 2: execOp2(); break;
    case 3: execOp3(); break;
    default:
      DEBUG_OUT<<"top=="<<top<<"\n";
      exec_invalid("sparcExec");
      break;
  }
  pc=npc;
  npc+=4;
  if(tookBranch) npc=branch_address;
  if(anull) { pc=npc; npc=pc+4; anull=false; }
  if(int_pending) handle_int(int_pending);
}


// TODO: implement the a bit
// TODO: implement cc0 and cc1 and p for p=true (prediction)
void ESparc::execBICC(bool p)
{
  tookBranch=false;
  unsigned int abit=(op>>29)&0x1;
  unsigned int cond=rd&0x0f;
  switch(cond) {
    case 0x0: // BN (never)
      tookBranch=false; break;
    case 0x1: // BE
      tookBranch=CC.Z; break;
    case 0x2: // BLE
      if(CC.N) tookBranch=true;
      if(CC.V) { if(tookBranch) tookBranch=false; else tookBranch=true; }
      if(CC.Z) tookBranch=true;
      break;
    case 0x3: // BL
      if(CC.N) tookBranch=true;
      if(CC.V) { if(tookBranch) tookBranch=false; else tookBranch=true; }
      break;
    case 0x4: // BLEU
      tookBranch=CC.C||CC.Z; break;
    case 0x5: // BCS
      tookBranch=CC.C; break;
    case 0x6: // BNEG
      tookBranch=CC.N; break;
    case 0x7: // BVS
      tookBranch=CC.V; break;
    case 0x8: // BA (always)
      tookBranch=true; break;
    case 0x9:  // BNE
      tookBranch=!CC.Z; break;
    case 0xa:  // BG
      tookBranch=(!CC.Z)&&(!CC.N); break;  // TODO: CHeck that this is correct
    case 0xb:  // BGE
      if(CC.N) tookBranch=true;
      if(CC.V) { if(tookBranch) tookBranch=false; else tookBranch=true; }
      tookBranch=!tookBranch;
      break;
    case 0xc:  // BGU
      tookBranch=!(CC.Z||CC.C); break;
    case 0xd: // BCC
      tookBranch=!CC.C; break;
    case 0xe: // BPOS
      tookBranch=!CC.N; break;
    case 0xf: // BVC
      tookBranch=!CC.V; break;
    default:
      DEBUG_OUT<<"BICC Bad cond="<<rd<<"\n";
      debugger("Error!\n");
      break;
  }
  if(abit) anull=true;
  if(tookBranch) {
    if(p) branch_address=pc+4*sign_ext(imm22&0x7ffff,19);
    else branch_address=pc+4*sign_ext(imm22,22);
    willJump(branch_address);
  }
}


// TODO: implement cc0 and cc1 for p=true (prediction)
void ESparc::disBICC(bool p)
{
  if(doTrace) {
    if(p) branch_address=pc+4*sign_ext(imm22&0x7ffff,19);
    else branch_address=pc+4*sign_ext(imm22,22);
    DEBUG_OUT<<"b";
    if(p) DEBUG_OUT<<"p";
    unsigned int abit=(op>>29)&0x1;
    switch(rd&0xf) {
      case 0x0: DEBUG_OUT<<"n"; break;  // Branch never
      case 0x1: DEBUG_OUT<<"e"; break;
      case 0x2: DEBUG_OUT<<"le"; break;
      case 0x3: DEBUG_OUT<<"l"; break;
      case 0x4: DEBUG_OUT<<"leu"; break;
      case 0x5: DEBUG_OUT<<"cs"; break;
      case 0x6: DEBUG_OUT<<"neg"; break;
      case 0x7: DEBUG_OUT<<"vs"; break;
      case 0x8: DEBUG_OUT<<"a"; break;  // Branch always
      case 0x9: DEBUG_OUT<<"ne"; break;
      case 0xa: DEBUG_OUT<<"g"; break;
      case 0xb: DEBUG_OUT<<"ge"; break;
      case 0xc: DEBUG_OUT<<"gu"; break;
      case 0xd: DEBUG_OUT<<"cc"; break;
      case 0xe: DEBUG_OUT<<"pos"; break;
      case 0xf: DEBUG_OUT<<"vc"; break;
      default: DEBUG_OUT<<"??"; break;
    }
    if(abit) DEBUG_OUT<<",a";
    DEBUG_OUT<<" $"<<branch_address;
    EMapSymbol *e=map->findAnyNearestPrevSymbol(branch_address);
    if(e) {
      DEBUG_OUT<<" ("<<e->getName();
      if(e->addr!=branch_address) {
        DEBUG_OUT<<"+0x"<<(branch_address-(e->addr));
      }
      DEBUG_OUT<<")";
    }
  }
}


void ESparc::execSETHI()
{
  writeReg(rd,sign_ext(imm22,22)<<10);
}


void ESparc::disSETHI()
{
  DEBUG_OUT<<"sethi $"<<sign_ext(imm22,22)<<",";
  printReg(rd);
}


// TODO: use jump() so we can trace branches
void ESparc::execCALL()
{
  branch_address=pc+4*disp30;
  writeReg(ESPARC_REG_OUTPUT+7,pc);
  tookBranch=true;
}


void ESparc::disCALL()
{
  branch_address=pc+4*disp30;
  DEBUG_OUT<<"call $"<<branch_address;
  EMapSymbol *e=map->findAnyNearestPrevSymbol(branch_address);
  if(e) {
    DEBUG_OUT<<" ("<<e->getName();
    if(e->addr!=branch_address) {
      DEBUG_OUT<<"+0x"<<(branch_address-(e->addr));
    }
    DEBUG_OUT<<")";
  }
}


void ESparc::execADD()
{
  ULONG val=readReg(rs1)+getS2();
  writeReg(rd,val);
}


void ESparc::disADD()
{
  DEBUG_OUT<<"add "; printReg(rs1); DEBUG_OUT<<",";
  printS2(); DEBUG_OUT<<","; printReg(rd);
}


void ESparc::execSUB()
{
  ULONG val=readReg(rs1)-getS2();
  writeReg(rd,val);
}


void ESparc::disSUB()
{
  DEBUG_OUT<<"sub "; printReg(rs1); DEBUG_OUT<<",";
  printS2(); DEBUG_OUT<<","; printReg(rd);
}


void ESparc::execSUBCC()
{
  ULONG val=readReg(rs1)-getS2();
  unsigned int a31=readReg(rs1)&ECPUABSTRACT_TOP_LONG;
  unsigned int b31=getS2()&ECPUABSTRACT_TOP_LONG;
  unsigned int r31=val&ECPUABSTRACT_TOP_LONG;
  CC.Z=(val==0);
  CC.N=(r31);
  CC.V=(a31&&(!b31)&&(!r31))||((!a31)&&b31&&r31);
  CC.C=((!a31)&&b31)||(b31&&r31)||(r31&&(!a31));
  writeReg(rd,val);
}


void ESparc::disSUBCC()
{
  DEBUG_OUT<<"subcc "; printReg(rs1); DEBUG_OUT<<",";
  printS2(); DEBUG_OUT<<","; printReg(rd);
}


void ESparc::execAND()
{
  ULONG val=readReg(rs1)&getS2();
  writeReg(rd,val);
}


void ESparc::disAND()
{
  DEBUG_OUT<<"and "; printReg(rs1); DEBUG_OUT<<",";
  printS2(); DEBUG_OUT<<","; printReg(rd);
}


void ESparc::execANDCC()
{
  ULONG val=readReg(rs1)&getS2();
  unsigned int a31=readReg(rs1)&ECPUABSTRACT_TOP_LONG;
  unsigned int b31=getS2()&ECPUABSTRACT_TOP_LONG;
  unsigned int r31=val&ECPUABSTRACT_TOP_LONG;
  CC.Z=(val==0);
  CC.N=(r31);
  CC.V=false;
  writeReg(rd,val);
}


void ESparc::disANDCC()
{
  DEBUG_OUT<<"andcc "; printReg(rs1); DEBUG_OUT<<",";
  printS2(); DEBUG_OUT<<","; printReg(rd);
}


void ESparc::execORCC()
{
  ULONG val=readReg(rs1)|getS2();
  unsigned int a31=readReg(rs1)&ECPUABSTRACT_TOP_LONG;
  unsigned int b31=getS2()&ECPUABSTRACT_TOP_LONG;
  unsigned int r31=val&ECPUABSTRACT_TOP_LONG;
  CC.Z=(val==0);
  CC.N=(r31);
  CC.V=false;
  writeReg(rd,val);
}


void ESparc::disORCC()
{
  DEBUG_OUT<<"orcc "; printReg(rs1); DEBUG_OUT<<",";
  printS2(); DEBUG_OUT<<","; printReg(rd);
}


void ESparc::execOR()
{
  ULONG val=readReg(rs1)|getS2();
  writeReg(rd,val);
}


void ESparc::execXOR()
{
  ULONG val=readReg(rs1)^getS2();
  writeReg(rd,val);
}


void ESparc::disOR()
{
  DEBUG_OUT<<"or "; printReg(rs1); DEBUG_OUT<<",";
  printS2(); DEBUG_OUT<<","; printReg(rd);
}


void ESparc::disXOR()
{
  DEBUG_OUT<<"xor "; printReg(rs1); DEBUG_OUT<<",";
  printS2(); DEBUG_OUT<<","; printReg(rd);
}


// 64 bit only
void ESparc::execMULX()
{
  ULONG val=readReg(rs1)*getS2();
  writeReg(rd,val);
}


// 64 bit only
void ESparc::disMULX()
{
  DEBUG_OUT<<"mulx "; printReg(rs1); DEBUG_OUT<<",";
  printS2(); DEBUG_OUT<<","; printReg(rd);
}


// TODO: regs may be wrong...
void ESparc::execTICC()
{
  unsigned int execTrap=false;
  switch(rd) {
    case 8: execTrap=true; break;
    default:
      DEBUG_OUT<<"TICC Bad cond="<<rd<<"\n";
      debugger("Error!\n");
      break;
  }
  if(execTrap) {
    unsigned int num=readReg(rs1);
    if(ibit) num+=(simm13&0x7f);
    else num+=readReg(rs2);
    num=num&0xff;
    bool ret=false;
    if(io) ret=io->systemCall(num);
    if(!ret) {
      DEBUG_OUT<<"TRAP #"<<num<<"\n";
      debugger();
    }
  }
}


void ESparc::disTICC()
{
  DEBUG_OUT<<"t";
  switch(rd&0xf) {
    case 0x0: DEBUG_OUT<<"n"; break;  // Branch never
    case 0x1: DEBUG_OUT<<"e"; break;
    case 0x2: DEBUG_OUT<<"le"; break;
    case 0x3: DEBUG_OUT<<"l"; break;
    case 0x4: DEBUG_OUT<<"leu"; break;
    case 0x5: DEBUG_OUT<<"cs"; break;
    case 0x6: DEBUG_OUT<<"neg"; break;
    case 0x7: DEBUG_OUT<<"vs"; break;
    case 0x8: DEBUG_OUT<<"a"; break;  // Branch always
    case 0x9: DEBUG_OUT<<"ne"; break;
    case 0xa: DEBUG_OUT<<"g"; break;
    case 0xb: DEBUG_OUT<<"ge"; break;
    case 0xc: DEBUG_OUT<<"gu"; break;
    case 0xd: DEBUG_OUT<<"cc"; break;
    case 0xe: DEBUG_OUT<<"pos"; break;
    case 0xf: DEBUG_OUT<<"vc"; break;
    default: DEBUG_OUT<<"??"; break;
  }
  DEBUG_OUT<<" ";
  printReg(rs1); DEBUG_OUT<<",";
  if(ibit) DEBUG_OUT<<"#"<<(simm13&0x7f);
  else printS2();
}


void ESparc::execSAVE()
{
  ULONG valrs1=readReg(rs1);
  ULONG valrs2=getS2();
  decCWP();
  writeReg(rd,valrs1+valrs2);
}


void ESparc::disSAVE()
{
  DEBUG_OUT<<"save "; printReg(rs1); DEBUG_OUT<<",";
  printS2(); DEBUG_OUT<<","; printReg(rd);
}


// TODO: use jump() so we can trace branches
void ESparc::execJMPL()
{
  ULONG ea=readReg(rs1)+getS2();
  branch_address=ea; tookBranch=true;
  writeReg(rd,pc);
}


void ESparc::disJMPL()
{
  ULONG ea=readReg(rs1)+getS2();
  DEBUG_OUT<<"jmpl [";  printReg(rs1);
  DEBUG_OUT<<"+";  printS2();
  DEBUG_OUT<<"],"; printReg(rd);
}


void ESparc::execRESTORE()
{
  ULONG valrs1=readReg(rs1);
  ULONG valrs2=getS2();
  incCWP();
  writeReg(rd,valrs1+valrs2);
}


void ESparc::execRETURN()
{
  ULONG ea=readReg(rs1)+getS2();
  incCWP();
  branch_address=ea; tookBranch=true;
}


void ESparc::disRESTORE()
{
  DEBUG_OUT<<"restore "; printReg(rs1); DEBUG_OUT<<",";
  printS2(); DEBUG_OUT<<","; printReg(rd);
}


void ESparc::disRETURN()
{
  ULONG ea=readReg(rs1)+getS2();
  DEBUG_OUT<<"return ";  printReg(rs1);
  DEBUG_OUT<<"+";  printS2();
}


// AKA LDUW
void ESparc::execLD()
{
  ULONG ea=readReg(rs1)+getS2();
  writeReg(rd,read32mem(ea));
}


// 64 bit
void ESparc::execLDX()
{
  // TODO: Need to extend to 64 bits
  ULONG ea=readReg(rs1)+getS2();
  writeReg(rd,read64mem(ea));
}


// AKA LDUW
void ESparc::disLD()
{
  ULONG ea=readReg(rs1)+getS2();
  DEBUG_OUT<<"ld [";  printReg(rs1);
  DEBUG_OUT<<"+";  printS2();
  DEBUG_OUT<<"],"; printReg(rd);
}


// 64 bit
void ESparc::disLDX()
{
  ULONG ea=readReg(rs1)+getS2();
  DEBUG_OUT<<"ldx [";  printReg(rs1);
  DEBUG_OUT<<"+";  printS2();
  DEBUG_OUT<<"],"; printReg(rd);
}


void ESparc::execLDUB()
{
  ULONG ea=readReg(rs1)+getS2();
  writeReg(rd,unsign_ext(map->read8(ea),8));
}


void ESparc::disLDUB()
{
  ULONG ea=readReg(rs1)+getS2();
  DEBUG_OUT<<"ldub [";  printReg(rs1);
  DEBUG_OUT<<"+";  printS2();
  DEBUG_OUT<<"],"; printReg(rd);
}


void ESparc::execLDUH()
{
  ULONG ea=readReg(rs1)+getS2();
  writeReg(rd,unsign_ext(read16mem(ea),16));
}


void ESparc::disLDUH()
{
  ULONG ea=readReg(rs1)+getS2();
  DEBUG_OUT<<"lduh [";  printReg(rs1);
  DEBUG_OUT<<"+";  printS2();
  DEBUG_OUT<<"],"; printReg(rd);
}


void ESparc::execLDSB()
{
  ULONG ea=readReg(rs1)+getS2();
  writeReg(rd,sign_ext(map->read8(ea),8));
}


void ESparc::disLDSB()
{
  ULONG ea=readReg(rs1)+getS2();
  DEBUG_OUT<<"ldsb [";  printReg(rs1);
  DEBUG_OUT<<"+";  printS2();
  DEBUG_OUT<<"],"; printReg(rd);
}


void ESparc::execLDSH()
{
  ULONG ea=readReg(rs1)+getS2();
  writeReg(rd,sign_ext(read16mem(ea),16));
}


// 64 bit only
void ESparc::execLDSW()
{
  ULONG ea=readReg(rs1)+getS2();
  writeReg(rd,sign_ext(read32mem(ea),32));
}


void ESparc::disLDSH()
{
  ULONG ea=readReg(rs1)+getS2();
  DEBUG_OUT<<"ldsh [";  printReg(rs1);
  DEBUG_OUT<<"+";  printS2();
  DEBUG_OUT<<"],"; printReg(rd);
}


// 64 bit only
void ESparc::disLDSW()
{
  ULONG ea=readReg(rs1)+getS2();
  DEBUG_OUT<<"ldsw [";  printReg(rs1);
  DEBUG_OUT<<"+";  printS2();
  DEBUG_OUT<<"],"; printReg(rd);
}


void ESparc::execLDDF()
{
  ULONG ea=readReg(rs1)+getS2();
  writeFReg(rd,read32mem(ea),ESPARC_FLOAT_DOUBLE);
}


void ESparc::disLDDF()
{
  ULONG ea=readReg(rs1)+getS2();
  DEBUG_OUT<<"lddf [";  printReg(rs1);
  DEBUG_OUT<<"+";  printS2();
  DEBUG_OUT<<"],"; printFReg(rd,ESPARC_FLOAT_DOUBLE);
}


void ESparc::execST()
{
  ULONG ea=readReg(rs1)+getS2();
  write32mem(ea,readReg(rd));
}


// 64 bits
void ESparc::execSTX()
{
  // Need to extend to 64 bits
  ULONG ea=readReg(rs1)+getS2();
  write64mem(ea,readReg(rd));
}


void ESparc::disST()
{
  ULONG ea=readReg(rs1)+getS2();
  DEBUG_OUT<<"st "; printReg(rd); DEBUG_OUT<<",[";
  printReg(rs1); DEBUG_OUT<<"+"; printS2();
  DEBUG_OUT<<"]";
}


// 64 bits
void ESparc::disSTX()
{
  ULONG ea=readReg(rs1)+getS2();
  DEBUG_OUT<<"stx "; printReg(rd); DEBUG_OUT<<",[";
  printReg(rs1); DEBUG_OUT<<"+"; printS2();
  DEBUG_OUT<<"]";
}


void ESparc::execSTB()
{
  ULONG ea=readReg(rs1)+getS2();
  map->write8(ea,unsign_ext(readReg(rd),8));
}


void ESparc::disSTB()
{
  ULONG ea=readReg(rs1)+getS2();
  DEBUG_OUT<<"stb "; printReg(rd); DEBUG_OUT<<",[";
  printReg(rs1); DEBUG_OUT<<"+"; printS2();
  DEBUG_OUT<<"]";
}


void ESparc::execSTDF()
{
  ULONG ea=readReg(rs1)+getS2();
  write32mem(ea,readFReg(rd,ESPARC_FLOAT_DOUBLE));
}


void ESparc::disSTDF()
{
  ULONG ea=readReg(rs1)+getS2();
  DEBUG_OUT<<"stdf "; printFReg(rd,ESPARC_FLOAT_DOUBLE); DEBUG_OUT<<",[";
  printReg(rs1); DEBUG_OUT<<"+"; printS2();
  DEBUG_OUT<<"]";
}


void ESparc::execSLL()
{
  unsigned int count,val=0;
  val=readReg(rs1);
  for(count=getS2();count>0;count--) {
    val=val<<1;  val=val&0x7fffffff;
  }
  writeReg(rd,val);
}


void ESparc::disSLL()
{
  DEBUG_OUT<<"sll ";  printReg(rs1);  DEBUG_OUT<<",";
  printS2(); DEBUG_OUT<<","; printReg(rd);
}


void ESparc::execSRL()
{
  unsigned int count,val=0;
  val=readReg(rs1);
  for(count=getS2();count>0;count--) {
    val=val>>1;  val=val&0x7fffffff;
  }
  writeReg(rd,val);
}


void ESparc::disSRL()
{
  DEBUG_OUT<<"srl ";  printReg(rs1);  DEBUG_OUT<<",";
  printS2(); DEBUG_OUT<<","; printReg(rd);
}


void ESparc::execSRA()
{
  unsigned int signmask,count,val=0;
  signmask=(((int)readReg(rs1))<0)<<31;
  val=readReg(rs1);
  for(count=getS2();count>0;count--) {
    val=val>>1;  val=val|signmask;
  }
  writeReg(rd,val);
}


void ESparc::disSRA()
{
  DEBUG_OUT<<"sra ";  printReg(rs1);  DEBUG_OUT<<",";
  printS2(); DEBUG_OUT<<","; printReg(rd);
}


ULONG ESparc::readReg(unsigned int n)
{
  unsigned int rn=0;
  ULONG ret=0;
  if(n<8) { ret=G[n]; }
  else {
    rn=(CWP*16+(n-8))%NUM_REG;
    ret=R[rn];
  }
#ifdef DEBUG_EXEC
  DEBUG_OUT<<"read reg "<<n<<" ("<<rn<<"), value is "<<ret<<"\n";
#endif
  return ret;
}


ULONG ESparc::readFReg(unsigned int n, unsigned int prec)
{
  ULONG ret=0;
  n=n&0x1f;
  ret=(unsigned int)F[n];
#ifdef DEBUG_EXEC
  DEBUG_OUT<<"read freg "<<n<<", value is "<<ret<<"\n";
#endif
  return ret;
}


void ESparc::writeReg(unsigned int n, ULONG val)
{
  unsigned int rn=0;
  if(n<8) { if(n) G[n]=val; }
  else {
    rn=(CWP*16+(n-8))%NUM_REG;
    R[rn]=val;
  }
#ifdef DEBUG_EXEC
  DEBUG_OUT<<"write "<<val<<" to reg "<<n<<" ("<<rn<<")\n";
#endif
}


void ESparc::writeFReg(unsigned int n, ULONG val, unsigned int prec)
{
  n=n&0x1f;
  F[n]=val;
#ifdef DEBUG_EXEC
  DEBUG_OUT<<"write freg "<<n<<", "<<val<<"\n";
#endif
}


void ESparc::printRegWindow()
{
  unsigned int i,j=0;
  for(i=0;i<8;i++) {
    if((i==0)||(i==4)) DEBUG_OUT<<"G=";
    DEBUG_OUT<<readReg(i)<<" ";
    j++;
    if(j==4) { j=0; DEBUG_OUT<<"\n"; }
  }
  j=0;
  for(i=8;i<32;i++) {
    if((i==8)||(i==12)) DEBUG_OUT<<"O=";
    if((i==16)||(i==20)) DEBUG_OUT<<"L=";
    if((i==24)||(i==28)) DEBUG_OUT<<"I=";
    DEBUG_OUT<<readReg(i)<<" ";
    j++;
    if(j==4) { j=0; DEBUG_OUT<<"\n"; }
  }
}


void ESparc::printReg(unsigned int num)
{
  if(num>31) { DEBUG_OUT<<"%???"; return; }
  if(num<8) { DEBUG_OUT<<"%g"<<((char)('0'+num)); return; }
  if(num<16) { DEBUG_OUT<<"%o"<<((char)('0'+num-8)); return; }
  if(num<24) { DEBUG_OUT<<"%l"<<((char)('0'+num-16)); return; }
  DEBUG_OUT<<"%i"<<((char)('0'+num-24));
}


void ESparc::printFReg(unsigned int num,unsigned int prec)
{
  //w->dec();
  DEBUG_OUT<<"%f"<<num;
  //w->hex();
}


void ESparc::windowOverflow()
{
  debugger("Reg Win overflow!");
}


void ESparc::windowUnderflow()
{
  debugger("Reg Win underflow!");
}


// Used by SAVE
void ESparc::decCWP()
{
  dirty[CWP]=true;
  CWP--;
  CWP%=NUM_WINDOWS;
  if(dirty[CWP]) windowOverflow();
}


// Used by RESTORE
void ESparc::incCWP()
{
  dirty[CWP]=false;
  CWP++;
  CWP%=NUM_WINDOWS;
  if(!dirty[CWP]) windowUnderflow();
}


ULONG ESparc::getS2()
{
  if(!ibit) { return readReg(rs2); }
  else { return sign_ext(simm13,13); }
}


void ESparc::printS2()
{
  if(!ibit) { printReg(rs2); }
  else { DEBUG_OUT<<"#$"<<sign_ext(simm13,13); }
}


// op=0, format 2
void ESparc::execOp0()
{
  imm22=op&0x3fffff;
  rd=(op>>25)&0x1f;
  rs1=(op>>14)&0x1f;
  unsigned int top=(op>>22)&0x7;
  switch(top) {
    case 1: execBICC(true); break;  // 64 bit
    case 2: execBICC(false); break;
    case 4: execSETHI(); break;
    default:
      DEBUG_OUT<<"top=="<<top<<"\n";
      exec_unimplemented("execOp0");
      break;
  }
}


// op=0, format 2
void ESparc::disOp0()
{
  imm22=op&0x3fffff;
  rd=(op>>25)&0x1f;
  rs1=(op>>14)&0x1f;
  unsigned int top=(op>>22)&0x7;
  switch(top) {
    case 1: disBICC(true); break;  // 64 bit
    case 2: disBICC(false); break;
    case 4: disSETHI(); break;
    default:
      DEBUG_OUT<<"top=="<<top<<" ";
      dis_illegal("disOp0");
      break;
  }
}


// op=1, format 1
void ESparc::execOp1()
{
  disp30=op&0x3fffffff;
  execCALL();
}


// op=1, format 1
void ESparc::disOp1()
{
  disp30=op&0x3fffffff;
  disCALL();
}


// op=2, format 3
void ESparc::execOp2()
{
  // predecode the instruction
  ibit=(op>>13)&0x1;
  rs1=(op>>14)&0x1f;
  rs2=op&0x1f;
  rd=(op>>25)&0x1f;
  simm13=op&0x1fff;
  unsigned int top=(op>>19)&0x3f;
  switch(top) {
    case 0:  execADD(); break;
    case 1:  execAND(); break;
    case 2:  execOR(); break;
    case 3:  execXOR(); break;
    case 4:  execSUB(); break;
    case 9:  execMULX(); break;  // 64 bit
    case 17: execANDCC(); break;
    case 18: execORCC(); break;
    case 20: execSUBCC(); break;
    case 37: execSLL(); break;
    case 38: execSRL(); break;
    case 39: execSRA(); break;
    case 57: execRETURN(); break;
    case 58: execTICC(); break;
    case 60: execSAVE(); break;
    case 56: execJMPL(); break;
    case 61: execRESTORE(); break;
    default:
      DEBUG_OUT<<"top=="<<top<<"\n";
      exec_unimplemented("execOp2");
      break;
  }
}


// op=2, format 3
void ESparc::disOp2()
{
  // predecode the instruction
  ibit=(op>>13)&0x1;
  rs1=(op>>14)&0x1f;
  rs2=op&0x1f;
  rd=(op>>25)&0x1f;
  simm13=op&0x1fff;
  unsigned int top=(op>>19)&0x3f;
  switch(top) {
    case 0:  disADD(); break;
    case 1:  disAND(); break;
    case 2:  disOR(); break;
    case 3:  disXOR(); break;
    case 4:  disSUB(); break;
    case 9:  disMULX(); break;  // 64 bit
    case 17: disANDCC(); break;
    case 18: disORCC(); break;
    case 20: disSUBCC(); break;
    case 37: disSLL(); break;
    case 38: disSRL(); break;
    case 39: disSRA(); break;
    case 58: disTICC(); break;
    case 57: disRETURN(); break;
    case 60: disSAVE(); break;
    case 56: disJMPL(); break;
    case 61: disRESTORE(); break;
    default:
      DEBUG_OUT<<"top=="<<top<<" ";
      dis_illegal("disOp2");
      break;
  }
}


// op=3, format 3
void ESparc::execOp3()
{
  // predecode the instruction
  ibit=(op>>13)&0x1;
  rs1=(op>>14)&0x1f;
  rs2=op&0x1f;
  rd=(op>>25)&0x1f;
  simm13=op&0x1fff;
  unsigned int top=(op>>19)&0x3f;
  switch(top) {
    case 0: execLD(); break;  // AKA LDUW
    case 1: execLDUB(); break;
    case 2: execLDUH(); break;
    case 4: execST(); break;
    case 5: execSTB(); break;
    case 6: execLDSH(); break;
    case 8: execLDSW(); break; // 64 bit
    case 9: execLDSB(); break;
    case 11: execLDX(); break; // 64 bit
    case 14: execSTX(); break; // 64 bit
    case 35: execLDDF(); break;
    case 39: execSTDF(); break;
    default:
      DEBUG_OUT<<"top=="<<top<<"\n";
      exec_unimplemented("execOp3");
      break;
  }
}


// op=3, format 3
void ESparc::disOp3()
{
  // predecode the instruction
  ibit=(op>>13)&0x1;
  rs1=(op>>14)&0x1f;
  rs2=op&0x1f;
  rd=(op>>25)&0x1f;
  simm13=op&0x1fff;
  unsigned int top=(op>>19)&0x3f;
  switch(top) {
    case 0: disLD(); break;  // aka LDUW
    case 1: disLDUB(); break;
    case 2: disLDUH(); break;
    case 4: disST(); break;
    case 5: disSTB(); break;
    case 6: disLDSH(); break;
    case 8: disLDSW(); break; // 64 bit
    case 9: disLDSB(); break;
    case 11: disLDX(); break; // 64 bit
    case 14: disSTX(); break; // 64 bit
    case 35: disLDDF(); break;
    case 39: disSTDF(); break;
    default:
      DEBUG_OUT<<"top=="<<top<<" ";
      dis_illegal("disOp3");
      break;
  }
}

