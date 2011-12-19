
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#include <portable/nickcpp/ANullStream.h>

#include <elib/EIOPia.h>
#include <elib/EHandler.h>

#include <cpulib/ECPUAbstract.h>


// Need to fix this...
#define ECPUABSTRACT_INT_IRQ 0


////////////////////////////////////////////////////////////////////////////////
//  EIOPia Class
////////////////////////////////////////////////////////////////////////////////

EIOPia::EIOPia(EHandler *parent, ULONG address) : EIOChip(parent,address)
{
  DEBUG_OUT<<"EIOPia created.\n";
  numTimers=0;
  numPorts=0;
  irq_int=0;
  reset();
}


void EIOPia::ioregs()
{
  //DEBUG_OUT<<"EIOPia "<<location<<":\n";
  DEBUG_OUT<<"irqen "<<irqen<<":\n";
  unsigned int t=0;
  for(t=0;t<numPorts;t++) {
    DEBUG_OUT<<"Port "<<t<<" data="<<portData[t]<<" DDR="<<portDDR[t]<<"\n";
  }
  for(t=0;t<numTimers;t++) {
    DEBUG_OUT<<"Timer "<<t<<" v="<<timer[t].value<<" i="<<timer[t].irq<<" s="<<timer[t].state<<" l="<<timer[t].latch<<" f="<<timer[t].free<<" r="<<timer[t].run<<"\n";
  }
}


void EIOPia::io_cycle()
{
  cycle_timers();
}


void EIOPia::reset()
{
  unsigned int t=0;
  for(t=0;t<memsize;t++) mem[t]=0;
  for(t=0;t<EIOPIA_MAX_NUM_PORTS;t++) {
    portData[t]=0;
    portDDR[t]=0;
  }
  for(t=0;t<EIOPIA_MAX_NUM_TIMERS;t++) {
    timer[t].irq=0;
    timer[t].state=0;
    timer[t].latch=0;
    timer[t].value=0;
    timer[t].free=false;
    timer[t].run=false;
  }
}


BYTE EIOPia::write8Reg(unsigned int reg,BYTE value)
{
  BYTE ret=value;
  debugger("EIOPia::write8Reg called!");
  return ret;
}


BYTE EIOPia::read8Reg(unsigned int reg)
{
  BYTE value=0;
  debugger("EIOPia::read8Reg called!");
  return value;
}


void EIOPia::cycle_timers()
{
  unsigned int speed=handler->getTimerSpeed();
  for(unsigned int t=0;t<numTimers;t++) {
    if(timer[t].run) {
      if(timer[t].value) timer[t].value-=speed;
      if(timer[t].value<0) timer[t].value=0;
      if(!timer[t].value) {
        //DEBUG_OUT<<"%";
        timer[t].value=0;
        if(irqen&timer[t].irq) {
          if(handler->getCPU()) handler->getCPU()->cause_int(irq_int);
          if(IFR) {
            temp=read8(IFR);
            temp=temp|0x80;
            temp=temp|timer[t].irq;
            write8(IFR,temp);
          }
        }
        if(timer[t].free) timer[t].value=timer[t].latch;
        else timer[t].run=false;
      }
    }
  }
}


void EIOPia::writePortByte(unsigned int pnum,unsigned int b)
{
  if(pnum<numPorts) portData[pnum]=b;
}


unsigned int EIOPia::readPortByte(unsigned int pnum)
{
  BYTE val=0;
  if(pnum<numPorts) val=portData[pnum];
  return val;
}

