#ifndef BINARYLIB_NAOUTDECODER_H
#define BINARYLIB_NAOUTDECODER_H


//#include <alib/abaseclass.h>
#include <binarylib/NBinaryDecoder.h>


class NAOUTDecoder : public NBinaryDecoder
{
public:
  NAOUTDecoder();
  static bool recognize(unsigned char *buf,char *fname=(char *)NULL);
  virtual char *whichCPU();
  virtual char *whichIO();
  virtual char *which() { return (char *)"NAOUTDecoder"; }
  virtual bool verifyOkay();
protected:
  bool verifyAOUT();
  bool verifyAUX();
};


#endif
