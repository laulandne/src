#ifndef DECODERLIB_ASONGDECODER_H
#define DECODERLIB_ASONGDECODER_H


#include <portable/ndecoder/ADecoder.h>


class ASong;


class ASongDecoder : public ADecoder
{
public:
  ASongDecoder(const char *fname);
  virtual ~ASongDecoder();
  //
  ASong *createNewSongObject();
  //
  unsigned long getplayingTime() const { return playingTime; }
  bool getStereo() const { return stereo; }
  int8_t *getTrackData(unsigned trackNum);
  unsigned int getNumTracks() const { return numTracks; }
  unsigned long getDataRate() const { return dataRate; }
protected:
  //
  void init();
  bool initBuffer();
  //
  unsigned long playingTime;
  unsigned long dataRate;
  unsigned int numTracks;
  bool stereo;
};


#endif // DECODERLIB_ASONGDECODER_H
