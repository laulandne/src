#ifndef DECODERLIB_AMOVIEDECODER_H
#define DECODERLIB_AMOVIEDECODER_H


#include <portable/nickcpp/AMovie.h>

#include "ADecoder.h"


class AMovie;


class AMovieDecoder : public ADecoder
{
public:
  AMovieDecoder(const char *fname);
  virtual ~AMovieDecoder();
  //
  AMovie *createNewMovieObject();
  //
  unsigned long getplayingTime() const { return playingTime; }
  bool getCompressed() const { return compressed; }
  bool getStereo() const { return stereo; }
  int8_t *getTrackData(unsigned int trackNum);
  unsigned long getDataRate() const { return dataRate; }
  unsigned int getNumTracks() const { return numTracks; }
protected:
  //
  virtual void init();
  bool initBuffer();
  //
  unsigned long playingTime;
  unsigned int numTracks;
  unsigned long dataRate;
  bool stereo;
  bool compressed;
};


#endif // DECODERLIB_AMOVIEDECODER_H
