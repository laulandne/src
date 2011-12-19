
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#include <emu/NCpmDos.h>

#include <portable/nickcpp/ANullStream.h>

#include <cpulib/Ez80.h>

#include <elib/EMemMapAbstract.h>
#include <elib/ERom.h>
#include <elib/ERam.h>
#include <elib/EOSEntry.h>

#include <disklib/DVfs.h>
#include <disklib/DImageCpm.h>

#include <portable/nickcpp/AStringHelper.h>


//////////////////////////////////////////////////////////////////////////////
//  NCpmDos Class
//////////////////////////////////////////////////////////////////////////////

NCpmDos::NCpmDos(EHandler *parent) : EOSHelper(parent)
{
  init();
  io=parent;
  reset();
}


NCpmDos::~NCpmDos()
{
  // TODO: free kernal entries
}


void NCpmDos::reset()
{
  EOSHelper::reset();
}


bool NCpmDos::systemCall(ULONG loc)
{
  if(!cpu) {
    reset();
    if(!cpu) {
      DEBUG_OUT<<"NCpmDos Couldn't handle the systemCall, no cpu!\n";
      return false;
    }
  }
  if(!dos) {
    reset();
    if(!dos) {
      DEBUG_OUT<<"NCpmDos Couldn't handle the systemCall, no dos!\n";
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



unsigned int NCpmDos::load(const char *filename,ULONG loc,ULONG off,unsigned int maxBytes)
{
  DEBUG_OUT<<"NCpmDos::load not implemented!\n";
  exit(5);
  return 0;
}


unsigned int NCpmDos::save(const char *filename,ULONG loc,unsigned int size)
{
  DEBUG_OUT<<"NCpmDos::save not implemented!\n";
  exit(5);
  return 0;
}


unsigned int NCpmDos::loadDirectory(const char *filename,ULONG loc,unsigned int maxBytes)
{
  DEBUG_OUT<<"NCpmDos::loadDirectory not implemented!\n";
  exit(5);
  return 0;
}
