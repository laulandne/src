
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#include <disklib/DControllerAbstract.h>

#include <cpulib/ECPUAbstract.h>

#include <portable/nickcpp/ANullStream.h>


/*
NOTES:

Atari drives...
810: wd1771
1050: wd2793-02
xf: wd1772

Atari ST:
WD1772

Amstrad:
n765

BBC:
original dfs 8271
then 1770 (and less common 1772)

Oric:
1793

IBM PC:
8272

*/


////////////////////////////////////////////////////////////////////////////////
//  DControllerAbstract Class
////////////////////////////////////////////////////////////////////////////////

DControllerAbstract::DControllerAbstract(EHandler *parent,ULONG address) :
  EIOChip(parent,address)
{
  DEBUG_OUT<<"DControllerAbstract::DControllerAbstract()\n";
  init();
}


DControllerAbstract::~DControllerAbstract()
{
}


void DControllerAbstract::init()
{
  motorState=0xff; // i.e. unknown state
  previousMotorState=0xff; // i.e. unknown state
  stepperOn=false;
  motorOn=false;
  writeMode=false;
  curDriveNum=0;  // NOTE: I number the drives 0 and 1 (not 1 and 2 like apple)
  headTrackIsOn=35;
  trackBufferPtr=0;
  unsigned int t=0;
  for(t=0;t<EDISK_BUFFER_SIZE;t++) trackBuffer[t]=0;
  for(t=0;t<EDISK_BUFFER_SIZE;t++) tempBuffer[t]=0;
}


void DControllerAbstract::ioregs()
{
  //DEBUG_OUT<<"Disk controller info:\n";
  DEBUG_OUT<<"headTrackIsOn "<<headTrackIsOn<<"\n";
  DEBUG_OUT<<"Stepper ";
  if(stepperOn) DEBUG_OUT<<"on "; else DEBUG_OUT<<"off ";
  DEBUG_OUT<<"motorState "<<motorState<<" (previous "<<previousMotorState<<")\n";
  DEBUG_OUT<<"Motor ";
  if(motorOn) DEBUG_OUT<<"on "; else DEBUG_OUT<<"off ";
  DEBUG_OUT<<"\n";
  DEBUG_OUT<<"Accessing drive "<<curDriveNum<<"\n";
  DEBUG_OUT<<"Data mode is ";
  if(writeMode) DEBUG_OUT<<"write "; else DEBUG_OUT<<"read ";
  DEBUG_OUT<<"\n";
  DEBUG_OUT<<"dataLatch "<<(short)dataLatch<<"\n";
  DEBUG_OUT<<"trackBufferPtr "<<trackBufferPtr<<"\n";
}


void DControllerAbstract::io_cycle()
{
}


void DControllerAbstract::reset()
{
  init();
}


BYTE DControllerAbstract::write8Reg(unsigned int reg,BYTE data)
{
  BYTE ret=data;
  return ret;
}


BYTE DControllerAbstract::read8Reg(unsigned int reg)
{
  BYTE ret=0;
  reg&=0xf;
  return ret;
}

