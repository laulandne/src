
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#include <portable/nickcpp/ANullStream.h>

#include <elib/EIOVia6522.h>
#include <elib/EHandler.h>

#include <cpulib/ECPUAbstract.h>


// Need to fix this...
#define ECPUABSTRACT_INT_IRQ 0


////////////////////////////////////////////////////////////////////////////////
//  EIOVia6522 Class
////////////////////////////////////////////////////////////////////////////////

EIOVia6522::EIOVia6522(EHandler *parent, ULONG address) : EIOPia(parent,address)
{
  DEBUG_OUT<<"EIOVia6522 created.\n";
  irq_int=ECPUABSTRACT_INT_IRQ;  // TODO: Should change this...
  numTimers=2;
  numPorts=2;
  reset();
}


void EIOVia6522::ioregs()
{
  DEBUG_OUT<<"EIOVia6522 "<<location<<":\n";
  EIOPia::ioregs();
}


void EIOVia6522::io_cycle()
{
  // Processing
  EIOPia::io_cycle();
  // Post processing
  move_timer(0x4,timer[0].value);
  move_timer(0x6,timer[0].value);
  move_timer(0x8,timer[1].value);
}


void EIOVia6522::reset()
{
  EIOPia::reset();
}


BYTE EIOVia6522::write8Reg(unsigned int reg,BYTE value)
{
  reg=reg&0xf;
  tval=value;
  switch(reg) {
    case 0x0:
      DEBUG_OUT<<"via port b write of "<<(int)tval<<"\n";
      writePortByte(1,tval);
      break;
    case 0xf: case 0x1:
      DEBUG_OUT<<"via port a write of "<<(int)tval<<"\n";
      writePortByte(0,tval);
      break;
    case 0x2:
      DEBUG_OUT<<"via value direction for b write of "<<(int)tval<<"\n";
      portDDR[1]=tval;
      break;
    case 0x3:
      DEBUG_OUT<<"via value direction for a write of "<<(int)tval<<"\n";
      portDDR[0]=tval;
      break;
    case 0x4:
      DEBUG_OUT<<"via timer 1 low latch write of "<<(int)tval<<"\n";
      timer[0].latch=(timer[0].latch&0xff00)+value;
      break;
    case 0x5:
      DEBUG_OUT<<"via timer 1 high latch write of "<<(int)tval<<"\n";
      timer[0].latch=(timer[0].latch&0x00ff)+(value<<8);
      timer[0].value=timer[0].latch;
      move_timer(4,timer[0].value);
      move_timer(6,timer[0].value);
      timer[0].run=true;
      if(timer[0].state) {
        temp=read8(IFR);
        temp=temp&(0xff-timer[0].irq);
        if(temp==0x80) temp=0;
        write8(IFR,temp);
        timer[0].state=0;
      }
      break;
    case 0x6:
      DEBUG_OUT<<"via timer 1 low load write of "<<(int)tval<<"\n";
      timer[0].latch=(timer[0].latch&0xff00)+value;
      break;
    case 0x7:
      DEBUG_OUT<<"via timer 1 high load write of "<<(int)tval<<"\n";
      timer[0].latch=(timer[0].latch&0x00ff)+(value<<8);
      timer[0].run=true;
      if(timer[0].state) {
        temp=read8(IFR);
        temp=temp&(0xff-timer[0].irq);
        if(temp==0x80) temp=0;
        write8(IFR,temp);
        timer[0].state=0;
      }
      break;
    case 0x8:
      DEBUG_OUT<<"via timer 2 low write of "<<(int)tval<<"\n";
      timer[1].latch=(timer[1].latch&0xff00)+value;
      break;
    case 0x9:
      DEBUG_OUT<<"via timer 2 high write of "<<(int)tval<<"\n";
      timer[1].latch=(timer[1].latch&0x00ff)+(value<<8);
      timer[1].value=timer[1].latch;
      move_timer(0xe848,timer[1].value);
      timer[1].run=true;
      if(timer[1].state) {
        temp=read8(IFR);
        temp=temp&(0xff-timer[1].irq);
        if(temp==0x80) temp=0;
        write8(IFR,temp);
        timer[1].state=0;
      }
      break;
    case 0xa:
      DEBUG_OUT<<"via shift register write of "<<(int)tval<<"\n";
      break;
    case 0xb:
      DEBUG_OUT<<"via aux control reg #1 write of "<<(int)tval<<"\n";
      if(value&0x40) timer[0].free=true; else timer[0].free=false;
      if(value&0x20) timer[1].free=true; else timer[1].free=false;
      break;
    case 0xc:
      DEBUG_OUT<<"via control reg (ca1, ca2, cb1, cb2) write of "<<(int)tval<<"\n";
      break;
    case 0xd:
      DEBUG_OUT<<"via interrupt flags write of "<<(int)tval<<"\n";
      break;
    case 0xe:
      DEBUG_OUT<<"via interrupt enable write of "<<(int)tval<<"\n";
      tval=value;
      if(tval&0x80) irqen=irqen|(tval&0x7f);  // set
      else irqen=irqen&(~tval);  // clear
      DEBUG_OUT<<"irqen is now "<<(int)irqen<<"\n";
      break;
    default:
      break;
  }
  mem[reg]=tval;
  return tval;
}


BYTE EIOVia6522::read8Reg(unsigned int reg)
{
  reg=reg&0xf;
  tval=mem[reg];
  switch(reg) {
    case 0x0:
      DEBUG_OUT<<"via port b read of "<<(int)tval<<"\n";
      tval=readPortByte(1);
      break;
    case 0xf: case 0x1:
      DEBUG_OUT<<"via port a read of "<<(int)tval<<"\n";
      tval=readPortByte(0);
      break;
    case 0x2:
      DEBUG_OUT<<"via value direction for b read of "<<(int)tval<<"\n";
      tval=portDDR[1];
      break;
    case 0x3:
      DEBUG_OUT<<"via value direction for a read of "<<(int)tval<<"\n";
      tval=portDDR[0];
      break;
    case 0x4:
      DEBUG_OUT<<"via timer 1 low read of "<<(int)tval<<"\n";
      timer[0].state++;
      break;
    case 0x5:
      DEBUG_OUT<<"via timer 1 high & counter read of "<<(int)tval<<"\n";
      break;
    case 0x6:
      DEBUG_OUT<<"via timer 1 low read of "<<(int)tval<<"\n";
      timer[0].state++;
      break;
    case 0x7:
      DEBUG_OUT<<"via timer 1 high read of "<<(int)tval<<"\n";
      break;
    case 0x8:
      DEBUG_OUT<<"via timer 2 low read of "<<(int)tval<<"\n";
      timer[1].state++;
      break;
    case 0x9:
      DEBUG_OUT<<"via timer 2 high read of "<<(int)tval<<"\n";
      break;
    case 0xa:
      DEBUG_OUT<<"via shift register read of "<<(int)tval<<"\n";
      break;
    case 0xb:
      DEBUG_OUT<<"via aux control reg read of "<<(int)tval<<"\n";
      break;
    case 0xc:
      DEBUG_OUT<<"via control reg (ca1, ca2, cb1, cb2) read of "<<(int)tval<<"\n";
      break;
    case 0xd:
      DEBUG_OUT<<"via interrupt flags read of "<<(int)tval<<"\n";
      break;
    case 0xe:
      DEBUG_OUT<<"via interrupt enable read of "<<(int)tval<<"\n";
      break;
    default:
      break;
  }
  return tval;
}


