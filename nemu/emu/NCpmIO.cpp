
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#include <emu/NCpmHandler.h>
#include <emu/NCpmIO.h>
#include <portable/nickcpp/ANullStream.h>

#include <portable/nickcpp/AStringHelper.h>

#include <portable/nickcpp/APalette.h>

#include <elib/EMemMapAbstract.h>
#include <elib/EModule.h>
#include <elib/ERom.h>
#include <elib/ERam.h>


////////////////////////////////////////////////////////////////////////////////
//  NCpmIO Class
////////////////////////////////////////////////////////////////////////////////

NCpmIO::NCpmIO(NCpmHandler *parent) : EMemIO(parent)
{
  DEBUG_OUT<<"Creating NCpmIO\n";
  if(type==N_CPM_IO_STANDARD) {
    location=0xc000;
    resize(0x3000);
  }
  reset();
}


NCpmIO::~NCpmIO()
{
}


void NCpmIO::reset()
{
}


void NCpmIO::ioregs()
{
}


void NCpmIO::io_cycle()
{
}


BYTE NCpmIO::write8(ULONG addr, BYTE value)
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


BYTE NCpmIO::read8(ULONG addr)
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


