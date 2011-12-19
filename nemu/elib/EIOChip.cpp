
//#define DEBUG_OUT std::cerr
#define DEBUG_OUT *aNullStream


#include <elib/EIOChip.h>


// Need to fix this...
#define ECPUABSTRACT_INT_IRQ 0


////////////////////////////////////////////////////////////////////////////////
//  EIOChip Class
////////////////////////////////////////////////////////////////////////////////


EIOChip::EIOChip(EHandler *parent, ULONG address) : EMemIO(parent)
{
  resize(0xff);  // NOTE: We won't be using all of it probably...
  setLocation(address);
  reset();
}


EIOChip::~EIOChip()
{
}


void EIOChip::init()
{
  unsigned int t;
  for(t=0;t<memsize;t++) mem[t]=0;
  for(t=0;t<8;t++) keys[t]=0;
  key_count=0;  key_max=0;
  keyScan=0xff;
  joyKey[0]=0;  joyKey[1]=0;
  irqen=0;
  IFR=0;
  readNotify=false;
  writeNotify=false;
  handleTimers=true;
  handlePorts=true;
  handleSerial=true;
}


void EIOChip::move_timer(ULONG address, UINT16 value)
{
  if(address<memsize) {
    mem[address]=(BYTE)(value&0xff);
    mem[address+1]=(BYTE)((value&0xff00)>>8);
  }
}


void EIOChip::ioregs()
{
}


void EIOChip::io_cycle()
{
}


void EIOChip::reset()
{
  init();
}


// NOTE: address is only 0-f
BYTE EIOChip::write8(ULONG address, BYTE val)
{
  unsigned int ad=address&0xf;
  BYTE value=val;
  //if(io->io_debug) *win<<"xia write of "<<value<<" at "<<ad<<"\n";
  // Here is where we need to decode the address
  value=write8Reg(ad,val);
  return value;
}


// NOTE: address is only 0-f
BYTE EIOChip::read8(ULONG address)
{
  unsigned int ad=address&0xf;
  //if(io->io_debug) *win<<"xia read at "<<ad<<"\n";
  unsigned int ival=0;
  // Here is where we need to decode the address
  BYTE value=read8Reg(ad);
  return value;
}


BYTE EIOChip::write8Reg(unsigned int reg,BYTE value)
{
  reg&=0xf;
  BYTE ret=value;
  mem[reg]=value;
  debugger("EIOChip::write8Reg called!");
  return ret;
}


BYTE EIOChip::read8Reg(unsigned int reg)
{
  reg&=0xf;
  BYTE value=0;
  value=mem[reg];
  debugger("EIOChip::read8Reg called!");
  return value;
}

