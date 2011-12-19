#ifndef ALIB_AINPUTSTREAM_H
#define ALIB_AINPUTSTREAM_H


// So you don't have to include it yourself, and can swap it in for a real stream any time
#include "ANullStream.h"


class AInputStream {
public:
  AInputStream& operator>>(unsigned long &s);
  AInputStream& operator>>(long &s);
  AInputStream& operator>>(unsigned int &s);
  AInputStream& operator>>(int &s);
  AInputStream& operator>>(char &s);
  AInputStream& operator>>(float &s);
  AInputStream& operator>>(double &s);
  //
  AInputStream& operator>>(char *s);
  //
protected:
  virtual unsigned int readBuf(char *s, unsigned int limit);
};


#endif // ALIB_AINPUTSTREAM_H

