
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#include <portable/nickcpp/AStringHelper.h>

#include <cpulib/ECPUAbstract.h>

#include <elib/EMemMapAbstract.h>
#include <elib/EHandler.h>

#include <portable/nickcpp/ANullStream.h>


//#define TRACE_ALL_JUMPS 1


////////////////////////////////////////////////////////////////////////////////
//  ECPUAbstract Class
////////////////////////////////////////////////////////////////////////////////

ECPUAbstract::ECPUAbstract()
{
  init();
  DEBUG_OUT<<"ECPUAbstract created...\n";
  cyclesPerSec=1000000;  // i.e. 1MHZ (for now)
}


void ECPUAbstract::init()
{
  // Init all data members
  debug=false;
  type=0;
  next=(ECPUAbstract *)NULL;
  map=(EMemMapAbstract *)NULL;
  io=(EHandler *)NULL;
  lastSymbol=(EMapSymbol *)NULL;
  trapHandler=(EHandler *)NULL;
  breakpoint=0;
  int_pending=0;
  op=0;  ea=0;  cycle=0;  op_len=0;  opcode=0;  val=0;  topcode=0;
  running=false;
  trace=false;
  doBreak=true;
  doTrace=false;
  doRun=false;
  cyclesPerSec=1000000;  // i.e. 1MHZ (for now)
  cycles=0;
  intLock=false;
  //
  hasResetEntry=false;
  resetEntry=0;
  hasResetStack=false;
  resetStack=0;
  //
  hasFPU=false;
  hasMMU=false;
  lastStackPtr=0;
  traceStack=false;
  lsb=false;
  bitness=8;
  stackTrace=new AList;
  canReassemble=true;
  // Set stack and frame ptr to arbitrary, but workable values
  //setStack(ECPUABSTRACT_DEFAULT_STACK);
  //setFramePtr(ECPUABSTRACT_DEFAULT_STACK);
  //setResetStack(ECPUABSTRACT_DEFAULT_STACK);
  //setResetEntry(ECPUABSTRACT_DEFAULT_CODE);
}


ECPUAbstract::~ECPUAbstract()
{
  DEBUG_OUT<<"ECPUAbstract destroyed.\n";
  stackTrace->kill();
  delete stackTrace;
  stackTrace=NULL;
}


const char *ECPUAbstract::getName()
{
  debugger("Parent getName called!\n");
  return "None";
}


void ECPUAbstract::setKind(const char *name)
{
  debugger("Parent setKind called!\n");
}


bool ECPUAbstract::setRegister(const char *name,ULONG value)
{
  debugger("Parent setRegister called!\n");
  return false;
}


ULONG ECPUAbstract::readRegister(const char *name)
{
  debugger("Parent readRegister called!\n");
  return 0;
}


void ECPUAbstract::changeMap(EMemMapAbstract *mmap)
{
  //debugger("!  Parent changeMap called!\n";
}


void ECPUAbstract::ints()
{
  // Should always be overridden...
}


void ECPUAbstract::setMap(EMemMapAbstract *m)
{
  map=m;
  changeMap(map);
}


void ECPUAbstract::execute()
{
  debugger("execute not implemented for this cpu!\n");
}


void ECPUAbstract::executeInstead(ULONG opcode)
{
  debugger("executeInstead not implemented for this cpu!\n");
}


void ECPUAbstract::handle_int(ULONG value)
{
  debugger("Parent handle_int called!\n");
}


void ECPUAbstract::setIO(EHandler *handler)
{
  io=handler;
  if(io) io->setCPU(this);
}


void ECPUAbstract::setTrapHandler(EHandler *handler)
{
  trapHandler=handler;
  if(trapHandler) trapHandler->setCPU(this);
}


void ECPUAbstract::cause_int(ULONG value)
{
  debugger("Parent cause_ints called!\n");
}


ULONG ECPUAbstract::disasm(ULONG loc)
{
  debugger("Parent disasm called!\n");
  return 0;
}


void ECPUAbstract::jump(ULONG location)
{
  debugger("ECPUAbstract::jump called!");
}


void ECPUAbstract::jsr(ULONG location)
{
  debugger("ECPUAbstract::jsr called!");
}


void ECPUAbstract::rts()
{
  debugger("ECPUAbstract::rts called!");
}


void ECPUAbstract::pushAddr(ULONG location)
{
  debugger("ECPUAbstract::pushAddr called!");
}


ULONG ECPUAbstract::pullAddr()
{
  debugger("ECPUAbstract::pullAddr called!");
  return 0;
}


