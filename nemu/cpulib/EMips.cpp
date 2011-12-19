
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


/*

The registers are refered to as r0-r31 here, instead of the names GNU
uses in gas.

The order of the operands in the disassembly may match what GNU uses more
closely than it does the official MIPS order...

NOTE: This code doesn't emulate a pipeline of any kind, so there are no
real hazards or stalls.  Delay slots are simulated.  Don't expect it to
act as a real pipelined RISC processor would under all conditions.

*/


#define DEBUG_EXEC 1

#define CHECK_SAFE 1

#define SHOW_INTS  1

//#define USE_FANCY_REG_NAMES 1


#include <cpulib/EMips.h>

#include <elib/EHandler.h>
#include <elib/EMemMapAbstract.h>
#include <elib/EModule.h>

#include <portable/nickcpp/ANullStream.h>

#include <portable/nickcpp/AStringHelper.h>


#include <cpulib/generated/EMips_tab.h>
#include <cpulib/generated/EMips_0_tab.h>
#include <cpulib/generated/EMips_1_tab.h>


////////////////////////////////////////////////////////////////////////////////
//  EMips Class
////////////////////////////////////////////////////////////////////////////////

EMips::EMips()
{
  DEBUG_OUT<<"EMips created...\n";
  init();
  //w=ww;
  reset();
}


EMips::~EMips()
{
  DEBUG_OUT<<"EMips destroyed.\n";
}


void EMips::init()
{
  // inherited members
  ECPURisc::init();
  bitness=32;
  branchesAreDelayed=true;
  r0IsZero=true;
  doTrace=false;  doRun=false;
  // our unique members
  unsigned int t=0;
  for(t=0;t<32;t++) G[t]=0;
  for(t=0;t<32;t++) F[t]=0.0;
  for(t=0;t<32;t++) cpr[t]=0;
  pc=0;  npc=0;
  ir=0;
  tookBranch=false;
  branch_address=0;
  lsb=false;
  anull=false;
  opName=(char *)NULL;
  opFormat=0;  opSubFormat=0;  opFlags=0;
  disFlag=false;
  op=0;  top=0;  bop=0;  op0=0;  op1=0;
  ea=0;  val=0;
  rs=0;  rt=0;  rd=0;  sa=0;
  imm=0;
  setStack(ECPUABSTRACT_DEFAULT_STACK);
  setFramePtr(ECPUABSTRACT_DEFAULT_STACK);
  setResetStack(ECPUABSTRACT_DEFAULT_STACK);
  setResetEntry(ECPUABSTRACT_DEFAULT_CODE);
}


