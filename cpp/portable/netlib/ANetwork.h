#ifndef NETLIB_ANETWORK_H
#define NETLIB_ANETWORK_H


#include "ANetAbstract.h"


#ifdef ASYS_MAC
#ifdef ALIB_NO_PCH
#ifdef MAC_USE_OT
#undef true
#include <OpenTptInternet.h>
typedef void * OTClientContextPtr;
#endif // MAC_USE_OT
#endif // ALIB_NO_PCH
#endif // ASYS_MAC


class ANetwork : public ANetAbstract
{
  friend class AHost;
public:
  ANetwork(char *path=(char *)NULL);
  virtual ~ANetwork();
protected:
  bool asyncDone;
  bool asyncError;
#ifndef ALIB_NO_NET
#ifdef ASYS_WINDOWS
#ifdef ASYS_WIN16
  // This is only used for Win16 where we link at run time...
  HINSTANCE winSockHandle;
#endif // ASYS_WIN16
  WSADATA wsaData;
  LPHOSTENT hostEnt;
  unsigned int sockVer;
#endif // ASYS_WINDOWS
#ifdef ASYS_MAC
  bool usingOT;
#ifdef MAC_USE_OT
  InetSvcRef refOT;
#if TARGET_CARBON
  OTClientContextPtr myContext;
#endif // TARGET_CARBON
  OSStatus asyncResult;
#endif // MAC_USE_OT
#endif // ASYS_MAC
#ifdef ASYS_UNIX_BASED
  struct hostent *hostEnt;
#endif // ASYS_UNIX_BASED
#ifdef ASYS_AMIGASDK
  struct hostent *hostEnt;
#endif // ASYS_AMIGASDK
#ifdef ASYS_BE
  struct hostent *hostEnt;
#endif // ASYS_BE
#endif // ALIB_NO_NET
};


#endif // NETLIB_ANETWORK_H


