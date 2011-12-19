
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#include <emu/NTrs80Handler.h>
#include <emu/NTrs80IO.h>
#include <portable/nickcpp/ANullStream.h>

#include <portable/nickcpp/AStringHelper.h>

#include <portable/nickcpp/APalette.h>

#include <elib/EMemMapAbstract.h>
#include <elib/EModule.h>
#include <elib/ERom.h>
#include <elib/ERam.h>


////////////////////////////////////////////////////////////////////////////////
//  NTrs80IO Class
////////////////////////////////////////////////////////////////////////////////

NTrs80IO::NTrs80IO(NTrs80Handler *parent) : EMemIO(parent)
{
  DEBUG_OUT<<"Creating NTrs80IO\n";
  if(type==N_TRS80_IO_MODEL1) {
    location=0x3000;
    resize(0xbff);
  }
  reset();
}


NTrs80IO::~NTrs80IO()
{
}


void NTrs80IO::reset()
{
}


void NTrs80IO::ioregs()
{
}


void NTrs80IO::io_cycle()
{
}


BYTE NTrs80IO::write8(ULONG addr, BYTE value)
{
  BYTE page=(addr>>8)&0xff;
  //BYTE nib2=addr&0x00f0;
  BYTE val=value;
  if(handler->io_debug) DEBUG_OUT<<"io write of "<<(short)value<<" at "<<addr<<"\n";
  switch(page) {
    default:
      break;
  }
  return val;
}


BYTE NTrs80IO::read8(ULONG addr)
{
  BYTE page=(addr>>8)&0xff;
  //BYTE nib2=ad&0x00f0;
  if(handler->io_debug) DEBUG_OUT<<"io read at "<<addr<<"\n";
  switch(page) {
    default:
      break;
  }
  return mem[addr-location];
}