bool EMips::recognize(const char *tname)
{
  const char *name=aDefaultStringHelper.removeSpaces2(aDefaultStringHelper.toLower(tname));
  if(!strcmp(name,"mips")) return true; else
  if(!strcmp(name,"mips64")) return true; else
  if(!strcmp(name,"mipsel")) return true; else
  if(!strcmp(name,"mipsel64")) return true; else
  if(!strcmp(name,"mips2000")) return true; else
  if(!strcmp(name,"mips2000a")) return true; else
  if(!strcmp(name,"mips2300")) return true; else
  if(!strcmp(name,"mips2600")) return true; else
  if(!strcmp(name,"mips2800")) return true; else
  if(!strcmp(name,"mips3000")) return true; else
  if(!strcmp(name,"mips3000a")) return true; else
  if(!strcmp(name,"mips4000")) return true; else
  if(!strcmp(name,"mips4200")) return true; else
  if(!strcmp(name,"mips4400")) return true; else
  if(!strcmp(name,"mips4600")) return true; else
  if(!strcmp(name,"mips5000")) return true; else
  if(!strcmp(name,"mips6000")) return true; else
  if(!strcmp(name,"mips8000")) return true; else
  if(!strcmp(name,"mips8010")) return true; else
  if(!strcmp(name,"mips10000")) return true; else
  if(!strcmp(name,"r2000")) return true; else
  if(!strcmp(name,"rs2000")) return true; else
  if(!strcmp(name,"2000")) return true; else
  if(!strcmp(name,"2000a")) return true; else
  if(!strcmp(name,"2300")) return true; else
  if(!strcmp(name,"2600")) return true; else
  if(!strcmp(name,"2800")) return true; else
  if(!strcmp(name,"3000")) return true; else
  if(!strcmp(name,"3000a")) return true; else
  if(!strcmp(name,"r3000")) return true; else
  if(!strcmp(name,"rs3000")) return true; else
  if(!strcmp(name,"r3000a")) return true; else
  if(!strcmp(name,"rs3000a")) return true; else
  if(!strcmp(name,"4x00")) return true; else
  if(!strcmp(name,"4000pc")) return true; else
  if(!strcmp(name,"r4000")) return true; else
  if(!strcmp(name,"rs4000")) return true; else
  if(!strcmp(name,"r4100")) return true; else
  if(!strcmp(name,"r4111")) return true; else
  if(!strcmp(name,"vr4110")) return true; else
  if(!strcmp(name,"vr4181")) return true; else
  if(!strcmp(name,"4200")) return true; else
  if(!strcmp(name,"r4200")) return true; else
  if(!strcmp(name,"rs4200")) return true; else
  if(!strcmp(name,"r4300")) return true; else
  if(!strcmp(name,"4400")) return true; else
  if(!strcmp(name,"4400pc")) return true; else
  if(!strcmp(name,"r4400")) return true; else
  if(!strcmp(name,"rs4400")) return true; else
  if(!strcmp(name,"r4600")) return true; else
  if(!strcmp(name,"r4700")) return true; else
  if(!strcmp(name,"r5000")) return true; else
  if(!strcmp(name,"r5200")) return true; else
  if(!strcmp(name,"r5900")) return true; else
  if(!strcmp(name,"r6000")) return true; else
  //if(!strcmp(name,"rs6000")) return true; else
  if(!strcmp(name,"r7000")) return true; else
  if(!strcmp(name,"r8000")) return true; else
  if(!strcmp(name,"r8010")) return true; else
  if(!strcmp(name,"r10000")) return true; else
  if(!strcmp(name,"r12000")) return true; else
  return false;
}


const char *EMips::getName()
{
  if(lsb) return "mipsel";
  return "mips";
}


void EMips::setKind(const char *tname)
{
  const char *name=aDefaultStringHelper.removeSpaces2(aDefaultStringHelper.toLower(tname));
  if(!strcmp(name,"mipsel")) lsb=true;
}


void EMips::reset()
{
#ifdef DEBUG_EXEC
  setTrace(true);
#endif
  unsigned int t;
  pc=0;
  npc=pc+4;
  ir=0;
  for(t=0;t<32;t++) { G[t]=0; }
  for(t=0;t<32;t++) { F[t]=0.0; }
  tookBranch=false; branch_address=0;
  op=0;  opName=(char *)NULL;
  top=0;  bop=0;
  opFormat=EMIPS_FORMAT_UNKNOWN; opSubFormat=0; opFlags=0;
  hasFPU=true; hasMMU=true;
  // TODO: where does a MIPS normally go on reset?
  //writeReg(25,resetEntry&0xffff0000);  // TODO: kludge!!!
  ULONG globPtr=0;
  if(map) {
    EModule *mod=map->getFirstModule();
    if(mod) {
      EMapSymbol *dataNode=mod->lookupNodeByName(".data");
      if(dataNode) globPtr=dataNode->addr;
      else DEBUG_OUT<<"Couldn't set gp, no .data node!\n";
    }
    else DEBUG_OUT<<"Couldn't set gp, no module!\n";
  }
  else DEBUG_OUT<<"Couldn't set gp, no map!\n";
  writeReg(0x1c,globPtr);
  clearStackTrace();
  ECPUAbstract::reset();
}


