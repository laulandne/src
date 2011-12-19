
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#define DEBUG_DIS_MODES 1

#define DEBUG_EXEC 1

#define CHECK_SAFE 1

#define SHOW_INTS  1


#include <cpulib/EArm.h>

#include <elib/EMemMapAbstract.h>
#include <elib/EModule.h>

#include <portable/nickcpp/ANullStream.h>

#include <portable/nickcpp/AStringHelper.h>


#include <cpulib/generated/EArm_tab.h>
#include <cpulib/generated/EThumb_tab.h>


// The different cpus...
#define E_CPU_NONE     0x00
#define E_CPU_ARM      0x01


////////////////////////////////////////////////////////////////////////////////
//  EArm Class
////////////////////////////////////////////////////////////////////////////////

EArm::EArm()
{
  DEBUG_OUT<<"EArm created...\n";
  init();
  type=E_CPU_ARM;
  //w=ww;
  reset();
}


EArm::~EArm()
{
  DEBUG_OUT<<"EArm destroyed.\n";
}


void EArm::init()
{
  ECPURisc::init();
  // Generic features
  doTrace=false;  doRun=false;
  // Set up stack and registers
  int_pending=0;
  intLock=false;
  lsb=true;
  pc=0;
  for(unsigned int i=0;i<15;i++) writeReg(i,0);
  cpuMode=0;
  setStack(ECPUABSTRACT_DEFAULT_STACK);
  setFramePtr(ECPUABSTRACT_DEFAULT_STACK);
  setResetStack(ECPUABSTRACT_DEFAULT_STACK);
  setResetEntry(ECPUABSTRACT_DEFAULT_CODE);
}


/* STATIC */
bool EArm::recognize(const char *tname)
{
  const char *name=aDefaultStringHelper.removeSpaces2(aDefaultStringHelper.toLower(tname));
  if(!strcmp(name,"arm")) return true; else
  if(!strcmp(name,"arm1")) return true; else
  if(!strcmp(name,"arm2")) return true; else
  if(!strcmp(name,"arm2a")) return true; else
  if(!strcmp(name,"arm3")) return true; else
  if(!strcmp(name,"arm3m")) return true; else
  if(!strcmp(name,"arm3g")) return true; else
  if(!strcmp(name,"arm4")) return true; else
  if(!strcmp(name,"arm4xm")) return true; else
  if(!strcmp(name,"arm4txm")) return true; else
  if(!strcmp(name,"arm4t")) return true; else
  if(!strcmp(name,"arm5")) return true; else
  if(!strcmp(name,"arm5xm")) return true; else
  if(!strcmp(name,"arm5t")) return true; else
  if(!strcmp(name,"arm5txm")) return true; else
  if(!strcmp(name,"arm5texp")) return true; else
  if(!strcmp(name,"arm5te")) return true; else
  if(!strcmp(name,"arm5tel")) return true; else
  if(!strcmp(name,"arm5tejl")) return true; else
  if(!strcmp(name,"armv1")) return true; else
  if(!strcmp(name,"armv2")) return true; else
  if(!strcmp(name,"armv2a")) return true; else
  if(!strcmp(name,"armv3")) return true; else
  if(!strcmp(name,"armv3m")) return true; else
  if(!strcmp(name,"armv3g")) return true; else
  if(!strcmp(name,"armv4")) return true; else
  if(!strcmp(name,"armv4xm")) return true; else
  if(!strcmp(name,"armv4txm")) return true; else
  if(!strcmp(name,"armv4t")) return true; else
  if(!strcmp(name,"armv5")) return true; else
  if(!strcmp(name,"armv5xm")) return true; else
  if(!strcmp(name,"armv5t")) return true; else
  if(!strcmp(name,"armv5txm")) return true; else
  if(!strcmp(name,"armv5texp")) return true; else
  if(!strcmp(name,"armv5te")) return true; else
  if(!strcmp(name,"armv5tel")) return true; else
  if(!strcmp(name,"armv5tejl")) return true; else
  if(!strcmp(name,"strongarm")) return true; else
  if(!strcmp(name,"xscale")) return true; else
  if(!strcmp(name,"pxa")) return true; else
  if(!strcmp(name,"pxa250")) return true; else
  if(!strcmp(name,"pxa253")) return true; else
  if(!strcmp(name,"pxa255")) return true; else
  if(!strcmp(name,"pxa260")) return true; else
  if(!strcmp(name,"pxa261")) return true; else
  if(!strcmp(name,"pxa262")) return true; else
  if(!strcmp(name,"pxa263")) return true; else
  if(!strcmp(name,"pxa270")) return true; else
  if(!strcmp(name,"pxa272")) return true; else
  return false;
}


