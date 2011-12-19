
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#define DEBUG_DIS_MODES 1

#define DEBUG_EXEC 1

#define CHECK_SAFE 1

#define SHOW_INTS  1


#include <cpulib/ESh.h>

#include <elib/EMemMapAbstract.h>
#include <elib/EModule.h>

#include <portable/nickcpp/ANullStream.h>

#include <portable/nickcpp/AStringHelper.h>


#include <cpulib/generated/ESh_tab.h>


// The different cpus...
#define E_CPU_NONE     0x00
#define E_CPU_SH       0x01


////////////////////////////////////////////////////////////////////////////////
//  ESh Class
////////////////////////////////////////////////////////////////////////////////

ESh::ESh()
{
  DEBUG_OUT<<"ESh created...\n";
  init();
  type=E_CPU_SH;
  //w=ww;
  reset();
}


ESh::~ESh()
{
  DEBUG_OUT<<"ESh destroyed.\n";
}


void ESh::init()
{
  ECPURisc::init();
  // Generic features
  doTrace=false;  doRun=false;
  // Set up stack and registers
  int_pending=0;
  intLock=false;
  pc=0; npc=pc+2;
  branchesAreDelayed=true;
  lsb=true;
  for(unsigned int t=0;t<16;t++) rrrr[t]=0;
  pr=0;
  dsr=0; a0=0; x0=0; x1=0; y1=0;
  mach=0;  macl=0;
  setStack(ECPUABSTRACT_DEFAULT_STACK);
  setFramePtr(ECPUABSTRACT_DEFAULT_STACK);
  setResetStack(ECPUABSTRACT_DEFAULT_STACK);
  setResetEntry(ECPUABSTRACT_DEFAULT_CODE);
}


/* STATIC */
bool ESh::recognize(const char *tname)
{
  const char *name=aDefaultStringHelper.removeSpaces2(aDefaultStringHelper.toLower(tname));
  if(!strcmp(name,"sh")) return true; else
  if(!strcmp(name,"sh64")) return true; else
  if(!strcmp(name,"sh1")) return true; else
  if(!strcmp(name,"sh2")) return true; else
  if(!strcmp(name,"sh3")) return true; else
  if(!strcmp(name,"sh3eb")) return true; else
  if(!strcmp(name,"sh4")) return true; else
  if(!strcmp(name,"sh4eb")) return true; else
  return false;
}


const char *ESh::getName()
{
  return "SuperHitachi";
}


void ESh::setKind(const char *tname)
{
  char *name=aDefaultStringHelper.removeSpaces2(aDefaultStringHelper.toLower(tname));
  type=E_CPU_SH;
}


void ESh::reset()
{
#ifdef DEBUG_EXEC
  setTrace(true);
#endif
  breakpoint=0xffff;  // since we start at 0, breakpoint's default
  intLock=false;
  for(unsigned int t=0;t<16;t++) rrrr[t]=0;
  pr=0;
  mach=0;  macl=0;
  pc=0;  // Jump to zero
  clearStackTrace();
  ECPUAbstract::reset();
}


bool ESh::setRegister(const char *tname,ULONG value)
{
  char *name=aDefaultStringHelper.removeSpaces2(aDefaultStringHelper.toLower(tname));
  bool ret=false;
  if(!strcmp(name,"pc"))
  { pc=value; ret=true; } else
  if(!strcmp(name,"sp"))
  { sp=value; ret=true; }
  //free(name);
  return ret;
}


ULONG ESh::readRegister(const char *tname)
{
  const char *name=aDefaultStringHelper.removeSpaces2(aDefaultStringHelper.toLower(tname));
  if(!strcmp(name,"sp")) { return sp; }
  return 0;
}


const char *ESh::getRegName(unsigned int num)
{
  switch(num) {
    case 0: return "r0"; break;
    case 1: return "r1"; break;
    case 2: return "r2"; break;
    case 3: return "r3"; break;
    case 4: return "r4"; break;
    case 5: return "r5"; break;
    case 6: return "r6"; break;
    case 7: return "r7"; break;
    case 8: return "r8"; break;
    case 9: return "r9"; break;
    case 10: return "r10"; break;
    case 11: return "r11"; break;
    case 12: return "r12"; break;
    case 13: return "r13"; break;
    case 14: return "r14"; break;
    case 15: return "r15"; break;
    default:
      debugger("Bad reg num in getRegName");
      break;
  }
  return "???";
}


