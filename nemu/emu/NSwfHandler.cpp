
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#include <emu/NSwfHandler.h>

#include <portable/nickcpp/ANullStream.h>

#include <portable/nickcpp/AStringHelper.h>


////////////////////////////////////////////////////////////////////////////////
//  NSwfHandler Class
////////////////////////////////////////////////////////////////////////////////

NSwfHandler::NSwfHandler()
{
  DEBUG_OUT<<"NSwfHandler constructor.\n";
}


NSwfHandler::~NSwfHandler()
{
  DEBUG_OUT<<"NSwfHandler destructor.\n";
  //if(!error()) freeIO();
  //err.set();
}


/* STATIC */
bool NSwfHandler::recognize(const char *tname)
{
  const char *name=aDefaultStringHelper.removeSpaces2(aDefaultStringHelper.toLower(tname));
  if(!strcmp(tname,"swf")) return true; else
  if(!strcmp(tname,"flash")) return true; else
  return false;
}


const char *NSwfHandler::getName()
{
  return "Flash";
}


void NSwfHandler::setKind(const char *tname)
{
  io_type=0;
  char *name=aDefaultStringHelper.removeSpaces2(aDefaultStringHelper.toLower(tname));
  io_type=N_SWF_IO_NONE;
  io_init(io_type);
}


void NSwfHandler::io_init(BYTE ttype)
{
  freeIO();
  // Now init according to the system type
  io_type=ttype;
  switch(io_type) {
    default:
      debugger("Unknown IO type!");
      io_type=N_SWF_IO_NONE;
      err.set();
      return;
      break;
  }
  mapParts();
}
