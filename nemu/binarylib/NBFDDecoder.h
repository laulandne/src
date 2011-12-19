#ifndef BINARYLIB_NBFDDECODER_H
#define BINARYLIB_NBFDDECODER_H


//#include <alib/abaseclass.h>
#include <binarylib/NBinaryDecoder.h>


#ifdef USE_LIBBFD
class NBFDDecoder : public NBinaryDecoder
{
public:
  NBFDDecoder();
  static bool recognize(unsigned char *buf,char *fname=(char *)NULL);
  virtual char *whichCPU();
  virtual char *whichIO();
  virtual bool verifyOkay();
  virtual char *which() { return (char *)"NBFDDecoder"; }
protected:
};
#endif // USE_LIBBFD


#endif