bool EMips::setRegister(const char *tname,ULONG value)
{
  const char *name=aDefaultStringHelper.removeSpaces2(aDefaultStringHelper.toLower(tname));
  bool ret=false;
  if(!strcmp(name,"pc"))
  { pc=value; ret=true; } else
  if(!strcmp(name,"npc"))
  { npc=value; ret=true; } else
  if((name[0]=='r')||(name[0]=='R')) {
    unsigned int n=aDefaultStringHelper.parseNumber(name+1);
    writeReg(n,value);
    ret=true;
  } else
  ret=false;
  return ret;
}


void EMips::setStack(ULONG val)
{
  writeReg(29,val);
}


ULONG EMips::readStack()
{
  return readReg(29);
}


void EMips::setFramePtr(ULONG val)
{
  writeReg(30,val);
}


ULONG EMips::readFramePtr()
{
  return readReg(30);
}


void EMips::setReturnReg(ULONG val)
{
  writeReg(2,val);
}


ULONG EMips::readReturnReg()
{
  return readReg(2);
}


ULONG EMips::readParamReg(unsigned int num)
{
  return readReg(num+4);
}


void EMips::setParamReg(unsigned int num,ULONG val)
{
  writeReg(num+4,val);
}


bool EMips::interp(const char *cmd,const char *a1,const char *a2,const char *a3)
{
  bool ret=false;
  if(!strcmp(cmd,"rm")) { wmregs(); ret=true; }
  if(!strcmp(cmd,"rf")) { wfregs(); ret=true; }
  return ret;
}


void EMips::wmregs()
{
}


void EMips::wfregs()
{
  if(!hasFPU) return;
  unsigned int t;
  // NOTE: That stuff with '0'+t is to get them to display with only one digit
  for(t=0;t<32;t++) {
    DEBUG_OUT<<"f";
    if(t<10) DEBUG_OUT<<"0";
    DEBUG_OUT<<t<<"="<<F[t]<<" ";
    if(!((t+1)%3)) { DEBUG_OUT<<"\n"; }
  }
  DEBUG_OUT<<"\n";
}


void EMips::wregs()
{
  DEBUG_OUT<<"PC="<<pc<<" nPC="<<npc<<" ";
  if(lsb) DEBUG_OUT<<"(Little)"; else DEBUG_OUT<<"(Big)";
  DEBUG_OUT<<"\n";
  for(unsigned int r=0;r<8;r++) {
    DEBUG_OUT<<getRegName(r*4)<<"=";
    for(unsigned int c=0;c<4;c++)  DEBUG_OUT<<G[r*4+c]<<" ";
    DEBUG_OUT<<"\n";
  }
}


void EMips::handle_int(ULONG value)
{
  // Need to handle these...
}


void EMips::cause_int(ULONG value)
{
  // Need to handle these...
}


ULONG EMips::disasm(ULONG loc)
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
  DEBUG_OUT<<ir<<"  ";
  //w->setHexDigits(d);
  op=ir;
  top=(op>>26)&0x3f;
  opName=EMips_name[top];
  opFormat=EMips_format[top];
  opSubFormat=EMips_subformat[top];
  opFlags=EMips_flags[top];
  decode(op);
  dishelper(opName);
  DEBUG_OUT<<"";
  DEBUG_OUT<<"\n";
  doTrace=old;
  pc=oldPC;
  return 4;
}


void EMips::execute(void)
{
  if(pc==breakpoint) { debugger("Breakpoint!\n"); return; }
  doRun=true;
  ir=read32mem(pc);
  op=ir;
  tookBranch=false;
  top=(op>>26)&0x3f;
  opName=EMips_name[top];
  opFormat=EMips_format[top];
  opSubFormat=EMips_subformat[top];
  opFlags=EMips_flags[top];
  decode(op);
  int topcode=top;
#include <cpulib/generated/EMips_exec.h>
  pc=npc;
  npc+=4;
  if(tookBranch) npc=branch_address;
  if(anull) { pc=npc; npc=pc+4; anull=false; }
  if(int_pending) handle_int(int_pending);
}


