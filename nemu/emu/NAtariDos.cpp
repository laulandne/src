
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#include <emu/NAtariDos.h>

#include <portable/nickcpp/ANullStream.h>

#include <cpulib/Em6502.h>

#include <elib/EMemMapAbstract.h>
#include <elib/ERom.h>
#include <elib/ERam.h>
#include <elib/EOSEntry.h>

#include <disklib/DVfs.h>
#include <disklib/DImageAtari.h>

#include <portable/nickcpp/AStringHelper.h>


/*
NOTES:

jmp ($ffff)?
sio: jsr $e459
???: jmp $eb2c

exe file format, one or more segments with following format...
00-01: $ffff (mandatory for first seg, optional for all others)
02-03: start addr
04-05: end addr
06-??: data...

$02e0: if filled in, run addr?
$02e2: if filled in, init addr?

*/


//////////////////////////////////////////////////////////////////////////////
//  NAtariDos Class
//////////////////////////////////////////////////////////////////////////////

NAtariDos::NAtariDos(EHandler *parent) : EOSHelper(parent)
{
  init();
  io=parent;
  reset();
}


NAtariDos::~NAtariDos()
{
  // TODO: free kernal entries
}


void NAtariDos::reset()
{
  EOSHelper::reset();
}


bool NAtariDos::systemCall(ULONG loc)
{
  if(!cpu) {
    reset();
    if(!cpu) {
      DEBUG_OUT<<"NAtariDos Couldn't handle the systemCall, no cpu!\n";
      return false;
    }
  }
  if(!dos) {
    reset();
    if(!dos) {
      DEBUG_OUT<<"NAtariDos Couldn't handle the systemCall, no dos!\n";
       return false;
    }
  }
  EMemMapAbstract *map=dos->getMap();
  if(!map) return false;
  bool ret=false;
  EOSEntry *n=findActual(loc);
  unsigned int original=0;
  if(n) original=n->origAddr;
  if(!original) return false;
  DEBUG_OUT<<"sysCall:"<<aDefaultStringHelper.myHex((unsigned short)original)<<"\n";
  bool doIt=false;
  unsigned int t,s,a;
  switch(original) {
    default:
      DEBUG_OUT<<"***unhandled sys call at "<<aDefaultStringHelper.myHex((unsigned short)loc)<<"!!!\n";
      break;
  }
  //if(!ret) {
    //DEBUG_OUT<<"***fall through sys call at "<<aDefaultStringHelper.myHex((unsigned short)loc)<<"!!!\n";
  //}
  //w->setHexDigits(d);
  if(ret) {
    DEBUG_OUT<<"ret was set!\n";
    if(cpu) cpu->rts();
  }
  else {
    DEBUG_OUT<<"(instead "<<loc<<": "<<(short)n->origByte<<")\n";
    if(cpu) {
      DEBUG_OUT<<"Going to jmp!\n";
      cpu->jump(loc);
      //cpu->executeInstead(n->origByte);
      ret=true;
    }
  }
  return ret;
}



unsigned int NAtariDos::load(const char *filename,ULONG loc,ULONG off,unsigned int maxBytes)
{
  DEBUG_OUT<<"NAtariDos::load not implemented!\n";
  exit(5);
  return 0;
}


unsigned int NAtariDos::save(const char *filename,ULONG loc,unsigned int size)
{
  DEBUG_OUT<<"NAtariDos::save not implemented!\n";
  exit(5);
  return 0;
}


unsigned int NAtariDos::loadDirectory(const char *filename,ULONG loc,unsigned int maxBytes)
{
  DEBUG_OUT<<"NAtariDos::loadDirectory not implemented!\n";
  exit(5);
  return 0;
}
