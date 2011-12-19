
//#define DEBUG_OUT std::cerr
#define DEBUG_OUT *aNullStream


#include <iostream>

#include "ANetAbstract.h"


//#define ALIB_HAVE_NAMESER_H 1


#ifdef ASYS_UNIX_BASED
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#ifdef ALIB_HAVE_NAMESER_H
#include <nameser.h>
#endif // ALIB_HAVE_NAMESER_H
#include <netdb.h>
#include <arpa/inet.h>
#endif // ASYS_UNIX_BASED

#ifdef ASYS_SOLARIS
extern "C" int gethostname(char *name,unsigned int namelen);
#endif // ASYS_SOLARIS


////////////////////////////////////////////////////////////////////////////////
//  ANetAbstract Class
////////////////////////////////////////////////////////////////////////////////

ANetAbstract::ANetAbstract()
{
  //dBug<<"(ANetAbstract constructed)\n";
  init();
}


void ANetAbstract::init()
{
  active=false;
  name=(char *)NULL;
  number=0;
}


ANetAbstract::~ANetAbstract()
{
  //dBug<<"(ANetAbstract destroyed)\n";
  active=false;
  name=(char *)NULL;
  number=0;
}


char *ANetAbstract::getAddressString()
{
  static char ret[80];
  unsigned char a,b,c,d;
  a=(unsigned char)((number>>24)&0xff);
  b=(unsigned char)((number>>16)&0xff);
  c=(unsigned char)((number>>8)&0xff);
  d=(unsigned char)(number&0xff);
  sprintf(ret,ATEXT("%d.%d.%d.%d"),a,b,c,d);
  return ret;
}