ULONG EMips::readReg(unsigned int n)
{
  unsigned int rn=n&0x1f;
#ifdef DEBUG_EXEC
  DEBUG_OUT<<"\tRead "<<G[rn]<<" from reg "<<getRegName(rn)<<"\n";
#endif
  if(rn) return G[rn];
  else return 0;
}


void EMips::writeReg(unsigned int n, ULONG val)
{
  unsigned int rn=n&0x1f;
  if(n) G[rn]=val;
#ifdef DEBUG_EXEC
  if(n) DEBUG_OUT<<"\tWrite "<<val<<" to reg "<<getRegName(rn)<<"\n";
  else DEBUG_OUT<<"Not writing to reg 0\n";
#endif
}


const char *EMips::getRegName(unsigned int num)
{
  switch(num) {
#ifdef USE_FANCY_REG_NAMES
    case 0x0: return "zero"; break;
    case 0x1: return "at"; break;
    case 0x2: return "v0"; break;
    case 0x3: return "v1"; break;
    case 0x4: return "a0"; break;
    case 0x5: return "a1"; break;
    case 0x6: return "a2"; break;
    case 0x7: return "a3"; break;
    case 0x8: return "t0"; break;
    case 0x9: return "t1"; break;
    case 0xa: return "t2"; break;
    case 0xb: return "t3"; break;
    case 0xc: return "t4"; break;
    case 0xd: return "t5"; break;
    case 0xe: return "t6"; break;
    case 0xf: return "t7"; break;
    case 0x10: return "s0"; break;
    case 0x11: return "s1"; break;
    case 0x12: return "s2"; break;
    case 0x13: return "s3"; break;
    case 0x14: return "s4"; break;
    case 0x15: return "s5"; break;
    case 0x16: return "s6"; break;
    case 0x17: return "s7"; break;
    case 0x18: return "t8"; break;
    case 0x19: return "t9"; break;
    case 0x1a: return "k0"; break;
    case 0x1b: return "k1"; break;
    case 0x1c: return "gp"; break;
    case 0x1d: return "sp"; break;
    case 0x1e: return "s8"; break;
    case 0x1f: return "ra"; break;
#else
    case 0x0: return "$0"; break;
    case 0x1: return "$1"; break;
    case 0x2: return "$2"; break;
    case 0x3: return "$3"; break;
    case 0x4: return "$4"; break;
    case 0x5: return "$5"; break;
    case 0x6: return "$6"; break;
    case 0x7: return "$7"; break;
    case 0x8: return "$8"; break;
    case 0x9: return "$9"; break;
    case 0xa: return "$10"; break;
    case 0xb: return "$11"; break;
    case 0xc: return "$12"; break;
    case 0xd: return "$13"; break;
    case 0xe: return "$14"; break;
    case 0xf: return "$15"; break;
    case 0x10: return "$16"; break;
    case 0x11: return "$17"; break;
    case 0x12: return "$18"; break;
    case 0x13: return "$19"; break;
    case 0x14: return "$20"; break;
    case 0x15: return "$21"; break;
    case 0x16: return "$22"; break;
    case 0x17: return "$23"; break;
    case 0x18: return "$24"; break;
    case 0x19: return "$25"; break;
    case 0x1a: return "$26"; break;
    case 0x1b: return "$27"; break;
    case 0x1c: return "$28"; break;
    case 0x1d: return "$sp"; break;
    case 0x1e: return "$fp"; break;
    case 0x1f: return "$31"; break;
#endif  // USE_FANCY_REG_NAMES
    default: return "unknown"; break;
  }
}


double EMips::readFloatReg(unsigned int n)
{
  unsigned int rn=n&0x1f;
  return F[rn];
}


void EMips::writeFloatReg(unsigned int n, double val)
{
  unsigned int rn=n&0x1f;
  F[rn]=val;
}


char *EMips::getFloatRegName(unsigned int num)
{
  return "getFloatRegName not implemented!";
}


