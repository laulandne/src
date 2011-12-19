
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#include <cpulib/ECPURisc.h>

#include <elib/EMemMapAbstract.h>

#include <portable/nickcpp/ANullStream.h>


//#define TRACE_ALL_JUMPS 1


////////////////////////////////////////////////////////////////////////////////
//  ECPURisc Class
////////////////////////////////////////////////////////////////////////////////

ECPURisc::ECPURisc() : ECPUAbstract()
{
  init();
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"ECPURisc created...\n";
#endif // DEBUG_VERBOSE
}


ECPURisc::~ECPURisc()
{
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"ECPURisc destroyed.\n";
#endif // DEBUG_VERBOSE
  stackTrace->kill();
}


void ECPURisc::init()
{
  ECPUAbstract::init();
  bitness=32;
  cyclesPerSec=1000000;  // i.e. 1MHZ (for now)
  branchesAreDelayed=false;
  r0IsZero=false;
  // Set stack and frame ptr to arbitrary, but workable values
  //setStack(ECPUABSTRACT_DEFAULT_STACK);
  //setFramePtr(ECPUABSTRACT_DEFAULT_STACK);
  //setResetStack(ECPUABSTRACT_DEFAULT_STACK);
  //setResetEntry(ECPUABSTRACT_DEFAULT_CODE);
}


void ECPURisc::setStack(ULONG location)
{
  sp=location;
}


ULONG ECPURisc::readStack()
{
  return sp;
}



// Call this instead of jump to advise of a delayed jump...this is just so we can do a trace
void ECPURisc::willJump(ULONG location)
{
#ifdef TRACE_ALL_JUMPS
  unsigned int old_pc=pc,old_npc=npc,old_op_len=op_len;
  jump(location);
  pc=old_pc;  npc=old_npc;  op_len=old_op_len;
#endif // TRACE_ALL_JUMPS
}


// NOTE: May have to overload with implementation dependant version
void ECPURisc::jump(ULONG location)
{
  pc=location;  npc=location;
  op_len=0;
#ifdef TRACE_ALL_JUMPS
  DEBUG_OUT<<"JUMP to "<<location;
  EMapSymbol *e=NULL;
  if(map) e=map->findAnyNearestPrevSymbol(location);
  if(e) {
    DEBUG_OUT<<" ("<<e->getName();
    if(e->addr!=location) {
      DEBUG_OUT<<"+0x"<<(location-(e->addr));
    }
    DEBUG_OUT<<")";
  }
  DEBUG_OUT<<"\n";
#endif // TRACE_ALL_JUMPS
}


// NOTE: May have to overload with implementation dependant version
void ECPURisc::jsr(ULONG location)
{
  pushAddr(pc+op_len);
  jump(location);
}


// NOTE: May have to overload with implementation dependant version
void ECPURisc::rts()
{
  ULONG ea=pullAddr();
  jump(ea);
}


// NOTE: May have to overload with implementation dependant version
void ECPURisc::pushAddr(ULONG location)
{
  if(!map) return;
  if(bitness!=32) debugger("default pushAddr, non-32-bits!");
  ULONG ea=readStack();
  ea-=4;
  map->write32(ea,location,lsb);
  setStack(ea);
}


// NOTE: May have to overload with implementation dependant version
ULONG ECPURisc::pullAddr()
{
  if(!map) return 0;
  if(bitness!=32) debugger("default pullAddr, non-32-bits!");
  ULONG ea=readStack();
  ULONG val=map->read32(ea,lsb);
  ea+=4;
  setStack(ea);
  return val;
}


const char *ECPURisc::getRegName(unsigned int n)
{
  const char *ret=NULL;
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
    case 0xb: ret="r11"; break;
    case 0xc: ret="r12"; break;
    case 0xd: ret="r13"; break;
    case 0xe: ret="r14"; break;
    case 0xf: ret="r15"; break;
    case 0x10: ret="r16"; break;
    case 0x11: ret="r17"; break;
    case 0x12: ret="r18"; break;
    case 0x13: ret="r19"; break;
    case 0x14: ret="r20"; break;
    case 0x15: ret="r21"; break;
    case 0x16: ret="r22"; break;
    case 0x17: ret="r23"; break;
    case 0x18: ret="r24"; break;
    case 0x19: ret="r25"; break;
    case 0x1a: ret="r26"; break;
    case 0x1b: ret="r27"; break;
    case 0x1c: ret="r28"; break;
    case 0x1d: ret="r29"; break;
    case 0x1e: ret="r30"; break;
    case 0x1f: ret="r31"; break;
    default: ret="???"; break;
  }
  return ret;
}


void ECPURisc::writeReg(unsigned int n,ULONG val)
{
  debugger("writeReg!");
}


ULONG ECPURisc::readReg(unsigned int n)
{
  debugger("readReg!");
  return 0;
}



