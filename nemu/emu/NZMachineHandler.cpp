
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#include <emu/NZMachineHandler.h>

#include <portable/nickcpp/ANullStream.h>

#include <portable/nickcpp/AStringHelper.h>


////////////////////////////////////////////////////////////////////////////////
//  NZMachine Class
////////////////////////////////////////////////////////////////////////////////

NZMachineHandler::NZMachineHandler()
{
  DEBUG_OUT<<"NZMachineHandler constructor.\n";
}


NZMachineHandler::~NZMachineHandler()
{
  DEBUG_OUT<<"NZMachineHandler destructor.\n";
  //if(!error()) freeIO();
  //err.set();
}


/* STATIC */
bool NZMachineHandler::recognize(const char *tname)
{
  const char *name=aDefaultStringHelper.removeSpaces2(aDefaultStringHelper.toLower(tname));
  if(!strcmp(tname,"zmachine")) return true; else
  return false;
}


const char *NZMachineHandler::getName()
{
  return "ZMachine";
}


void NZMachineHandler::setKind(const char *tname)
{
  io_type=0;
  const char *name=aDefaultStringHelper.removeSpaces2(aDefaultStringHelper.toLower(tname));
  io_type=N_ZMACHINE_IO_NONE;
  io_init(io_type);
}


void NZMachineHandler::io_init(BYTE ttype)
{
  freeIO();
  // Now init according to the system type
  io_type=ttype;
  switch(io_type) {
    default:
      debugger("Unknown IO type!");
      io_type=N_ZMACHINE_IO_NONE;
      err.set();
      return;
      break;
  }
  mapParts();
}