const char *EArm::getName()
{
  return "ARM";
}


void EArm::setKind(const char *tname)
{
  const char *name=aDefaultStringHelper.removeSpaces2(aDefaultStringHelper.toLower(tname));
  type=E_CPU_ARM;
}


void EArm::reset()
{
#ifdef DEBUG_EXEC
  setTrace(true);
#endif
  breakpoint=0xffffffff;  // since we start at 0, breakpoint's default
  intLock=false;
  pc=0;  // Jump to zero
  cpsr=0;
  clearStackTrace();
  ECPUAbstract::reset();
}


bool EArm::setRegister(const char *tname,ULONG value)
{
  const char *name=aDefaultStringHelper.removeSpaces2(aDefaultStringHelper.toLower(tname));
  bool ret=false;
  if(!strcmp(name,"pc"))
  { pc=value; ret=true; } else
  // TODO: Doesn't ARM use one of the general regs as stack?!?
  if(!strcmp(name,"sp"))
  { sp=value; ret=true; }
  //free(name);
  return ret;
}


ULONG EArm::readRegister(const char *tname)
{
  const char *name=aDefaultStringHelper.removeSpaces2(aDefaultStringHelper.toLower(tname));
  // TODO: Doesn't ARM use one of the general regs as stack?!?
  if(!strcmp(name,"sp")) { return sp; }
  return 0;
}


void EArm::setStack(ULONG location)
{
  // TODO: make sure in sync with R13
  sp=location;
}


ULONG EArm::readStack()
{
  // TODO: make sure in sync with R13
  return sp;
}


void EArm::setFramePtr(ULONG location)
{
  // TODO: use R11
}


ULONG EArm::readFramePtr()
{
  // TODO: use R11
  return sp;
}


const char *EArm::getRegName(unsigned int n)
{
  const char *ret;
  switch(n) {
    case 0x0: ret="r0"; break;
    case 0x1: ret="r1"; break;
    case 0x2: ret="r2"; break;
    case 0x3: ret="r3"; break;
    case 0x4: ret="r4"; break;
    case 0x5: ret="r5"; break;
    case 0x6: ret="r6"; break;
    case 0x7: ret="r7"; break;
    case 0x8: ret="r8"; break;
    case 0x9: ret="r9"; break;
    case 0xa: ret="r10"; break;
    case 0xb: ret="fp"; break;
    case 0xc: ret="ip"; break;
    case 0xd: ret="sp"; break;
    case 0xe: ret="lr"; break;
    case 0xf: ret="pc"; break;
    default: ret="???"; break;
  }
  return ret;
}


void EArm::writeReg(unsigned int n,ULONG val)
{
  if(n==15) { pc=val;  return; }
  if(n==0xd) { sp=val;  return; }
  rrrr[n]=val;
}


ULONG EArm::readReg(unsigned int n)
{
  if(n==15) return pc;
  if(n==0xd) return sp;
  return rrrr[n];
}


void EArm::wregs()
{
  DEBUG_OUT<<"PC="<<pc<<"\n";
  DEBUG_OUT<<"CPSR="<<cpsr<<"\n";
  for(unsigned int i=0;i<8;i++) {
    DEBUG_OUT<<getRegName(i)<<"="<<(ULONG)readReg(i)<<"  ";
    DEBUG_OUT<<getRegName(i+8)<<"="<<(ULONG)readReg(i+8)<<"\n";
  }
}


void EArm::handle_int(ULONG value)
{
}


void EArm::cause_int(ULONG value)
{
  int_pending=0;
  switch(value) {
    default:
      debugger("Illegal interrupt requested!\n");
      break;
  }
}


void EArm::ints()
{
}


void EArm::execute()
{
  if(pc==breakpoint) { debugger("Breakpoint!\n"); return; }
  opcode=read32mem(pc); // always little endian?
  unsigned int topcode=(opcode>>20)&0xff;
  unsigned int ccode=(opcode>>28)&0xf;
  //op_len=EARM_size[topcode];
  op_len=4;
  cycle=EArm_time[topcode];
  mode=EArm_format[topcode];
  decode();
  ULONG loc=pc;
  if(willExecute) {
#include <cpulib/generated/EArm_exec.h>
  }
  pc+=op_len;
  cycles+=cycle;
  if(int_pending) handle_int(int_pending);
}


