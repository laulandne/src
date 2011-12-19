
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


/*

The name of this file is a bit of a misnomer...its more of a general Java
system call handler...and in fact, not even just Java...so maybe a better
name would be "system_call.cpp"...

NOTE: We assume a memory map with 2m of ram mapped at 0 and 2m of ram mapped
at the 2g mark. (What did I want it at the 2g mark for...?)
We'll put the stack at the 2m mark growing down.
We want a page fault for any unmapped memory (not just a no-op like usual)

*/


#include <emu/NJavaHandler.h>

#include <portable/nickcpp/ANullStream.h>

#include <cpulib/ECPUAbstract.h>

#include <elib/EMemMapAbstract.h>

#include <portable/nickcpp/AStringHelper.h>


////////////////////////////////////////////////////////////////////////////////
//  NJavaHandler Class
////////////////////////////////////////////////////////////////////////////////

NJavaHandler::NJavaHandler()
{
  DEBUG_OUT<<"NJavaHandler constructor.\n";
  io_type=0;
  init();
}


NJavaHandler::~NJavaHandler()
{
  DEBUG_OUT<<"NJavaHandler destructor.\n";
  //if(!error()) freeIO();
  //err.set();
}


/* STATIC */
bool NJavaHandler::recognize(const char *tname)
{
  const char *name=aDefaultStringHelper.removeSpaces2(aDefaultStringHelper.toLower(tname));
  if(!strcmp(name,"java")) return true;
  return false;
}


const char *NJavaHandler::getName()
{
  switch(io_type) {
    case N_JAVA_IO_GENERIC:
      return "Java (generic)";
      break;
    default:
      return "Java (unknown)";
      break;
  }
}


// Note: No else on last one...
void NJavaHandler::setKind(const char *tname)
{
  io_type=N_JAVA_IO_GENERIC;
  char *name=aDefaultStringHelper.removeSpaces2(aDefaultStringHelper.toLower(tname));
  if(!strcmp(name,"java")) io_type=N_JAVA_IO_GENERIC;
  io_init(io_type);
}


void NJavaHandler::io_init(BYTE ttype)
{
  freeIO();
  // Now init according to the system type
  io_type=ttype;
  switch(io_type) {
    case N_JAVA_IO_GENERIC:
      break;
    default:
      debugger("Unknown IO type!");
      io_type=N_JAVA_IO_NONE;
      err.set();
      return;
      break;
  }
  mapParts();
  // Leave 256 bytes for underflow above stack...
  resetStack=0x200000-256;
  //map->clearVideoMap();
}


void NJavaHandler::my_reset()
{
  if(cpu) {
    cpu->setResetStack(resetStack);
    cpu->setStack(resetStack);
    cpu->setFramePtr(resetStack);
    cpu->setResetEntry(resetAddr);
    cpu->jump(resetAddr);
  }
}


bool NJavaHandler::interp(const char *cmd,const char *a1,const char *a2,const char *a3)
{
  bool done=false;
  return done;
}


bool NJavaHandler::systemCall(unsigned int n)
{
  debugger("NJavaHandler::systemCall not implemented.");
  return false;
}


void NJavaHandler::loadInitialClass()
{
  DEBUG_OUT<<"loadInitialClass...nothing here!\n";
}

