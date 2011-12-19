
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#include <portable/nickcpp/ANullStream.h>

#include <elib/EIOTia6525.h>


// Need to fix this...
#define ECPUABSTRACT_INT_IRQ 0


/*

Data port a
Data port b
Data port c
DDR a
DDR b
DDR c
control 1?
control 2?

*/


////////////////////////////////////////////////////////////////////////////////
//  EIOTia6525 Class
////////////////////////////////////////////////////////////////////////////////

EIOTia6525::EIOTia6525(EHandler *parent, ULONG address) : EIOPia(parent,address)
{
  DEBUG_OUT<<"EIOTia6525 created.\n";
  numPorts=3;
  reset();
}


void EIOTia6525::ioregs()
{
  DEBUG_OUT<<"EIOTia6525 "<<location<<":\n";
  EIOPia::ioregs();
}


void EIOTia6525::io_cycle()
{
  EIOPia::io_cycle();
}


void EIOTia6525::reset()
{
  EIOPia::reset();
}


BYTE EIOTia6525::write8Reg(unsigned int reg,BYTE value)
{
  reg=reg&0x7;
  tval=value;
  switch(reg) {
    case 0x0:
      DEBUG_OUT<<"EIOTia6525 port a write of "<<(int)tval<<"\n";
      writePortByte(0,tval);
      break;
     case 0x1:
      DEBUG_OUT<<"EIOTia6525 port b write of "<<(int)tval<<"\n";
      writePortByte(1,tval);
      break;
     case 0x2:
      DEBUG_OUT<<"EIOTia6525 port c write of "<<(int)tval<<"\n";
      writePortByte(2,tval);
      break;
    case 0x3:
      DEBUG_OUT<<"EIOTia6525 value direction for b write of "<<(int)tval<<"\n";
      portDDR[0]=tval;
      break;
    case 0x4:
      DEBUG_OUT<<"EIOTia6525 value direction for b write of "<<(int)tval<<"\n";
      portDDR[1]=tval;
      break;
    case 0x5:
      DEBUG_OUT<<"EIOTia6525 value direction for b write of "<<(int)tval<<"\n";
      portDDR[2]=tval;
      break;
    case 0x6:
      DEBUG_OUT<<"EIOTia6525 port control 1 write of "<<(int)tval<<"\n";
      break;
     case 0x8:
      DEBUG_OUT<<"EIOTia6525 port control 2 write of "<<(int)tval<<"\n";
      break;
    default:
      break;
  }
  mem[reg]=tval;
  return tval;
}


BYTE EIOTia6525::read8Reg(unsigned int reg)
{
  reg=reg&0x7;
  tval=mem[reg];
  switch(reg) {
    case 0x0:
      DEBUG_OUT<<"EIOTia6525 port a read of "<<(int)tval<<"\n";
      tval=readPortByte(0);
      break;
     case 0x1:
      DEBUG_OUT<<"EIOTia6525 port b read of "<<(int)tval<<"\n";
      tval=readPortByte(1);
      break;
     case 0x2:
      DEBUG_OUT<<"EIOTia6525 port c read of "<<(int)tval<<"\n";
      tval=readPortByte(2);
      break;
    case 0x3:
      DEBUG_OUT<<"EIOTia6525 value direction for a read of "<<(int)tval<<"\n";
      tval=portDDR[0];
      break;
    case 0x4:
      DEBUG_OUT<<"EIOTia6525 value direction for b read of "<<(int)tval<<"\n";
      tval=portDDR[1];
      break;
    case 0x5:
      DEBUG_OUT<<"EIOTia6525 value direction for c read of "<<(int)tval<<"\n";
      tval=portDDR[2];
      break;
    case 0x6:
      DEBUG_OUT<<"EIOTia6525 port control 1 read of "<<(int)tval<<"\n";
      break;
    case 0x7:
      DEBUG_OUT<<"EIOTia6525 port control 2 read of "<<(int)tval<<"\n";
      break;
    default:
      break;
  }
  return tval;
}