void ECPUAbstract::setStack(ULONG location)
{
  debugger("Parent setStack called!\n");
}


ULONG ECPUAbstract::readStack()
{
  debugger("Parent readStack called!\n");
  return 0;
}


void ECPUAbstract::setMachineSpecificRegister(unsigned int n,unsigned int val)
{
  debugger("Parent setMachineSpecificRegister called!\n");
}


unsigned int ECPUAbstract::readMachineSpecificRegister(unsigned int n)
{
  debugger("Parent readMachineSpecificRegister called!\n");
  return 0;
}


void ECPUAbstract::setFramePtr(ULONG location)
{
  debugger("Parent setFramePtr called!\n");
}


ULONG ECPUAbstract::readFramePtr()
{
  debugger("Parent readFramePtr called!\n");
  return 0;
}


void ECPUAbstract::reset()
{
  DEBUG_OUT<<"ECPUAbstract::reset()\n";
  if(hasResetStack) setStack(resetStack);
  if(hasResetStack) setFramePtr(resetStack);
  if(hasResetEntry) { DEBUG_OUT<<"Jumping to resetEntry:"<<resetEntry<<"\n";  jump(resetEntry); }
}


void ECPUAbstract::nmi()
{
  //DEBUG_OUT<<"Parent nmi called!\n";
  debugger("Parent nmi called!\n");
}


void ECPUAbstract::debugger(const char *str)
{
  if(debug) return;
  op_len=0;
  //disasm();
  debug=true;
  DEBUG_OUT<<str<<"\n";
}


void ECPUAbstract::wregs()
{
  debugger("Parent wregs called!\n");
}


// TODO: This is just bare-bones to get the job done...
void ECPUAbstract::setFPU(const char *n)
{
  if(n) hasFPU=true;
}


void ECPUAbstract::setReturnReg(ULONG val)
{
  debugger("Parent setReturnReg called!\n");
}


ULONG ECPUAbstract::readReturnReg()
{
  debugger("Parent readReturnReg called!\n");
  return 0;
}


ULONG ECPUAbstract::readParamReg(unsigned int num)
{
  debugger("Parent readParamReg called!\n");
  return 0;
}


void ECPUAbstract::setParamReg(unsigned int num,ULONG val)
{
  debugger("Parent setParamReg called!\n");
}


// NOTE numBits is how many bits it currently is
long ECPUAbstract::sign_ext(ULONG num, ULONG numbits)
{
  //return (int)((num>>(32-numbits))<<(32-numbits));
  ULONG dbits=32-numbits;
  //DEBUG_OUT<<"num was "<<num<<", dbits was "<<dbits<<"\n";
  ULONG rval=num<<dbits;
  ULONG sign=rval&ECPUABSTRACT_TOP_LONG;
  //DEBUG_OUT<<"sign was "<<sign<<", rval was "<<rval<<"\n";
  for(ULONG t=0;t<dbits;t++) rval=(sign|rval)>>1;
  rval=sign|rval;
  return (int)rval;
}


// NOTE numBits is how many bits it currently is
ULONG ECPUAbstract::unsign_ext(ULONG num, ULONG numbits)
{
  int val;
  val=sign_ext(num,numbits);
  if(val<0) val= -val;
  return (ULONG)val;
}


/*
ULONG ECPUAbstract::getMaskSize(ULONG size)
{
  ULONG ret=0;
  switch(size) {
    case 1:  ret=0xff;  break;
    case 2:  ret=0xffff; break;
    case 3:  ret=0xffffff; break;
    case 4:  ret=0xffffffff; break;
    default:
      debugger("bad size in getMaskSize!");
      break;
  }
  return ret;
}


// TODO: may be wrong due to little-endian-ness
ULONG ECPUAbstract::getInverseMaskSize(ULONG size)
{
  return 0xffffffff-getMaskSize(size);
}


// TODO: may be wrong due to little-endian-ness
ULONG ECPUAbstract::getTopBitSize(ULONG size)
{
  ULONG ret=0;
  switch(size) {
    case 1:  ret=0x80;  break;
    case 2:  ret=0x8000;  break;
    case 3:  ret=0x800000; break;
    case 4:  ret=0x80000000; break;
    default:
      debugger("bad size in getTopBitSize!");
      break;
  }
  return ret;
}


ULONG ECPUAbstract::getNotTopMaskSize(ULONG size)
{
  return getMaskSize(size)-getTopBitSize(size);
}
*/


