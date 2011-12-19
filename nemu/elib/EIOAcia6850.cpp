
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#include <portable/nickcpp/ANullStream.h>

#include <elib/EIOAcia6850.h>


// Need to fix this...
#define ECPUABSTRACT_INT_IRQ 0


/*

*/


////////////////////////////////////////////////////////////////////////////////
//  EIOAcia6850 Class
////////////////////////////////////////////////////////////////////////////////

EIOAcia6850::EIOAcia6850(EHandler *parent, ULONG address) : EIOAcia(parent,address)
{
  DEBUG_OUT<<"EIOAcia6850 created.\n";
  reset();
}


void EIOAcia6850::ioregs()
{
  DEBUG_OUT<<"EIOAcia6850 "<<location<<":\n";
  EIOAcia::ioregs();
}


void EIOAcia6850::io_cycle()
{
  EIOAcia::io_cycle();
}


void EIOAcia6850::reset()
{
  EIOAcia::reset();
}


BYTE EIOAcia6850::write8Reg(unsigned int reg,BYTE value)
{
  reg=reg&0x6;
  tval=value;
  switch(reg) {
    case 0x0:
      DEBUG_OUT<<"EIOAcia6850 port a write of "<<(int)tval<<"\n";
      break;
    case 0x2:
      DEBUG_OUT<<"EIOAcia6850 port b write of "<<(int)tval<<"\n";
      break;
    case 0x4:
      DEBUG_OUT<<"EIOAcia6850 port a control write of "<<(int)tval<<"\n";
      break;
    case 0x6:
      DEBUG_OUT<<"EIOAcia6850 port b control write of "<<(int)tval<<"\n";
      break;
    default:
      break;
  }
  mem[reg]=tval;
  return tval;
}


BYTE EIOAcia6850::read8Reg(unsigned int reg)
{
  reg=reg&0x6;
  tval=mem[reg];
  switch(reg) {
    case 0x0:
      DEBUG_OUT<<"EIOAcia6850 port a read of "<<(int)tval<<"\n";
      break;
    case 0x2:
      DEBUG_OUT<<"EIOAcia6850 port b read of "<<(int)tval<<"\n";
      break;
    case 0x4:
      DEBUG_OUT<<"EIOAcia6850 port a control read of "<<(int)tval<<"\n";
      break;
    case 0x6:
      DEBUG_OUT<<"EIOAcia6850 port b control read of "<<(int)tval<<"\n";
      break;
    default:
      break;
  }
  return tval;
}

