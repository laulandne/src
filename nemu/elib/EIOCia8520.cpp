
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#include <portable/nickcpp/ANullStream.h>

#include <elib/EIOCia8520.h>


// Need to fix this...
#define ECPUABSTRACT_INT_IRQ 0


////////////////////////////////////////////////////////////////////////////////
//  EIOCia8520 Class
////////////////////////////////////////////////////////////////////////////////

EIOCia8520::EIOCia8520(EHandler *parent, ULONG address) : EIOVia6522(parent,address)
{
  DEBUG_OUT<<"EIOCia8520 created.\n";
  irq_int=ECPUABSTRACT_INT_IRQ;  // TODO: Should change this...
  numTimers=2;
  numPorts=2;
  reset();
}


void EIOCia8520::ioregs()
{
  DEBUG_OUT<<"EIOCia8520 "<<location<<":\n";
  EIOVia6522::ioregs();
}


void EIOCia8520::io_cycle()
{
  EIOVia6522::io_cycle();
}


void EIOCia8520::reset()
{
  tod=0;
  EIOVia6522::reset();
}


BYTE EIOCia8520::write8Reg(unsigned int reg,BYTE value)
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


BYTE EIOCia8520::read8Reg(unsigned int reg)
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

