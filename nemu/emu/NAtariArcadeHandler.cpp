
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#include <emu/NAtariArcadeHandler.h>
#include <emu/NAtariArcadeIO.h>
#include <emu/NAtariArcadeVideo.h>

#include <elib/ESoundIOPokey.h>
#include <elib/EKeys.h>
#include <elib/EMemMapAbstract.h>

#include <portable/nickcpp/AStringHelper.h>

#include <portable/nickcpp/ANullStream.h>

#include <cpulib/Em6502.h>


////////////////////////////////////////////////////////////////////////////////////
//  NAtariArcadeHandler Class
////////////////////////////////////////////////////////////////////////////////////


NAtariArcadeHandler::NAtariArcadeHandler()
{
  DEBUG_OUT<<"NAtariArcadeHandler constructor.\n";
}


NAtariArcadeHandler::~NAtariArcadeHandler()
{
  DEBUG_OUT<<"NAtariArcadeHandler destructor.\n";
}


/* STATIC */
unsigned int NAtariArcadeHandler::name2type(const char *tname)
{
  const char *name=aDefaultStringHelper.removeSpaces2(aDefaultStringHelper.toLower(tname));
  if(!strcmp(name,"misslecommand")) return N_ATARIARCADE_IO_MISSLECMD;
  if(!strcmp(name,"centipede")) return N_ATARIARCADE_IO_CENTIPEDE;
  if(!strcmp(name,"milipede")) return N_ATARIARCADE_IO_MILIPEDE;
  if(!strcmp(name,"tetris")) return N_ATARIARCADE_IO_TETRIS;
  if(!strcmp(name,"crystalcastles")) return N_ATARIARCADE_IO_CCASTLES;
  if(!strcmp(name,"warlords")) return N_ATARIARCADE_IO_WARLORDS;
  return N_ATARIARCADE_IO_NONE;
}


/* STATIC */
bool NAtariArcadeHandler::recognize(const char *tname)
{
  if(name2type(tname)) return true;
  return false;
}


const char *NAtariArcadeHandler::getName()
{
  if(io_type==N_ATARIARCADE_IO_CENTIPEDE) return "Arcade Centipede"; else
  if(io_type==N_ATARIARCADE_IO_MILIPEDE) return "Arcade Milipede"; else
  return "Unknown 'Pede";
}


void NAtariArcadeHandler::setKind(const char *tname)
{
  io_type=name2type(tname);
  io_init(io_type);
}


void NAtariArcadeHandler::io_init(BYTE ttype)
{
  freeIO();
  // Now init according to the system type
  io_type=ttype;
  switch(io_type) {
    case N_ATARIARCADE_IO_CENTIPEDE:
      myIO=new NAtariArcadeIO(this);
      myVideo=new NAtariArcadeVideo(this,32,30);
      myVideo->setSpriteRegisters(myIO,0x7c0);
      break;
    case N_ATARIARCADE_IO_MILIPEDE:
      myIO=new NAtariArcadeIO(this);
      myVideo=new NAtariArcadeVideo(this,32,30);
      break;
    default:
      debugger("Unknown/unimplemented IO type!");
      io_type=N_ATARIARCADE_IO_NONE;
      err.set();
      return;
      break;
  }
  mapParts();
}


bool NAtariArcadeHandler::read8ret(ULONG addr,BYTE *ret)
{
  unsigned int apage=(addr>>8);
  switch(apage) {
    default:
      return false;
      break;
  }
  return false;
}


bool NAtariArcadeHandler::write8ret(ULONG addr,BYTE val,BYTE *ret)
{
  unsigned int apage=(addr>>8);
  switch(apage) {
    default:
      return false;
      break;
  }
  return false;
}
