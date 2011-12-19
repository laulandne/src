
//#define DEBUG_OUT std::cerr
#define DEBUG_OUT *aNullStream


#include <iostream>
#include <stdio.h>
#include <string.h>

#include "AFileOutputStream.h"


AFileOutputStream *aStdOutStream=new AFileOutputStream(stdout);
AFileOutputStream *aStdErrStream=new AFileOutputStream(stderr);


////////////////////////////////////////////////////////////////////////////////
//  AFileOutputStream Class
////////////////////////////////////////////////////////////////////////////////

AFileOutputStream::AFileOutputStream(char *nm)
{
  init();
}


AFileOutputStream::AFileOutputStream(FILE *f)
{
  init();
  theFile=f;
}


AFileOutputStream::~AFileOutputStream()
{
}


void AFileOutputStream::init()
{
}


unsigned int AFileOutputStream::writeBuf(const char *s)
{
  if(!s) return 0;
  if(!theFile) return 0;
  unsigned int ret=strlen(s);
  for(unsigned int t=0;t<ret;t++) fputc(s[t],theFile);
  //fflush(theFile);
  return ret;
}
