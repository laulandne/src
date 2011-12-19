
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#include <disklib/DControllerDisk2.h>
#include <disklib/DImageApple2.h>

#include <cpulib/ECPUAbstract.h>

#include <elib/EHandler.h>

#include <portable/nickcpp/ANullStream.h>


////////////////////////////////////////////////////////////////////////////////
//  DControllerDisk2 Class
////////////////////////////////////////////////////////////////////////////////

DControllerDisk2::DControllerDisk2(EHandler *parent,ULONG address) : DControllerAbstract(parent,address)
{
  DEBUG_OUT<<"DControllerDisk2::DControllerDisk2()\n";
  init();
}


DControllerDisk2::~DControllerDisk2()
{
}


void DControllerDisk2::ioregs()
{
  DEBUG_OUT<<"DControllerDisk2 @"<<location<<"\n";
  DControllerAbstract::ioregs();
}


void DControllerDisk2::io_cycle()
{
  DControllerAbstract::io_cycle();
}


void DControllerDisk2::reset()
{
  DControllerAbstract::reset();
}


BYTE DControllerDisk2::write8Reg(unsigned int reg,BYTE data)
{
  BYTE ret=data;
  reg&=0xf;
  switch(reg) {
    case 0x0:  // stepper phase 0 off
    case 0x1:  // stepper phase 0 on
    case 0x2:  // stepperphase 1 off
    case 0x3:  // stepper phase 1 on
    case 0x4:  // stepperphase 2 off
    case 0x5:  // stepperphase 2 on
    case 0x6:  // stepperphase 3 off
    case 0x7:  // stepperphase 3 on
    case 0x8:  // motor off
    case 0x9:  // motor on
    case 0xa:  // drive 0
    case 0xb:  // drive 1
      strobe8Reg(reg);
      break;
    case 0xc:  // read data latch
      strobe8Reg(reg);
      break;
    case 0xd:  // write data latch
      dataLatch=data;
      break;
    case 0xe:  // prepare latch for input
    case 0xf:  // prepare latch for output
      strobe8Reg(reg);
      break;
    default:
      handler->getCPU()->debugger("Bad Disk2 write addr!\n");
      break;
  }
  return ret;
}


BYTE DControllerDisk2::read8Reg(unsigned int reg)
{
  BYTE ret=0;
  reg&=0xf;
  switch(reg) {
    case 0x0:  // stepper phase 0 off
    case 0x1:  // stepper phase 0 on
    case 0x2:  // stepperphase 1 off
    case 0x3:  // stepper phase 1 on
    case 0x4:  // stepperphase 2 off
    case 0x5:  // stepperphase 2 on
    case 0x6:  // stepperphase 3 off
    case 0x7:  // stepperphase 3 on
    case 0x8:  // motor off
    case 0x9:  // motor on
    case 0xa:  // drive 0
    case 0xb:  // drive 1
      strobe8Reg(reg);
      break;
    case 0xc:  // read data latch
      if(writeMode) {
        strobe8Reg(reg);
      }
      else {
        ret=readEncodedDiskByte();
      }
      break;
    case 0xd:  // write data latch
      // TODO: this could be part of sensing write protection...
      break;
    case 0xe:  // prepare latch for input
      strobe8Reg(reg);
      ret=0x80;  // This means the disk is write protected...
      break;
    case 0xf:  // prepare latch for output
      strobe8Reg(reg);
      break;
    default:
      handler->getCPU()->debugger("Bad Disk2 read addr!\n");
      break;
  }
  return ret;
}


