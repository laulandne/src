
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#include <elib/ESoundIOPokey.h>
#include <elib/EHandler.h>

#include <portable/nickcpp/ANullStream.h>


////////////////////////////////////////////////////////////////////////////////
//  ESoundIOPokey Class
////////////////////////////////////////////////////////////////////////////////

ESoundIOPokey::ESoundIOPokey(EHandler *parent, ULONG address) : ESoundIOAbstract(parent,address)
{
  DEBUG_OUT<<"ESoundIOPokey::ESoundIOPokey()\n";
  resize(0xf);
  setLocation(address);
  reset();
}


ESoundIOPokey::~ESoundIOPokey()
{
}


void ESoundIOPokey::reset()
{
  ESoundIOAbstract::reset();
  irq1en=0;
  skctl=0;
  audf1=0;  audf2=0;  audf3=0;  audf4=0;
  key_count=0;  key_max=2;
}


void ESoundIOPokey::ioregs()
{
  DEBUG_OUT<<"ESoundIOPokey "<<location<<":\n";
  DEBUG_OUT<<"irqEN="<<irq1en<<"\n";
  DEBUG_OUT<<"skCTL="<<skctl<<"\n";
  DEBUG_OUT<<"audf1="<<audf1<<"  audf2="<<audf2<<"\n";
  DEBUG_OUT<<"audf3="<<audf3<<"  audf4="<<audf4<<"\n";
  ESoundIOAbstract::ioregs();
}


void ESoundIOPokey::io_cycle()
{
  ESoundIOAbstract::io_cycle();
}


// NOTE: POKEY may be located at different addresses...
BYTE ESoundIOPokey::write8(ULONG address, BYTE value)
{
  unsigned int ad=address&0xf;
  switch(ad) {
    case 0x0:  // AUDF1
      audf1=value;
      break;
    case 0x1:  // AUDC1
      break;
    case 0x2:  // AUDF2
      audf2=value;
      break;
    case 0x3:  // AUDC2
      break;
    case 0x4:  // AUDF3
      audf3=value;
      break;
    case 0x5:  // AUDC3
      break;
    case 0x6:  // AUDF4
      audf4=value;
      break;
    case 0x7:  // AUDC4
      break;
    case 0x8:  // AUDCTL
      break;
    case 0x9:  // STIMER
      // write any value to reset AUDx freq timers
      break;
    case 0xa:  // SKREST
      // reset bits 5-7 of serial port status at 53775 to 1
      break;
    case 0xb:  // POTGO
      break;
    case 0xc:  // unused
      break;
    case 0xd:  // SEROUT
      break;
    case 0xe:  // IRQEN
#ifdef SHOW_INTS
      DEBUG_OUT<<"{IRQ changed to "<<value<<"}\n";
#endif
      irq1en=value;
      break;
    case 0xf:  // SKCTL
      skctl=value;
      break;
    default:
      DEBUG_OUT<<"pokey write of "<<value<<" at "<<ad<<"\n";
      break;
  }
  return value;
}


// NOTE: POKEY may be located at different addresses...
BYTE ESoundIOPokey::read8(ULONG address)
{
  unsigned int ad=address&0xf;
  unsigned int ival=0;
  BYTE value=mem[ad];
  switch(ad) {
    case 0x0:  // POT0
      value=0;
      break;
    case 0x1:  // POT1
      value=0;
      break;
    case 0x2:  // POT2
      value=0;
      break;
    case 0x3:  // POT3
      value=0;
      break;
    case 0x4:  // POT4
      value=0;
      break;
    case 0x5:  // POT5
      value=0;
      break;
    case 0x6:  // POT6
      value=0;
      break;
    case 0x7:  // POT7
      value=0;
      break;
    case 0x8:  // ALLPOT
      value=0;
      break;
    case 0x9:  // KBCODE
      value=(BYTE)handler->getKeypress();
      if(key_count) key_count--;
      if(!key_count) handler->setKeypress(0);
      if(!value) value=0xff;
      if(value==0x03) value=0;   /* fake the L key */
      DEBUG_OUT<<"Keyboard read at "<<address<<" of "<<value<<"\n";
      break;
    case 0x0a: // RANDOM
      value=rand()&0xff;
      break;
    case 0xb:  // POTGO
      value=0;
      break;
    case 0xc:  // unused
      value=0;
      break;
    case 0xd:  // SERIN
      value=0;
      break;
    case 0xe:  // IRQST
      ival=0;
      value=0xff;  // no IRQs caused by hardware that pokey knows of
      if(handler->getKeypress()) ival=0x40;  // Caused by keypress
      value=(BYTE)(value-ival);
      DEBUG_OUT<<"IRQ ST read at "<<address<<" of "<<value<<"\n";
      break;
    case 0xf:
      break;
    default:
      DEBUG_OUT<<"pokey read of "<<value<<" at "<<ad<<"\n";
      break;
  }
  return value;
}

