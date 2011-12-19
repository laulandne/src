
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#include <elib/ESoundIOYmxxxx.h>
#include <elib/EHandler.h>

#include <portable/nickcpp/ANullStream.h>


////////////////////////////////////////////////////////////////////////////////
//  ESoundIOYmxxxx Class
////////////////////////////////////////////////////////////////////////////////

ESoundIOYmxxxx::ESoundIOYmxxxx(EHandler *parent, ULONG address) : ESoundIOAbstract(parent,address)
{
  DEBUG_OUT<<"ESoundIOYmxxxx::ESoundIOYmxxxx()\n";
  //resize(0xf);
  setLocation(address);
  reset();
}


ESoundIOYmxxxx::~ESoundIOYmxxxx()
{
}


void ESoundIOYmxxxx::reset()
{
  ESoundIOAbstract::reset();
}


void ESoundIOYmxxxx::ioregs()
{
  DEBUG_OUT<<"ESoundIOYmxxxx "<<location<<":\n";
  ESoundIOAbstract::ioregs();
}


void ESoundIOYmxxxx::io_cycle()
{
  ESoundIOAbstract::io_cycle();
}


BYTE ESoundIOYmxxxx::write8(ULONG address, BYTE value)
{
  unsigned int ad=address&0xf;
  switch(ad) {
    default:
      DEBUG_OUT<<"sound io write at "<<address<<" of "<<value<<"\n";
      break;
  }
  return value;
}


BYTE ESoundIOYmxxxx::read8(ULONG address)
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