void EMips::disVarious()
{
  //DEBUG_OUT<<"(disVarious)\n";
  switch(top) {
    case 0:  // "SPECIAL"
      //DEBUG_OUT<<"(disVarious special)\n";
      bop=op&0x3f;
      opName=EMips_0_name[bop];
      opFormat=EMips_0_format[bop];
      opSubFormat=EMips_0_subformat[bop];
      opFlags=EMips_0_flags[bop];
      if(opFormat==EMIPS_FORMAT_VARIOUS) opFormat=EMIPS_FORMAT_NONE;
      //DEBUG_OUT<<"Got SPECIAL, going to decode again...\n";
      decode(op);
      break;
    case 1:  // "REGIMM"
      //DEBUG_OUT<<"(disVarious regimm)\n";
      bop=rt;  // set by preious call to decode() in mipsDis()...
      opName=EMips_1_name[bop];
      opFormat=EMips_1_format[bop];
      opSubFormat=EMips_1_subformat[bop];
      opFlags=EMips_1_flags[bop];
      if(opFormat==EMIPS_FORMAT_VARIOUS) opFormat=EMIPS_FORMAT_NONE;
      //DEBUG_OUT<<"Got REGIMM, going to decode again...\n";
      decode(op);
      break;
    default:
      DEBUG_OUT<<"top=="<<top<<"\n";
      debugger("Bad top in disVarious!");
      break;
  }
}


void EMips::dishelper(const char *oname)
{
  //unsigned int dig=w->getHexDigits();
  //w->setHexDigits(1);
  switch(opFormat) {
    case EMIPS_FORMAT_NONE:
      DEBUG_OUT<<oname;
      break;
    case EMIPS_FORMAT_CONST:
      DEBUG_OUT<<oname<<" 0x"<<ea;
      break;
    case EMIPS_FORMAT_REG_REG_CONST:
      switch(opSubFormat) {
        case EMIPS_SUBFORMAT_NONE:
          DEBUG_OUT<<oname<<" "<<getRegName(rt)<<","<<getRegName(rs)<<",0x"<<imm;
          break;
        case EMIPS_SUBFORMAT_BASED:
          DEBUG_OUT<<oname<<" "<<getRegName(rt)<<",0x"<<imm<<"("<<getRegName(rs)<<")";
          break;
        default:
          debugger("dishelper: bad subformat for REG_REG_CONST!");
          break;
      }
      break;
    case EMIPS_FORMAT_REG_REG_REG:
      DEBUG_OUT<<oname<<" "<<getRegName(rd)<<","<<getRegName(rs)<<","<<getRegName(rt);
      break;
    default:
      debugger("dishelper: bad opFormat!");
      break;
  }
  if(opFlags==EMIPS_BRANCH) DEBUG_OUT<<"  ; ???";
  //w->setHexDigits(dig);
}


void EMips::decode(ULONG inst)
{
  switch(opFormat) {
    case EMIPS_FORMAT_NONE:
      break;
    case EMIPS_FORMAT_CONST:
      imm=(op&0x03ffffff)<<2;
      ea=imm+(pc&0xf0000000);
      break;
    case EMIPS_FORMAT_REG_REG_CONST:
      rt=(op>>16)&0x1f;
      rs=(op>>21)&0x1f;
      imm=sign_ext(op&0xffff,16);
      break;
    case EMIPS_FORMAT_REG_REG_REG:
      sa=(op>>6)&0x1f;
      rd=(op>>11)&0x1f;
      rt=(op>>16)&0x1f;
      rs=(op>>21)&0x1f;
      break;
    case EMIPS_FORMAT_VARIOUS:
      if(!disFlag) {
        disFlag=true;
        disVarious();
        disFlag=false;
      }
      else debugger("Caught in disVarious loop in decode!\n");
      break;
    default:
      DEBUG_OUT<<"decode: opFormat is "<<opFormat<<"!\n";
      DEBUG_OUT<<"decode: top was "<<top<<" bop was "<<bop<<"\n";
      debugger("Bad opFormat in decode()!\n");
      break;
  }
}