ULONG ESh::readStack()
{
  return readReg(15);
}


void ESh::setStack(ULONG location)
{
  writeReg(15,location);
}


void ESh::writeReg(unsigned int n,ULONG val)
{
  rrrr[n]=val;
#ifdef DEBUG_EXEC
  DEBUG_OUT<<"writeReg("<<n<<") "<<val<<"\n";
#endif // DEBUG_EXEC
}


ULONG ESh::readReg(unsigned int n)
{
  ULONG val=rrrr[n];
#ifdef DEBUG_EXEC
  DEBUG_OUT<<"readReg("<<n<<") "<<val<<"\n";
#endif // DEBUG_EXEC
  return val;
}


const char *ESh::getSpecialRegName(unsigned int num)
{
  switch(num) {
    case 0: return "mach"; break;
    case 1: return "macl"; break;
    case 2: return "pr"; break;
    case 4: return "x0"; break;
    case 6: return "dsr"; break;
    case 7: return "a0"; break;
    case 9: return "x1"; break;
    case 10: return "y0"; break;
    case 11: return "y1"; break;
    default:
      debugger("Bad reg num in getSpecialRegName");
      break;
  }
  return "???";
}


ULONG ESh::readSpecialReg(unsigned int num)
{
  ULONG val=0;
  switch(num) {
    case 0: val=mach; break;
    case 1: val=macl; break;
    case 2: val=pr; break;
    case 4: val=x0; break;
    case 6: val=dsr; break;
    case 7: val=a0; break;
    case 9: val=x1; break;
    case 10: val=y0; break;
    case 11: val=y1; break;
    default:
      debugger("Bad reg num in readSpecialReg");
      break;
  }
  return val;
}


void ESh::writeSpecialReg(unsigned int num,ULONG val)
{
  switch(num) {
    case 0: mach=val; break;
    case 1: macl=val; break;
    case 2: pr=val; break;
    case 4: x0=val; break;
    case 6: dsr=val; break;
    case 7: a0=val; break;
    case 9: x1=val; break;
    case 10: y0=val; break;
    case 11: y1=val; break;
    default:
      debugger("Bad reg num in writeSpecialReg");
      break;
  }
}


void ESh::wregs()
{
  DEBUG_OUT<<"pc="<<pc<<" ";
  if(lsb) DEBUG_OUT<<"(Little)"; else DEBUG_OUT<<"(Big)";
  DEBUG_OUT<<"  pr="<<pr<<"\n";
  for(unsigned int i=0;i<8;i++) {
    DEBUG_OUT<<getRegName(i)<<"="<<(ULONG)rrrr[i]<<"  ";
    DEBUG_OUT<<getRegName(i+8)<<"="<<(ULONG)rrrr[i+8]<<"\n";
  }
  DEBUG_OUT<<"mach="<<mach<<" macl="<<macl<<"\n";
}


void ESh::handle_int(ULONG value)
{
}


void ESh::cause_int(ULONG value)
{
  int_pending=0;
  switch(value) {
    default:
      debugger("Illegal interrupt requested!\n");
      break;
  }
}


void ESh::ints()
{
  //if(w) {
  //}
}


void ESh::execute()
{
  if(pc==breakpoint) { debugger("Breakpoint!\n"); return; }
  opcode=read16mem(pc); // always little endian?
  unsigned int topcode=(opcode>>12)&0xf;
  op_len=2;
  tookBranch=false;
  mode=ESh_format[topcode];
  cycle=ESh_time[topcode];
  mode=ESh_format[topcode];
  ULONG loc=pc;
  decode();
#include <cpulib/generated/ESh_exec.h>
  pc+=op_len;
  cycles+=cycle;
  pc=npc;
  npc+=2;
  if(tookBranch) npc=branch_address;
  if(anull) { pc=npc; npc=pc+2; anull=false; }
  if(int_pending) handle_int(int_pending);
}


