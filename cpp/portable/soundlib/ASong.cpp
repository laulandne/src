
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#include <iostream>

#include <portable/nickcpp/ANullStream.h>

#include "ASong.h"
#include "AAudioDevice.h"


////////////////////////////////////////////////////////////////////////////////
//  ASong Class
////////////////////////////////////////////////////////////////////////////////

ASong::ASong()
{
  DEBUG_OUT<<"ASong() constructed\n";
  init();
}


ASong::ASong(unsigned char *theData, unsigned long theDataRate, unsigned long thePlayingTime)
{
  DEBUG_OUT<<"ASong() constructed\n";
  init();
  numTracks=0;
  dataRate=theDataRate;
  playingTime=thePlayingTime;
}


ASong::~ASong()
{
  DEBUG_OUT<<"ASong des\n";
}


void ASong::init()
{
  ASoundAbstract::init();
  playingTime=0;
  numTracks=0;
  dataRate=0;
}


void ASong::play(unsigned long speed)
{
  if(!okayDokey) verifyOkay();
  if(!okayDokey) return;
  //DEBUG_OUT.warning("ASong::play not implemented!\n");
}


void ASong::playASync(unsigned long speed)
{
  play(speed);
}


void ASong::playLooped(unsigned long speed)
{
  playASyncLooped(speed);
}


void ASong::playASyncLooped(unsigned long speed)
{
  play(speed);
}


void ASong::verifyOkay()
{
  // Convert to format needed to play on current device...
  //if(!myDevice) { DEBUG_OUT<<"Can't verify.  No device!\n"; return; }
  //if(myDevice->verifyOkay(this)) okayDokey=true; else okayDokey=false;
  if(!okayDokey) DEBUG_OUT<<"Yowza!  Couldn't convert song to match hardware!!!\n";
}
