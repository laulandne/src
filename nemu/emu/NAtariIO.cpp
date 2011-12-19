
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#include <emu/NAtariHandler.h>
#include <emu/NAtariIO.h>
#include <emu/NAtariVideo.h>
#include <portable/nickcpp/ANullStream.h>

#include <portable/nickcpp/AStringHelper.h>

#include <portable/nickcpp/APalette.h>

#include <elib/EMemMapAbstract.h>
#include <elib/ESoundIOPokey.h>
#include <elib/EModule.h>
#include <elib/ERom.h>
#include <elib/ERam.h>
#include <elib/EIOPia6520.h>

#include <cpulib/Em6502.h>


////////////////////////////////////////////////////////////////////////////////
//  NAtariIO Class
////////////////////////////////////////////////////////////////////////////////

NAtariIO::NAtariIO(NAtariHandler *parent) : EMemIO(parent)
{
  DEBUG_OUT<<"Creating NAtariIO\n";
  unsigned int pokeyaddr=0xd200;
  if(type==N_ATARI_IO_ATARI5200) {
    location=0xc000;
    resize(0x3000);
    pokeyaddr=0xe800;
  }
  else {
    location=0xd000;
    resize(0x800);
  }
  writeOnly=0x0f;
  DLI_NMI=false;  VBI_NMI=false;
  pia=new EIOPia6520(parent,0);
  if(!pia) err.set();
  pokey=new ESoundIOPokey(parent,pokeyaddr);  // at e800 in 5200, d200 in 400/800
  if(!pokey) err.set();
  reset();
}


NAtariIO::~NAtariIO()
{
  if(pia) delete pia;
  pia=(EIOPia6520 *)NULL;
  if(pokey) delete pokey;
  pokey=(ESoundIOPokey *)NULL;
}


void NAtariIO::reset()
{
  if(pia) pia->reset();
  if(pokey) pokey->reset();
}


void NAtariIO::ioregs()
{
  DEBUG_OUT<<"DLI_NMI="<<DLI_NMI<<"  VBI_NMI="<<VBI_NMI<<"\n";
  DEBUG_OUT<<"Display list is at "<<handler->getMap()->read16l(0xd402)<<"\n";
  if(pia) pia->ioregs();
  if(pokey) pokey->ioregs();
}


void NAtariIO::io_cycle()
{
  if(pia) pia->io_cycle();
  if(pokey) pokey->io_cycle();
}


BYTE NAtariIO::write8(ULONG addr, BYTE value)
{
  BYTE page=(addr>>8)&0xff;
  //BYTE nib2=addr&0x00f0;
  BYTE val=value;
  if(handler->io_debug) DEBUG_OUT<<"io write of "<<(short)value<<" at "<<addr<<"\n";
  switch(page) {
    case 0xc0:
      if(type==N_ATARI_IO_ATARI5200) val=write8_gtia(addr&0xff,value);
      break;
    case 0xd0:
      if(type!=N_ATARI_IO_ATARI5200) val=write8_gtia(addr&0xff,value);
      break;
    case 0xd2:
      if(type!=N_ATARI_IO_ATARI5200) val=pokey->write8(addr,value);
      break;
    case 0xd3:
      if(type!=N_ATARI_IO_ATARI5200) val=write8_pia(addr,value);
      break;
    case 0xd4:
      val=write8_antic(addr,value);  break;
    case 0xe8:
      if(type==N_ATARI_IO_ATARI5200) val=pokey->write8(addr,value);
      break;
    default:
      break;
  }
  return val;
}


BYTE NAtariIO::read8(ULONG addr)
{
  BYTE page=(addr>>8)&0xff;
  //BYTE nib2=ad&0x00f0;
  if(handler->io_debug) DEBUG_OUT<<"io read at "<<addr<<"\n";
  switch(page) {
    case 0xc0:
      if(type==N_ATARI_IO_ATARI5200) return read8_gtia(addr&0xff);
      break;
    case 0xd0:
      if(type!=N_ATARI_IO_ATARI5200) return read8_gtia(addr&0xff);
      break;
    case 0xd2:
      if(type!=N_ATARI_IO_ATARI5200) return pokey->read8(addr);
      break;
    case 0xd3:
      if(type!=N_ATARI_IO_ATARI5200) return read8_pia(addr);
      break;
    case 0xd4:
      return read8_antic(addr);  break;
    case 0xe8:
      if(type==N_ATARI_IO_ATARI5200) return pokey->read8(addr);
      break;
    default:
      break;
  }
  return mem[addr-location];
}


