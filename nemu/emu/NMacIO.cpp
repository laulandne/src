
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#include <emu/NMacHandler.h>
#include <emu/NMacIO.h>
#include <emu/NTinyHandler.h>
//#include <emu/generated/nmactraps.h>
//#include <emu/generated/nmacglbls.h>

#include <cpulib/Em68k.h>
#include <cpulib/EPowerPC.h>

#include <elib/EMemMapAbstract.h>
#include <elib/EModule.h>
#include <elib/ERom.h>
#include <elib/ERam.h>
#include <elib/EIOVia6522.h>
#include <elib/EIOScc8530.h>

#include <disklib/DControllerIWM.h>

#include <portable/nickcpp/ANullStream.h>

#include <portable/nickcpp/AStringHelper.h>


////////////////////////////////////////////////////////////////////////////////
//  NMacIO Class
////////////////////////////////////////////////////////////////////////////////

NMacIO::NMacIO(NMacHandler *parent) : EMemIO(parent)
{
  via1=(EIOVia6522 *)NULL;  via2=(EIOVia6522 *)NULL;
  switch(type) {
    case N_MAC_IO_PLUS:
      location=0x400000;
      resize(64*1024,0xffffff-location);
      via1=new EIOVia6522(parent,0);
      if(!via1) err.set();
      break;
    case N_MAC_IO_GENERIC24:
      location=0x900000;
      resize(64*1024,0xffffff-location);
      via1=new EIOVia6522(parent,1);
      if(!via1) err.set();
      via2=new EIOVia6522(parent,2);
      if(!via2) err.set();
      break;
    case N_MAC_IO_GENERIC32:
    case N_MAC_IO_MAC68k_NUBUS:
    case N_MAC_IO_POWERMAC_NUBUS:
    case N_MAC_IO_POWERMAC_PCI:
      location=0x40000000;
      resize(64*1024,0xffffffff-location);
      via1=new EIOVia6522(parent,1);
      if(!via1) err.set();
      via2=new EIOVia6522(parent,2);
      if(!via2) err.set();
      break;
    default: DEBUG_OUT<<"Unknown Mac hardware type!\n"; break;
  }
  iwm=new DControllerIWM(parent,0);
  if(!iwm) err.set();
  scc=new EIOScc8530(parent,0);
  if(!scc) err.set();
  reset();
}


NMacIO::~NMacIO()
{
  if(via1) delete via1;
  if(via2) delete via2;
  via1=NULL;  via2=NULL;
  if(iwm) delete iwm;
  iwm=NULL;
  if(scc) delete scc;
  scc=NULL;
}


void NMacIO::reset()
{
  DEBUG_OUT<<"NMacIO::reset().\n";
  ERam *ramm=(ERam *)handler->getMap()->findArea("ram_area");
  if(ramm) {
    handler->getMap()->unmap(ramm);
    ramm->setLocation(0x600000);
    handler->getMap()->map(ramm);
  }
  ERom *romm=(ERom *)handler->getMap()->findArea("mac.rom");
  if(romm) {
    if(romm->getLocation()) romAddr=romm->getLocation();
    handler->getMap()->unmap(romm);
    romm->setLocation(0);
    handler->getMap()->map(romm);
    romSize=romm->getSize();
    DEBUG_OUT<<"Rom belongs at "<<romAddr<<" and is "<<romSize<<" bytes.\n";
  }
  if(via1) via1->reset();
  if(via2) via2->reset();
  if(iwm) iwm->reset();
  if(scc) scc->reset();
}


void NMacIO::ioregs()
{
  if(via1) via1->ioregs();
  if(via2) via2->ioregs();
  if(iwm) iwm->ioregs();
  if(scc) scc->ioregs();
}


void NMacIO::io_cycle()
{
  if(via1) via1->io_cycle();
  if(via2) via2->io_cycle();
  if(iwm) iwm->io_cycle();
  if(scc) scc->io_cycle();
}


