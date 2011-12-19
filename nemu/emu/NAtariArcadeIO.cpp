
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#include <emu/NAtariArcadeHandler.h>
#include <emu/NAtariArcadeIO.h>
#include <emu/NAtariArcadeVideo.h>

#include <elib/ESoundIOPokey.h>
#include <elib/EKeys.h>
#include <elib/EMemMapAbstract.h>

#include <portable/nickcpp/AStringHelper.h>

#include <portable/nickcpp/ANullStream.h>
//#include <guilib/APrivateBitmap.h>

#include <portable/nickcpp/APalette.h>

#include <cpulib/Em6502.h>


////////////////////////////////////////////////////////////////////////////////////
//  NAtariArcadeIOLow Class
////////////////////////////////////////////////////////////////////////////////////

/*
NAtariArcadeIOLow::NAtariArcadeIOLow(NAtariArcadeHandler *parent) : EMemIO(parent)
{
  location=0x0400;
  resize(0x0c00);
  reset();
}


NAtariArcadeIOLow::~NAtariArcadeIOLow()
{
}


void NAtariArcadeIOLow::reset()
{
  // NOTE: Pokeys handled by NAtariArcadeIO class
}


void NAtariArcadeIOLow::ioregs()
{
  // NOTE: Pokeys handled by NAtariArcadeIO class
}


void NAtariArcadeIOLow::io_cycle()
{
  // NOTE: Pokeys handled by NAtariArcadeIO class
}


BYTE NAtariArcadeIOLow::write8(ULONG addr, BYTE value)
{
  NAtariArcadeIO *a=(NAtariArcadeIO *)io->getIO();
  BYTE val=value;
  if(io->io_debug) DEBUG_OUT<<"io write of "<<(short)value<<" at "<<addr<<"\n";
  switch(addr) {
    case 0x0400:  case 0x0401:  case 0x0402:  case 0x0403:
    case 0x0404:  case 0x0405:  case 0x0406:  case 0x0407:
    case 0x0408:  case 0x0409:  case 0x040a:  case 0x040b:
    case 0x040c:  case 0x040d:  case 0x040e:  case 0x040f:
      value=a->pokey1->write8(addr,value);
      break;
    case 0x0800:  case 0x0801:  case 0x0802:  case 0x0803:
    case 0x0804:  case 0x0805:  case 0x0806:  case 0x0807:
    case 0x0808:  case 0x0809:  case 0x080a:  case 0x080b:
    case 0x080c:  case 0x080d:  case 0x080e:  case 0x080f:
      value=a->pokey2->write8(addr,value);
      break;
    default:
      DEBUG_OUT<<"io write of "<<(short)value<<" at "<<addr<<"\n";
      break;
  }
  return value;
}


BYTE NAtariArcadeIOLow::read8(ULONG addr)
{
  if(io->io_debug) DEBUG_OUT<<"io read at "<<addr<<"\n";
  NAtariArcadeIO *a=(NAtariArcadeIO *)io->getIO();
  BYTE val=mem[addr-location];
  switch(addr) {
    case 0x0400:  case 0x0401:  case 0x0402:  case 0x0403:
    case 0x0404:  case 0x0405:  case 0x0406:  case 0x0407:
    case 0x0408:  case 0x0409:  case 0x040a:  case 0x040b:
    case 0x040c:  case 0x040d:  case 0x040e:  case 0x040f:
      val=a->pokey1->read8(addr);
      break;
    case 0x0800:  case 0x0801:  case 0x0802:  case 0x0803:
    case 0x0804:  case 0x0805:  case 0x0806:  case 0x0807:
    case 0x0808:  case 0x0809:  case 0x080a:  case 0x080b:
    case 0x080c:  case 0x080d:  case 0x080e:  case 0x080f:
      val=a->pokey2->read8(addr);
      break;
    default:
      DEBUG_OUT<<"io read at "<<addr<<"\n";
      break;
  }
  return val;
}
*/


////////////////////////////////////////////////////////////////////////////////////
//  NAtariArcadeIO Class
////////////////////////////////////////////////////////////////////////////////////

