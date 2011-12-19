#ifndef ALIB_AFILEOUTPUTSTREAM_H
#define ALIB_AFILEOUTPUTSTREAM_H


#include <stdio.h>
#include <string.h>

#include "AOutputStream.h"


class AFileOutputStream : public AOutputStream
{
public:
  AFileOutputStream(char *nm=(char *)NULL);
  AFileOutputStream(FILE *f);
  virtual ~AFileOutputStream();
protected:
  void init();
  virtual unsigned int writeBuf(const char *s);
  FILE *theFile;
};



extern AFileOutputStream *aStdOutStream;
extern AFileOutputStream *aStdErrStream;


#endif // ALIB_AFILEOUTPUTSTREAM_H

