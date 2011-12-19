
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#include <cpulib/ECPU8Bit.h>

#include <elib/EMemMapAbstract.h>

#include <portable/nickcpp/ANullStream.h>


//#define TRACE_ALL_JUMPS 1


////////////////////////////////////////////////////////////////////////////////
//  ECPU8Bit Class
////////////////////////////////////////////////////////////////////////////////

ECPU8Bit::ECPU8Bit() : ECPUAbstract()
{
  init();
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"ECPU8Bit created...\n";
#endif // DEBUG_VERBOSE
  cyclesPerSec=1000000;  // i.e. 1MHZ (for now)
}


ECPU8Bit::~ECPU8Bit()
{
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"ECPU8Bit destroyed.\n";
#endif // DEBUG_VERBOSE
  stackTrace->kill();
}


void ECPU8Bit::init()
{
  ECPUAbstract::init();
  bitness=8;
  cyclesPerSec=1000000;  // i.e. 1MHZ (for now)
  stackBase=0;  stackMask=0xffff;
  stackCorrection=0;
}


void ECPU8Bit::stack_push(BYTE val)
{
  map->write8(stackBase+sp,val);
  stack_dec();
}


// NOTE: Little endian!
void ECPU8Bit::stack_pushw(UINT16 val)
{
  BYTE low=(BYTE)(val&0xff);
  BYTE high=(BYTE)((val>>8)&0xff);
  stack_push(high);
  stack_push(low);
}


BYTE ECPU8Bit::stack_pop()
{
  stack_inc();
  BYTE val=map->read8(stackBase+sp);
  return val;
}


// NOTE: Little endian!
UINT16 ECPU8Bit::stack_popw()
{
  BYTE low=stack_pop();
  BYTE high=stack_pop();
  ULONG val=low+(high<<8);
  return val;
}


void ECPU8Bit::stack_inc()
{
  ULONG v=(sp+1);
#ifdef CHECK_STACK
  if(v>stackMask) {
    debugger("stack underflow");
  }
#endif // CHECK_STACK
  sp=v&stackMask;
}


void ECPU8Bit::stack_dec()
{
  ULONG v=(sp-1);
#ifdef CHECK_STACK
  if(v>stackMask) {
    debugger("stack overflow");
  }
#endif // CHECK_STACK
  sp=v&stackMask;
}


// NOTE: Little endian!
ULONG ECPU8Bit::getImmWord(ULONG addr)
{
  return map->read16l(addr);
}


unsigned int ECPU8Bit::getImmByte(ULONG addr)
{
  return map->read8(addr);
}


// NOTE: Set actual address, not contents of sp (esp on 6502)
void ECPU8Bit::setStack(ULONG location)
{
  ULONG val=(location-stackBase)&stackMask;
  //DEBUG_OUT<<"val="<<val<<" location="<<location<<" stackMask="<<stackMask<<" stackBase="<<stackBase<<"\n";
  sp=val;
  if(((sp&stackMask)+stackBase)!=location) {
    debugger("ECPU8Bit::setStack attempted to set out of range value");
  }
}


// NOTE: Returns actual address, not contents of sp (esp on 6502)
ULONG ECPU8Bit::readStack()
{
  ULONG val=(sp&stackMask)+stackBase;
  //DEBUG_OUT<<"val="<<val<<" sp="<<sp<<" stackMask="<<stackMask<<" stackBase="<<stackBase<<"\n";
  return val;
}



void ECPU8Bit::jump(ULONG location)
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
void ECPU8Bit::jsr(ULONG location)
{
  pushAddr(pc+op_len);
  jump(location);
}


// NOTE: May have to overload with implementation dependant version
void ECPU8Bit::rts()
{
  ULONG ea=pullAddr()+1;
  jump(ea);
}


// NOTE: May have to overload with implementation dependant version
void ECPU8Bit::pushAddr(ULONG location)
{
  if(!map) return;
  ULONG ea=readStack();
  ea-=2;
  map->write16(ea,location,lsb);
  //DEBUG_OUT<<"ECPU8Bit::pushAddr sp="<<sp<<" ea="<<ea<<" location="<<location<<"\n";
  setStack(ea);
}


// NOTE: May have to overload with implementation dependant version
ULONG ECPU8Bit::pullAddr()
{
  if(!map) { exit(5); return 0; }
  ULONG ea=readStack();
  ULONG val=map->read16(ea+stackCorrection,lsb);  // TODO: Is the +1 always right?!?
  ea+=2;
  //DEBUG_OUT<<"ECPU8Bit::pullAddr sp="<<sp<<" ea="<<ea<<" val="<<val<<"\n";
  setStack(ea);
  return val;
}

