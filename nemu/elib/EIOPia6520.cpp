
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#include <portable/nickcpp/ANullStream.h>

#include <elib/EIOPia6520.h>


// Need to fix this...
#define ECPUABSTRACT_INT_IRQ 0


/*

6520 PIA reg map
  0: read/write to port a or data dir a
  1: read/write to port b or data dir b
  2: port a control
  3: port a control

*/


////////////////////////////////////////////////////////////////////////////////
//  EIOPia6520 Class
////////////////////////////////////////////////////////////////////////////////

EIOPia6520::EIOPia6520(EHandler *parent, ULONG address) : EIOPia(parent,address)
{
  DEBUG_OUT<<"EIOPia6520 created.\n";
  numPorts=2;
  reset();
}


void EIOPia6520::ioregs()
{
  DEBUG_OUT<<"EIOPia6520 "<<location<<":\n";
  EIOPia::ioregs();
}


void EIOPia6520::io_cycle()
{
  EIOPia::io_cycle();
}


void EIOPia6520::reset()
{
  EIOPia::reset();
}


BYTE EIOPia6520::write8Reg(unsigned int reg,BYTE value)
{
  reg=reg&0x3;
  tval=value;
  switch(reg) {
    case 0x0:
      DEBUG_OUT<<"EIOPia6520 port a write of "<<(int)tval<<"\n";
      writePortByte(0,tval);
      break;
     case 0x1:
      DEBUG_OUT<<"EIOPia6520 port b write of "<<(int)tval<<"\n";
      writePortByte(1,tval);
      break;
     case 0x2:
      DEBUG_OUT<<"EIOPia6520 port a control write of "<<(int)tval<<"\n";
      break;
     case 0x3:
      DEBUG_OUT<<"EIOPia6520 port b control write of "<<(int)tval<<"\n";
      break;
    default:
      break;
  }
  mem[reg]=tval;
  return tval;
}


BYTE EIOPia6520::read8Reg(unsigned int reg)
{
  reg=reg&0x3;
  tval=mem[reg];
  switch(reg) {
    case 0x0:
      DEBUG_OUT<<"EIOPia6520 port a read of "<<(int)tval<<"\n";
      tval=readPortByte(0);
      break;
     case 0x1:
      DEBUG_OUT<<"EIOPia6520 port b read of "<<(int)tval<<"\n";
      tval=readPortByte(1);
      break;
     case 0x2:
      DEBUG_OUT<<"EIOPia6520 port a control read of "<<(int)tval<<"\n";
      break;
     case 0x3:
      DEBUG_OUT<<"EIOPia6520 port b control read of "<<(int)tval<<"\n";
      break;
    default:
      break;
  }
  return tval;
}

