
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#include <elib/ESoundIOSid.h>
#include <elib/EHandler.h>

#include <portable/nickcpp/ANullStream.h>


////////////////////////////////////////////////////////////////////////////////
//  ESoundIOSid Class
////////////////////////////////////////////////////////////////////////////////

ESoundIOSid::ESoundIOSid(EHandler *parent, ULONG address) : ESoundIOAbstract(parent,address)
{
  DEBUG_OUT<<"ESoundIOSid::ESoundIOSid()\n";
  //resize(0xf);
  setLocation(address);
  reset();
}


ESoundIOSid::~ESoundIOSid()
{
}


void ESoundIOSid::reset()
{
  ESoundIOAbstract::reset();
}


void ESoundIOSid::ioregs()
{
  DEBUG_OUT<<"ESoundIOSid "<<location<<":\n";
  ESoundIOAbstract::ioregs();
}


void ESoundIOSid::io_cycle()
{
  ESoundIOAbstract::io_cycle();
}


BYTE ESoundIOSid::write8(ULONG address, BYTE value)
{
  unsigned int ad=address&0xf;
  switch(ad) {
    default:
      DEBUG_OUT<<"sound io write at "<<address<<" of "<<value<<"\n";
      break;
  }
  return value;
}


BYTE ESoundIOSid::read8(ULONG address)
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

