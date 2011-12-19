
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


/*

NOTE: This code doesn't work if an int is 16-bits...

*/


#include <emu/NAmigaIO.h>
#include <emu/NAmigaHandler.h>
#include <emu/generated/NAmiga_reg_tab.h>
#include <emu/NTinyHandler.h>

#include <portable/nickcpp/ANullStream.h>

#include <elib/EMemMapAbstract.h>
#include <elib/EModule.h>
#include <elib/ERom.h>
#include <elib/ERam.h>
#include <elib/EIOCia8520.h>

#include <cpulib/Em68k.h>


////////////////////////////////////////////////////////////////////////////////
//  NAmigaIO Class
////////////////////////////////////////////////////////////////////////////////

NAmigaIO::NAmigaIO(NAmigaHandler *parent) : EMemIO(parent)
{
  switch(type) {
    case N_NAMIGA_IO_ECS:
    case N_NAMIGA_IO_AGA:
      location=0xb00000;
      resize(64*1024,0xffffff-location);  // actually bigger than in reality
      break;
    default:
      DEBUG_OUT<<"Unknown Amiga hardware type!\n";
      break;
  }
  cia1=new EIOCia8520(parent,1);
  if(!cia1) err.set();
  cia2=new EIOCia8520(parent,2);
  if(!cia2) err.set();
  reset();
}


NAmigaIO::~NAmigaIO()
{
  if(cia1) delete cia1;
  cia1=(EIOCia8520 *)NULL;
  if(cia2) delete cia2;
  cia2=(EIOCia8520 *)NULL;
}


void NAmigaIO::reset()
{
  DEBUG_OUT<<"NAmigaIO::reset().\n";
  lastLEDState=false;  // power light is dim
  if(cia1) cia1->reset();
  if(cia2) cia2->reset();
}


void NAmigaIO::ioregs()
{
  if(cia1) cia1->ioregs();
  if(cia2) cia2->ioregs();
}


void NAmigaIO::io_cycle()
{
  if(cia1) cia1->io_cycle();
  if(cia2) cia2->io_cycle();
}


BYTE NAmigaIO::write8(ULONG address,BYTE value)
{
  ERom *foo=(ERom *)NULL;
  ULONG ad=(ULONG)address;
  unsigned int page=(address>>16)&0xf0;
  switch(page) {
    case 0xa0: case 0xb0:
      value=write8_cia(address,value);
      break;
    case 0xc0: case 0xd0:
      // images of the io chips
      value=write8_custom(address,value);
      break;
    case 0xe0:
      value=write8_autoconfig(address,value);
      break;
    case 0xf0:
      // "expansion rom" area, but with no rom there...
      break;
    default:
      debugger("write to unknown io address!");
      DEBUG_OUT<<"io write of "<<(short)value<<" at "<<address<<"\n";
      break;
  }
  return value;
}


BYTE NAmigaIO::read8(ULONG address)
{
  BYTE val=mem[(address-location)&(actualSize-1)];
  unsigned int page=(address>>16)&0xf0;
  switch(page) {
    case 0xa0: case 0xb0:
      val=read8_cia(address);
      break;
    case 0xc0: case 0xd0:
      // images of the io chips
      val=read8_custom(address);
      break;
    case 0xe0:
      val=read8_autoconfig(address);
      break;
    case 0xf0:
      // "expansion rom" area, but with no rom there...
      break;
    default:
      debugger("read from unknown io address!");
      DEBUG_OUT<<"io read at "<<address<<"\n";
      break;
  }
  return val;
}


BYTE NAmigaIO::write8_cia(ULONG address,BYTE value)
{
  unsigned int a=address&0xff;
  switch(a) {
    case 0xe001:  // rom overlay, led, etc.
      break;
    case 0xe201:  // ???
      break;
    default:
      // TODO: How can I tell if an address is meant for cia1 vs cia2?
      cia1->write8(a,value);
      break;
  }
  return value;
}


BYTE NAmigaIO::write8_rtc(ULONG address, BYTE value)
{
  BYTE ret=value;
  unsigned int a=address&0xffff;
  DEBUG_OUT<<"RTC write of "<<(short)value<<" at "<<address<<"\n";
  //debugger("write to unknown rtc address!");
  return ret;
}


BYTE NAmigaIO::write8_custom(ULONG address, BYTE value)
{
  BYTE ret=value;
  // TODO: what exactly should this mask be?
  unsigned int reg=address&0x1ff;
  unsigned int r=reg&0xff0;
  unsigned int r2=reg/2;
  switch(reg) {
    case 0x180:  case 0x190:  case 0x1a0:  case 0x1b0:
      // The color registers
      mem[reg]=ret;
      break;
    default:
      DEBUG_OUT<<"CUSTOM write of "<<(short)value<<" to reg "<<address<<"\n";
      //DEBUG_OUT<<NAmigaRegName[r2]<<": "<<NAmigaRegDesc[r2]<<"\n";
      mem[reg]=ret;
      break;
  }
  return ret;
}


BYTE NAmigaIO::write8_autoconfig(ULONG address, BYTE value)
{
  BYTE ret=value;
  unsigned int a=address&0xffff;
  DEBUG_OUT<<"AUTOCONFIG write of "<<(short)value<<" at "<<address<<"\n";
  debugger("write to unknown autoconfig address!");
  return ret;
}


BYTE NAmigaIO::read8_autoconfig(ULONG address)
{
  BYTE ret=0;
  unsigned int a=address&0xffff;
  DEBUG_OUT<<"AUTOCONFIG read at "<<address<<"\n";
  debugger("read from unknown autoconfig address!");
  return ret;
}


BYTE NAmigaIO::read8_custom(ULONG address)
{
  BYTE ret=0;
  // TODO: what exactly should this mask be?
  unsigned int reg=address&0x1ff;
  unsigned int r=reg&0xff0;
  switch(r) {
    case 0x180:  case 0x190:  case 0x1a0:  case 0x1b0:
      // The color registers
      ret=mem[reg];
      break;
    default:
      DEBUG_OUT<<"CUSTOM read at reg "<<address<<"\n";
      //DEBUG_OUT<<NAmigaRegName[r2]<<": "<<NAmigaRegDesc[r2]<<"\n";
      ret=mem[reg];
      break;
  }
  return ret;
}


BYTE NAmigaIO::read8_rtc(ULONG address)
{
  BYTE ret=0;
  unsigned int a=address&0xffff;
  DEBUG_OUT<<"RTC read at "<<address<<"\n";
  //debugger("read from unknown rtc address!");
  return ret;
}


BYTE NAmigaIO::read8_cia(ULONG address)
{
  unsigned int a=address&0xff;
  BYTE ret=0;
  switch(a) {
    default:
      // TODO: How can I tell if an address is meant for cia1 vs cia2?
      ret=cia1->read8(address);
      break;
  }
  return ret;
}
