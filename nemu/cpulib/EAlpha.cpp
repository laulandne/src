
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#define DEBUG_DIS_MODES 1

#define DEBUG_EXEC 1

#define CHECK_SAFE 1

#define SHOW_INTS  1


#include <cpulib/EAlpha.h>

#include <elib/EMemMapAbstract.h>
#include <elib/EModule.h>

#include <portable/nickcpp/ANullStream.h>

#include <portable/nickcpp/AStringHelper.h>


#include <cpulib/generated/EAlpha_tab.h>


// The different cpus...
#define E_CPU_NONE     0x00
#define E_CPU_ALPHA    0x01


////////////////////////////////////////////////////////////////////////////////
//  EAlpha Class
////////////////////////////////////////////////////////////////////////////////

EAlpha::EAlpha()
{
  DEBUG_OUT<<"EAlpha created...\n";
  init();
  type=E_CPU_ALPHA;
  //w=ww;
  reset();
}


EAlpha::~EAlpha()
{
  DEBUG_OUT<<"EAlpha destroyed.\n";
}


void EAlpha::init()
{
  ECPURisc::init();
  // Generic features
  doTrace=false;  doRun=false;
  // Set up stack and registers
  int_pending=0;
  intLock=false;
  lsb=true;
  for(unsigned int t=0;t<32;t++) { writeReg(t,0);  F[t]=0.0; }
  setStack(ECPUABSTRACT_DEFAULT_STACK);
  setFramePtr(ECPUABSTRACT_DEFAULT_STACK);
  setResetStack(ECPUABSTRACT_DEFAULT_STACK);
  setResetEntry(ECPUABSTRACT_DEFAULT_CODE);
}


/* STATIC */
bool EAlpha::recognize(const char *tname)
{
  const char *name=aDefaultStringHelper.removeSpaces2(aDefaultStringHelper.toLower(tname));
  if(!strcmp(name,"alpha")) return true; else
  if(!strcmp(name,"alpha32")) return true; else
  if(!strcmp(name,"alpha64")) return true; else
  if(!strcmp(name,"ev7")) return true; else
  if(!strcmp(name,"ev79")) return true; else
  return false;
}


const char *EAlpha::getName()
{
  return "Alpha";
}


void EAlpha::setKind(const char *tname)
{
  const char *name=aDefaultStringHelper.removeSpaces2(aDefaultStringHelper.toLower(tname));
  type=E_CPU_ALPHA;
}


void EAlpha::reset()
{
#ifdef DEBUG_EXEC
  setTrace(true);
#endif
  breakpoint=0xffffffff;  // since we start at 0, breakpoint's default
  intLock=false;
  for(unsigned int t=0;t<32;t++) { writeReg(t,0);  F[t]=0.0; }
  pc=0;  // Jump to zero
  clearStackTrace();
  ECPUAbstract::reset();
}


bool EAlpha::setRegister(const char *tname,ULONG value)
{
  const char *name=aDefaultStringHelper.removeSpaces2(aDefaultStringHelper.toLower(tname));
  bool ret=false;
  if(!strcmp(name,"pc"))
  { pc=value; ret=true; } else
  if(!strcmp(name,"sp"))
  { sp=value; ret=true; }
  //free(name);
  return ret;
}


ULONG EAlpha::readRegister(const char *name)
{
  if(!strcmp(name,"sp")) { return sp; }
  return 0;
}


void EAlpha::wregs()
{
  DEBUG_OUT<<"PC="<<pc<<"\n";
  for(unsigned int r=0;r<8;r++) {
    DEBUG_OUT<<getRegName(r*4)<<"=";
    for(unsigned int c=0;c<4;c++)  DEBUG_OUT<<rrrr[r*4+c]<<" ";
    DEBUG_OUT<<"\n";
  }
}


void EAlpha::handle_int(ULONG value)
{
}


void EAlpha::cause_int(ULONG value)
{
  int_pending=0;
  switch(value) {
    default:
      debugger("Illegal interrupt requested!\n");
      break;
  }
}


