#ifndef ALIB_AFILEINPUTSTREAM_H
#define ALIB_AFILEINPUTSTREAM_H


#include "AInputStream.h"


class AFileInputStream : public AInputStream
{
public:
  AFileInputStream(char *nm=(char *)NULL);
  AFileInputStream(FILE *f);
  virtual ~AFileInputStream();
protected:
  void init();
  virtual unsigned int readBuf(char *s, unsigned int limit);
  FILE *theFile;
};


extern AFileInputStream *aStdInStream;


#endif // ALIB_AFILEINPUTSTREAM_H

