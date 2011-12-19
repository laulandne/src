
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#include <emu/NPalmHandler.h>
#include <emu/NPalmIO.h>

#include <portable/nickcpp/ANullStream.h>

#include <elib/EMemMapAbstract.h>
#include <elib/EVideoIOBitmap.h>

#include <cpulib/Em68k.h>
#include <cpulib/EArm.h>

#include <portable/nickcpp/AStringHelper.h>


////////////////////////////////////////////////////////////////////////////////
//  NPalmHandler Class
////////////////////////////////////////////////////////////////////////////////

NPalmHandler::NPalmHandler()
{
  DEBUG_OUT<<"NPalmHandler constructor.\n";
  romName="palm.rom";
  romSize=0;
}


NPalmHandler::~NPalmHandler()
{
  DEBUG_OUT<<"NPalmHandler destructor.\n";
  //if(!error()) freeIO();
  //err.set();
}


unsigned int NPalmHandler::name2type(const char *name)
{
  std::cerr<<"NPalmHandler name2type not implemented!\n"; exit(5);
}


/* STATIC */
bool NPalmHandler::recognize(const char *tname)
{
  const char *name=aDefaultStringHelper.removeSpaces2(aDefaultStringHelper.toLower(tname));
  if(!strcmp(name,"java")) return true;
  return false;
}


const char *NPalmHandler::getName()
{
  switch(io_type) {
    case N_PALM_IO_GENERIC:
      return "Palm (generic)";
      break;
    default:
      return "Palm (unknown)";
      break;
  }
}


// Note: No else on last one...
void NPalmHandler::setKind(const char *tname)
{
  io_type=N_PALM_IO_GENERIC;
  const char *name=aDefaultStringHelper.removeSpaces2(aDefaultStringHelper.toLower(tname));
  if(!strcmp(name,"palm")) io_type=N_PALM_IO_GENERIC;
  io_init(io_type);
}


void NPalmHandler::io_init(BYTE ttype)
{
  freeIO();
  // Now init according to the system type
  io_type=ttype;
  switch(io_type) {
    case N_PALM_IO_GENERIC:
      myIO=new NPalmIO(this);
      myVideo=new EVideoIOBitmap(this,20,20);
      break;
    default:
      debugger("Unknown IO type!");
      io_type=N_PALM_IO_NONE;
      err.set();
      return;
      break;
  }
  mapParts();
  // Leave 256 bytes for underflow above stack...
  resetStack=0x200000-256;
  //map->clearVideoMap();
  romOverlay=true;
}


void NPalmHandler::my_reset()
{
  Em68kHandler::my_reset();
}


bool NPalmHandler::interp(const char *cmd,const char *a1,const char *a2,const char *a3)
{
  bool done=false;
  return done;
}


bool NPalmHandler::systemCall(unsigned int n)
{
  debugger("NPalmHandler::systemCall not implemented.");
  return false;
}


bool NPalmHandler::read8ret(ULONG addr,BYTE *ret)
{
  if(Em68kHandler::read8ret(addr,ret)) return true;
  unsigned int apage=(addr>>16);
  //DEBUG_OUT<<"read8ret: apage="<<apage<<" a="<<a<<"...\n";
  switch(apage) {
    default:
      return false;
      break;
  }
  return false;
}


bool NPalmHandler::write8ret(ULONG addr,BYTE val,BYTE *ret)
{
  if(Em68kHandler::write8ret(addr,val,ret)) return true;
  unsigned int apage=(addr>>16);
  //DEBUG_OUT<<"read8ret: apage="<<apage<<" a="<<a<<"...\n";
  switch(apage) {
    default:
      return false;
      break;
  }
  return false;
}
