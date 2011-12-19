
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#include <cpulib/ECPUCisc.h>

#include <elib/EMemMapAbstract.h>

#include <portable/nickcpp/ANullStream.h>


//#define TRACE_ALL_JUMPS 1


////////////////////////////////////////////////////////////////////////////////
//  ECPUCisc Class
////////////////////////////////////////////////////////////////////////////////

ECPUCisc::ECPUCisc() : ECPUAbstract()
{
  init();
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"ECPUCisc created...\n";
#endif // DEBUG_VERBOSE
  cyclesPerSec=1000000;  // i.e. 1MHZ (for now)
}


ECPUCisc::~ECPUCisc()
{
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"ECPUCisc destroyed.\n";
#endif // DEBUG_VERBOSE
  stackTrace->kill();
}


void ECPUCisc::init()
{
  ECPUAbstract::init();
  bitness=16;
  cyclesPerSec=1000000;  // i.e. 1MHZ (for now)
  // Set stack and frame ptr to arbitrary, but workable values
  //setStack(ECPUABSTRACT_DEFAULT_STACK);
  //setFramePtr(ECPUABSTRACT_DEFAULT_STACK);
  //setResetStack(ECPUABSTRACT_DEFAULT_STACK);
  //setResetEntry(ECPUABSTRACT_DEFAULT_CODE);
}


void ECPUCisc::setStack(ULONG location)
{
  sp=location;
}


ULONG ECPUCisc::readStack()
{
  return sp;
}


void ECPUCisc::jump(ULONG location)
{
  pc=location;
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
void ECPUCisc::jsr(ULONG location)
{
  pushAddr(pc+op_len);
  jump(location);
}


// NOTE: May have to overload with implementation dependant version
void ECPUCisc::rts()
{
  ULONG ea=pullAddr();
  jump(ea);
}


// NOTE: May have to overload with implementation dependant version
void ECPUCisc::pushAddr(ULONG location)
{
  if(!map) return;
  ULONG ea=readStack();
  switch(bitness) {
    case 16:
      ea-=2;
      map->write16(ea,location,lsb);
      break;
    case 32:
      ea-=4;
      map->write32(ea,location,lsb);
      break;
    default:
      debugger("Default pushAddr, unknown bitness!");
      break;
  }
  setStack(ea);
}


// NOTE: May have to overload with implementation dependant version
ULONG ECPUCisc::pullAddr()
{
  if(!map) return 0;
  ULONG ea=readStack();
  ULONG val=0;
  switch(bitness) {
    case 16:
      val=map->read16(ea,lsb);
      ea+=2;
      break;
    case 32:
      val=map->read32(ea,lsb);
      ea+=4;
      break;
    default:
      debugger("Default pullAddr, unknown bitness!");
      break;
  }
  setStack(ea);
  return val;
}


