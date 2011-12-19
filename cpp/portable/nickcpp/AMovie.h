#ifndef ALIB_AMOVIE_H
#define ALIB_AMOVIE_H


#include "AFrameAbstract.h"


class AMovie : public AFrameAbstract {
public:
  AMovie();
  //AMovie(char *fname);
  AMovie(unsigned char *theData, unsigned long theDataRate, unsigned long thePlayingTime);
  virtual ~AMovie();
  unsigned long getplayingTime() const { return playingTime; }
  bool getStereo() const { return stereo; }
  bool getCompressed() const { return compressed; }
  unsigned char *getTrackData(unsigned trackNum);
  unsigned int getNumTracks() const { return numTracks; }
  unsigned long getDataRate() const { return dataRate; }
  void play(unsigned long speed=0);
  void playLooped(unsigned long speed=0);
  void playASync(unsigned long speed=0);
  void playASyncLooped(unsigned long speed=0);
protected:
  virtual void init();
  void verifyOkay();
  //
  unsigned long playingTime;
  unsigned int numTracks;
  unsigned long dataRate;
  bool stereo;
  bool compressed;
  bool okayDokey;
};


#endif // ALIB_AMOVIE_H

