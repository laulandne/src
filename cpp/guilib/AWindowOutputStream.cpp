
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#include <iostream>

#include "AWindowOutputStream.h"

#include <portable/nickcpp/AFileOutputStream.h>


AWindowOutputStream *aDefaultAWindowOutputStream=NULL;


////////////////////////////////////////////////////////////////////////////////
//  AWindowOutputStream Class
////////////////////////////////////////////////////////////////////////////////

AWindowOutputStream::AWindowOutputStream(AWindow *w,AFileOutputStream *f)
{
  init();
  aDefaultAWindowOutputStream=this;
}


AWindowOutputStream::~AWindowOutputStream()
{
  if(aDefaultAWindowOutputStream==this) aDefaultAWindowOutputStream=NULL;
}


void AWindowOutputStream::init()
{
  theWin=NULL;
  theFile=NULL;
}


unsigned int AWindowOutputStream::writeBuf(const char *s)
{
  unsigned int ret=1;
  if(theFile) *theFile<<s;
  if(theWin) *theWin<<s;
  DEBUG_OUT<<s;
  return ret;
}


void AWindowOutputStream::warning(const char *s)
{
  *this<<s<<"\n";
  exit(5);
}
