
//#define DEBUG_OUT std::cerr
#define DEBUG_OUT *aNullStream


#include <iostream>

#include "ASocketNonBlocking.h"


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
//  ASocketNonBLocking Class
////////////////////////////////////////////////////////////////////////////////

ASocketNonBlocking::ASocketNonBlocking(AHost *h,char *path) : ASocket(h,path)
{
  bool err=false;
#ifndef ALIB_NO_NET
#ifdef ASYS_UNIX_BASED
  err=fcntl(sock,F_SETFL,O_NONBLOCK);
#endif // ASYS_UNIX_BASED
#endif // ALIB_NO_NET
  //if(err) DEBUG_OUT.warning("Couldn't set socket to non-blocking!\n");
}


ASocketNonBlocking::ASocketNonBlocking(AHost *h,unsigned long port) : ASocket(h,port)
{
  bool err=false;
#ifndef ALIB_NO_NET
#ifdef ASYS_UNIX_BASED
  err=fcntl(sock,F_SETFL,O_NONBLOCK);
#endif // ASYS_UNIX_BASED
#endif // ALIB_NO_NET
  //if(err) DEBUG_OUT.warning("Couldn't set socket to non-blocking!\n");
}


ASocketNonBlocking::~ASocketNonBlocking()
{
}


long ASocketNonBlocking::send(void *data,unsigned long size)
{
  return ASocket::send(data,size);
}


long ASocketNonBlocking::recv(void *data,unsigned long size)
{
  return ASocket::recv(data,size);
}



