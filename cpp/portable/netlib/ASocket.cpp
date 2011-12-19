
//#define DEBUG_OUT std::cerr
#define DEBUG_OUT *aNullStream


#include <iostream>

#include <portable/nickcpp/ANullStream.h>

#include "ASocket.h"
#include "AHost.h"


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


#ifdef ASYS_WIN32CE
LPSERVENT getservbyname(char *name, char *something);
LPSERVENT getservbyname(char *name, char *something)
{
  static SERVENT s;
  return &s;
}
#endif // ASYS_WIN32CE


////////////////////////////////////////////////////////////////////////////////
//  ASocket Class
////////////////////////////////////////////////////////////////////////////////

ASocket::ASocket(AHost *h, char *serviceName)
{
  init();
  active=false;
  name=(char *)NULL;
  if(!h->isActive()) { DEBUG_OUT<<"Host isn't active.\n"; return; }
  host=h;
#ifndef ALIB_NO_NET
#ifdef ASYS_WINDOWS
  // NOTE: The 0 is for "default protocol"
  sock=socket(PF_INET,SOCK_STREAM,0);
  if(sock==INVALID_SOCKET) {
    DEBUG_OUT<<"Couldn't create socket!\n";
  }
  else {
    servEnt=getservbyname(serviceName,(char *)NULL);
    number=servEnt->s_port;
    active=false;
    if(!openTheSocket(number)) {
      DEBUG_OUT<<"Couldn't openTheSocket!\n";
    }
    else {
      DEBUG_OUT<<"Got socket.\n";
      active=true;
    }
  }
#endif // ASYS_WINDOWS
#ifdef ASYS_UNIX_BASED
  // NOTE: The 0 is for "default protocol"
  sock=socket(PF_INET,SOCK_STREAM,0);
  if(sock== -1) {
    DEBUG_OUT<<"Couldn't create socket!\n";
  }
  else {
    servEnt=getservbyname(serviceName,(char *)NULL);
    number=servEnt->s_port;
    active=false;
    if(!openTheSocket(number)) {
      DEBUG_OUT<<"Couldn't openTheSocket!\n";
    }
    else {
      DEBUG_OUT<<"Got socket.\n";
      active=true;
    }
  }
#endif // ASYS_UNIX_BASED
#endif // ALIB_NO_NET
}


ASocket::ASocket(AHost *h, unsigned long port)
{
  init();
  active=false;
  name=(char *)NULL;
  if(!h->isActive()) { DEBUG_OUT<<"Host isn't active.\n"; return; }
  host=h;
#ifndef ALIB_NO_NET
#ifdef ASYS_WINDOWS
  // NOTE: The 0 is for "default protocol"
  sock=socket(PF_INET,SOCK_STREAM,0);
  if(sock==INVALID_SOCKET) {
    DEBUG_OUT<<"Couldn't create socket!\n";
  }
  else {
    number=port;
    active=false;
    if(!openTheSocket(number)) {
      DEBUG_OUT<<"Couldn't openTheSocket!\n";
    }
    else {
      DEBUG_OUT<<"Got socket.\n";
      active=true;
    }
  }
#endif // ASYS_WINDOWS
#ifdef ASYS_UNIX_BASED
  // The 0 is for "default protocol"
  sock=socket(PF_INET,SOCK_STREAM,0);
  if(sock== -1) {
    DEBUG_OUT<<"Couldn't create socket!\n";
  }
  else {
    number=port;
    active=false;
    if(!openTheSocket(number)) {
      DEBUG_OUT<<"Couldn't openTheSocket!\n";
    }
    else {
      DEBUG_OUT<<"Got socket.\n";
      active=true;
    }
  }
#endif // ASYS_UNIX_BASED
#endif // ALIB_NO_NET
}


bool ASocket::openTheSocket(unsigned long port)
{
  bool retVal=false;
  if(active) return false;
  DEBUG_OUT<<"Attempting to contact "<<host->getAddressString()<<":"<<port<<"\n";
#ifndef ALIB_NO_NET
#ifdef ASYS_WINDOWS
  sockAddr.sin_family=AF_INET;
  sockAddr.sin_port=(unsigned short)htons((short)port);
  sockAddr.sin_addr.S_un.S_addr=htonl(host->getAddress());
  bytes=connect(sock,(LPSOCKADDR)&sockAddr,sizeof(sockAddr));
  if(bytes) {
    DEBUG_OUT<<"Couldn't connect socket!\n";
    retVal=false;
  }
  else {
    //int err=fcntl(sock,F_SETFL,O_NONBLOCK);
    //if(err) DEBUG_OUT<<"Couldn't set socket to non-blocking!\n";
    retVal=true;
  }
#endif // ASYS_WINDOWS
#ifdef ASYS_UNIX_BASED
  sockAddr.sin_family=AF_INET;
  sockAddr.sin_port=htons(port);
  sockAddr.sin_addr.s_addr=htonl(host->getAddress());
  bytes=connect(sock,(sockaddr *)&sockAddr,sizeof(sockAddr));
  if(bytes) {
    DEBUG_OUT<<"Couldn't connect socket!\n";
    retVal=false;
  }
  else {
    retVal=true;
  }
#endif // ASYS_UNIX_BASED
#endif // ALIB_NO_NET
  return retVal;
}


ASocket::~ASocket()
{
  active=false;
#ifndef ALIB_NO_NET
#ifdef ASYS_WINDOWS
  if(sock) closesocket(sock);
  sock=(SOCKET)NULLHANDLE;
#endif // ASYS_WINDOWS
#ifdef ASYS_UNIX_BASED
  if(sock!= -1) close(sock);
  sock= -1;
#endif // ASYS_UNIX_BASED
#endif // ALIB_NO_NET
}


long ASocket::send(void *data,unsigned long size)
{
  long retVal=ANET_SOCKET_ERROR;
#ifndef ALIB_NO_NET
#ifdef ASYS_WINDOWS
  retVal=::send(sock,(char *)data,size,0);
#endif // ASYS_WINDOWS
#ifdef ASYS_UNIX_BASED
  //retVal=::send(sock,(char *)data,size,0);
  retVal=::write(sock,(char *)data,size);
#endif // ASYS_UNIX_BASED
#endif // ALIB_NO_NET
  return retVal;
}


long ASocket::recv(void *data,unsigned long size)
{
  long retVal=ANET_SOCKET_ERROR;
#ifndef ALIB_NO_NET
#ifdef ASYS_WINDOWS
  retVal=::recv(sock,(char *)data,size,0);
#endif // ASYS_WINDOWS
#ifdef ASYS_UNIX_BASED
  //retVal=::recv(sock,(char *)data,size,0);
  retVal=::read(sock,(char *)data,size);
#endif // ASYS_UNIX_BASED
#endif // ALIB_NO_NET
  return retVal;
}
