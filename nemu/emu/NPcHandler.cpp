
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#include <emu/NPcHandler.h>
#include <emu/NPcIO.h>
#include <emu/NTinyHandler.h>

#include <portable/nickcpp/ANullStream.h>

#include <elib/EMemMapAbstract.h>
#include <elib/EVideoIOMoto684x.h>

#include <cpulib/Ex86.h>


////////////////////////////////////////////////////////////////////////////////
//  NPcHandler Class
////////////////////////////////////////////////////////////////////////////////

NPcHandler::NPcHandler()
{
  DEBUG_OUT<<"NPcHandler constructor.\n";
  io_type=0;
  init();
}


NPcHandler::~NPcHandler()
{
  DEBUG_OUT<<"NPcHandler destructor.\n";
  //if(!error()) freeIO();
  //err.set();
}


/* STATIC */
bool NPcHandler::recognize(const char *tname)
{
  const char *name=aDefaultStringHelper.removeSpaces2(aDefaultStringHelper.toLower(tname));
  if(!strcmp(name,"pc")) return true; else
  if(!strcmp(name,"nt")) return true; else
  if(!strcmp(name,"winnt")) return true; else
  if(!strcmp(name,"windows")) return true; else
  if(!strcmp(name,"win16")) return true; else
  if(!strcmp(name,"win3")) return true; else
  if(!strcmp(name,"win32")) return true; else
  if(!strcmp(name,"win386")) return true; else
  if(!strcmp(name,"win31")) return true; else
  if(!strcmp(name,"chicago")) return true; else
  if(!strcmp(name,"daytona")) return true; else
  if(!strcmp(name,"memphis")) return true; else
  if(!strcmp(name,"win95")) return true; else
  if(!strcmp(name,"win98")) return true; else
  if(!strcmp(name,"os2")) return true; else
  if(!strcmp(name,"os/2")) return true; else
  if(!strcmp(name,"warp")) return true; else
  if(!strcmp(name,"merlin")) return true; else
  if(!strcmp(name,"dos")) return true; else
  if(!strcmp(name,"msdos")) return true; else
  if(!strcmp(name,"pcdos")) return true; else
  if(!strcmp(name,"drdos")) return true; else
  if(!strcmp(name,"dotnet")) return true; else
  if(!strcmp(name,"mono")) return true; else
  return false;
}


const char *NPcHandler::getName()
{
  if(io_type==N_PC_IO_GENERIC) return "Windows (generic)"; else
  return "Windows (Unknown)";
}


void NPcHandler::setKind(const char *tname)
{
  io_type=0;
  const char *name=aDefaultStringHelper.removeSpaces2(aDefaultStringHelper.toLower(tname));
  io_type=N_PC_IO_GENERIC;
  io_init(io_type);
}


void NPcHandler::io_init(BYTE ttype)
{
  init();
  freeIO();
  // Now init according to the system type
  io_type=ttype;
  switch(io_type) {
    case N_PC_IO_GENERIC:
      myIO=new NPcIO(this);
      myVideo=new EVideoIOMoto684x(this,80,24);
      break;
    default:
      debugger("Unknown IO type!");
      io_type=N_PC_IO_NONE;
      err.set();
      return;
      break;
  }
  mapParts();
}


void NPcHandler::my_reset()
{
  if(cpu) {
    cpu->setResetStack(resetStack);
    cpu->setStack(resetStack);
    cpu->setFramePtr(resetStack);
    cpu->setResetEntry(resetAddr);
    cpu->jump(resetAddr);
  }
}


bool NPcHandler::interp(const char *cmd,const char *a1,const char *a2,const char *a3)
{
  bool done=false;
  return done;
}


bool NPcHandler::systemCall(unsigned int n)
{
  if(cpu) {
    NTinyHandler *foo=(NTinyHandler *)cpu->getTrapHandler();
    if(foo) {
      return foo->systemCall(n);
    }
  }
  return false;
}


void NPcHandler::my_reset_postCPU()
{
  if(cpu) {
    if(!cpu->getTrapHandler()) {
      NTinyHandler *foo=new NTinyHandler(this);
      if(foo) cpu->setTrapHandler(foo);
    }
  }
}


void NPcHandler::io_out8(ULONG addr, BYTE val)
{
  DEBUG_OUT<<"out8 "<<addr<<","<<(int)val<<"\n";
  switch(addr) {
    case 0xff:
      // Cassette IO
      break;
    default:
      debugger("NPcHandler::io_out8 unimplemented address");
      break;
  }
}


BYTE NPcHandler::io_in8(ULONG addr)
{
  BYTE ret=0;
  DEBUG_OUT<<"in8 "<<addr<<"\n";
  switch(addr) {
    default:
      debugger("NPcHandler::io_in8 unimplemented address");
      break;
  }
  return ret;
}