bool ECPUAbstract::interp(const char *cmd,const char *a1,const char *a2,const char *a3)
{
  return false;
}


void ECPUAbstract::clearStackTrace()
{
  stackTrace->kill();
}


void ECPUAbstract::showStackTrace()
{
  if(!traceStack) DEBUG_OUT<<"NOTE: We are not tracing the stack currently.\n";
  stackTrace->reset();
  if(stackTrace->atEnd()) {
    DEBUG_OUT<<"Stack is empty\n";
    return;
  }
  ULONG t=0;
  ULONG addr=0;
  bool working=true;
  while(working) {
    if(stackTrace->atEnd()) working=false;
    else {
      DEBUG_OUT<<aDefaultStringHelper.myDec(t)<<": ";
      stackTraceThing *thing;
      thing=(stackTraceThing *)stackTrace->info();
      ULONG st_addr=thing->st;
      ULONG pc_addr=thing->pc;
      ULONG to_addr=thing->addr;
      stackTrace->advance();
      DEBUG_OUT<<to_addr;
      DEBUG_OUT<<" (pc "<<pc_addr<<" ";
      DEBUG_OUT<<"st "<<st_addr<<")\n";
      t++;
    }
  }
}


// addr is where we're about ot jump to
void ECPUAbstract::stackTracePush(ULONG addr)
{
  if(!traceStack) return;
  ULONG pc_addr=getPC();
  ULONG st_addr=readStack();
  stackTraceThing *thing=new stackTraceThing;
  thing->st=st_addr;
  thing->pc=pc_addr;
  thing->addr=addr;
  stackTrace->push((void *)thing);
  //DEBUG_OUT<<"Jump from "<<pc_addr<<" to "<<addr<<" (st "<<st_addr<<")\n";
  //DEBUG_OUT<<"Jump to "<<addr<<" (from "<<pc_addr<<" st "<<st_addr<<")\n";
}


// addr is where we were before we returned
void ECPUAbstract::stackTracePop(ULONG addr)
{
  if(!traceStack) return;
  stackTraceThing *thing=(stackTraceThing *)stackTrace->pop();
  ULONG taddr=thing->addr;
  ULONG pc_addr=thing->pc;
  ULONG st_addr=thing->st;
  delete thing;
  ULONG stackReally=readStack()-2;
  //DEBUG_OUT<<"(Return to "<<getPC()<<" from "<<addr;
  //DEBUG_OUT<<" (st "<<readStack()<<")\n";
  //DEBUG_OUT<<" st "<<st_addr<<")\n";
  //DEBUG_OUT<<" from "<<taddr<<" at "<<pc_addr<<" on "<<st_addr<<"\n";
  if(stackReally!=st_addr) {
    debugger("Stack may be out of whack");
  }
}


void ECPUAbstract::startTracingStack()
{
  clearStackTrace();
  traceStack=true;
}


void ECPUAbstract::endTracingStack()
{
  clearStackTrace();
  traceStack=false;
}


void ECPUAbstract::setCFlag(ULONG val)
{
  debugger("ECPUAbstract::setCFlag not implemented!");
}


void ECPUAbstract::setZFlag(ULONG val)
{
  debugger("ECPUAbstract::setZFlag not implemented!");
}


void ECPUAbstract::setNFlag(ULONG val)
{
  debugger("ECPUAbstract::setNFlag not implemented!");
}


void ECPUAbstract::setVFlag(ULONG val)
{
  debugger("ECPUAbstract::setVFlag not implemented!");
}


void ECPUAbstract::setFlagsNZ(ULONG val,unsigned int size)
{
  switch(size) {
    case ECPUABSTRACT_SIZE_BYTE:
      setZFlag(((val&ECPUABSTRACT_MASK_BYTE)==0));  setNFlag(val&ECPUABSTRACT_TOP_BYTE);
      break;
    case ECPUABSTRACT_SIZE_WORD:
      setZFlag(((val&ECPUABSTRACT_MASK_WORD)==0));  setNFlag(val&ECPUABSTRACT_TOP_WORD);
      break;
    case ECPUABSTRACT_SIZE_LONG:
      setZFlag((val==0));  setNFlag(val&ECPUABSTRACT_TOP_LONG);
      break;
    default:
      debugger("Illegal size in setFlagsNZ!\n");
      break;
  }
}


void ECPUAbstract::setFlagsNZvc(ULONG val,unsigned int size)
{
  setFlagsNZ(val,size);
  setVFlag(0);  setCFlag(0);
}