ULONG EArm::disasm(ULONG loc)
{
  unsigned int topcode;
  const char *name="???";
  opcode=read32mem(loc); // always little endian?
  topcode=(opcode>>20)&0xff;
  unsigned int ccode=(opcode>>28)&0xf;
  lastSymbol=map->findAnyExactSymbol(loc);
  if(lastSymbol) DEBUG_OUT<<lastSymbol->getName()<<":\n";
  DEBUG_OUT<<"";
  DEBUG_OUT<<"."; DEBUG_OUT<<loc;  DEBUG_OUT<<"  ";
  op_len=4;
  mode=EArm_format[topcode];
  name=EArm_name[topcode];
  decode();
  unsigned int t;
  for(t=0;t<op_len;t++) DEBUG_OUT<<(short)map->read8(loc+t)<<" ";
  DEBUG_OUT<<"  ";
  DEBUG_OUT<<name<<" ";
  printAddrMode(loc,topcode,mode);
  DEBUG_OUT<<"";
  DEBUG_OUT<<"\n";
  return op_len;
}


unsigned int EArm::printAddrMode(ULONG loc,unsigned int topcode,unsigned int mode)
{
  opcode1=topcode;
  if(op1.mode) { disOperand(loc,&op1); }
  if(op2.mode) { DEBUG_OUT<<",";  disOperand(loc,&op2); }
  if(op3.mode) { DEBUG_OUT<<",";  disOperand(loc,&op3); }
#ifdef DEBUG_DIS_MODES
  DEBUG_OUT<<" ;"<<opcode1<<","<<opcode2<<","<<mode;
#endif // DEBUG_DIS_MODES
  return 0;
}


void EArm::printCC(unsigned int cond)
{
  switch(cond) {
    case EARM_CC_EQ: DEBUG_OUT<<"eq"; break;
    case EARM_CC_NE: DEBUG_OUT<<"ne"; break;
    case EARM_CC_CS: DEBUG_OUT<<"cs"; break;
    case EARM_CC_CC: DEBUG_OUT<<"cc"; break;
    case EARM_CC_MI: DEBUG_OUT<<"mi"; break;
    case EARM_CC_PL: DEBUG_OUT<<"pl"; break;
    case EARM_CC_VS: DEBUG_OUT<<"vs"; break;
    case EARM_CC_VC: DEBUG_OUT<<"vc"; break;
    case EARM_CC_HI: DEBUG_OUT<<"hi"; break;
    case EARM_CC_LS: DEBUG_OUT<<"ls"; break;
    case EARM_CC_GE: DEBUG_OUT<<"ge"; break;
    case EARM_CC_LT: DEBUG_OUT<<"lt"; break;
    case EARM_CC_GT: DEBUG_OUT<<"gt"; break;
    case EARM_CC_LE: DEBUG_OUT<<"le"; break;
    case EARM_CC_ALWAYS: break;
    case EARM_CC_NEVER: break;
    default: DEBUG_OUT<<"???"; break;
  }
}


void EArm::disOperand(ULONG loc,ECPUOperand *op)
{
  EMapSymbol *e=NULL;
  unsigned int t;
  bool first=true;
  switch(op->mode) {
    case EARM_NONE:
      break;
    case EARM_REG:
      DEBUG_OUT<<getRegName(op->reg);
      break;
    case EARM_INDREG:
      DEBUG_OUT<<"("<<getRegName(op->reg);
      DEBUG_OUT<<",#"<<(USHORT)op->val;
      DEBUG_OUT<<")";
      break;
    case EARM_MULTREG:
      DEBUG_OUT<<"{";
      for(t=0;t<16;t++) {
        val=power2(t);
        if(val&op->val) {
          if(!first) DEBUG_OUT<<",";
          DEBUG_OUT<<getRegName(t);
          first=false;
        }
      }
      DEBUG_OUT<<"}";
      break;
    case EARM_IMM:
      DEBUG_OUT<<"#"<<op->val;
      break;
    case EARM_DISP:
      ea=loc+op->val+8;
      DEBUG_OUT<<"$"<<ea;
      e=map->findAnyNearestPrevSymbol(ea);
      if(e) {
        DEBUG_OUT<<" ("<<e->getName();
        if(e->addr!=ea) {
          DEBUG_OUT<<"+0x"<<(ea-(e->addr));
        }
        DEBUG_OUT<<")";
      }
      break;
    default:
      DEBUG_OUT<<"Mode was "<<op->mode<<"\n";
      debugger("Bad mode in disOperand");
      break;
  }
}


