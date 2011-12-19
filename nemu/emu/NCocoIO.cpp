
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#include <emu/NCocoHandler.h>
#include <emu/NCocoIO.h>
#include <portable/nickcpp/ANullStream.h>

#include <portable/nickcpp/AStringHelper.h>

#include <portable/nickcpp/APalette.h>

#include <elib/EMemMapAbstract.h>
#include <elib/EModule.h>
#include <elib/ERom.h>
#include <elib/ERam.h>
#include <elib/EIOVia6522.h>


////////////////////////////////////////////////////////////////////////////////
//  NCocoIO Class
////////////////////////////////////////////////////////////////////////////////

NCocoIO::NCocoIO(NCocoHandler *parent) : EMemIO(parent)
{
  DEBUG_OUT<<"Creating NCocoIO\n";
  //

  switch(type) {
    case N_COCO_IO_STANDARD:
      location=0x300;
      resize(0x100);
      break;
    default:
      debugger("Unknown IO type!");
      err.set();
      break;
  }
  reset();
}


NCocoIO::~NCocoIO()
{
}


void NCocoIO::reset()
{
}


void NCocoIO::ioregs()
{
}


void NCocoIO::io_cycle()
{
}


BYTE NCocoIO::write8(ULONG addr, BYTE value)
{
  BYTE page=(addr>>8)&0xff;
  //BYTE nib2=addr&0x00f0;
  BYTE val=value;
  switch(page) {
    default:
      DEBUG_OUT<<"io write of "<<(short)value<<" at "<<addr<<"\n";
      break;
  }
  return val;
}


BYTE NCocoIO::read8(ULONG addr)
{
  BYTE page=(addr>>8)&0xff;
  //BYTE nib2=ad&0x00f0;
  BYTE val=mem[addr-location];
  switch(page) {
    default:
      DEBUG_OUT<<"io read at "<<addr<<"\n";
      break;
  }
  return val;
}


