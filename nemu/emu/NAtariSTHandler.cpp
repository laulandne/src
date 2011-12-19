
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#include <emu/NAtariSTHandler.h>
#include <emu/NAtariSTIO.h>

#include <portable/nickcpp/ANullStream.h>

#include <portable/nickcpp/AStringHelper.h>

#include <portable/nickcpp/APalette.h>

#include <cpulib/Em68k.h>

#include <elib/EMemMapAbstract.h>
#include <elib/EModule.h>
#include <elib/ERom.h>
#include <elib/ERam.h>
#include <elib/EVideoIOBitmap.h>


/* Rough Atari ST 24-bit memory map:
00 0000 - 20 0000  ram
e0 0000 - f0 0000  512k rom
f0 0000 - fa 0000  i/o
fa 0000 - fc 0000  128k cart rom
fc 0000 - ff 0000  192k rom
ff 0000 - ff ffff  i/o
*/


////////////////////////////////////////////////////////////////////////////////
//  NAtariSTHandler Class
////////////////////////////////////////////////////////////////////////////////

NAtariSTHandler::NAtariSTHandler()
{
  DEBUG_OUT<<"NAtariSTHandler constructor.\n";
  romName="atarist.rom";
  romSize=0;
}


NAtariSTHandler::~NAtariSTHandler()
{
  DEBUG_OUT<<"NAtariSTHandler destructor.\n";
  //if(!error()) freeIO();
  //err.set();
}


unsigned int NAtariSTHandler::name2type(const char *name)
{
  std::cerr<<"NAtariSTHandler name2type not implemented!\n"; exit(5);
}


/* STATIC */
bool NAtariSTHandler::recognize(const char *tname)
{
  const char *name=aDefaultStringHelper.removeSpaces2(aDefaultStringHelper.toLower(tname));
  if(!strcmp(name,"atarist")) return true; else
  if(!strcmp(name,"st")) return true; else
  if(!strcmp(name,"ataritt")) return true; else
  if(!strcmp(name,"tt")) return true; else
  if(!strcmp(name,"falcon")) return true; else
  if(!strcmp(name,"falcon030")) return true; else
  return false;
}


const char *NAtariSTHandler::getName()
{
  if(io_type==N_ATARIST_IO_ST) return "Atari ST"; else
  if(io_type==N_ATARIST_IO_TT) return "Atari TT"; else
  if(io_type==N_ATARIST_IO_FALCON) return "Atari Falcon"; else
  return "Atari ST";
}


// Note: No else on last one...
void NAtariSTHandler::setKind(const char *tname)
{
  io_type=0;
  const char *name=aDefaultStringHelper.removeSpaces2(aDefaultStringHelper.toLower(tname));
  if(!strcmp(name,"atarist")) io_type=N_ATARIST_IO_ST; else
  if(!strcmp(name,"st")) io_type=N_ATARIST_IO_ST; else
  if(!strcmp(name,"ataritt")) io_type=N_ATARIST_IO_TT; else
  if(!strcmp(name,"tt")) io_type=N_ATARIST_IO_TT; else
  if(!strcmp(name,"falcon")) io_type=N_ATARIST_IO_FALCON; else
  if(!strcmp(name,"falcon030")) io_type=N_ATARIST_IO_FALCON;
  io_init(io_type);
}


void NAtariSTHandler::io_init(BYTE ttype)
{
  freeIO();
  // Now init according to the system type
  io_type=ttype;
  switch(io_type) {
    case N_ATARIST_IO_ST:
    case N_ATARIST_IO_TT:
    case N_ATARIST_IO_FALCON:
      myIO=new NAtariSTIO(this);
      myVideo=new EVideoIOBitmap(this,20,20);
      break;
    default:
      debugger("Unknown IO type!");
      io_type=N_ATARIST_IO_NONE;
      err.set();
      return;
      break;
  }
  mapParts();
  resetStack=0x200000-256;
  romOverlay=true;
}


void NAtariSTHandler::my_reset()
{
  Em68kHandler::my_reset();
}


void NAtariSTHandler::my_reset_postCPU()
{
}


bool NAtariSTHandler::read8ret(ULONG addr,BYTE *ret)
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


bool NAtariSTHandler::write8ret(ULONG addr,BYTE val,BYTE *ret)
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

