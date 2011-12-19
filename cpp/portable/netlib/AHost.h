#ifndef NETLIB_AHOST_H
#define NETLIB_AHOST_H


#include "ANetAbstract.h"


class ANetwork;


class AHost : public ANetAbstract
{
public:
  AHost(ANetwork *n,char *path=(char *)NULL);
  AHost(ANetwork *n,unsigned long addr);
  virtual ~AHost();
  static unsigned long lookup(char *addr);
#ifdef ASYS_WIN32CE
  struct servent *my_getservbyname(char *,char *);
#endif // ASYS_WIN32CE
protected:
  ANetwork *net;
#ifndef ALIB_NO_NET
#ifdef ASYS_WINDOWS
  LPHOSTENT hostEnt;
#endif // ASYS_WINDOWS
#ifdef ASYS_UNIX_BASED
  struct hostent *hostEnt;
#endif // ASYS_UNIX_BASED
#ifdef ASYS_BE
  struct hostent *hostEnt;
#endif // ASYS_BE
#ifdef ASYS_AMIGASDK
  struct hostent *hostEnt;
#endif // ASYS_AMIGASDK
#endif // ALIB_NO_NET
};


#endif // NETLIB_AHOST_H