ULONG ESh::disasm(ULONG loc)
{
  unsigned int topcode;
  unsigned int len=0;
  opname="???";
  opcode=read16mem(loc); // always little endian?
  topcode=(opcode>>12)&0xf;
  lastSymbol=map->findAnyExactSymbol(loc);
  if(lastSymbol) DEBUG_OUT<<lastSymbol->getName()<<":\n";
  DEBUG_OUT<<"";
  DEBUG_OUT<<"."; DEBUG_OUT<<loc;  DEBUG_OUT<<"  ";
  len=2;
  mode=ESh_format[topcode];
  opname=ESh_name[topcode];
  decode();
  DEBUG_OUT<<opcode<<"  ";
  DEBUG_OUT<<opname<<" ";
  //DEBUG_OUT<<"Len was "<<len<<"\n";
  len+=printAddrMode(loc,topcode,mode);
  //DEBUG_OUT<<"Len now is "<<len<<"\n";
  DEBUG_OUT<<"";
  DEBUG_OUT<<"\n";
  return len;
}


unsigned int ESh::printAddrMode(ULONG loc,unsigned int topcode,unsigned int mode)
{
  if(op1.mode) { disOperand(loc,&op1); }
  if(op2.mode) { DEBUG_OUT<<",";  disOperand(loc,&op2); }
#ifdef DEBUG_DIS_MODES
  DEBUG_OUT<<" ; top="<<topcode<<",mode="<<mode<<":r1="<<r1<<",r2="<<r2<<",imm4="<<imm4;
#endif // DEBUG_DIS_MODES
  return 0;
}


void ESh::decode()
{
  op1.init();  op2.init();
  r1=(opcode>>8)&0xf;
  r2=(opcode>>4)&0xf;
  imm4=opcode&0xf;
  imm8=opcode&0xff;
  switch(mode) {
    case ESH_NONE:
      break;
    case ESH_SPECIAL:
      decode_special();
      break;
    case ESH_REG_REG:
      op1.mode=ESH_REG;  op1.reg=r2;
      op2.mode=ESH_REG;  op2.reg=r1;
      break;
    case ESH_IMM_REG:
      op1.mode=ESH_IMM;  op1.val=sign_ext(imm8,8);
      op2.mode=ESH_REG;  op2.reg=r1;
      break;
    case ESH_MOV:
      op1.mode=ESH_REG;  op1.reg=r2;
      op2.mode=ESH_INDREG;  op2.reg=r1;  op2.val=imm4;
      break;
    case ESH_REG:
      op1.mode=ESH_REG;  op1.reg=r1;
      break;
    case ESH_INDREG:
      op1.mode=ESH_INDREG;  op1.reg=r1;  op1.val=imm4;
      break;
    case ESH_DISPPC:
      op1.mode=ESH_DISPPC;  op1.reg=sign_ext(imm8,8);
      break;
    case ESH_DISPREG4_REG:
      op1.mode=ESH_DISPREG;  op1.reg=r1;  op1.val=imm4*4;  // TODO: sign_ext?
      op2.mode=ESH_REG;  op2.reg=r2;
      break;
    case ESH_DISPPC2_REG:
      op1.mode=ESH_DISPPC;  op1.val=imm8*2;  // TODO: sign_ext?
      op2.mode=ESH_REG;  op2.reg=r1;
      break;
    case ESH_DISPPC4_REG:
      op1.mode=ESH_DISPPC;  op1.val=imm8*4;  // TODO: sign_ext?
      op2.mode=ESH_REG;  op2.reg=r1;
      break;
    case ESH_REG_DISPREG4:
      op1.mode=ESH_REG;  op1.reg=r2;
      op2.mode=ESH_DISPREG;  op2.reg=r1;  op2.val=imm4*4;  // TODO: sign_ext?
      break;
    default:
      DEBUG_OUT<<"mode was "<<mode<<"\n";
      debugger("bad mode in decode!");
      break;
  }
}