void DControllerDisk2::strobe8Reg(unsigned int reg)
{
  reg&=0xf;
  switch(reg) {
    case 0x0:  // stepper phase 0 off
      //DEBUG_OUT<<"Stepper: 0 off\n";
      DEBUG_OUT<<"Stepper: 0 off\n";
      stepperOn=false;
      previousMotorState=motorState;
      motorState=0;
      break;
    case 0x1:  // stepper phase 0 on
      //DEBUG_OUT<<"Stepper: 0 on\n";
      DEBUG_OUT<<"Stepper: 0 on\n";
      stepperOn=true;
      //previousMotorState=motorState;
      motorState=0;
      if(previousMotorState==1) {
        if(!headTrackIsOn) {
          //DEBUG_OUT<<"Bumped into head stop at track 0...\n";
          //DEBUG_OUT<<"Bumped into head stop at track 0...\n";
        }
        else headTrackIsOn--;
        DEBUG_OUT<<"Moving head down one to track "<<headTrackIsOn<<"...\n";
        DEBUG_OUT<<"Moving head down one to track "<<headTrackIsOn<<"...\n";
        readAndEncodeWholeTrack(headTrackIsOn);
      }
      break;
    case 0x2:  // stepperphase 1 off
      //DEBUG_OUT<<"Stepper: 1 off\n";
      DEBUG_OUT<<"Stepper: 1 off\n";
      stepperOn=false;
      previousMotorState=motorState;
      motorState=1;
      break;
    case 0x3:  // stepper phase 1 on
      //DEBUG_OUT<<"Stepper: 1 on\n";
      DEBUG_OUT<<"Stepper: 1 on\n";
      stepperOn=true;
      //previousMotorState=motorState;
      motorState=1;
      if(previousMotorState==0) {
        if(headTrackIsOn==35) {
          //DEBUG_OUT<<"Bumped into head stop at track 35...\n";
          //DEBUG_OUT<<"Bumped into head stop at track 35...\n";
        }
        else headTrackIsOn++;
        DEBUG_OUT<<"Moving head up one to track "<<headTrackIsOn<<"...\n";
        DEBUG_OUT<<"Moving head up one to track "<<headTrackIsOn<<"...\n";
        readAndEncodeWholeTrack(headTrackIsOn);
      }
      break;
    case 0x4:  // stepperphase 2 off
      //DEBUG_OUT<<"Stepper: 2 off\n";
      DEBUG_OUT<<"Stepper: 2 off\n";
      stepperOn=false;
      previousMotorState=motorState;
      motorState=3;
      break;
    case 0x5:  // stepperphase 2 on
      //DEBUG_OUT<<"Stepper: 2 on\n";
      DEBUG_OUT<<"Stepper: 2 on\n";
      stepperOn=true;
      //previousMotorState=motorState;
      motorState=2;
      if(previousMotorState==3) {
        if(!headTrackIsOn) {
          //DEBUG_OUT<<"Bumped into head stop at track 0...\n";
          //DEBUG_OUT<<"Bumped into head stop at track 0...\n";
        }
        else headTrackIsOn--;
        DEBUG_OUT<<"Moving head down one to track "<<headTrackIsOn<<"...\n";
        DEBUG_OUT<<"Moving head down one to track "<<headTrackIsOn<<"...\n";
        readAndEncodeWholeTrack(headTrackIsOn);
      }
      break;
    case 0x6:  // stepperphase 3 off
      //DEBUG_OUT<<"Stepper: 3 off\n";
      DEBUG_OUT<<"Stepper: 3 off\n";
      stepperOn=false;
      previousMotorState=motorState;
      motorState=3;
      if(previousMotorState==2) {
        if(headTrackIsOn==35) {
          //DEBUG_OUT<<"Bumped into head stop at track 35...\n";
          //DEBUG_OUT<<"Bumped into head stop at track 35...\n";
        }
        else headTrackIsOn++;
        DEBUG_OUT<<"Moving head up one to track "<<headTrackIsOn<<"...\n";
        DEBUG_OUT<<"Moving head up one to track "<<headTrackIsOn<<"...\n";
        readAndEncodeWholeTrack(headTrackIsOn);
      }
      break;
    case 0x7:  // stepperphase 3 on
      //DEBUG_OUT<<"Stepper: 3 on\n";
      DEBUG_OUT<<"Stepper: 3 on\n";
      stepperOn=true;
      //previousMotorState=motorState;
      motorState=3;
      break;
    case 0x8:  // motor off
      DEBUG_OUT<<"Motor: off\n";
      DEBUG_OUT<<"Motor: off\n";
      motorOn=false;
      break;
    case 0x9:  // motor on
      DEBUG_OUT<<"Motor: on\n";
      DEBUG_OUT<<"Motor: on\n";
      motorOn=true;
      break;
    case 0xa:  // drive 0
      DEBUG_OUT<<"Drive: 0\n";
      DEBUG_OUT<<"Drive: 0\n";
      curDriveNum=0;
      break;
    case 0xb:  // drive 1
      DEBUG_OUT<<"Drive: 1\n";
      DEBUG_OUT<<"Drive: 1\n";
      curDriveNum=1;
      break;
    case 0xc:  // read data latch
      if(writeMode) {
        // TODO: This would write the previously loaded dataLatch
        // But we're read-only for now, so nothing is done.
      }
      else {
        handler->getCPU()->debugger("Strobe to read data latch in read mode!\n");
      }
      break;
    case 0xe:  // prepare latch for input
      DEBUG_OUT<<"Mode: read\n";
      DEBUG_OUT<<"Mode: read\n";
      writeMode=false;
      break;
    case 0xf:  // prepare latch for output
      DEBUG_OUT<<"Mode: write\n";
      DEBUG_OUT<<"Mode: write\n";
      writeMode=true;
      break;
    default:
      handler->getCPU()->debugger("Bad Disk2 strobe addr!\n");
      break;
  }
}


BYTE DControllerDisk2::readEncodedDiskByte()
{
  BYTE ret=0;
  // NOTE: High bit signifies valid data...
  // NOTE: We assume trackBuffer already has everything we need in it...
  ret=trackBuffer[trackBufferPtr];
  trackBufferPtr++;
  if(trackBufferPtr==EDISK2_NIB_TRACK_SIZE) {
    //DEBUG_OUT<<"trackBufferPtr wrapped around.\n";
    //DEBUG_OUT<<"trackBufferPtr wrapped around.\n";
    trackBufferPtr=0;
  }
  DEBUG_OUT<<"read "<<(short)ret<<" from "<<trackBufferPtr<<" in nib buffer...\n";
  return ret;
}


void DControllerDisk2::readAndEncodeWholeTrack(unsigned int tnum)
{
  //DEBUG_OUT<<"readAndEncodeWholeTrack("<<tnum<<")\n";
  unsigned int t=0;
  trackBufferPtr=0;
  for(t=0;t<EDISK2_NIB_TRACK_SIZE;t++) trackBuffer[t]=0;
  for(t=0;t<EDISK2_TRACK_SIZE;t++) tempBuffer[t]=0;
  for(t=0;t<EDISK2_NUM_SECTORS;t++) {
    // TODO: need to get io's dos somehow to call this...
    //readTrackSector(tnum,t);
  }
  bool ret=DImageApple2::encodeNibs(tnum,tempBuffer,EDISK2_TRACK_SIZE,
    trackBuffer,EDISK2_NIB_TRACK_SIZE);
  if(!ret) handler->debugger("DImageApple2::encodeNibs had problem encoding nibbles!");
}