void EMips::exec_special()
{
  bop=op&0x3f;
  int topcode=bop;
  switch(top) {
    case 0:
      opName=EMips_0_name[bop];
      opFormat=EMips_0_format[bop];
      opSubFormat=EMips_0_subformat[bop];
#include <cpulib/generated/EMips_0_exec.h>
      break;
    default:
      DEBUG_OUT<<"top was "<<top<<" bop was "<<bop<<"\n";
      debugger("unknown top in exec_special!");
      break;
  }
}


void EMips::exec_regimm()
{
  top=(op>>21)&0x1f;
  opName=EMips_1_name[top];
  opFormat=EMips_1_format[top];
  opSubFormat=EMips_1_subformat[top];
  int topcode=top;
#include <cpulib/generated/EMips_1_exec.h>
}


void EMips::exec_add()
{
  exec_unimplemented(opName);
}


void EMips::exec_addu()
{
  unsigned int val=readReg(rs);
  unsigned int rreg=readReg(rt);
  unsigned int nval=val+rreg;
  writeReg(rd,nval);
}


void EMips::exec_dadd()
{
  exec_unimplemented(opName);
}


void EMips::exec_daddu()
{
  ULONG val=readReg(rs);
  ULONG rreg=readReg(rt);
  ULONG nval=val+rreg;
  writeReg(rd,nval);
}


void EMips::exec_addi()
{
  exec_unimplemented(opName);
}


void EMips::exec_addiu()
{
  unsigned int val=readReg(rs);
  unsigned int nval=val+imm;
  writeReg(rt,nval);
}


void EMips::exec_and()
{
  exec_unimplemented(opName);
}


void EMips::exec_andi()
{
  exec_unimplemented(opName);
}


void EMips::exec_beq()
{
  tookBranch=false;
  exec_unimplemented(opName);
}


void EMips::exec_bne()
{
  tookBranch=false;
  exec_unimplemented(opName);
}


void EMips::exec_blez()
{
  tookBranch=false;
  exec_unimplemented(opName);
}


void EMips::exec_bgtz()
{
  tookBranch=false;
  exec_unimplemented(opName);
}


void EMips::exec_beql()
{
  tookBranch=false;
  exec_unimplemented(opName);
}


void EMips::exec_bnel()
{
  tookBranch=false;
  exec_unimplemented(opName);
}


void EMips::exec_blezl()
{
  tookBranch=false;
  exec_unimplemented(opName);
}


void EMips::exec_bgtzl()
{
  tookBranch=false;
  exec_unimplemented(opName);
}


void EMips::exec_bltzal()
{
  tookBranch=false;
  exec_unimplemented(opName);
}


void EMips::exec_bgezal()
{
  tookBranch=false;
  exec_unimplemented(opName);
}


void EMips::exec_bltzall()
{
  tookBranch=false;
  exec_unimplemented(opName);
}


void EMips::exec_bgezall()
{
  tookBranch=false;
  exec_unimplemented(opName);
}


void EMips::exec_bltz()
{
  tookBranch=false;
  exec_unimplemented(opName);
}


void EMips::exec_bgez()
{
  tookBranch=false;
  exec_unimplemented(opName);
}


void EMips::exec_bltzl()
{
  tookBranch=false;
  exec_unimplemented(opName);
}


void EMips::exec_bgezl()
{
  tookBranch=false;
  exec_unimplemented(opName);
}


void EMips::exec_break()
{
  exec_unimplemented(opName);
}


void EMips::exec_cache()
{
  exec_unimplemented(opName);
}


void EMips::exec_copx()
{
  exec_unimplemented(opName);
}


void EMips::exec_daddi()
{
  exec_unimplemented(opName);
}


void EMips::exec_daddiu()
{
  exec_unimplemented(opName);
}


void EMips::exec_div()
{
  exec_unimplemented(opName);
}


void EMips::exec_ddivu()
{
  exec_unimplemented(opName);
}


