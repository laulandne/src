
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#include <portable/nickcpp/ANullStream.h>

#include <elib/EIOPia8255.h>


// Need to fix this...
#define ECPUABSTRACT_INT_IRQ 0


/*

8255 PIA reg map
  0: read/write to port a or data dir a
  1: read/write to port b or data dir b
  2: read/write to port c or data dir c

*/


////////////////////////////////////////////////////////////////////////////////
//  EIOPia8255 Class
////////////////////////////////////////////////////////////////////////////////

EIOPia8255::EIOPia8255(EHandler *parent, ULONG address) : EIOPia(parent,address)
{
  DEBUG_OUT<<"EIOPia8255 created.\n";
  numPorts=3;
  reset();
}


void EIOPia8255::ioregs()
{
  DEBUG_OUT<<"EIOPia8255 "<<location<<":\n";
  EIOPia::ioregs();
}


void EIOPia8255::io_cycle()
{
  EIOPia::io_cycle();
}


void EIOPia8255::reset()
{
  EIOPia::reset();
}


// TODO: How do we tell data vs ddr writes?!?
BYTE EIOPia8255::write8Reg(unsigned int reg,BYTE value)
{
  reg=reg&0x3;
  tval=value;
  switch(reg) {
    case 0x0:
      DEBUG_OUT<<"EIOPia8255 port a write of "<<(int)tval<<"\n";
      writePortByte(0,tval);
      break;
     case 0x1:
      DEBUG_OUT<<"EIOPia8255 port b write of "<<(int)tval<<"\n";
      writePortByte(1,tval);
      break;
     case 0x2:
      DEBUG_OUT<<"EIOPia8255 port c write of "<<(int)tval<<"\n";
      writePortByte(2,tval);
      break;
    default:
      debugger("EIOPia8255::write8Reg unhandled reg!");
      break;
  }
  mem[reg]=tval;
  return tval;
}


// TODO: How do we tell data vs ddr reads?!?
BYTE EIOPia8255::read8Reg(unsigned int reg)
{
  reg=reg&0x3;
  tval=mem[reg];
  switch(reg) {
    case 0x0:
      DEBUG_OUT<<"EIOPia8255 port a read of "<<(int)tval<<"\n";
      tval=readPortByte(0);
      break;
     case 0x1:
      DEBUG_OUT<<"EIOPia8255 port b read of "<<(int)tval<<"\n";
      tval=readPortByte(1);
      break;
     case 0x2:
      DEBUG_OUT<<"EIOPia8255 port c read of "<<(int)tval<<"\n";
      tval=readPortByte(2);
      break;
    default:
      debugger("EIOPia8255::read8Reg unhandled reg!");
      break;
  }
  return tval;
}

