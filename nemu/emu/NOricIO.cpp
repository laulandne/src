
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#include <emu/NOricHandler.h>
#include <emu/NOricIO.h>
#include <portable/nickcpp/ANullStream.h>

#include <portable/nickcpp/AStringHelper.h>

#include <portable/nickcpp/APalette.h>

#include <elib/EMemMapAbstract.h>
#include <elib/EModule.h>
#include <elib/ERom.h>
#include <elib/ERam.h>
#include <elib/EIOVia6522.h>


////////////////////////////////////////////////////////////////////////////////
//  NOricIO Class
////////////////////////////////////////////////////////////////////////////////

NOricIO::NOricIO(NOricHandler *parent) : EMemIO(parent)
{
  DEBUG_OUT<<"Creating NOricIO\n";
  //

  switch(type) {
    case N_ORIC_IO_STANDARD:
      location=0x300;
      resize(0x100);
      via=new EIOVia6522(parent,0);
      break;
    default:
      debugger("Unknown IO type!");
      err.set();
      break;
  }
  reset();
}


NOricIO::~NOricIO()
{
  if(via) delete via;
  via=NULL;
}


void NOricIO::reset()
{
  if(via) via->reset();
}


void NOricIO::ioregs()
{
  if(via) via->ioregs();
}


void NOricIO::io_cycle()
{
  if(via) via->io_cycle();
}


BYTE NOricIO::write8(ULONG addr, BYTE value)
{
  BYTE page=(addr>>8)&0xff;
  //BYTE nib2=addr&0x00f0;
  BYTE val=value;
  switch(page) {
    case 0x3:
      // via
      //DEBUG_OUT<<"via write of "<<(short)value<<" at "<<addr<<"\n";
      val=via->write8Reg(addr&0xf,val);
      break;
    default:
      DEBUG_OUT<<"io write of "<<(short)value<<" at "<<addr<<"\n";
      break;
  }
  return val;
}


BYTE NOricIO::read8(ULONG addr)
{
  BYTE page=(addr>>8)&0xff;
  //BYTE nib2=ad&0x00f0;
  BYTE val=mem[addr-location];
  switch(page) {
    case 0x3:
      // via
      val=via->read8Reg(addr&0xf);
      //DEBUG_OUT<<"via read of "<<(short)val<<" at "<<addr<<"\n";
      break;
    default:
      DEBUG_OUT<<"io read at "<<addr<<"\n";
      break;
  }
  return val;
}