void EAlpha::ints()
{
}


const char *EAlpha::getRegName(unsigned int n)
{
  char *ret;
  switch(n) {
    case 0x0: ret="v0"; break;
    case 0x1: ret="t0"; break;
    case 0x2: ret="r2"; break;
    case 0x3: ret="r3"; break;
    case 0x4: ret="r4"; break;
    case 0x5: ret="r5"; break;
    case 0x6: ret="r6"; break;
    case 0x7: ret="r7"; break;
    case 0x8: ret="r8"; break;
    case 0x9: ret="r9"; break;
    case 0xa: ret="r10"; break;
    case 0xb: ret="r11"; break;
    case 0xc: ret="r12"; break;
    case 0xd: ret="r13"; break;
    case 0xe: ret="r14"; break;
    case 0xf: ret="fp"; break;
    case 0x10: ret="a0"; break;
    case 0x11: ret="a1"; break;
    case 0x12: ret="a2"; break;
    case 0x13: ret="a3"; break;
    case 0x14: ret="a4"; break;
    case 0x15: ret="a5"; break;
    case 0x16: ret="t8"; break;
    case 0x17: ret="t9"; break;
    case 0x18: ret="t10"; break;
    case 0x19: ret="t11"; break;
    case 0x1a: ret="ra"; break;
    case 0x1b: ret="t12"; break;
    case 0x1c: ret="at"; break;
    case 0x1d: ret="gp"; break;
    case 0x1e: ret="sp"; break;
    case 0x1f: ret="zero"; break;
    default: ret="???"; break;
  }
  return ret;
}


void EAlpha::writeReg(unsigned int n,ULONG val)
{
  if(n!=31) rrrr[n]=val;
  if(n==0x1e) sp=val;
#ifdef DEBUG_EXEC
  DEBUG_OUT<<"writeReg("<<n<<") "<<val<<"\n";
#endif // DEBUG_EXEC
}


ULONG EAlpha::readReg(unsigned int n)
{
  ULONG val=rrrr[n];
  if(n==0x1e) val=sp;
  if(n==31) val=0;
#ifdef DEBUG_EXEC
  DEBUG_OUT<<"readReg("<<n<<") "<<val<<"\n";
#endif // DEBUG_EXEC
  return val;
}


void EAlpha::execute()
{
  if(pc==breakpoint) { debugger("Breakpoint!\n"); return; }
  opcode=read32mem(pc); // always little endian?
  unsigned int topcodew=(opcode>>26)&0x3f;
  op_len=4;
  cycle=EAlpha_time[topcodew];
  mode=EAlpha_format[topcodew];
  ULONG loc=pc;
  decode();
#include <cpulib/generated/EAlpha_exec.h>
  pc+=op_len;
  cycles+=cycle;
  if(int_pending) handle_int(int_pending);
}


ULONG EAlpha::disasm(ULONG loc)
{
  unsigned int topcodew;
  unsigned int len=0;
  const char *name="???";
  opcode=read32mem(loc); // always little endian?
  topcode=(opcode>>26)&0x3f;
  lastSymbol=map->findAnyExactSymbol(loc);
  if(lastSymbol) DEBUG_OUT<<lastSymbol->getName()<<":\n";
  DEBUG_OUT<<"";
  DEBUG_OUT<<"."; DEBUG_OUT<<loc;  DEBUG_OUT<<"  ";
  len=4;
  mode=EAlpha_format[topcodew];
  name=EAlpha_name[topcodew];
  decode();
  DEBUG_OUT<<opcode;
  DEBUG_OUT<<"   ";
  DEBUG_OUT<<name<<" ";
  len+=printAddrMode(loc,topcodew,mode);
  DEBUG_OUT<<"";
  DEBUG_OUT<<"\n";
  return len;
}