BYTE NMacIO::write8(ULONG addr, BYTE value)
{
  BYTE val=value;
  if(handler->io_debug) DEBUG_OUT<<"io write of "<<(short)value<<" at "<<addr<<"\n";
  switch(type) {
    case N_MAC_IO_PLUS: val=write8_macplus(addr,value); break;
    case N_MAC_IO_GENERIC24: val=write8_mac24(addr,value); break;
    case N_MAC_IO_GENERIC32:
    case N_MAC_IO_MAC68k_NUBUS:
    case N_MAC_IO_POWERMAC_NUBUS:
    case N_MAC_IO_POWERMAC_PCI:
      val=write8_mac32(addr,value);
      break;
   default: DEBUG_OUT<<"Unknown Mac hardware type!\n"; break;
  }
  return val;
}


BYTE NMacIO::read8(ULONG addr)
{
  if(handler->io_debug) DEBUG_OUT<<"io read at "<<addr<<"\n";
  switch(type) {
    case N_MAC_IO_PLUS: return read8_macplus(addr); break;
    case N_MAC_IO_GENERIC24: return read8_mac24(addr); break;
    case N_MAC_IO_GENERIC32:
    case N_MAC_IO_MAC68k_NUBUS:
    case N_MAC_IO_POWERMAC_NUBUS:
    case N_MAC_IO_POWERMAC_PCI:
      return read8_mac32(addr);
      break;
    default: DEBUG_OUT<<"Unknown Mac hardware type!\n"; break;
  }
  return mem[(addr-location)&(actualSize-1)];
}


BYTE NMacIO::write8_macplus(ULONG address,BYTE value)
{
  unsigned int page=(address>>16)&0xf0;
  switch(page) {
    case 0x50:
      value=write8_scsi(address,value);
      break;
    case 0x80:  case 0x90:
      value=write8_scc(address,value);  // TODO: Shouldn't this be a read?
      break;
    case 0xa0:  case 0xb0:
      value=write8_scc(address,value);
      break;
    case 0xc0:  case 0xd0:
      value=write8_iwm(address,value);
      break;
    case 0xe0:
      value=write8_via(address,value);
      break;
    case 0xf0:
      break;
    default:
      if((address>romAddr)&&(address<(romAddr+romSize))) {
        value=remapRom(address);
      }
      else {
        debugger("write to unknown io address!");
        DEBUG_OUT<<"io write of "<<(short)value<<" at "<<address<<"\n";
      }
      break;
  }
  return value;
}


BYTE NMacIO::write8_mac24(ULONG address,BYTE value)
{
  unsigned int page=(address>>16)&0xf0;
  switch(page) {
    case 0x90:
      value=write8_slot24(address,0x9,value);
      break;
    case 0xa0:
      value=write8_slot24(address,0xa,value);
      break;
    case 0xb0:
      value=write8_slot24(address,0xb,value);
      break;
    case 0xc0:
      value=write8_slot24(address,0xc,value);
      break;
    case 0xd0:
      value=write8_slot24(address,0xd,value);
      break;
    case 0xe0:
      value=write8_slot24(address,0xe,value);
      break;
    case 0xf0:
      value=write8_io24(address,value);
      break;
    default:
      if((address>romAddr)&&(address<(romAddr+romSize))) {
        value=remapRom(address);
      }
      else {
        debugger("write to unknown io address!");
        DEBUG_OUT<<"io write of "<<(short)value<<" at "<<address<<"\n";
      }
      break;
  }
  return value;
}


BYTE NMacIO::write8_mac32(ULONG address,BYTE value)
{
  unsigned int p=(address>>24)&0xff;
  unsigned int s=(address>>24)&0xf;
  unsigned int page=(address>>24)&0xf0;
  switch(page) {
    case 0x50:
      value=write8_io32(address,value);
      break;
    case 0x90:
      value=write8_slot32(address,0x9,value);
      break;
    case 0xa0:
      value=write8_slot32(address,0xa,value);
      break;
    case 0xb0:
      value=write8_slot32(address,0xb,value);
      break;
    case 0xc0:
      value=write8_slot32(address,0xc,value);
      break;
    case 0xd0:
      value=write8_slot32(address,0xd,value);
      break;
    case 0xe0:
      value=write8_slot32(address,0xe,value);
      break;
    case 0xf0:
      value=write8_slot32(address,s,value);
      break;
    default:
      if((address>romAddr)&&(address<(romAddr+romSize))) {
        value=remapRom(address);
      }
      else {
        debugger("write to unknown io address!");
        DEBUG_OUT<<"io write of "<<(short)value<<" at "<<address<<"\n";
      }
      break;
  }
  return value;
}