void EMips::exec_ddiv()
{
  exec_unimplemented(opName);
}


void EMips::exec_divu()
{
  exec_unimplemented(opName);
}


void EMips::exec_j()
{
  tookBranch=true;
  branch_address=ea;
  if(tookBranch) willJump(branch_address);
}


void EMips::exec_jal()
{
  tookBranch=true;
  writeReg(31,pc+8);
  branch_address=ea;
  if(tookBranch) willJump(branch_address);
}


void EMips::exec_jr()
{
  tookBranch=true;
  if(rd) debugger("Invalid JR format, rd!=0");
  ea=readReg(rs);
  if(ea&0x3) debugger("Attempt to jump to non-word-aligned ea!");
  branch_address=ea;
  if(tookBranch) willJump(branch_address);
}


void EMips::exec_jalr()
{
  tookBranch=true;
  if(rs==rd) debugger("JALR where rs and rd are equal!\n");
  ea=readReg(rs);
  if(ea&0x3) debugger("Attempt to jump to non-word-aligned ea!");
  writeReg(rd,pc+8);
  branch_address=ea;
  if(tookBranch) willJump(branch_address);
}


void EMips::exec_lui()
{
  // NOTE: rs is unused
  val=(imm<<16)&0xffff0000;
  writeReg(rt,val);
}


void EMips::exec_lb()
{
  exec_unimplemented(opName);
}


void EMips::exec_lh()
{
  exec_unimplemented(opName);
}


void EMips::exec_lwl()
{
  exec_unimplemented(opName);
}


void EMips::exec_lwr()
{
  exec_unimplemented(opName);
}


void EMips::exec_lw()
{
  ea=readReg(rs)+imm;
  writeReg(rt,read32mem(ea));
}


void EMips::exec_lbu()
{
  exec_unimplemented(opName);
}


void EMips::exec_lhu()
{
  exec_unimplemented(opName);
}


void EMips::exec_ll()
{
  exec_unimplemented(opName);
}


void EMips::exec_lwcx()
{
  exec_unimplemented(opName);
}


void EMips::exec_ldcx()
{
  exec_unimplemented(opName);
}


void EMips::exec_lld()
{
  exec_unimplemented(opName);
}


void EMips::exec_ldl()
{
  exec_unimplemented(opName);
}


void EMips::exec_ldr()
{
  exec_unimplemented(opName);
}


void EMips::exec_lwu()
{
  exec_unimplemented(opName);
}


void EMips::exec_ld()
{
  exec_unimplemented(opName);
}


void EMips::exec_mult()
{
  exec_unimplemented(opName);
}


void EMips::exec_multu()
{
  exec_unimplemented(opName);
}


void EMips::exec_dmult()
{
  exec_unimplemented(opName);
}


void EMips::exec_dmultu()
{
  exec_unimplemented(opName);
}


void EMips::exec_mflo()
{
  exec_unimplemented(opName);
}


void EMips::exec_mtlo()
{
  exec_unimplemented(opName);
}


void EMips::exec_mfhi()
{
  exec_unimplemented(opName);
}


void EMips::exec_mthi()
{
  exec_unimplemented(opName);
}


void EMips::exec_nor()
{
  exec_unimplemented(opName);
}


void EMips::exec_or()
{
  unsigned int val=readReg(rs);
  unsigned int rreg=readReg(rt);
  unsigned int nval=val|rreg;
  writeReg(rd,nval);
}


void EMips::exec_ori()
{
  unsigned int val=readReg(rs);
  unsigned int nval=val|imm;
  writeReg(rd,nval);
}


void EMips::exec_sll()
{
  unsigned int val=readReg(rt);
  unsigned int nval=val<<sa;
  writeReg(rd,nval);
}


void EMips::exec_srl()
{
  exec_unimplemented(opName);
}


void EMips::exec_sra()
{
  exec_unimplemented(opName);
}


void EMips::exec_sllv()
{
  exec_unimplemented(opName);
}