unsigned int EAlpha::printAddrMode(ULONG loc,unsigned int topcode,unsigned int mode)
{
  if(op1.mode) { disOperand(loc,&op1); }
  if(op2.mode) { DEBUG_OUT<<",";  disOperand(loc,&op2); }
  if(op3.mode) { DEBUG_OUT<<",";  disOperand(loc,&op3); }
#ifdef DEBUG_DIS_MODES
  DEBUG_OUT<<" ; "<<topcode;
#endif // DEBUG_DIS_MODES
  return 0;
}


void EAlpha::decode()
{
  op1.init();  op2.init();  op3.init();
  unsigned int ra,rb,rc,imm;
  switch(mode) {
    case EALPHA_REG_DISP:
      ra=(opcode>>21)&0x1f;
      imm=sign_ext(opcode&0x1fffff,21);
      op1.mode=EALPHA_REG;  op1.reg=ra;
      op2.mode=EALPHA_IMM;  op2.reg=imm;
      break;
    case EALPHA_REG_REG_DISP:
      ra=(opcode>>21)&0x1f;
      rb=(opcode>>16)&0x1f;
      imm=sign_ext(opcode&0xffff,16);
      op1.mode=EALPHA_REG;  op1.reg=ra;
      op2.mode=EALPHA_INDREG;  op2.reg=rb;  op2.val=imm;
      break;
    case EALPHA_REG_REG_REG:
      ra=(opcode>>21)&0x1f;
      rb=(opcode>>16)&0x1f;
      rc=opcode&0x1f;
      op1.mode=EALPHA_REG;  op1.reg=ra;
      op2.mode=EALPHA_REG;  op2.reg=rb;
      op3.mode=EALPHA_REG;  op3.reg=rc;
      break;
    case EALPHA_REG_REG:
      ra=(opcode>>16)&0x1f;
      rb=opcode&0x1f;
      op1.mode=EALPHA_REG;  op1.reg=ra;
      op2.mode=EALPHA_REG;  op2.reg=rb;
      break;
    default:
      debugger("bad mode in decode!");
      break;
  }
}


