
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#include <portable/nickcpp/ANullStream.h>

#include <elib/EIOAcia6551.h>


// Need to fix this...
#define ECPUABSTRACT_INT_IRQ 0


/*

*/


////////////////////////////////////////////////////////////////////////////////
//  EIOAcia6551 Class
////////////////////////////////////////////////////////////////////////////////

EIOAcia6551::EIOAcia6551(EHandler *parent, ULONG address) : EIOAcia(parent,address)
{
  DEBUG_OUT<<"EIOAcia6551 created.\n";
  reset();
}


void EIOAcia6551::ioregs()
{
  DEBUG_OUT<<"EIOAcia6551 "<<location<<":\n";
  EIOAcia::ioregs();
}


void EIOAcia6551::io_cycle()
{
  EIOAcia::io_cycle();
}


void EIOAcia6551::reset()
{
  EIOAcia::reset();
}


BYTE EIOAcia6551::write8Reg(unsigned int reg,BYTE value)
{
  reg=reg&0x6;
  tval=value;
  switch(reg) {
    case 0x0:
      DEBUG_OUT<<"EIOAcia6551 port write of "<<(int)tval<<"\n";
      break;
    case 0x2:
      DEBUG_OUT<<"EIOAcia6551 port control 1 of "<<(int)tval<<"\n";
      break;
    case 0x4:
      DEBUG_OUT<<"EIOAcia6551 port control 2 write of "<<(int)tval<<"\n";
      break;
    case 0x6:
      DEBUG_OUT<<"EIOAcia6551 port control 3 write of "<<(int)tval<<"\n";
      break;
    default:
      break;
  }
  mem[reg]=tval;
  return tval;
}


BYTE EIOAcia6551::read8Reg(unsigned int reg)
{
  reg=reg&0x6;
  tval=mem[reg];
  switch(reg) {
    case 0x0:
      DEBUG_OUT<<"EIOAcia6551 port read of "<<(int)tval<<"\n";
      break;
    case 0x2:
      DEBUG_OUT<<"EIOAcia6551 port control 1 of "<<(int)tval<<"\n";
      break;
    case 0x4:
      DEBUG_OUT<<"EIOAcia6551 port control 2 read of "<<(int)tval<<"\n";
      break;
    case 0x6:
      DEBUG_OUT<<"EIOAcia6551 port control 3 read of "<<(int)tval<<"\n";
      break;
    default:
      break;
  }
  return tval;
}