NAtariArcadeIO::NAtariArcadeIO(NAtariArcadeHandler *parent) : EMemIO(parent)
{
  DEBUG_OUT<<"Creating NAtariArcadeIO\n";
  //
  pokey1=(ESoundIOPokey *)NULL;
  pokey2=(ESoundIOPokey *)NULL;
  //
  switch(type)
  {
    case N_ATARIARCADE_IO_CENTIPEDE:
      resize(6144);
      location=0x0800;
      pokey1=new ESoundIOPokey(parent,0x1000);
      if(!pokey1) err.set();
      break;
    case N_ATARIARCADE_IO_MILIPEDE:
      resize(0x2000);
      location=0x2000;
      pokey1=new ESoundIOPokey(parent,0x1000);
      if(!pokey1) err.set();
      pokey2=new ESoundIOPokey(parent,0x1000);
      if(!pokey2) err.set();
      break;
    default:
      debugger("Unknown/unimplemented IO type!");
      err.set();
      break;
  }
  //
  vflag=true;
  //
  reset();
}


NAtariArcadeIO::~NAtariArcadeIO()
{
  if(pokey1) delete pokey1;
  pokey1=NULL;
  if(pokey2) delete pokey2;
  pokey2=NULL;
}


void NAtariArcadeIO::ioregs()
{
  if(pokey1) pokey1->ioregs();
  if(pokey2) pokey2->ioregs();
}


void NAtariArcadeIO::reset()
{
  if(pokey1) pokey1->reset();
  if(pokey2) pokey2->reset();
}


void NAtariArcadeIO::io_cycle()
{
  if(pokey1) pokey1->io_cycle();
  if(pokey2) pokey2->io_cycle();
}


BYTE NAtariArcadeIO::write8(ULONG addr, BYTE value)
{
  BYTE val=value;
  switch(type)
  {
    case N_ATARIARCADE_IO_CENTIPEDE:
      val=write8_centi(addr,value);
      break;
    case N_ATARIARCADE_IO_MILIPEDE:
      val=write8_mili(addr,value);
      break;
    default:
      debugger("Unknown/unimplemented IO write8!");
      break;
  }
  return val;
}


BYTE NAtariArcadeIO::read8(ULONG addr)
{
  switch(type)
  {
    case N_ATARIARCADE_IO_CENTIPEDE:
      return read8_mili(addr);
      break;
    case N_ATARIARCADE_IO_MILIPEDE:
      return read8_centi(addr);
      break;
    default:
      debugger("Unknown/unimplemented IO read8!");
      break;
  }
  return mem[addr-location];
}


BYTE NAtariArcadeIO::write8_centi(ULONG address,BYTE value)
{
  BYTE g;
  switch(address) {
    case 0x7c0:  case 0x7c1:  case 0x7c2:  case 0x7c3:
    case 0x7c4:  case 0x7c5:  case 0x7c6:  case 0x7c7:
    case 0x7c8:  case 0x7c9:  case 0x7ca:  case 0x7cb:
    case 0x7cc:  case 0x7cd:  case 0x7ce:  case 0x7cf:
      // Sprite gfx
      g=value&0x1e;
      g+=64;
      g=g/2;
      handler->getVideo()->getSprite(address-0x7c0)->gfx=g&0xff;
      break;
    case 0x7d0:  case 0x7d1:  case 0x7d2:  case 0x7d3:
    case 0x7d4:  case 0x7d5:  case 0x7d6:  case 0x7d7:
    case 0x7d8:  case 0x7d9:  case 0x7da:  case 0x7db:
    case 0x7dc:  case 0x7dd:  case 0x7de:  case 0x7df:
      // Sprite y
      handler->getVideo()->getSprite(address-0x7d0)->y=256-value;
      break;
    case 0x7e0:  case 0x7e1:  case 0x7e2:  case 0x7e3:
    case 0x7e4:  case 0x7e5:  case 0x7e6:  case 0x7e7:
    case 0x7e8:  case 0x7e9:  case 0x7ea:  case 0x7eb:
    case 0x7ec:  case 0x7ed:  case 0x7ee:  case 0x7ef:
      // Sprite x
      handler->getVideo()->getSprite(address-0x7e0)->x=256-value;
      break;
    case 0x7f0:  case 0x7f1:  case 0x7f2:  case 0x7f3:
    case 0x7f4:  case 0x7f5:  case 0x7f6:  case 0x7f7:
    case 0x7f8:  case 0x7f9:  case 0x7fa:  case 0x7fb:
    case 0x7fc:  case 0x7fd:  case 0x7fe:  case 0x7ff:
      // Sprite color
      // TODO: Color must be massaged, of course...
      handler->getVideo()->getSprite(address-0x7f0)->color=value;
      break;
    case 0x1000:  case 0x1001:  case 0x1002:  case 0x1003:
    case 0x1004:  case 0x1005:  case 0x1006:  case 0x1007:
    case 0x1008:  case 0x1009:  case 0x100a:  case 0x100b:
    case 0x100c:  case 0x100d:  case 0x100e:  case 0x100f:
      pokey1->write8(address,value);
      break;
    case 0x1800:  // IRQ ack
      break;
    case 0x1c00:  // Left coin counter
      break;
    case 0x1c01:  // Center coin counter
      break;
    case 0x1c02:  // Right coin counter
      break;
    case 0x1c03:  // Player 1 start LED
      break;
    case 0x1c04:  // Player 2 start LED
      break;
    case 0x1c07:  // Trackball flip
      break;
    case 0x2000:  // Clear watchdog
      break;
    default:
      if((address>0x13ff)&&(address<0x1600)) {
        // write colors...
        return value;
      }
      if((address>0x15ff)&&(address<0x1800)) {
        // write EA ROM...
        return value;
      }
      DEBUG_OUT<<"io write of "<<(short)value<<" at "<<address<<"\n";
      break;
  }
  return value;
}