void EAlpha::disOperand(ULONG loc,ECPUOperand *op)
{
  EMapSymbol *e=NULL;
  bool first=true;
  switch(op->mode) {
    case EALPHA_NONE:
      break;
    case EALPHA_REG:
      DEBUG_OUT<<getRegName(op->reg);
      break;
    case EALPHA_INDREG:
      DEBUG_OUT<<(USHORT)op->val;
      DEBUG_OUT<<"("<<getRegName(op->reg);
      DEBUG_OUT<<")";
      break;
    case EALPHA_IMM:
      DEBUG_OUT<<"#"<<op->val;
      break;
    case EALPHA_DISP:
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


ULONG EAlpha::readOperand(ECPUOperand *op)
{
  ULONG val=0;
  switch(op->mode) {
    case EALPHA_NONE:
      break;
    case EALPHA_REG:
      val=readReg(op->reg);
      break;
    case EALPHA_INDREG:
      ea=readReg(op->reg)+op->val;
      val=read32mem(ea);
      break;
    case EALPHA_IMM:
      val=op->val;
      break;
    case EALPHA_DISP:
      val=pc+op->val+8;
      break;
    default:
      DEBUG_OUT<<"Mode was "<<op->mode<<"\n";
      debugger("Bad mode in readOperand");
      break;
  }
#ifdef DEBUG_EXEC
  DEBUG_OUT<<"readOperand "<<val<<"\n";
#endif // DEBUG_EXEC
  return val;
}


void EAlpha::writeOperand(ECPUOperand *op,unsigned int val)
{
  switch(op->mode) {
    case EALPHA_NONE:
      break;
    case EALPHA_REG:
      if(op->reg!=31) writeReg(op->reg,val);
      break;
    case EALPHA_INDREG:
      ea=readReg(op->reg)+op->val;
      write32mem(ea,val);
      break;
    default:
      DEBUG_OUT<<"Mode was "<<op->mode<<"\n";
      debugger("Bad mode in writeOperand");
      break;
  }
#ifdef DEBUG_EXEC
  DEBUG_OUT<<"writeOperand "<<val<<"\n";
#endif // DEBUG_EXEC
}


ULONG EAlpha::getOperandEA(ECPUOperand *op)
{
  ULONG val=0;
  switch(op->mode) {
    case EALPHA_NONE:
      break;
    case EALPHA_REG:
      val=readReg(op->reg);
      break;
    case EALPHA_INDREG:
      ea=readReg(op->reg)+op->val;
      val=ea;
      break;
    case EALPHA_IMM:
      val=op->val;
      break;
    case EALPHA_DISP:
      val=pc+op->val+8;
      break;
    default:
      DEBUG_OUT<<"Mode was "<<op->mode<<"\n";
      debugger("Bad mode in getOperandEA");
      break;
  }
#ifdef DEBUG_EXEC
  DEBUG_OUT<<"getOperandEA "<<val<<"\n";
#endif // DEBUG_EXEC
  return val;
}


void EAlpha::do_store(ULONG addr,ULONG val,unsigned int size)
{
  // TODO: obviously size is ignored...
  write32mem(addr,val);
#ifdef DEBUG_EXEC
  DEBUG_OUT<<"do_store("<<addr<<","<<val<<")\n";
#endif // DEBUG_EXEC
}


ULONG EAlpha::do_load(ULONG addr,unsigned int size)
{
  // TODO: obviously size is ignored...
  ULONG val=read32mem(addr);
#ifdef DEBUG_EXEC
  DEBUG_OUT<<"do_load("<<addr<<")="<<val<<"\n";
#endif // DEBUG_EXEC
  return val;
}


void EAlpha::exec_lda(unsigned int loc,unsigned int opcode)
{
  ULONG addr=getOperandEA(&op2);
  ULONG val=do_load(addr,0);
  writeOperand(&op1,val);
}


void EAlpha::exec_ldah(unsigned int loc,unsigned int opcode)
{
  ULONG addr=getOperandEA(&op2);
  ULONG val=do_load(addr,0);
  writeOperand(&op1,val);
}


void EAlpha::exec_ldq(unsigned int loc,unsigned int opcode)
{
  ULONG addr=getOperandEA(&op2);
  ULONG val=do_load(addr,0);
  writeOperand(&op1,val);
}


void EAlpha::exec_ldl(unsigned int loc,unsigned int opcode)
{
  ULONG addr=getOperandEA(&op2);
  ULONG val=do_load(addr,0);
  writeOperand(&op1,val);
}


void EAlpha::exec_stl(unsigned int loc,unsigned int opcode)
{
  ULONG addr=getOperandEA(&op2);
  ULONG val=readOperand(&op1);
  do_store(addr,val,0);
}


void EAlpha::exec_stq(unsigned int loc,unsigned int opcode)
{
  ULONG addr=getOperandEA(&op2);
  ULONG val=readOperand(&op1);
  do_store(addr,val,0);
}


void EAlpha::exec_mov(unsigned int loc,unsigned int opcode)
{
  ULONG val=readOperand(&op1);
  writeOperand(&op2,val);
}


void EAlpha::exec_jsr(unsigned int loc,unsigned int opcode)
{
  do_jump(loc,opcode);
}


void EAlpha::exec_jmp(unsigned int loc,unsigned int opcode)
{
  do_jump(loc,opcode);
}


void EAlpha::exec_ret(unsigned int loc,unsigned int opcode)
{
  do_jump(loc,opcode);
}


void EAlpha::exec_jsr_coroutine(unsigned int loc,unsigned int opcode)
{
  do_jump(loc,opcode);
}


void EAlpha::do_jump(unsigned int loc,unsigned int opcode)
{
  ULONG addr=getOperandEA(&op2);
  ULONG val=pc+op_len;
  writeOperand(&op1,val);
  jump(addr);
}


