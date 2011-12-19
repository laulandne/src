
//#define DEBUG_OUT std::cerr
#define DEBUG_OUT *aNullStream


#include <elib/EIOScc8530.h>

#include <cpulib/ECPUAbstract.h>

#include <portable/nickcpp/ANullStream.h>


/*

read/write data a
read/write data b
read/write control a
read/write control b

*/


////////////////////////////////////////////////////////////////////////////////
//  EIOScc8530 Class
////////////////////////////////////////////////////////////////////////////////

EIOScc8530::EIOScc8530(EHandler *parent,ULONG address) : EIOAcia(parent,address)
{
  DEBUG_OUT<<"EIOScc8530 created.\n";
  reset();
}


void EIOScc8530::ioregs()
{
  DEBUG_OUT<<"EIOScc8530 "<<location<<"\n";
  EIOAcia::ioregs();
}


void EIOScc8530::io_cycle()
{
  EIOAcia::io_cycle();
}


void EIOScc8530::reset()
{
  EIOAcia::reset();
}


// TODO: No idea if this is correct...
BYTE EIOScc8530::write8Reg(unsigned int reg,BYTE data)
{
  reg=reg&0x6;
  tval=data;
  switch(reg) {
    case 0x0:
      DEBUG_OUT<<"EIOScc8530 port a write of "<<(int)tval<<"\n";
      break;
    case 0x2:
      DEBUG_OUT<<"EIOScc8530 port b write of "<<(int)tval<<"\n";
      break;
    case 0x4:
      DEBUG_OUT<<"EIOScc8530 port a control write of "<<(int)tval<<"\n";
      break;
    case 0x6:
      DEBUG_OUT<<"EIOScc8530 port b control write of "<<(int)tval<<"\n";
      break;
    default:
      break;
  }
  mem[reg]=tval;
  return tval;
}


// TODO: No idea if this is correct...
BYTE EIOScc8530::read8Reg(unsigned int reg)
{
  reg=reg&0x6;
  tval=mem[reg];
  switch(reg) {
    case 0x0:
      DEBUG_OUT<<"EIOScc8530 port a read of "<<(int)tval<<"\n";
      break;
    case 0x2:
      DEBUG_OUT<<"EIOScc8530 port b read of "<<(int)tval<<"\n";
      break;
    case 0x4:
      DEBUG_OUT<<"EIOScc8530 port a control read of "<<(int)tval<<"\n";
      break;
    case 0x6:
      DEBUG_OUT<<"EIOScc8530 port b control read of "<<(int)tval<<"\n";
      break;
    default:
      break;
  }
  return tval;
}