void ESh::decode_special()
{
  unsigned int topcode=(opcode>>12)&0xf;
  bool good=false;
  switch(topcode) {
    case 0:
      switch(imm4) {
        case 2:
          opname="lds";
          op2.mode=ESH_SPECIALREG;  op2.reg=r2;
          op1.mode=ESH_REG;  op1.reg=r1;
          break;
        case 9:
          if((!r1)&&(!r2)) {
            good=true;
            opname="nop";
          }
          if(!good) {
            debugger("bad regs for imm4==9 for special0 in decode_special!");
          }
          break;
        case 10:
          opname="sts";
          op1.mode=ESH_SPECIALREG;  op1.reg=r2;
          op2.mode=ESH_REG;  op2.reg=r1;
          break;
        default:
          DEBUG_OUT<<"imm4 was "<<imm4<<"\n";
          debugger("bad imm4 for special0 in decode_special!");
          break;
      }
      break;
    case 4:
      switch(imm4) {
        case 2:
          opname="sts_l";
          op1.mode=ESH_SPECIALREG;  op1.reg=r2;
          op2.mode=ESH_PREDECINDREG;  op2.reg=r1;
          break;
        case 10:
          opname="lds_l";
          op2.mode=ESH_SPECIALREG;  op2.reg=r2;
          op1.mode=ESH_POSTINCINDREG;  op1.reg=r1;
          break;
        case 11:
          opname="jsr";
          op1.mode=ESH_INDREG;  op1.reg=r1;  op1.val=2;  // mode is 2
          break;
       default:
          DEBUG_OUT<<"imm4 was "<<imm4<<"\n";
          debugger("bad imm4 for special4 in decode_special!");
          break;
      }
      break;
    default:
      DEBUG_OUT<<"topcode was "<<topcode<<"\n";
      debugger("bad topcode in decode_special!");
      break;
  }
}


void ESh::disOperand(ULONG loc,ECPUOperand *op)
{
  EMapSymbol *e=NULL;
  bool first=true;
  switch(op->mode) {
    case ESH_NONE:
      break;
    case ESH_REG:
      DEBUG_OUT<<getRegName(op->reg);
      break;
    case ESH_PREDECINDREG:
      DEBUG_OUT<<"@-"<<getRegName(op->reg);
      break;
    case ESH_POSTINCINDREG:
      DEBUG_OUT<<"@"<<getRegName(op->reg)<<"+";
      break;
    case ESH_SPECIALREG:
      DEBUG_OUT<<getSpecialRegName(op->reg);
      break;
    case ESH_INDREG:
      switch(op->val)
      {
        case 2:
          DEBUG_OUT<<"@"<<getRegName(op->reg);
          break;
        case 3:
          DEBUG_OUT<<getRegName(op->reg);
          break;
        case 6:
          DEBUG_OUT<<"@-"<<getRegName(op->reg);
          break;
        default:
          DEBUG_OUT<<"val was "<<op->val<<"\n";
          debugger("Bad val for indreg!");
          break;
      }
      break;
    case ESH_DISPREG:
      DEBUG_OUT<<"@("<<op->val<<","<<getRegName(op->reg)<<")";
      break;
    case ESH_DISPPC:
      ea=pc+op->val+4;
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
    case ESH_IMM:
      DEBUG_OUT<<"#"<<op->val;
      break;
    default:
      DEBUG_OUT<<"Mode was "<<op->mode<<"\n";
      debugger("Bad mode in disOperand");
      break;
  }
}


ULONG ESh::readOperand(ECPUOperand *op)
{
  ULONG val=0;
  switch(op->mode) {
    case ESH_NONE:
      break;
    case ESH_REG:
      val=readReg(op->reg);
      break;
    case ESH_SPECIALREG:
      val=readSpecialReg(op->reg);
      break;
    case ESH_INDREG:
      switch(op->val)
      {
        case 2:
          ea=readReg(op->reg);
          val=read32mem(ea);
          break;
        case 3:
          val=readReg(op->reg);
          break;
        default:
          DEBUG_OUT<<"val was "<<op->val<<"\n";
          debugger("Bad val for indreg!");
          break;
      }
      break;
    case ESH_POSTINCINDREG:
      ea=readReg(op->reg);
      writeReg(op->reg,readReg(op->reg)+4);
      val=read32mem(ea);
      break;
    case ESH_IMM:
      val=op->val;
      break;
    case ESH_DISPREG:
      val=readReg(op->reg)+op->val;
      break;
    case ESH_DISPPC:
      val=pc+op->val+4;
      break;
    default:
      DEBUG_OUT<<"Mode was "<<op->mode<<"\n";
      debugger("Bad mode in readOperand");
      break;
  }
  return val;
}


