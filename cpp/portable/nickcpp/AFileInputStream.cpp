
//#define DEBUG_OUT std::cerr
#define DEBUG_OUT *aNullStream


#include <iostream>
#include <stdio.h>

#include "AFileInputStream.h"


AFileInputStream *aStdInStream=new AFileInputStream(stdin);


////////////////////////////////////////////////////////////////////////////////
//  AFileInputStream Class
////////////////////////////////////////////////////////////////////////////////

AFileInputStream::AFileInputStream(char *nm)
{
  init();
}


AFileInputStream::AFileInputStream(FILE *f)
{
  init();
  theFile=f;
}


AFileInputStream::~AFileInputStream()
{
}


void AFileInputStream::init()
{
}


unsigned int AFileInputStream::readBuf(char *s,unsigned int limit)
{
  if(!s) return 0;
  if(!theFile) return 0;
  return 0;
}