void EMips::exec_srlv()
{
  exec_unimplemented(opName);
}


void EMips::exec_srav()
{
  exec_unimplemented(opName);
}


void EMips::exec_dsll()
{
  exec_unimplemented(opName);
}


void EMips::exec_dsrl()
{
  exec_unimplemented(opName);
}


void EMips::exec_dsra()
{
  exec_unimplemented(opName);
}


void EMips::exec_dsll32()
{
  exec_unimplemented(opName);
}


void EMips::exec_dsrl32()
{
  exec_unimplemented(opName);
}


void EMips::exec_dsra32()
{
  exec_unimplemented(opName);
}


void EMips::exec_dsllv()
{
  exec_unimplemented(opName);
}


void EMips::exec_dsrlv()
{
  exec_unimplemented(opName);
}


void EMips::exec_dsrav()
{
  exec_unimplemented(opName);
}


void EMips::exec_slti()
{
  exec_unimplemented(opName);
}


void EMips::exec_sltiu()
{
  exec_unimplemented(opName);
}


void EMips::exec_sb()
{
  exec_unimplemented(opName);
}


void EMips::exec_sh()
{
  exec_unimplemented(opName);
}


void EMips::exec_swl()
{
  exec_unimplemented(opName);
}


void EMips::exec_sw()
{
  ea=readReg(rs)+imm;
  write32mem(ea,readReg(rt));
}


void EMips::exec_swr()
{
  exec_unimplemented(opName);
}


void EMips::exec_sc()
{
  exec_unimplemented(opName);
}


void EMips::exec_swcx()
{
  exec_unimplemented(opName);
}


void EMips::exec_sdcx()
{
  exec_unimplemented(opName);
}


void EMips::exec_sdc()
{
  exec_unimplemented(opName);
}


void EMips::exec_sdl()
{
  exec_unimplemented(opName);
}


void EMips::exec_sdr()
{
  exec_unimplemented(opName);
}


void EMips::exec_sd()
{
  ea=readReg(rs)+imm;
  write64mem(ea,readReg(rt));
}


void EMips::exec_sub()
{
  exec_unimplemented(opName);
}


void EMips::exec_subu()
{
  exec_unimplemented(opName);
}


void EMips::exec_dsub()
{
  exec_unimplemented(opName);
}


void EMips::exec_dsubu()
{
  exec_unimplemented(opName);
}


void EMips::exec_slt()
{
  exec_unimplemented(opName);
}


void EMips::exec_sltu()
{
  exec_unimplemented(opName);
}


void EMips::exec_sync()
{
  exec_unimplemented(opName);
}


void EMips::exec_syscall()
{
  bool ret=false;
  if(io) ret=io->systemCall(15);  // 15 is the magic number tiny.cpp looks for
  if(!ret) {
    DEBUG_OUT<<"SYSCALL\n";
    debugger();
  }
}


void EMips::exec_tge()
{
  exec_unimplemented(opName);
}


void EMips::exec_tgeu()
{
  exec_unimplemented(opName);
}


void EMips::exec_tlt()
{
  exec_unimplemented(opName);
}

void EMips::exec_tltu()
{
  exec_unimplemented(opName);
}


void EMips::exec_teq()
{
  exec_unimplemented(opName);
}


void EMips::exec_tne()
{
  exec_unimplemented(opName);
}


void EMips::exec_tnei()
{
  exec_unimplemented(opName);
}


void EMips::exec_teqi()
{
  exec_unimplemented(opName);
}


void EMips::exec_tltiu()
{
  exec_unimplemented(opName);
}


void EMips::exec_tlti()
{
  exec_unimplemented(opName);
}


void EMips::exec_tgeiu()
{
  exec_unimplemented(opName);
}


void EMips::exec_tgei()
{
  exec_unimplemented(opName);
}


void EMips::exec_xor()
{
  exec_unimplemented(opName);
}


void EMips::exec_xori()
{
  exec_unimplemented(opName);
}



