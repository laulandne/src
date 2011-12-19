
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#include <emu/NGenesisHandler.h>
#include <emu/NGenesisIO.h>

#include <portable/nickcpp/ANullStream.h>

#include <portable/nickcpp/AStringHelper.h>

#include <portable/nickcpp/APalette.h>

#include <cpulib/Em68k.h>
#include <cpulib/Ez80.h>

#include <elib/EMemMapAbstract.h>
#include <elib/EModule.h>
#include <elib/ERom.h>
#include <elib/ERam.h>
#include <elib/EVideoIOTms99xx.h>


////////////////////////////////////////////////////////////////////////////////
//  NGenesisIO Class
////////////////////////////////////////////////////////////////////////////////

NGenesisIO::NGenesisIO(NGenesisHandler *parent) : EMemIO(parent)
{
  switch(type) {
    case N_GENESIS_IO_GENESIS:
      location=0xa10000;
      resize(0x1f);
      break;
    default:
      DEBUG_OUT<<"Unknown Genesis hardware type!\n";
      break;
  }
  reset();
}


NGenesisIO::~NGenesisIO()
{
}


void NGenesisIO::reset()
{
  DEBUG_OUT<<"NGenesisIO::reset().\n";
  segaLatch=0;
}


void NGenesisIO::ioregs()
{
}


void NGenesisIO::io_cycle()
{
}


BYTE NGenesisIO::write8(ULONG address,BYTE value)
{
  if(!actualSize) exit(5);
  ERom *foo=(ERom *)NULL;
  ULONG ad=((ULONG)address&0xfe)+1;
  ULONG ad2=(ULONG)address&0xfff;
  unsigned int page=(address>>16)&0xff;
  unsigned int bpage=(address>>12)&0xfff;
  switch(bpage) {
    case 0xa00:
      switch(ad2) {
        default:
          debugger("write to unknown $a00 io address!");
          DEBUG_OUT<<"io write at "<<address<<"\n";
          break;
      }
      break;
    case 0xa10:
      switch(ad) {
        case 0x001:
          debugger("write to io version reg!");
          break;
        case 0x002: case 0x003: case 0x004: case 0x005: case 0x006: case 0x007:
          debugger("write to io data ctrl1/ctrl2/exp!");
          break;
        case 0x008: case 0x009: case 0x00a: case 0x00b: case 0x00c: case 0x00d:
          debugger("write to io control 1/2/e!");
          break;
        case 0x00e: case 0x00f: case 0x010: case 0x011: case 0x012: case 0x013:
          debugger("write to io txdata/rxdata/smode #1!");
          break;
        case 0x014: case 0x015: case 0x016: case 0x017: case 0x018: case 0x019:
          debugger("write to io txdata/rxdata/smode #2!");
          break;
        case 0x01a: case 0x01b: case 0x01c: case 0x01d: case 0x01e: case 0x01f:
          debugger("write to io txdata/rxdata/smode #e!");
          break;
        default:
          debugger("write to unknown $a10 io address!");
          DEBUG_OUT<<"io write at "<<address<<"\n";
          break;
      }
      break;
    case 0xa11:
      switch(ad2) {
        case 0x000: case 0x001:
          debugger("write to ctrl memory mode!");
          break;
        case 0x100: case 0x101:
          debugger("write to ctrl z80 busreq!");
          break;
        case 0x200: case 0x201:
          debugger("write to ctrl z80 reset!");
          break;
        default:
          debugger("write to unknown $a11 io address!");
          DEBUG_OUT<<"io write at "<<address<<"\n";
          break;
      }
      break;
    case 0xa14:
      switch(ad2) {
        case 0x000:
          if(value==0x53) segaLatch++;
          break;
        case 0x001:
          if(value==0x45) segaLatch++;
          if(value==1) {
            DEBUG_OUT<<"Game is requesting that os rom be disabled...\n";
          }
          break;
        case 0x002:
          if(value==0x47) segaLatch++;
          break;
        case 0x003:
          if(value==0x41) segaLatch++;
          if(segaLatch==4) {
            DEBUG_OUT<<"Game successfully wrote 'sega' to latch...\n";
          }
          break;
        default:
          debugger("write to unknown $a14 io address!");
          DEBUG_OUT<<"io write at "<<address<<"\n";
          break;
      }
      break;
    case 0xc00:
      switch(ad2) {
        case 0x000: case 0x001: case 0x002: case 0x003:
          debugger("write to vdp data!");
          break;
        case 0x004: case 0x005: case 0x006: case 0x007:
          debugger("write to vdp control!");
          break;
        case 0x008: case 0x009:
          debugger("write to vdp hv counter!");
          break;
        case 0x010: case 0x011:
          debugger("write to vdp psg 76489!");
          break;
        default:
          debugger("write to unknown $c00 io address!");
          DEBUG_OUT<<"io write at "<<address<<"\n";
          break;
      }
      break;
    default:
      debugger("write to unknown io address!");
      DEBUG_OUT<<"io write of "<<(short)value<<" at "<<address<<"\n";
      break;
  }
  return value;
}


