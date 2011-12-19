
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#include <portable/nickcpp/ANullStream.h>

#include <elib/EIOCia6526.h>


// Need to fix this...
#define ECPUABSTRACT_INT_IRQ 0


////////////////////////////////////////////////////////////////////////////////
//  EIOCia6526 Class
////////////////////////////////////////////////////////////////////////////////

EIOCia6526::EIOCia6526(EHandler *parent, ULONG address) : EIOVia6522(parent,address)
{
  DEBUG_OUT<<"EIOCia6526 created.\n";
  irq_int=ECPUABSTRACT_INT_IRQ;  // TODO: Should change this...
  numTimers=2;
  numPorts=2;
  reset();
}


void EIOCia6526::ioregs()
{
  DEBUG_OUT<<"EIOCia6526 "<<location<<":\n";
  EIOVia6522::ioregs();
}


void EIOCia6526::io_cycle()
{
  EIOVia6522::io_cycle();
}


void EIOCia6526::reset()
{
  tod=0;
  EIOVia6522::reset();
}


BYTE EIOCia6526::write8Reg(unsigned int reg,BYTE value)
{
  reg=reg&0xf;
  tval=value;
  switch(reg) {
    default:
      tval=EIOVia6522::write8Reg(reg,value);
      break;
  }
  mem[reg]=tval;
  return tval;
}


BYTE EIOCia6526::read8Reg(unsigned int reg)
{
  reg=reg&0xf;
  tval=mem[reg];
  switch(reg) {
    default:
      tval=EIOVia6522::read8Reg(reg);
      break;
  }
  return tval;
}


