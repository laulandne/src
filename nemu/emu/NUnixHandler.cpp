
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#include <emu/NUnixHandler.h>
#include <emu/NTinyHandler.h>

#include <portable/nickcpp/ANullStream.h>

#include <cpulib/ECPUAbstract.h>

#include <elib/EMemMapAbstract.h>
#include <elib/EVideoIOTextOnly.h>


////////////////////////////////////////////////////////////////////////////////
//  NUnixHandler Class
////////////////////////////////////////////////////////////////////////////////

NUnixHandler::NUnixHandler()
{
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"NUnixHandler constructor.\n";
#endif // DEBUG_VERBOSE
  io_type=0;
  init();
}


NUnixHandler::~NUnixHandler()
{
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"NUnixHandler destructor.\n";
#endif // DEBUG_VERBOSE
  //if(!error()) freeIO();
  //err.set();
}


/* STATIC */
bool NUnixHandler::recognize(const char *tname)
{
  const char *name=aDefaultStringHelper.removeSpaces2(aDefaultStringHelper.toLower(tname));
  if(!strcmp(name,"linux")) return true;
  if(!strcmp(name,"sunos")) return true;
  if(!strcmp(name,"hpux")) return true;
  if(!strcmp(name,"hp/ux")) return true;
  if(!strcmp(name,"aix")) return true;
  if(!strcmp(name,"aux")) return true;
  if(!strcmp(name,"a/ux")) return true;
  if(!strcmp(name,"freebsd")) return true;
  if(!strcmp(name,"bsd")) return true;
  if(!strcmp(name,"netbsd")) return true;
  if(!strcmp(name,"irix")) return true;
  if(!strcmp(name,"unix")) return true;
  if(!strcmp(name,"solaris")) return true;
  if(!strcmp(name,"solaris2")) return true;
  if(!strcmp(name,"darwin")) return true;
  if(!strcmp(name,"mach")) return true;
  return false;
}


const char *NUnixHandler::getName()
{
  switch(io_type) {
    case N_UNIX_IO_GENERIC:
      return "Unix (generic)";
      break;
    case N_UNIX_IO_LINUX:
      return "Unix (Emulating Linux)";
      break;
    case N_UNIX_IO_SOLARIS:
      return "Unix (Emulating Solaris)";
      break;
    case N_UNIX_IO_DARWIN:
      return "Unix (Emulating Darwin)";
      break;
    default:
      return "Unix (unknown)";
      break;
  }
}


// Note: No else on last one...
void NUnixHandler::setKind(const char *tname)
{
  io_type=N_UNIX_IO_GENERIC;
  const char *name=aDefaultStringHelper.removeSpaces2(aDefaultStringHelper.toLower(tname));
  if(!strcmp(name,"linux")) io_type=N_UNIX_IO_LINUX;
  if(!strcmp(name,"solaris")) io_type=N_UNIX_IO_SOLARIS;
  if(!strcmp(name,"solaris2")) io_type=N_UNIX_IO_SOLARIS;
  if(!strcmp(name,"darwin")) io_type=N_UNIX_IO_DARWIN;
  io_init(io_type);
}


void NUnixHandler::io_init(BYTE ttype)
{
  init();
  freeIO();
  // Now init according to the system type
  io_type=ttype;
  switch(io_type) {
    case N_UNIX_IO_GENERIC:
    case N_UNIX_IO_LINUX:
    case N_UNIX_IO_SOLARIS:
    case N_UNIX_IO_DARWIN:
      break;
    default:
      debugger("Unknown IO type!");
      io_type=N_UNIX_IO_NONE;
      err.set();
      return;
      break;
  }
  mapParts();
  // Leave 256 bytes for underflow above stack...
  resetStack=0x200000-256;
  //map->clearVideoMap();
}


void NUnixHandler::my_reset()
{
  if(cpu) {
    cpu->setResetStack(resetStack);
    cpu->setStack(resetStack);
    cpu->setFramePtr(resetStack);
    cpu->setResetEntry(resetAddr);
    cpu->jump(resetAddr);
  }
}


bool NUnixHandler::interp(const char *cmd,const char *a1,const char *a2,const char *a3)
{
  bool done=false;
  return done;
}


bool NUnixHandler::systemCall(unsigned int n)
{
  if(cpu) {
    NTinyHandler *foo=(NTinyHandler *)cpu->getTrapHandler();
    if(foo) {
      return foo->systemCall(n);
    }
  }
  return false;
}



void NUnixHandler::my_reset_postCPU()
{
  if(cpu) {
    if(!cpu->getTrapHandler()) {
      NTinyHandler *foo=new NTinyHandler(this);
      if(foo) cpu->setTrapHandler(foo);
    }
  }
}



