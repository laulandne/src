#ifndef BINARYLIB_NBFLTDECODER_H
#define BINARYLIB_NBFLTDECODER_H


//#include <alib/abaseclass.h>
#include <binarylib/NBinaryDecoder.h>


class NBFLTDecoder : public NBinaryDecoder
{
public:
  NBFLTDecoder();
  static bool recognize(unsigned char *buf,char *fname=(char *)NULL);
  virtual char *whichCPU();
  virtual char *whichIO();
  virtual char *which() { return (char *)"NBFLTDecoder"; }
  virtual bool verifyOkay();
  virtual bool decodeNodes();
protected:
  void readHeader();
};


#endif