BYTE NAtariArcadeIO::write8_mili(ULONG address,BYTE value)
{
  switch(address) {
    case 0x2480:  case 0x2481:  case 0x2482:  case 0x2483:
    case 0x2484:  case 0x2485:  case 0x2486:  case 0x2487:
    case 0x2488:  case 0x2489:  case 0x248a:  case 0x248b:
    case 0x248c:  case 0x248d:  case 0x248e:  case 0x248f:
      // Color registers
      break;
    case 0x2680:  // Clear Watchdog
      break;
    default:
      DEBUG_OUT<<"io write of "<<(short)value<<" at "<<address<<"\n";
      break;
  }
  return value;
}


BYTE NAtariArcadeIO::read8_mili(ULONG address)
{
  BYTE val=mem[address-location];
  switch(address) {
    case 0x2000:
      val=0;
      break;
    case 0x2001:
      val=0;
      break;
    case 0x2010:
      val=0;
      break;
    case 0x2011:
      val=0x80;
      break;
    default:
      DEBUG_OUT<<"io read at "<<address<<"\n";
      break;
  }
  return val;
}


BYTE NAtariArcadeIO::read8_centi(ULONG address)
{
  BYTE val=mem[address-location];
  switch(address) {
    case 0x0800:  // Options #1
      val=0x54;  // Atari's suggested settings
      break;
    case 0x0801:  // Options #2
      val=0x00;  // Atari's suggested settings/free play
      break;
    case 0x0c00:  // Track horiz, vblank, self-test, cocktail
      val=0xff;
      val&=0xef;  // Not cocktail
      //if(io->getKeypress()==E_KEY_TEST) val&=0xdf;
      val&=0xbf;  // Not vblank
      if(handler->getVideo()->inVBlank()) {
        val|=0x40;
        //DEBUG_OUT<<"&";
      }
      break;
    case 0x0c01:  // Coins, slam, fire and start buttons
      val=0xff;
      /*
      if(io->getKeypress()==E_KEY_SLAM) val&=0xef;
      if(io->getKeypress()==E_KEY_COIN) val&=0x1f;
      if(io->getKeypress()==E_KEY_START2) val&=0xfd;
      if(io->getKeypress()==E_KEY_START1) val&=0xfe;
      */
      break;
    case 0xc02:  // Trackball vert
      val=0xff;
      break;
    case 0xc03:  // Joystick input
      val=0xff;
      break;
    case 0x1000:  case 0x1001:  case 0x1002:  case 0x1003:
    case 0x1004:  case 0x1005:  case 0x1006:  case 0x1007:
    case 0x1008:  case 0x1009:  case 0x100a:  case 0x100b:
    case 0x100c:  case 0x100d:  case 0x100e:  case 0x100f:
      val=pokey1->read8(address);
      break;
    default:
      if((address>0x15ff)&&(address<0x1800)) {
        // read EA ROM...
        return val;
      }
      DEBUG_OUT<<"io read at "<<address<<"\n";
      break;
  }
  return val;
}