BYTE NGenesisIO::read8(ULONG address)
{
  if(!actualSize) exit(5);
  BYTE val=mem[(address-location)&(actualSize-1)];
  ULONG ad=((ULONG)address&0xfe)+1;
  ULONG ad2=(ULONG)address&0xfff;
  unsigned int page=(address>>16)&0xff;
  unsigned int bpage=(address>>12)&0xfff;
  switch(bpage) {
    case 0xa00:
      switch(ad2) {
        default:
          debugger("read from unknown $a00 io address!");
          DEBUG_OUT<<"io read at "<<address<<"\n";
          break;
      }
      break;
    case 0xa10:
      switch(ad) {
        case 0x001:
          // version register
          val=0xff;
          DEBUG_OUT<<"read of version reg...\n";
          break;
        case 0x002: case 0x003: case 0x004: case 0x005: case 0x006: case 0x007:
          debugger("read from io data ctrl1/ctrl2/exp!");
          break;
        case 0x008: case 0x009: case 0x00a: case 0x00b: case 0x00c: case 0x00d:
          debugger("read from io control 1/2/e!");
          break;
        case 0x00e: case 0x00f: case 0x010: case 0x011: case 0x012: case 0x013:
          debugger("read from io txdata/rxdata/smode #1!");
          break;
        case 0x014: case 0x015: case 0x016: case 0x017: case 0x018: case 0x019:
          debugger("read from io txdata/rxdata/smode #2!");
          break;
        case 0x01a: case 0x01b: case 0x01c: case 0x01d: case 0x01e: case 0x01f:
          debugger("read from io txdata/rxdata/smode #e!");
          break;
        default:
          debugger("read from unknown $a10 io address!");
          DEBUG_OUT<<"io read at "<<address<<"\n";
          break;
      }
      break;
    case 0xa11:
      switch(ad2) {
        case 0x000: case 0x001:
          debugger("read from ctrl memory mode!");
          break;
        case 0x100: case 0x101:
          debugger("read from ctrl z80 busreq!");
          break;
        case 0x200: case 0x201:
          debugger("read from ctrl z80 reset!");
          break;
        default:
          debugger("read from unknown $a11 io address!");
          DEBUG_OUT<<"io read at "<<address<<"\n";
          break;
      }
      break;
    case 0xa14:
      switch(ad2) {
        default:
          debugger("read from unknown $a14 io address!");
          DEBUG_OUT<<"io read at "<<address<<"\n";
          break;
      }
      break;
    case 0xc00:
      switch(ad2) {
        case 0x000: case 0x001: case 0x002: case 0x003:
          debugger("read from vdp data!");
          break;
        case 0x004: case 0x005: case 0x006: case 0x700:
          debugger("read from vdp control!");
          break;
        case 0x008: case 0x009:
          debugger("read from vdp hv counter!");
          break;
        case 0x010: case 0x011:
          debugger("read from vdp psg 76489!");
          break;
        default:
          debugger("read from unknown $c00 io address!");
          DEBUG_OUT<<"io read at "<<address<<"\n";
          break;
      }
      break;
    default:
      debugger("read from unknown io address!");
      DEBUG_OUT<<"io read at "<<address<<"\n";
      break;
  }
  return val;
}


