
//#define DEBUG_OUT std::cerr
#define DEBUG_OUT *aNullStream



#include <iostream>

#include "AWindowInputStream.h"

#include <portable/nickcpp/AFileInputStream.h>


AWindowInputStream *aDefaultAWindowInputStream=NULL;


////////////////////////////////////////////////////////////////////////////////
//  AWindowInputStream Class
////////////////////////////////////////////////////////////////////////////////

AWindowInputStream::AWindowInputStream(AWindow *w,AFileInputStream *f)
{
  init();
  aDefaultAWindowInputStream=this;
}


AWindowInputStream::~AWindowInputStream()
{
  if(aDefaultAWindowInputStream==this) aDefaultAWindowInputStream=NULL;
}


void AWindowInputStream::init()
{
  theWin=NULL;
  theFile=NULL;
}


unsigned int AWindowInputStream::readBuf(char *s,unsigned int limit)
{
  if(!s) return 0;
  unsigned int ret=0;
  //if(theFile) ret=theFile->readBuf(s,limit);
  //if(theWin) ret=theWin->readBuf(s,limit);
  return ret;
}

