
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#include <emu/NGenesisHandler.h>
#include <emu/NGenesisIO.h>

#include <portable/nickcpp/ANullStream.h>

#include <portable/nickcpp/AStringHelper.h>

#include <portable/nickcpp/APalette.h>

#include <cpulib/Em68k.h>
#include <cpulib/Ez80.h>

#include <elib/EMemMapAbstract.h>
#include <elib/EModule.h>
#include <elib/ERom.h>
#include <elib/ERam.h>
#include <elib/EVideoIOTms99xx.h>


/*
Notes:

MegaDrive Memory Map
000000-3FFFFF : 4 Megas Rom Cardtrige
400000-9FFFFF : Reserved
A00000-AFFFFF : I/O Ports
B00000-BFFFFF : Reserved
C00000-DFFFFF : Vdp
E00000-FEFFFF : Ram Image
FF0000-FFFFFF : 64kb RAM

*/


////////////////////////////////////////////////////////////////////////////////
//  NGenesisHandler Class
////////////////////////////////////////////////////////////////////////////////

NGenesisHandler::NGenesisHandler()
{
  DEBUG_OUT<<"NGenesisHandler constructor.\n";
  romName="genesis.rom";
}


NGenesisHandler::~NGenesisHandler()
{
  DEBUG_OUT<<"NGenesisHandler destructor.\n";
  //if(!error()) freeIO();
  //err.set();
}


void NGenesisHandler::setKind(const char *tname)
{
  io_type=name2type(tname);
  io_init(io_type);
}


const char *NGenesisHandler::getName()
{
  if(io_type==N_GENESIS_IO_GENESIS) return "Sega Genesis/Megadrive";
  return "Unknown Genesis";
}


/* STATIC */
bool NGenesisHandler::recognize(const char *tname)
{
  if(name2type(tname)) return true;
  return false;
}


/* STATIC */
unsigned int NGenesisHandler::name2type(const char *tname)
{
  const char *name=aDefaultStringHelper.removeSpaces2(aDefaultStringHelper.toLower(tname));
  if(!strcmp(name,"genesis")) return N_GENESIS_IO_GENESIS;
  if(!strcmp(name,"megadrive")) return N_GENESIS_IO_GENESIS;
  return N_GENESIS_IO_NONE;
}


void NGenesisHandler::io_init(BYTE ttype)
{
  freeIO();
  // Now init according to the system type
  io_type=ttype;
  switch(io_type) {
    case N_GENESIS_IO_GENESIS:
      myIO=new NGenesisIO(this);
      myVideo=new EVideoIOTms99xx(this,40,28);
      break;
    default:
      debugger("Unknown IO type!");
      io_type=N_GENESIS_IO_NONE;
      err.set();
      return;
      break;
  }
  mapParts();
  resetStack=0x200000-256;
  romOverlay=true;
}


void NGenesisHandler::my_reset()
{
  Em68kHandler::my_reset();
}


void NGenesisHandler::my_reset_postCPU()
{
}


bool NGenesisHandler::read8ret(ULONG addr,BYTE *ret)
{
  if(Em68kHandler::read8ret(addr,ret)) return true;
  unsigned int apage=(addr>>16)&0xff;
  unsigned int bpage=(apage>>4)&0xf;
  ULONG a=0;
  ULONG actual=0;
  //DEBUG_OUT<<"read8ret: apage="<<apage<<" a="<<a<<"...\n";
  switch(bpage) {
    case 4: case 5: case 6: case 7: case 8: case 9:
      *ret=0xff00;  // should be msb of next instruction, not ff...
      return true;
      break;
    case 0xa: case 0xb:  case 0xc:  case 0xd:
      *ret=myIO->read8(addr);
      return true;
      break;
    case 0xe: case 0xf:
      a=addr&0xffff;
      if(!theRam) theRam=(ERam *)map->findArea("ram");
      actual=0xff0000|a;
      DEBUG_OUT<<"NGenesisHandler::read8ret mirroring "<<addr<<" to "<<actual<<"\n";
      *ret=theRam->read8(actual);
      return true;
      break;
    default:
      return false;
      break;
  }
  return false;
}


bool NGenesisHandler::write8ret(ULONG addr,BYTE val,BYTE *ret)
{
  if(Em68kHandler::write8ret(addr,val,ret)) return true;
  unsigned int apage=(addr>>16)&0xff;
  unsigned int bpage=(apage>>4)&0xf;
  ULONG a=0;
  ULONG actual=0;
  //DEBUG_OUT<<"read8ret: apage="<<apage<<" a="<<a<<"...\n";
  switch(bpage) {
    case 4: case 5: case 6: case 7: case 8: case 9:
      *ret=0xff00;  // should be msb of next instruction, not ff...
      return true;
      break;
    case 0xa: case 0xb:  case 0xc:  case 0xd:
      *ret=myIO->write8(addr,val);
      return true;
      break;
    case 0xe: case 0xf:
      a=addr&0xffff;
      if(!theRam) theRam=(ERam *)map->findArea("ram");
      actual=0xff0000|a;
      DEBUG_OUT<<"NGenesisHandler::read8ret mirroring "<<addr<<" to "<<actual<<"\n";
      *ret=theRam->write8(actual,val);
      return true;
      break;
    default:
      return false;
      break;
  }
  return false;
}