BYTE NMacIO::read8_macplus(ULONG address)
{
  unsigned int page=(address>>16)&0xf0;
  BYTE val=0;
  switch(page) {
    case 0x50:
      val=read8_scsi(address);
      break;
    case 0x80:  case 0x90:
      val=read8_scc(address);
      break;
    case 0xa0:  case 0xb0:
      val=read8_scc(address);  // TODO: shouldn't this be a write?
      break;
    case 0xc0: case 0xd0:
      val=read8_iwm(address);
      break;
    case 0xe0:
      val=read8_via(address);
      break;
    case 0xf0:
      break;
    default:
      if((address>romAddr)&&(address<(romAddr+romSize))) {
        val=remapRom(address);
      }
      else {
        debugger("read from unknown io address!");
        DEBUG_OUT<<"io read at "<<address<<"\n";
      }
      break;
  }
  return val;
}


BYTE NMacIO::read8_mac24(ULONG address)
{
  BYTE val=0;
  unsigned int page=(address>>16)&0xf0;
  switch(page) {
    case 0x90:
      val=read8_slot24(address,0x9);
      break;
    case 0xa0:
      val=read8_slot24(address,0xa);
      break;
    case 0xb0:
      val=read8_slot24(address,0xb);
      break;
    case 0xc0:
      val=read8_slot24(address,0xc);
      break;
    case 0xd0:
      val=read8_slot24(address,0xd);
      break;
    case 0xe0:
      val=read8_slot24(address,0xe);
      break;
    case 0xf0:
      val=read8_io24(address);
      break;
    default:
      if((address>romAddr)&&(address<(romAddr+romSize))) {
        val=remapRom(address);
      }
      else {
        debugger("read from unknown io address!");
        DEBUG_OUT<<"io read at "<<address<<"\n";
      }
      break;
  }
  return val;
}


BYTE NMacIO::read8_mac32(ULONG address)
{
  BYTE val=0;
  unsigned int p=(address>>24)&0xff;
  unsigned int s=(address>>24)&0xf;
  unsigned int page=(address>>24)&0xf0;
  switch(page) {
    case 0x50:
      val=read8_io32(address);
      break;
    case 0x90:
      val=read8_slot32(address,0x9);
      break;
    case 0xa0:
      val=read8_slot32(address,0xa);
      break;
    case 0xb0:
      val=read8_slot32(address,0xb);
      break;
    case 0xc0:
      val=read8_slot32(address,0xc);
      break;
    case 0xd0:
      val=read8_slot32(address,0xd);
      break;
    case 0xe0:
      val=read8_slot32(address,0xe);
      break;
    case 0xf0:
      val=read8_slot32(address,s);
      break;
    default:
      if((address>romAddr)&&(address<(romAddr+romSize))) {
        val=remapRom(address);
      }
      else {
        debugger("read from unknown io address!");
        DEBUG_OUT<<"io read at "<<address<<"\n";
      }
      break;
  }
  return val;
}


BYTE NMacIO::write8_scc(ULONG address, BYTE value)
{
  unsigned int a=address&0xffff;
  BYTE ret=value;
  scc->write8(address,value);
  return ret;
}


BYTE NMacIO::write8_scsi(ULONG address, BYTE value)
{
  unsigned int a=address&0xffff;
  BYTE ret=value;
  DEBUG_OUT<<"SCSI write of "<<(short)value<<" at "<<address<<"\n";
  debugger("write to unknown scsi address!");
  return ret;
}


BYTE NMacIO::write8_iwm(ULONG address, BYTE value)
{
  unsigned int a=address&0xffff;
  BYTE ret=value;
  iwm->write8(address,value);
  return ret;
}


BYTE NMacIO::write8_via(ULONG address, BYTE value)
{
  unsigned int a=address&0xff;
  switch(a) {
    default:
      // TODO: How can I tell if an address is meant for via1 vs via2?
      via1->write8(a,value);
      break;
  }
  return value;
}


