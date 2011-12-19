
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#include <elib/ESoundIOSn764xx.h>
#include <elib/EHandler.h>

#include <portable/nickcpp/ANullStream.h>


////////////////////////////////////////////////////////////////////////////////
//  ESoundIOSn764xx Class
////////////////////////////////////////////////////////////////////////////////

ESoundIOSn764xx::ESoundIOSn764xx(EHandler *parent, ULONG address) : ESoundIOAbstract(parent,address)
{
  DEBUG_OUT<<"ESoundIOSn764xx::ESoundIOSn764xx()\n";
  //resize(0xf);
  setLocation(address);
  reset();
}


ESoundIOSn764xx::~ESoundIOSn764xx()
{
}


void ESoundIOSn764xx::reset()
{
  ESoundIOAbstract::reset();
}


void ESoundIOSn764xx::ioregs()
{
  DEBUG_OUT<<"ESoundIOSn764xx "<<location<<":\n";
  ESoundIOAbstract::ioregs();
}


void ESoundIOSn764xx::io_cycle()
{
  ESoundIOAbstract::io_cycle();
}


BYTE ESoundIOSn764xx::write8(ULONG address, BYTE value)
{
  unsigned int ad=address&0xf;
  switch(ad) {
    default:
      DEBUG_OUT<<"sound io write at "<<address<<" of "<<value<<"\n";
      break;
  }
  return value;
}


BYTE ESoundIOSn764xx::read8(ULONG address)
{
  unsigned int ad=address&0xf;
  unsigned int ival=0;
  BYTE value=mem[ad];
  switch(ad) {
    default:
      DEBUG_OUT<<"sound io read at "<<address<<" of "<<value<<"\n";
      break;
  }
  return value;
}

