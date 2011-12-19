
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


/*

Code for running old Atari vector based arcade games such as Asteroids

*/


#include <emu/NAtariVectorHandler.h>
#include <emu/NAtariVectorIO.h>
#include <emu/NAtariVectorVideo.h>

#include <portable/nickcpp/ANullStream.h>

#include <portable/nickcpp/AStringHelper.h>


////////////////////////////////////////////////////////////////////////////////
//  NAtariVectorIOLow Class
////////////////////////////////////////////////////////////////////////////////

/*
NAtariVectorIOLow::NAtariVectorIOLow(NAtariVectorHandler *parent) : EMemIO(parent)
{
  location=0x0800;
  resize(0x1800);
}


BYTE NAtariVectorIOLow::write8(ULONG addr, BYTE value)
{
  BYTE val=value;
  if(io->io_debug) DEBUG_OUT<<"io write of "<<(short)value<<" at "<<addr<<"\n";
  switch(addr) {
    default:
      DEBUG_OUT<<"io write of "<<(short)value<<" at "<<addr<<"\n";
      break;
  }
  return val;
}


BYTE NAtariVectorIOLow::read8(ULONG addr)
{
  if(io->io_debug) DEBUG_OUT<<"io read at "<<addr<<"\n";
  BYTE val=mem[addr-location];
  switch(addr) {
    default:
      DEBUG_OUT<<"io read at "<<addr<<"\n";
      break;
  }
  return val;
}
*/


////////////////////////////////////////////////////////////////////////////////
//  NAtariVectorIO Class
////////////////////////////////////////////////////////////////////////////////

NAtariVectorIO::NAtariVectorIO(NAtariVectorHandler *parent) : EMemIO(parent)
{
  DEBUG_OUT<<"Creating NAtariVectorIO\n";
  //
  switch(type)
  {
    case N_ATARIVEC_IO_ASTEROIDS:
      resize(0x4c00);
      location=0x0400;
      break;
    case N_ATARIVEC_IO_TEMPEST:
      resize(0x5000);
      location=0x4000;
      break;
    default:
      debugger("Unknown/unimplemented IO type!");
      err.set();
      break;
  }
  //
  reset();
}


BYTE NAtariVectorIO::write8(ULONG addr, BYTE value)
{
  BYTE val=value;
  switch(type)
  {
    case N_ATARIVEC_IO_ASTEROIDS:
      val=write8_asteroids(addr,value);
      break;
    case N_ATARIVEC_IO_TEMPEST:
      val=write8_tempest(addr,value);
      break;
    default:
      debugger("Unknown/unimplemented IO write8!");
      break;
  }
  return val;
}


BYTE NAtariVectorIO::read8(ULONG addr)
{
  switch(type)
  {
    case N_ATARIVEC_IO_ASTEROIDS:
      return read8_asteroids(addr);
      break;
    case N_ATARIVEC_IO_TEMPEST:
      return read8_tempest(addr);
      break;
    default:
      debugger("Unknown/unimplemented IO read8!");
      break;
  }
  return mem[addr-location];
}


BYTE NAtariVectorIO::write8_asteroids(ULONG address,BYTE value)
{
  switch(address) {
    default:
      DEBUG_OUT<<"io write of "<<(short)value<<" at "<<address<<"\n";
      break;
  }
  return value;
}


BYTE NAtariVectorIO::write8_tempest(ULONG address,BYTE value)
{
  switch(address) {
    case 0x5000:  // Clear Watchdog
      break;
    default:
      DEBUG_OUT<<"io write of "<<(short)value<<" at "<<address<<"\n";
      break;
  }
  return value;
}


BYTE NAtariVectorIO::read8_tempest(ULONG address)
{
  BYTE val=mem[address-location];
  switch(address) {
    case 0x4000:
      val=0;
      break;
    default:
      DEBUG_OUT<<"io read at "<<address<<"\n";
      break;
  }
  return val;
}


BYTE NAtariVectorIO::read8_asteroids(ULONG address)
{
  BYTE val=mem[address-location];
  switch(address) {
    default:
      DEBUG_OUT<<"io read at "<<address<<"\n";
      break;
  }
  return val;
}


