
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#include <emu/NPcHandler.h>
#include <emu/NPcIO.h>
#include <emu/NTinyHandler.h>

#include <portable/nickcpp/ANullStream.h>

#include <elib/EMemMapAbstract.h>
#include <elib/EVideoIOMoto684x.h>

#include <cpulib/Ex86.h>


////////////////////////////////////////////////////////////////////////////////
//  NPCIO Class
////////////////////////////////////////////////////////////////////////////////

NPcIO::NPcIO(NPcHandler *parent) : EMemIO(parent)
{
}


BYTE NPcIO::write8(ULONG addr, BYTE value)
{
  BYTE val=value;
  if(handler->io_debug) DEBUG_OUT<<"io write of "<<(short)value<<" at "<<addr<<"\n";
  if(type==N_PC_IO_GENERIC) val=write8_pc_generic(addr,value);
  return val;
}


BYTE NPcIO::read8(ULONG addr)
{
  if(handler->io_debug) DEBUG_OUT<<"io read at "<<addr<<"\n";
  if(type==N_PC_IO_GENERIC) return read8_pc_generic(addr);
  return mem[addr-location];
}


BYTE NPcIO::write8_pc_generic(ULONG address,BYTE value)
{
  switch(address) {
    default:
      DEBUG_OUT<<"io write of "<<(short)value<<" at "<<address<<"\n";
      break;
  }
  return value;
}


BYTE NPcIO::read8_pc_generic(ULONG address)
{
  BYTE val=mem[address-location];
  switch(address) {
    default:
      DEBUG_OUT<<"io read at "<<address<<"\n";
      break;
  }
  return val;
}




