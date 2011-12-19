
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#include <emu/NPalmHandler.h>
#include <emu/NPalmIO.h>

#include <portable/nickcpp/ANullStream.h>

#include <portable/nickcpp/AStringHelper.h>

#include <portable/nickcpp/APalette.h>

#include <cpulib/Em68k.h>

#include <elib/EMemMapAbstract.h>
#include <elib/EModule.h>
#include <elib/ERom.h>
#include <elib/ERam.h>


////////////////////////////////////////////////////////////////////////////////
//  NPalmIO Class
////////////////////////////////////////////////////////////////////////////////

NPalmIO::NPalmIO(NPalmHandler *parent) : EMemIO(parent)
{
  switch(type) {
    case N_PALM_IO_GENERIC:
      location=0xff0000;
      resize(64*1024);
      break;
    default:
      std::cerr<<"Unknown Palm hardware type!\n";
      exit(5);
      break;
  }
  reset();
}


NPalmIO::~NPalmIO()
{
}


void NPalmIO::reset()
{
}


void NPalmIO::ioregs()
{
}


void NPalmIO::io_cycle()
{
}


BYTE NPalmIO::write8(ULONG address,BYTE value)
{
  ULONG ad=(ULONG)address;
  unsigned int page=(address>>16)&0xf0;
  unsigned int chip=(address>>8)&0xffff;
  switch(chip) {
    default:
      debugger("write to unknown io address!");
      DEBUG_OUT<<"io write of "<<(short)value<<" at "<<address<<"\n";
      break;
  }
  return value;
}


BYTE NPalmIO::read8(ULONG address)
{
  BYTE val=mem[(address-location)&(actualSize-1)];
  unsigned int page=(address>>16)&0xf0;
  unsigned int chip=(address>>8)&0xffff;
  switch(chip) {
    default:
      debugger("read from unknown io address!");
      DEBUG_OUT<<"io read at "<<address<<"\n";
      break;
  }
  return val;
}

