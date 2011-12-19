
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#include <emu/NFamicomHandler.h>
#include <emu/NFamicomIO.h>

#include <portable/nickcpp/ANullStream.h>

#include <portable/nickcpp/AStringHelper.h>

#include <portable/nickcpp/APalette.h>

#include <cpulib/Em6502.h>

#include <elib/EMemMapAbstract.h>
#include <elib/EModule.h>
#include <elib/ERom.h>
#include <elib/ERam.h>


////////////////////////////////////////////////////////////////////////////////
//  NFamicomIO Class
////////////////////////////////////////////////////////////////////////////////

NFamicomIO::NFamicomIO(NFamicomHandler *parent) : EMemIO(parent)
{
  switch(type) {
    case N_FAMICOM_IO_FAMICOM:
      location=0x2000;
      resize(0x3000);
      break;
    default:
      DEBUG_OUT<<"Unknown Famicom hardware type!\n";
      break;
  }
  reset();
}


NFamicomIO::~NFamicomIO()
{
}


void NFamicomIO::reset()
{
  DEBUG_OUT<<"NFamicomIO::reset().\n";
}


void NFamicomIO::ioregs()
{
}


void NFamicomIO::io_cycle()
{
}


BYTE NFamicomIO::write8(ULONG address,BYTE value)
{
  ERom *foo=(ERom *)NULL;
  unsigned int bpage=(address>>12)&0xf;
  unsigned int ad=address&0xf;
  unsigned int ad2=address&0xff;
  switch(bpage) {
    case 0x2:
      switch(ad) {
        case 0x00:
          DEBUG_OUT<<"write of ppu control reg #1!\n";
          break;
        case 0x01:
          DEBUG_OUT<<"write of ppu control reg #2!\n";
          break;
        case 0x02:
          DEBUG_OUT<<"write of ppu status reg!\n";
          break;
        case 0x03:
          DEBUG_OUT<<"write of spr-ram addr reg!\n";
          break;
        case 0x04:
          DEBUG_OUT<<"write of spr-ram io reg!\n";
          break;
        case 0x05:
          DEBUG_OUT<<"write of vram addr reg #1!\n";
          break;
        case 0x06:
          DEBUG_OUT<<"write of ppu vram addr reg #2!\n";
          break;
        case 0x07:
          DEBUG_OUT<<"write of ppu vram io reg!\n";
          break;
        default:
          debugger("write to unknown $2xxx io address!");
          DEBUG_OUT<<"io write of "<<(short)value<<" at "<<address<<"\n";
          break;
      }
      break;
    case 0x4:
      switch(ad2) {
        case 0x00:
          DEBUG_OUT<<"write of pAPU pulse #1 control reg!\n";
          break;
        case 0x01:
          DEBUG_OUT<<"write of pAPU pulse #1 ramp control reg!\n";
          break;
        case 0x02:
          DEBUG_OUT<<"write of pAPU pulse #1 ft control reg!\n";
          break;
        case 0x03:
          DEBUG_OUT<<"write of pAPU pulse #1 ct control reg!\n";
          break;
        case 0x04:
          DEBUG_OUT<<"write of pAPU pulse #2 control reg!\n";
          break;
        case 0x05:
          DEBUG_OUT<<"write of pAPU pulse #2 ramp control reg!\n";
          break;
        case 0x06:
          DEBUG_OUT<<"write of pAPU pulse #2 ft control reg!\n";
          break;
        case 0x07:
          DEBUG_OUT<<"write of pAPU pulse #2 ct control reg!\n";
          break;
        case 0x08:
          DEBUG_OUT<<"write of pAPU triangle control reg #1!\n";
          break;
        case 0x09:
          DEBUG_OUT<<"write of pAPU triangle control reg #2!\n";
          break;
        case 0x0a:
          DEBUG_OUT<<"write of pAPU triangle freq reg #1!\n";
          break;
        case 0x0b:
          DEBUG_OUT<<"write of pAPU triangle freq reg #2!\n";
          break;
        case 0x0c:
          DEBUG_OUT<<"write of pAPU noise control reg!\n";
          break;
        case 0x0d:
          DEBUG_OUT<<"write of unknown reg!\n";
          break;
        case 0x0e:
          DEBUG_OUT<<"write of pAPU noise freq reg #1!\n";
          break;
        case 0x0f:
          DEBUG_OUT<<"write of pAPU noise freq reg #2!\n";
          break;
        case 0x10:
          DEBUG_OUT<<"write of pAPU delta mod control reg!\n";
          break;
        case 0x11:
          DEBUG_OUT<<"write of pAPU delta mod da reg!\n";
          break;
        case 0x12:
          DEBUG_OUT<<"write of pAPU delta mod addr reg!\n";
          break;
        case 0x13:
          DEBUG_OUT<<"write of pAPU delta mod data len reg!\n";
          break;
        case 0x14:
          DEBUG_OUT<<"write of sprite dma reg!\n";
          break;
        case 0x15:
          DEBUG_OUT<<"write of pAPU sound/vclock reg!\n";
          break;
        case 0x16:
          DEBUG_OUT<<"write of joypad #1 reg!\n";
          break;
        case 0x17:
          DEBUG_OUT<<"write of joypad #2 reg!\n";
          break;
        default:
          debugger("write to unknown $4xxx io address!");
          DEBUG_OUT<<"io write of "<<(short)value<<" at "<<address<<"\n";
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


BYTE NFamicomIO::read8(ULONG address)
{
  BYTE val=mem[(address-location)&(actualSize-1)];
  unsigned int bpage=(address>>12)&0xf;
  unsigned int ad=address&0xf;
  unsigned int ad2=address&0xff;
  switch(bpage) {
    case 0x2:
      switch(ad) {
        case 0x00:
          DEBUG_OUT<<"read of ppu control reg #1!\n";
          break;
        case 0x01:
          DEBUG_OUT<<"read of ppu control reg #2!\n";
          break;
        case 0x02:
          DEBUG_OUT<<"read of ppu status reg!\n";
          break;
        case 0x03:
          DEBUG_OUT<<"read of spr-ram addr reg!\n";
          break;
        case 0x04:
          DEBUG_OUT<<"read of spr-ram io reg!\n";
          break;
        case 0x05:
          DEBUG_OUT<<"read of vram addr reg #1!\n";
          break;
        case 0x06:
          DEBUG_OUT<<"read of ppu vram addr reg #2!\n";
          break;
        case 0x07:
          DEBUG_OUT<<"read of ppu vram io reg!\n";
          break;
        default:
          debugger("read to unknown $2xxx io address!");
          DEBUG_OUT<<"io read of at "<<address<<"\n";
          break;
      }
      break;
    case 0x4:
      switch(ad2) {
        case 0x00:
          DEBUG_OUT<<"read of pAPU pulse #1 control reg!\n";
          break;
        case 0x01:
          DEBUG_OUT<<"read of pAPU pulse #1 ramp control reg!\n";
          break;
        case 0x02:
          DEBUG_OUT<<"read of pAPU pulse #1 ft control reg!\n";
          break;
        case 0x03:
          DEBUG_OUT<<"read of pAPU pulse #1 ct control reg!\n";
          break;
        case 0x04:
          DEBUG_OUT<<"read of pAPU pulse #2 control reg!\n";
          break;
        case 0x05:
          DEBUG_OUT<<"read of pAPU pulse #2 ramp control reg!\n";
          break;
        case 0x06:
          DEBUG_OUT<<"read of pAPU pulse #2 ft control reg!\n";
          break;
        case 0x07:
          DEBUG_OUT<<"read of pAPU pulse #2 ct control reg!\n";
          break;
        case 0x08:
          DEBUG_OUT<<"read of pAPU triangle control reg #1!\n";
          break;
        case 0x09:
          DEBUG_OUT<<"read of pAPU triangle control reg #2!\n";
          break;
        case 0x0a:
          DEBUG_OUT<<"read of pAPU triangle freq reg #1!\n";
          break;
        case 0x0b:
          DEBUG_OUT<<"read of pAPU triangle freq reg #2!\n";
          break;
        case 0x0c:
          DEBUG_OUT<<"read of pAPU noise control reg!\n";
          break;
        case 0x0d:
          DEBUG_OUT<<"read of unknown reg!\n";
          break;
        case 0x0e:
          DEBUG_OUT<<"read of pAPU noise freq reg #1!\n";
          break;
        case 0x0f:
          DEBUG_OUT<<"read of pAPU noise freq reg #2!\n";
          break;
        case 0x10:
          DEBUG_OUT<<"read of pAPU delta mod control reg!\n";
          break;
        case 0x11:
          DEBUG_OUT<<"read of pAPU delta mod da reg!\n";
          break;
        case 0x12:
          DEBUG_OUT<<"read of pAPU delta mod addr reg!\n";
          break;
        case 0x13:
          DEBUG_OUT<<"read of pAPU delta mod data len reg!\n";
          break;
        case 0x14:
          DEBUG_OUT<<"read of sprite dma reg!\n";
          break;
        case 0x15:
          DEBUG_OUT<<"read of pAPU sound/vclock reg!\n";
          break;
        case 0x16:
          DEBUG_OUT<<"read of joypad #1 reg!\n";
          break;
        case 0x17:
          DEBUG_OUT<<"read of joypad #2 reg!\n";
          break;
        default:
          debugger("read to unknown $4xxx io address!");
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