BYTE NAtariIO::strobe(ULONG addr,BYTE val)
{
  if(handler->io_debug) DEBUG_OUT<<"io strobe at "<<addr<<"\n";
  switch(addr) {
    default:
      break;
  }
  return val;
}


BYTE NAtariIO::write8_antic(ULONG address, BYTE value)
{
  // NOTE: Antic is ALWAYS located at 0xd400
  // TODO: mask register #
  switch(address) {
    case 0xd40e:  // NMIEN
      if(value&0x80) DLI_NMI=true; else DLI_NMI=false;
      if(value&0x40) VBI_NMI=true; else VBI_NMI=false;
      if(VBI_NMI) handler->getVideo()->setRasterInt(128,E6502_INT_NMI);
      else handler->getVideo()->unsetRasterInt();
      break;
    case 0xd40f:  // NMIRES
      value=0;
      break;
    default:
      mem[address-location]=value;
      break;
  }
  return value;
}


BYTE NAtariIO::write8_gtia(ULONG address, BYTE value)
{
  // NOTE: GTIA may be located at different addresses...
  // so address has been &0xff
  // TODO: mask register #
  switch(address) {
    case 0x00: case 0x01: case 0x02: case 0x03:
    case 0x04: case 0x05: case 0x06: case 0x07:
    case 0x08: case 0x09: case 0x0a: case 0x0b:
    case 0x0c: case 0x0d: case 0x0e: case 0x0f:
    case 0x10: case 0x11: case 0x12: case 0x13:
    case 0x14: case 0x15: case 0x16: case 0x17:
    case 0x18: case 0x19: case 0x1a: case 0x1b:
    case 0x1c: case 0x1d: case 0x1e:
      break;
    case 0x1f:  // CONSOL
      break;
    default:
      mem[address]=value;
      break;
  }
  return value;
}


BYTE NAtariIO::write8_pia(ULONG address, BYTE value)
{
  // TODO: mask register #
  switch(address) {
    default:
      mem[address-location]=value;
      break;
  }
  return value;
}


BYTE NAtariIO::read8_pia(ULONG address)
{
  // TODO: mask register #
  BYTE value=mem[address-location];
  switch(address) {
    default:
      //DEBUG_OUT<<"read at "<<address<<" of "<<value<<"\n";
      break;
  }
  return value;
}


BYTE NAtariIO::read8_gtia(ULONG address)
{
  // NOTE: GTIA may be located at different addresses...
  // so address has been &0xff
  BYTE value=mem[address];
  // TODO: mask register #
  switch(address) {
    case 0x00: case 0x01: case 0x02: case 0x03:
    case 0x04: case 0x05: case 0x06: case 0x07:
    case 0x08: case 0x09: case 0x0a: case 0x0b:
    case 0x0c: case 0x0d: case 0x0e: case 0x0f:
    case 0x10: case 0x11: case 0x12: case 0x13:
      value=0x00;
      break;
    case 0x14:  // PAL
      value=0x0e;  // We are NTSC
      break;
    case 0x15: case 0x16: case 0x17:
    case 0x18: case 0x19: case 0x1a: case 0x1b:
    case 0x1c: case 0x1d: case 0x1e:
      value=0x00;
      break;
    case 0x1f:  // CONSOL
      value=0x07;  // No console keys pressed
      break;
    default:
      //DEBUG_OUT<<"read at "<<address<<" of "<<value<<"\n";
      break;
  }
  return value;
}


BYTE NAtariIO::read8_antic(ULONG address)
{
  // NOTE: Antic is ALWAYS located at 0xd400
  BYTE value=mem[address-location];
  // TODO: mask register #
  switch(address) {
    case 0xd40b:  // VCOUNT
      return handler->getVideo()->getRaster();
      break;
    case 0xd40e:  // NMIEN
      return writeOnly;
      break;
    case 0xd40f:  // NMIRES
      value=0;  // We didn't cause the NMI
      if(VBI_NMI&&handler->getVideo()->getIntCaused()) value=0x40;
      break;
    default:
      //DEBUG_OUT<<"read at "<<address<<" of "<<value<<"\n";
      break;
  }
  return value;
}
