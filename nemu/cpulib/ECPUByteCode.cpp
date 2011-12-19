
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#include <cpulib/ECPUByteCode.h>

#include <elib/EMemMapAbstract.h>

#include <portable/nickcpp/ANullStream.h>


//#define TRACE_ALL_JUMPS 1


////////////////////////////////////////////////////////////////////////////////
//  ECPUByteCode Class
////////////////////////////////////////////////////////////////////////////////

ECPUByteCode::ECPUByteCode() : ECPUAbstract()
{
  init();
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"ECPUByteCode created...\n";
#endif // DEBUG_VERBOSE
  cyclesPerSec=1000000;  // i.e. 1MHZ (for now)
}


ECPUByteCode::~ECPUByteCode()
{
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"ECPUByteCode destroyed.\n";
#endif // DEBUG_VERBOSE
  stackTrace->kill();
  methods.kill();
  classes.kill();
  objects.kill();
}


void ECPUByteCode::init()
{
  ECPUAbstract::init();
  bitness=8;
  lsb=false;
  cyclesPerSec=1000000;  // i.e. 1MHZ (for now)
  obj.type=0;  obj.len=0;  obj.val=0;
  // Set stack and frame ptr to arbitrary, but workable values
  //setStack(ECPUABSTRACT_DEFAULT_STACK);
  //setFramePtr(ECPUABSTRACT_DEFAULT_STACK);
  //setResetStack(ECPUABSTRACT_DEFAULT_STACK);
  //setResetEntry(ECPUABSTRACT_DEFAULT_CODE);
}


void ECPUByteCode::setStack(ULONG location)
{
  sp=location;
}


ULONG ECPUByteCode::readStack()
{
  return sp;
}


void ECPUByteCode::pushObj(ECPUByteCodeObject *obj)
{
  DEBUG_OUT<<"ECPUByteCode::pushObj...\n";
  sp-=4;
  map->write32(sp,obj->type,lsb); sp-=4;
  map->write32(sp,obj->len,lsb); sp-=4;
  map->write32(sp,obj->val,lsb); sp-=4;
  map->write32(sp,obj->name,lsb);
}


ECPUByteCodeObject *ECPUByteCode::popObj()
{
  DEBUG_OUT<<"ECPUByteCode::popObj...\n";
  obj.name=map->read32(sp,lsb); sp+=4;
  obj.val=map->read32(sp,lsb); sp+=4;
  obj.len=map->read32(sp,lsb); sp+=4;
  obj.type=map->read32(sp,lsb); sp+=4;
  return &obj;
}


ECPUByteCodeObject *ECPUByteCode::topObj()
{
  obj.name=map->read32(sp,lsb);
  obj.val=map->read32(sp-4,lsb);
  obj.len=map->read32(sp-8,lsb);
  obj.type=map->read32(sp-12,lsb);
  return &obj;
}


ECPUByteCodeObject *ECPUByteCode::getNObj(unsigned int n)
{
  ULONG offset=n*16;
  obj.name=map->read32(sp-offset,lsb);
  obj.val=map->read32(sp-offset-4,lsb);
  obj.len=map->read32(sp-offset-8,lsb);
  obj.type=map->read32(sp-offset-12,lsb);
  return &obj;
}


void ECPUByteCode::stackInc()
{
  sp-=16;
}


void ECPUByteCode::stackDec()
{
  sp+=16;
}


void ECPUByteCode::jump(ULONG location)
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
void ECPUByteCode::jsr(ULONG location)
{
  pushAddr(pc+op_len);
  jump(location);
}


// NOTE: May have to overload with implementation dependant version
void ECPUByteCode::rts()
{
  ULONG ea=pullAddr();
  jump(ea);
}


// NOTE: May have to overload with implementation dependant version
void ECPUByteCode::pushAddr(ULONG location)
{
  if(!map) return;
  ULONG ea=readStack();
  ea-=4;
  map->write32(ea,location,lsb);
  setStack(ea);
}


// NOTE: May have to overload with implementation dependant version
ULONG ECPUByteCode::pullAddr()
{
  if(!map) return 0;
  ULONG ea=readStack();
  ULONG val=map->read32(ea,lsb);
  ea+=4;
  setStack(ea);
  return val;
}