void ESh::writeOperand(ECPUOperand *op,ULONG val)
{
  switch(op->mode) {
    case ESH_NONE:
      break;
    case ESH_REG:
      writeReg(op->reg,val);
      break;
    case ESH_SPECIALREG:
      writeSpecialReg(op->reg,val);
      break;
    case ESH_PREDECINDREG:
      writeReg(op->reg,readReg(op->reg)-4);
      ea=readReg(op->reg);
      write32mem(ea,val);
      break;
    case ESH_INDREG:
      switch(op->val)
      {
        case 2:
          ea=readReg(op->reg);
          write32mem(ea,val);
          break;
        case 3:
          writeReg(op->reg,val);
          break;
        case 6:
          writeReg(op->reg,readReg(op->reg)-4);
          ea=readReg(op->reg);
          write32mem(ea,val);
          break;
        default:
          DEBUG_OUT<<"val was "<<op->val<<"\n";
          debugger("Bad val for indreg!");
          break;
      }
      break;
    case ESH_DISPREG:
      ea=readReg(op->reg)+op->val;
      write32mem(ea,val);
      break;
    case ESH_DISPPC:
      ea=pc+op->val+4;
      write32mem(ea,val);
      break;
    default:
      DEBUG_OUT<<"Mode was "<<op->mode<<"\n";
      debugger("Bad mode in writeOperand");
      break;
  }
}


void ESh::exec_nop(unsigned int loc,unsigned int opcode)
{
  // Nothing to do
}


void ESh::exec_mov(unsigned int loc,unsigned int opcode)
{
  ULONG val=readOperand(&op1);
  writeOperand(&op2,val);
}


void ESh::exec_mov_l(unsigned int loc,unsigned int opcode)
{
  ULONG val=readOperand(&op1);
  writeOperand(&op2,val);
}


void ESh::exec_mov_w(unsigned int loc,unsigned int opcode)
{
  ULONG val=readOperand(&op1)&0xffff;
  writeOperand(&op2,val);
}


void ESh::exec_add(unsigned int loc,unsigned int opcode)
{
  ULONG val=readOperand(&op1)+readOperand(&op2);
  writeOperand(&op2,val);
}


void ESh::exec_bcc(unsigned int loc,unsigned int opcode)
{
  debugger("bcc unimplemented");
}


void ESh::exec_bra(unsigned int loc,unsigned int opcode)
{
  debugger("bra unimplemented");
}


void ESh::exec_bsr(unsigned int loc,unsigned int opcode)
{
  debugger("bsr unimplemented");
}


void ESh::exec_special0(unsigned int loc,unsigned int opcode)
{
  ULONG val;
  bool good=false;
  switch(imm4) {
    case 2:
      // lds from special reg
      val=readOperand(&op1);
      writeOperand(&op2,val);
      break;
    case 9:
      if((!r1)&&(!r2)) {  good=true;  exec_nop(loc,opcode); }
      if(!good) {
        debugger("bad regs for imm4==9 in exec_special0");
       }
      break;
    case 10:
      // sts from special reg
      val=readOperand(&op1);
      writeOperand(&op2,val);
      break;
    default:
      DEBUG_OUT<<"imm4 was "<<imm4<<"\n";
      debugger("bad imm4 in exec_special0");
      break;
  }
}


void ESh::exec_special4(unsigned int loc,unsigned int opcode)
{
  ULONG val;
  bool good=false;
  switch(imm4) {
    case 2:
      // sts.l from special reg with predec
      val=readOperand(&op1);
      writeOperand(&op2,val);
      break;
    case 10:
      // lds.l from special reg with postinc
      val=readOperand(&op1);
      writeOperand(&op2,val);
      break;
    case 11:
      // jsr indreg
      pr=pc;
      ea=readOperand(&op1);
      jump(ea);
      break;
    default:
      DEBUG_OUT<<"imm4 was "<<imm4<<"\n";
      debugger("bad imm4 in exec_special4");
      break;
  }
}


