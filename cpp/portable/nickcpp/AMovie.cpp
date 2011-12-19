
//#define DEBUG_OUT std::cerr
#define DEBUG_OUT *aNullStream


#include <iostream>

#include "AMovie.h"

#include <portable/nickcpp/ANullStream.h>


////////////////////////////////////////////////////////////////////////////////
//  AMovie Class
////////////////////////////////////////////////////////////////////////////////

AMovie::AMovie()
{
  DEBUG_OUT<<"AMovie() contructed.\n";
  init();
}


AMovie::AMovie(unsigned char *theData, unsigned long theDataRate, unsigned long thePlayingTime)
{
  DEBUG_OUT<<"AMovie() contructed.\n";
  init();
  dataRate=theDataRate;
  playingTime=thePlayingTime;
}


/*
AMovie::AMovie(char *fname)
{
  DEBUG_OUT<<"AMovie("<<fname<<") contructed.\n";
  init();
  DEBUG_OUT<<"Going to open \""<<fname<<"\"\n";
  AMovieDecoder *readFile=(AMovieDecoder *)ASmartMovieDecoder::pickMovieDecoder(fname);
  if(!readFile) {
    //DEBUG_OUT.warning("Couldn't create decoder for movie!\n");
    //errorFlag=true;
    return;
  }
  //if(readFile->error()) {
    //DEBUG_OUT.warning("Error decoding movie!\n");  errorFlag=true;  return;
  //}
  if(!readFile->isMovie()) {
    //DEBUG_OUT.warning("Error!  Not an movie!\n");
    //errorFlag=true;
    return;
  }
  readFile->which();
  // TODO: more here
}
*/


AMovie::~AMovie()
{
  DEBUG_OUT<<"AMovie des.\n";
}


void AMovie::init()
{
  playingTime=0;
  dataRate=0;
  stereo=false;
  compressed=false;
  okayDokey=false;
}


void AMovie::play(unsigned long speed)
{
  if(!okayDokey) verifyOkay();
  if(!okayDokey) return;
  //DEBUG_OUT.warning("AMovie::play not implemented!\n";
}


void AMovie::playASync(unsigned long speed)
{
  play(speed);
}


void AMovie::playLooped(unsigned long speed)
{
  playASyncLooped(speed);
}


void AMovie::playASyncLooped(unsigned long speed)
{
  play(speed);
}


void AMovie::verifyOkay()
{
  // Convert to format needed to play on current device...
  //if(error()) { DEBUG_OUT<<"Can't verify, error!\n"; return; }
  //TODO:
  if(!okayDokey) DEBUG_OUT<<"Yowza!  Couldn't convert movie to match hardware!!!\n";
}


