#ifndef ALIB_ASONG_H
#define ALIB_ASONG_H


#include "ASoundAbstract.h"


class ASong : public ASoundAbstract
{
public:
  //ASong(char *filename);
  ASong();
  ASong(unsigned char *theData, unsigned long theDataRate, unsigned long thePlayingTime);
  virtual ~ASong();
  void play(unsigned long speed=0);
  void playLooped(unsigned long speed=0);
  void playASync(unsigned long speed=0);
  void playASyncLooped(unsigned long speed=0);
  unsigned long getPlayingTime() const { return playingTime; }
  unsigned int getNumTracks() const { return numTracks; }
private:
  void verifyOkay();
  void init();
  unsigned long playingTime;
  unsigned int numTracks;
  unsigned dataRate;
};


#endif // ALIB_ASONG_H
