#ifndef NETLIB_ASOCKET_H
#define NETLIB_ASOCKET_H


#include "ANetAbstract.h"


class AHost;


#define ANET_SOCKET_ERROR -1


class ASocket : public ANetAbstract
{
public:
  ASocket(AHost *h,char *path=(char *)NULL);
  ASocket(AHost *h,unsigned long port);
  virtual ~ASocket();
  long send(void *data,unsigned long size);
  long recv(void *data,unsigned long size);
protected:
  bool openTheSocket(unsigned long port);
  AHost *host;
  int bytes;
#ifndef ALIB_NO_NET
#ifdef ASYS_WINDOWS
  SOCKET sock;
  LPSERVENT servEnt;
  SOCKADDR_IN sockAddr;
#endif // ASYS_WINDOWS
#ifdef ASYS_UNIX_BASED
  int sock;
  struct servent *servEnt;
  struct sockaddr_in sockAddr;
#endif // ASYS_UNIX_BASED
#ifdef ASYS_AMIGASDK
  int sock;
  struct servent *servEnt;
  struct sockaddr_in sockAddr;
#endif // ASYS_AMIGASDK
#ifdef ASYS_BE
  int sock;
  struct servent *servEnt;
  struct sockaddr_in sockAddr;
#endif // ASYS_BE
#endif // ALIB_NO_NET
};


#endif // NETLIB_ASOCKET_H