unsigned int EArm::readOperand(ECPUOperand *op)
{
  val=0;
  switch(op->mode) {
    case EARM_NONE:
      break;
    case EARM_REG:
      val=readReg(op->reg);
      break;
    case EARM_INDREG:
      ea=readReg(op->reg)+op->val;
      val=read32mem(ea);
      break;
    case EARM_MULTREG:
      //debugger("readOperand multreg not implemented");
      break;
    case EARM_IMM:
      val=op->val;
      break;
    case EARM_DISP:
      val=pc+op->val+8;
      break;
    default:
      DEBUG_OUT<<"Mode was "<<op->mode<<"\n";
      debugger("Bad mode in readOperand");
      break;
  }
#ifdef DEBUG_EXEC
  DEBUG_OUT<<"readOperand("<<op->mode<<","<<op->reg<<","<<op->val<<")="<<val<<"\n";
#endif // DEBUG_EXEC
  return val;
}


void EArm::writeOperand(ECPUOperand *op,unsigned int val)
{
  switch(op->mode) {
    case EARM_NONE:
      break;
    case EARM_REG:
      writeReg(op->reg,val);
      break;
    case EARM_INDREG:
      ea=readReg(op->reg)+op->val;
      write32mem(ea,val);
      break;
    case EARM_MULTREG:
      //debugger("writeOperand multreg not implemented");
      break;
    default:
      DEBUG_OUT<<"Mode was "<<op->mode<<"\n";
      debugger("Bad mode in writeOperand");
      break;
  }
#ifdef DEBUG_EXEC
  DEBUG_OUT<<"writeOperand("<<op->mode<<","<<op->reg<<","<<op->val<<")="<<val<<"\n";
#endif // DEBUG_EXEC
}


void EArm::decode()
{
  willExecute=false;
  cond=(opcode>>28)&0xf;
  op1.init();  op2.init();  op3.init();
  unsigned int rn,rd,rs,rm,imm;
  switch(mode) {
    case EARM_NONE:
      break;
    case EARM_SPECIAL:
      decode_special();
      break;
    case EARM_ILLEGAL:
      debugger("illegal mode in decode!");
      break;
    case EARM_REG_IMM:
      rn=(opcode>>16)&0xf;
      rd=(opcode>>12)&0xf;
      rs=(opcode>>8)&0xf;
      opcode2=(opcode>>4)&0xf;
      imm=opcode&0xf;
      op1.mode=EARM_REG;  op1.reg=rd;
      op2.mode=EARM_IMM;  op2.val=imm;
      break;
    case EARM_REG_IMM2:
      rn=(opcode>>16)&0xf;
      rd=(opcode>>12)&0xf;
      rs=(opcode>>8)&0xf;
      opcode2=(opcode>>4)&0xf;
      imm=opcode&0xf;
      op1.mode=EARM_REG;  op1.reg=rn;
      op2.mode=EARM_IMM;  op2.val=imm;
      break;
    case EARM_REG_REG:
      rn=(opcode>>16)&0xf;
      rd=(opcode>>12)&0xf;
      rs=(opcode>>8)&0xf;
      opcode2=(opcode>>4)&0xf;
      rm=opcode&0xf;
      op1.mode=EARM_REG;  op1.reg=rd;
      op2.mode=EARM_REG;  op2.reg=rm;
      break;
    case EARM_REG_REG2:
      rn=(opcode>>16)&0xf;
      rd=(opcode>>12)&0xf;
      rs=(opcode>>8)&0xf;
      opcode2=(opcode>>4)&0xf;
      rm=opcode&0xf;
      op1.mode=EARM_REG;  op1.reg=rn;
      op2.mode=EARM_REG;  op2.reg=rm;
      break;
    case EARM_REG_REG_REG:
      rn=(opcode>>16)&0xf;
      rd=(opcode>>12)&0xf;
      rs=(opcode>>8)&0xf;
      opcode2=(opcode>>4)&0xf;
      rm=opcode&0xf;
      op1.mode=EARM_REG;  op1.reg=rd;
      op2.mode=EARM_REG;  op2.reg=rn;
      op3.mode=EARM_REG;  op3.reg=rm;
      break;
    case EARM_REG_REG_IMM:
      rn=(opcode>>16)&0xf;
      rd=(opcode>>12)&0xf;
      rs=(opcode>>8)&0xf;
      opcode2=(opcode>>4)&0xf;
      imm=opcode&0xf;
      op1.mode=EARM_REG;  op1.reg=rd;
      op2.mode=EARM_REG;  op2.reg=rn;
      op3.mode=EARM_IMM;  op3.val=imm;
      break;
    case EARM_DISP:
      imm=sign_ext(opcode&0xffffff,24);
      op1.mode=EARM_DISP;  op1.val=imm*4;
      break;
    case EARM_REG_ADDR:
      rn=(opcode>>16)&0xf;
      rd=(opcode>>12)&0xf;
      imm=opcode&0xfff;
      op1.mode=EARM_REG;  op1.reg=rd;
      op2.mode=EARM_INDREG;  op2.reg=rn;  op2.val=-imm;
      break;
    case EARM_MULTREG:
      rn=(opcode>>16)&0xf;
      imm=opcode&0xffff;
      op1.mode=EARM_REG;  op1.reg=rn;
      op2.mode=EARM_MULTREG;  op2.val=imm;
      break;
    default:
      DEBUG_OUT<<"Mode was "<<mode<<"\n";
      debugger("unknown mode in decode!");
      break;
  }
  if(cond==EARM_CC_NEVER) {
    decode_special();
  }
  else {
    if(cond==EARM_CC_ALWAYS) willExecute=true;
    else {
      // TODO: Check cond code, set willExecute to true if necessary
    }
  }
}