BYTE NMacIO::write8_io24(ULONG address, BYTE value)
{
  BYTE ret=value;
  unsigned int a=address&0xffff;
  DEBUG_OUT<<"IO24 write of "<<(short)value<<" at "<<address<<"\n";
  debugger("write to unknown io24 address!");
  return ret;
}


BYTE NMacIO::write8_io32(ULONG address, BYTE value)

{
  BYTE ret=value;
  unsigned int a=address&0xffff;
  DEBUG_OUT<<"IO32 write of "<<(short)value<<" at "<<address<<"\n";
  debugger("write to unknown io32 address!");
  return ret;
}


BYTE NMacIO::write8_slot24(ULONG address,unsigned int num,BYTE value)
{
  BYTE ret=value;
  unsigned int a=address&0xffff;
  DEBUG_OUT<<"Slot24 #"<<num<<" write of "<<(short)value<<" at "<<address<<"\n";
  debugger("write to unknown slot24 address!");
  return ret;
}


BYTE NMacIO::write8_slot32(ULONG address,unsigned int num,BYTE value)
{
  BYTE ret=value;
  unsigned int a=address&0xffff;
  DEBUG_OUT<<"Slot32 #"<<num<<" write of "<<(short)value<<" at "<<address<<"\n";
  debugger("write to unknown slot32 address!");
  return ret;
}


BYTE NMacIO::read8_slot32(ULONG address,unsigned int num)
{
  BYTE ret=0;
  unsigned int a=address&0xffff;
  DEBUG_OUT<<"Slot32 #"<<num<<" read at "<<address<<"\n";
  debugger("read from unknown slot32 address!");
  return ret;
}


BYTE NMacIO::read8_slot24(ULONG address,unsigned int num)
{
  BYTE ret=0;
  unsigned int a=address&0xffff;
  DEBUG_OUT<<"Slot24 #"<<num<<" read at "<<address<<"\n";
  debugger("read from unknown slot24 address!");
  return ret;
}


BYTE NMacIO::read8_io32(ULONG address)
{
  BYTE ret=0;
  unsigned int a=address&0xffff;
  DEBUG_OUT<<"IO32 read at "<<address<<"\n";
  debugger("read from unknown io32 address!");
  return ret;
}


BYTE NMacIO::read8_io24(ULONG address)
{
  BYTE ret=0;
  unsigned int a=address&0xffff;
  DEBUG_OUT<<"IO24 read at "<<address<<"\n";
  debugger("read from unknown io24 address!");
  return ret;
}


BYTE NMacIO::read8_via(ULONG address)
{
  unsigned int a=address&0xff;
  BYTE ret=0;
  switch(a) {
    default:
      // TODO: How can I tell if an address is meant for via1 vs via2?
      ret=via1->read8(address);
      break;
  }
  return ret;
}


BYTE NMacIO::read8_iwm(ULONG address)
{
  unsigned int a=address&0xffff;
  BYTE ret=iwm->read8(address);
  return ret;
}


BYTE NMacIO::read8_scsi(ULONG address)
{
  unsigned int a=address&0xffff;
  BYTE ret=0;
  DEBUG_OUT<<"SCSI read at "<<address<<"\n";
  debugger("read from unknown scsi address!");
  return ret;
}


BYTE NMacIO::read8_scc(ULONG address)
{
  unsigned int a=address&0xffff;
  BYTE ret=scc->read8(address);
  return ret;
}


BYTE NMacIO::remapRom(ULONG address)
{
  ERom *romm=(ERom *)handler->getMap()->findArea("mac.rom");
  if(romm) {
    if(romm->getLocation()==romAddr) return romm->read8(address);
  }
  DEBUG_OUT<<"remapRom...\n";
  ERam *ramm=(ERam *)handler->getMap()->findArea("ram_area");
  if(ramm) {
    handler->getMap()->unmap(ramm);
    ramm->setLocation(0);  // assume ram goes at 0
  }
  else DEBUG_OUT<<"Couldn't find ram.\n";
  if(romm) {
    handler->getMap()->unmap(romm);
    romm->setLocation(romAddr);
  }
  else DEBUG_OUT<<"Couldn't find rom.\n";
  handler->getMap()->map(this);
  if(ramm) handler->getMap()->map(ramm);
  if(romm) handler->getMap()->map(romm);
  return romm->read8(address);
}
