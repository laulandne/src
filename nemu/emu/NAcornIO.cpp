
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#include <emu/NAcornHandler.h>
#include <emu/NAcornIO.h>
#include <portable/nickcpp/ANullStream.h>

#include <portable/nickcpp/AStringHelper.h>

#include <portable/nickcpp/APalette.h>

#include <elib/EMemMapAbstract.h>
#include <elib/EModule.h>
#include <elib/ERom.h>
#include <elib/ERam.h>
#include <elib/EIOPia8255.h>
#include <elib/EIOVia6522.h>
#include <elib/EIOAcia6850.h>

#include <disklib/DController827x.h>


////////////////////////////////////////////////////////////////////////////////
//  NAcornIO Class
////////////////////////////////////////////////////////////////////////////////

NAcornIO::NAcornIO(NAcornHandler *parent) : EMemIO(parent)
{
  DEBUG_OUT<<"Creating NAcornIO\n";
  //
  pia=NULL;
  viaa=NULL;
  viab=NULL;
  acia=NULL;
  diskio=NULL;
  //
  //DEBUG_OUT<<"type is "<<(int)type<<"\n";
  switch(type) {
    case N_ACORN_IO_ATOM:
      location=0xb000;
      resize(0x1000);
      // optional VIA at 0xb800
      pia=new EIOPia8255(parent,0);
      break;
    case N_ACORN_IO_BBC:
      resize(0x300);
      location=0xfc00;
      viaa=new EIOVia6522(parent,0);
      viab=new EIOVia6522(parent,0);
      acia=new EIOAcia6850(parent,0);
      diskio=new DController827x(parent,0);
      break;
    default:
      debugger("Unknown IO type!");
      err.set();
      break;
  }
  reset();
}


NAcornIO::~NAcornIO()
{
  if(pia) delete pia;
  pia=NULL;
  if(acia) delete acia;
  acia=NULL;
  if(viaa) delete viaa;
  viaa=NULL;
  if(viab) delete viab;
  viab=NULL;
  if(diskio) delete diskio;
  diskio=NULL;
}


void NAcornIO::reset()
{
  if(pia) pia->reset();
  if(acia) acia->reset();
  if(viaa) viaa->reset();
  if(viab) viab->reset();
  if(diskio) diskio->reset();
}


void NAcornIO::ioregs()
{
  if(pia) pia->ioregs();
  if(acia) acia->ioregs();
  if(viaa) viaa->ioregs();
  if(viab) viab->ioregs();
  if(diskio) diskio->ioregs();
}


void NAcornIO::io_cycle()
{
  if(pia) pia->io_cycle();
  if(acia) acia->io_cycle();
  if(viaa) viaa->io_cycle();
  if(viab) viab->io_cycle();
  if(diskio) diskio->io_cycle();
}


BYTE NAcornIO::write8(ULONG addr, BYTE value)
{
  if(type==N_ACORN_IO_BBC) return write8bbc(addr,value);
  else return write8atom(addr,value);
  return 0;
}


BYTE NAcornIO::read8(ULONG addr)
{
  if(type==N_ACORN_IO_BBC) return read8bbc(addr);
  else return read8atom(addr);
  return 0;
}


BYTE NAcornIO::write8atom(ULONG addr, BYTE value)
{
  BYTE page=(addr>>8)&0xff;
  //BYTE nib2=addr&0x00f0;
  BYTE val=value;
  switch(page) {
    default:
      DEBUG_OUT<<"io write of "<<aDefaultStringHelper.myHex2(value)<<" at "<<aDefaultStringHelper.myHex4(addr)<<"\n";
      break;
  }
  return val;
}


BYTE NAcornIO::read8atom(ULONG addr)
{
  BYTE page=(addr>>8)&0xff;
  //BYTE nib2=ad&0x00f0;
  BYTE val=mem[addr-location];
  switch(page) {
    default:
      DEBUG_OUT<<"io read at "<<aDefaultStringHelper.myHex4(addr)<<"\n";
      break;
  }
  return val;
}


BYTE NAcornIO::write8bbc(ULONG addr, BYTE value)
{
  BYTE page=(addr>>8)&0xff;
  BYTE nib2=addr&0x00f0;
  BYTE val=value;
  switch(nib2) {
    case 0x00:
      if(addr&0x8) {
        // 6845
        DEBUG_OUT<<"io write of m6845\n";
      }
      else {
        // 6850
        DEBUG_OUT<<"io write of m6850\n";
      }
      break;
    case 0x10:
      // serial ula
      DEBUG_OUT<<"io write of serial ula\n";
      break;
    case 0x20:
      // video ula
      DEBUG_OUT<<"io write of video ula\n";
      break;
    case 0x30:
      // 74ls161
      DEBUG_OUT<<"io write of 74ls161\n";
      break;
    case 0x40:  case 0x50:
      // 6522
      DEBUG_OUT<<"io write of m6522\n";
      break;
    case 0x60:  case 0x70:
      // 6522
      DEBUG_OUT<<"io write of m6522\n";
      break;
    case 0x80:  case 0x90:
      // 8271
      DEBUG_OUT<<"io write of 8271\n";
      break;
    case 0xa0:
      // 68b54
      DEBUG_OUT<<"io write of m68b54\n";
      break;
    case 0xc0:
      // uPD7002
      DEBUG_OUT<<"io write of uPD7002\n";
      break;
    case 0xe0:
      // tube ula
      DEBUG_OUT<<"io write of tube ula\n";
      break;
    default:
      DEBUG_OUT<<"io write of "<<aDefaultStringHelper.myHex2(value)<<" at "<<aDefaultStringHelper.myHex4(addr)<<"\n";
      break;
  }
  return val;
}


BYTE NAcornIO::read8bbc(ULONG addr)
{
  BYTE page=(addr>>8)&0xff;
  BYTE nib2=addr&0x00f0;
  BYTE val=mem[addr-location];
  switch(page) {
    case 0x00:
      if(addr&0x8) {
        // 6845
        DEBUG_OUT<<"io read of m6845\n";
      }
      else {
        // 6850
        DEBUG_OUT<<"io read of m6850\n";
      }
      break;
    case 0x10:
      // serial ula
      DEBUG_OUT<<"io read of serial ula\n";
      break;
    case 0x20:
      // video ula
      DEBUG_OUT<<"io read of video ula\n";
      break;
    case 0x30:
      // 74ls161
      DEBUG_OUT<<"io read of 74ls161\n";
      break;
    case 0x40:  case 0x50:
      // 6522
      DEBUG_OUT<<"io read of m6522\n";
      break;
    case 0x60:  case 0x70:
      // 6522
      DEBUG_OUT<<"io read of m6522\n";
      break;
    case 0x80:  case 0x90:
      // 8271
      DEBUG_OUT<<"io read of m8271\n";
      break;
    case 0xa0:
      // 68b54
      DEBUG_OUT<<"io read of m68b54\n";
      break;
    case 0xc0:
      // uPD7002
      DEBUG_OUT<<"io read of uPD7002\n";
      break;
    case 0xe0:
      // tube ula
      DEBUG_OUT<<"io read of tube ula\n";
      break;
    default:
      DEBUG_OUT<<"io read at "<<aDefaultStringHelper.myHex4(addr)<<"\n";
      break;
  }
  return val;
}