void EArm::decode_special()
{
  // TODO: figure out if this is one of the special opcodes.
  willExecute=false;
}


void EArm::exec_mov(unsigned int loc,unsigned int opcode)
{
  ULONG val=readOperand(&op2);
  writeOperand(&op1,val);
}


void EArm::exec_mvn(unsigned int loc,unsigned int opcode)
{
  debugger("mvn unimplemented");
}


void EArm::exec_add(unsigned int loc,unsigned int opcode)
{
  debugger("add unimplemented");
}


void EArm::exec_and(unsigned int loc,unsigned int opcode)
{
  debugger("and unimplemented");
}


void EArm::exec_cmp(unsigned int loc,unsigned int opcode)
{
  debugger("cmp unimplemented");
}


void EArm::exec_cmn(unsigned int loc,unsigned int opcode)
{
  debugger("cmn unimplemented");
}


void EArm::exec_sub(unsigned int loc,unsigned int opcode)
{
  ULONG val=readOperand(&op2)-readOperand(&op3);
  writeOperand(&op1,val);
}


void EArm::exec_str(unsigned int loc,unsigned int opcode)
{
  ULONG val=readOperand(&op1);
  writeOperand(&op2,val);
}


void EArm::exec_ldr(unsigned int loc,unsigned int opcode)
{
  ULONG val=readOperand(&op2);
  writeOperand(&op1,val);
}


void EArm::exec_strb(unsigned int loc,unsigned int opcode)
{
  debugger("strb unimplemented");
}


void EArm::exec_ldrb(unsigned int loc,unsigned int opcode)
{
  debugger("ldrb unimplemented");
}


// Probably still not right...
void EArm::exec_ldm(unsigned int loc,unsigned int opcode)
{
  bool dir=false;  // direction (inc or dec)
  bool pre=false;  // pre or post inc or dec
  unsigned int addr=readReg(op1.reg);
  for(unsigned int t=0;t<16;t++) {
    unsigned int vbit=power2(t);
    if(vbit&op2.val) {
      if(pre) { if(dir) addr+=4; else addr-=4; }
      val=read32mem(addr);
      if(!pre) { if(dir) addr+=4; else addr-=4; }
      writeReg(t,val);
      DEBUG_OUT<<"ldm reg#"<<t<<"="<<val<<" from "<<addr<<"\n";
    }
  }
}


// Probably still not right...
void EArm::exec_stm(unsigned int loc,unsigned int opcode)
{
  bool dir=false;  // direction (inc or dec)
  bool pre=false;  // pre or post inc or dec
  unsigned int addr=readReg(op1.reg);
  for(unsigned int t=0;t<16;t++) {
    unsigned int vbit=power2(t);
    if(vbit&op2.val) {
      val=readReg(t);
      if(pre) { if(dir) addr+=4; else addr-=4; }
      write32mem(addr,val);
      if(!pre) { if(dir) addr+=4; else addr-=4; }
      DEBUG_OUT<<"ldm reg#"<<t<<"="<<val<<" from "<<addr<<"\n";
    }
  }
}


void EArm::exec_bl(unsigned int loc,unsigned int opcode)
{
  writeReg(14,pc);
  jump(readOperand(&op1));
}


void EArm::exec_bcc(unsigned int loc,unsigned int opcode)
{
  debugger("bcc unimplemented");
}