// NOTE: Order may not be "val1 op val2 = val3" (which you'd guess)
// We rearrange the three so we can use the same function for NEG, SUB and ADD
void ECPUAbstract::setFlagsVC(ULONG val1,ULONG val2,ULONG val3,unsigned int size)
{
  ULONG sign1=0,sign2=0,sign3=0;
  switch(size) {
    case ECPUABSTRACT_SIZE_BYTE:
      sign1=val1&ECPUABSTRACT_TOP_BYTE;
      sign2=val2&ECPUABSTRACT_TOP_BYTE;
      sign3=val3&ECPUABSTRACT_TOP_BYTE;
      break;
    case ECPUABSTRACT_SIZE_WORD:
      sign1=val1&ECPUABSTRACT_TOP_WORD;
      sign2=val2&ECPUABSTRACT_TOP_WORD;
      sign3=val3&ECPUABSTRACT_TOP_WORD;
      break;
    case ECPUABSTRACT_SIZE_LONG:
      sign1=val1&ECPUABSTRACT_TOP_WORD;
      sign2=val2&ECPUABSTRACT_TOP_WORD;
      sign3=val3&ECPUABSTRACT_TOP_WORD;
      break;
    default:
      debugger("Illegal size in setFlagsVC!\n");
      break;
  }
  setVFlag((sign1&&sign2&&(!sign3))||((!sign1)&&(!sign2)&&sign3));
  setCFlag((sign1&&sign2)||(sign2&&(!sign3))||(sign1&&(!sign3)));
}


void ECPUAbstract::setBitness(unsigned int val)
{
  // Don't do anything...by default we can't switch
}


void ECPUAbstract::setResetEntry(ULONG addr)
{
  DEBUG_OUT<<"ECPUAbstract setting reset entry to "<<addr<<"\n";
  resetEntry=addr;
  hasResetEntry=true;
}


void ECPUAbstract::setResetStack(ULONG addr)
{
  resetStack=addr;
  hasResetStack=true;
}


// NOTE: Utility function
unsigned int ECPUAbstract::popCount(BYTE q)
{
  unsigned int ret=0;
  if(q&0x80) ret++;
  if(q&0x40) ret++;
  if(q&0x20) ret++;
  if(q&0x10) ret++;
  if(q&0x08) ret++;
  if(q&0x04) ret++;
  if(q&0x02) ret++;
  if(q&0x01) ret++;
  return ret;
}


// NOTE: Utility function
unsigned int ECPUAbstract::power2(unsigned int v)
{
  unsigned int ret=1;
  if(!v) return ret;
  for(unsigned int t=0;t<v;t++) {
    ret*=2;
  }
  return ret;
}


void ECPUAbstract::my_yield()
{
  // doesn't do anything for now...
}


ULONG ECPUAbstract::my_fork()
{
  // doesn't do anything for now...
  return 0;
}


ULONG ECPUAbstract::my_exec(const char *name)
{
  // doesn't do anything for now...
  return 0;
}


void ECPUAbstract::exec_unimplemented(const char *s,unsigned int loc,unsigned int op)
{
  DEBUG_OUT<<"exec_unimplemented: loc="<<loc<<" op="<<op<<"\n";
  debugger(s);
}


void ECPUAbstract::exec_unknown(const char *s,unsigned int loc,unsigned int op)
{
  DEBUG_OUT<<"exec_unknown: loc="<<loc<<" op="<<op<<"\n";
  debugger(s);
}


void ECPUAbstract::exec_illegal(const char *s,unsigned int loc,unsigned int op)
{
  DEBUG_OUT<<"exec_illegal: loc="<<loc<<" op="<<op<<"\n";
  debugger(s);
}


void ECPUAbstract::dis_illegal(const char *s,unsigned int loc,unsigned int op)
{
  DEBUG_OUT<<"dis_illegal: loc="<<loc<<" op="<<op<<"\n";
  debugger(s);
}


void ECPUAbstract::dis_unimplemented(const char *str,unsigned int loc,unsigned int op)
{
  DEBUG_OUT<<"dis_unimplemented: loc="<<loc<<" op="<<op<<"\n";
  debugger(str);
}


void ECPUAbstract::exec_invalid(const char *s)
{
  DEBUG_OUT<<"Internal error, exec_invalid!\n";
  debugger(s);
}


void ECPUAbstract::dis_invalid(const char *s)
{
  DEBUG_OUT<<"Internal error, dis_invalid!\n";
  debugger(s);
}


void ECPUAbstract::printHex(ULONG val,unsigned int digits)
{
  DEBUG_OUT<<val;
}


