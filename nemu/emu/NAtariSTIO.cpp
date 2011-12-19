
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#include <emu/NAtariSTHandler.h>
#include <emu/NAtariSTIO.h>

#include <portable/nickcpp/ANullStream.h>

#include <portable/nickcpp/AStringHelper.h>

#include <portable/nickcpp/APalette.h>

#include <cpulib/Em68k.h>

#include <elib/EMemMapAbstract.h>
#include <elib/EModule.h>
#include <elib/ERom.h>
#include <elib/ERam.h>


////////////////////////////////////////////////////////////////////////////////
//  NAtariSTIO Class
////////////////////////////////////////////////////////////////////////////////

NAtariSTIO::NAtariSTIO(NAtariSTHandler *parent) : EMemIO(parent)
{
  switch(type) {
    case N_ATARIST_IO_ST:
    case N_ATARIST_IO_TT:
    case N_ATARIST_IO_FALCON:
      location=0xff0000;
      resize(64*1024);
      break;
    default:
      DEBUG_OUT<<"Unknown AtariST hardware type!\n";
      break;
  }
  reset();
}


NAtariSTIO::~NAtariSTIO()
{
}


void NAtariSTIO::reset()
{
}


void NAtariSTIO::ioregs()
{
}


void NAtariSTIO::io_cycle()
{
}


BYTE NAtariSTIO::write8(ULONG address,BYTE value)
{
  ULONG ad=(ULONG)address;
  unsigned int page=(address>>16)&0xf0;
  unsigned int chip=(address>>8)&0xffff;
  switch(chip) {
    case 0xf000:
      // Falcon IDE controller
      break;
    case 0xff80:
      // MMU/CPU/Misc control
      break;
    case 0xff82:
      // SHIFTER/VIDEL
      break;
    case 0xff84: case 0xff85:
      // TT Palette
      break;
    case 0xff86:
      // DMA/FDC
      break;
    case 0xff87:
      // SCSI
      break;
    case 0xff88:
      // Sound
      break;
    case 0xff89:
      // Sound DMA/DSP
      break;
    case 0xff8a:
      // Blitter
      break;
    case 0xff8c:
      // SCC
      break;
    case 0xff8e:
      // VME
      break;
    case 0xff92:
      // Extended joyports
      break;
    case 0xff98:
      // Videl palette
      break;
    case 0xffa2:
      // Falcon DSP
      break;
    case 0xfffa:
      // MFP 68901 chip
      break;
    case 0xfffc:
      // 6850 ACIA chips and realtime clock
      break;
    default:
      debugger("write to unknown io address!");
      DEBUG_OUT<<"io write of "<<(short)value<<" at "<<address<<"\n";
      break;
  }
  return value;
}


BYTE NAtariSTIO::read8(ULONG address)
{
  BYTE val=mem[(address-location)&(actualSize-1)];
  unsigned int page=(address>>16)&0xf0;
  unsigned int chip=(address>>8)&0xffff;
  switch(chip) {
    case 0xf000:
      // Falcon IDE controller
      break;
    case 0xff80:
      // MMU/CPU/Misc control
      break;
    case 0xff82:
      // SHIFTER/VIDEL
      break;
    case 0xff84: case 0xff85:
      // TT Palette
      break;
    case 0xff86:
      // DMA/FDC
      break;
    case 0xff87:
      // SCSI
      break;
    case 0xff88:
      // Sound
      break;
    case 0xff89:
      // Sound DMA/DSP
      break;
    case 0xff8a:
      // Blitter
      break;
    case 0xff8c:
      // SCC
      break;
    case 0xff8e:
      // VME
      break;
    case 0xff92:
      // Extended joyports
      break;
    case 0xff98:
      // Videl palette
      break;
    case 0xffa2:
      // Falcon DSP
      break;
    case 0xfffa:
      // MFP 68901 chip
      break;
    case 0xfffc:
      // 6850 ACIA chips and realtime clock
      break;
    default:
      debugger("read from unknown io address!");
      DEBUG_OUT<<"io read at "<<address<<"\n";
      break;
  }
  return val;
}

