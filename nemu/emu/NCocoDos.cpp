
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#include <emu/NCocoDos.h>

#include <portable/nickcpp/ANullStream.h>

#include <cpulib/Em6502.h>

#include <elib/EMemMapAbstract.h>
#include <elib/ERom.h>
#include <elib/ERam.h>
#include <elib/EOSEntry.h>

#include <disklib/DVfs.h>

#include <portable/nickcpp/AStringHelper.h>


//////////////////////////////////////////////////////////////////////////////
//  NCocoDos Class
//////////////////////////////////////////////////////////////////////////////

NCocoDos::NCocoDos(EHandler *parent) : EOSHelper(parent)
{
  init();
  io=parent;
  reset();
}


NCocoDos::~NCocoDos()
{
  // TODO: free kernal entries
}


void NCocoDos::reset()
{
  EOSHelper::reset();
}


bool NCocoDos::systemCall(ULONG loc)
{
  if(!cpu) {
    reset();
    if(!cpu) {
      DEBUG_OUT<<"NCocoDos Couldn't handle the systemCall, no cpu!\n";
      return false;
    }
  }
  if(!dos) {
    reset();
    if(!dos) {
      DEBUG_OUT<<"NCocoDos Couldn't handle the systemCall, no dos!\n";
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



unsigned int NCocoDos::load(const char *filename,ULONG loc,ULONG off,unsigned int maxBytes)
{
  DEBUG_OUT<<"NCocoDos::load not implemented!\n";
  exit(5);
  return 0;
}


unsigned int NCocoDos::save(const char *filename,ULONG loc,unsigned int size)
{
  DEBUG_OUT<<"NCocoDos::save not implemented!\n";
  exit(5);
  return 0;
}


unsigned int NCocoDos::loadDirectory(const char *filename,ULONG loc,unsigned int maxBytes)
{
  DEBUG_OUT<<"NCocoDos::loadDirectory not implemented!\n";
  exit(5);
  return 0;
}
