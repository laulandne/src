
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#include <disklib/DControllerWD177x.h>

#include <cpulib/ECPUAbstract.h>

#include <elib/EHandler.h>

#include <portable/nickcpp/ANullStream.h>


////////////////////////////////////////////////////////////////////////////////
//  DControllerWD177x Class
////////////////////////////////////////////////////////////////////////////////

DControllerWD177x::DControllerWD177x(EHandler *parent,ULONG address) : DControllerAbstract(parent,address)
{
  DEBUG_OUT<<"DControllerWD177x::DControllerWD177x()\n";
  init();
}


DControllerWD177x::~DControllerWD177x()
{
}


void DControllerWD177x::ioregs()
{
  DEBUG_OUT<<"DControllerWD177x @"<<location<<"\n";
  DControllerAbstract::ioregs();
}


void DControllerWD177x::io_cycle()
{
  DControllerAbstract::io_cycle();
}


void DControllerWD177x::reset()
{
  DControllerAbstract::reset();
}


BYTE DControllerWD177x::write8Reg(unsigned int reg,BYTE value)
{
  if(reg>0x0a) {
    handler->getCPU()->debugger("Write past end of FDC!!!\n");
    return 0;
  }
  switch(reg) {
    case 0: // CONTROL
      DEBUG_OUT<<"FDC write CONTROL of "<<(short)value<<"\n";
      break;
    case 1: // COMMAND
      DEBUG_OUT<<"FDC write COMMAND of "<<(short)value<<"\n";
      break;
    case 2: // STAT A
      DEBUG_OUT<<"FDC write STAT A of "<<(short)value<<"\n";
      value=0;
      break;
    case 3: // STAT B
      DEBUG_OUT<<"FDC write STAT B of "<<(short)value<<"\n";
      value=0;
      break;
    case 4: // TRACK
      DEBUG_OUT<<"FDC write TRACK of "<<(short)value<<"\n";
      break;
    case 5: // SECTOR
      DEBUG_OUT<<"FDC write SECTOR of "<<(short)value<<"\n";
      break;
    case 6: // SIDE
      DEBUG_OUT<<"FDC write SIDE of "<<(short)value<<"\n";
      break;
    case 7: // DATA
      DEBUG_OUT<<"FDC write DATA of "<<(short)value<<"\n";
      value=0xff;
      break;
    case 8: // CLOCK
      DEBUG_OUT<<"FDC write CLOCK of "<<(short)value<<"\n";
      break;
    case 9: // STEP
      DEBUG_OUT<<"FDC write STEP of "<<(short)value<<"\n";
      break;
    case 10: // P CODE
      DEBUG_OUT<<"FDC write P CODE of "<<(short)value<<"\n";
      value=0;
      break;
    default:
      DEBUG_OUT<<"C65 FDC write at "<<reg<<" of "<<(unsigned int)value<<"\n";
      break;
  }
  return value;
}


BYTE DControllerWD177x::read8Reg(unsigned int reg)
{
  BYTE value=0;
  if(reg>0x0a) {
    handler->getCPU()->debugger("Read past end of FDC!!!\n");
    return 0;
  }
  switch(reg) {
    case 0: // CONTROL
      DEBUG_OUT<<"FDC read CONTROL of "<<(short)value<<"\n";
      break;
    case 1: // COMMAND
      DEBUG_OUT<<"FDC read COMMAND of "<<(short)value<<"\n";
      break;
    case 2: // STAT A
      value=0;
      DEBUG_OUT<<"FDC read STAT A of "<<(short)value<<"\n";
      break;
    case 3: // STAT B
      value=0;
      DEBUG_OUT<<"FDC read STAT B of "<<(short)value<<"\n";
      break;
    case 4: // TRACK
      DEBUG_OUT<<"FDC read TRACK of "<<(short)value<<"\n";
      break;
    case 5: // SECTOR
      DEBUG_OUT<<"FDC read SECTOR of "<<(short)value<<"\n";
      break;
    case 6: // SIDE
      DEBUG_OUT<<"FDC read SIDE of "<<(short)value<<"\n";
      break;
    case 7: // DATA
      value=0xff;
      DEBUG_OUT<<"FDC read DATA of "<<(short)value<<"\n";
      break;
    case 8: // CLOCK
      DEBUG_OUT<<"FDC read CLOCK of "<<(short)value<<"\n";
      break;
    case 9: // STEP
      DEBUG_OUT<<"FDC read STEP of "<<(short)value<<"\n";
      break;
    case 10: // P CODE
      value=0;
      DEBUG_OUT<<"FDC read P CODE of "<<(short)value<<"\n";
      break;
    default:
      DEBUG_OUT<<"C65 FDC read at "<<reg<<" of "<<(unsigned int)value<<"\n";
      break;
  }
  return value;
}


