#ifndef NETLIB_ANETABSTRACT_H
#define NETLIB_ANETABSTRACT_H


#include <portable/nickcpp/AError.h>


#ifdef WIN32
#define __USE_W32_SOCKETS 1
#endif // WIN32

#include <guilib/ATypeDefs.h>


#ifdef ASYS_UNIX_BASED
#include <netinet/in.h>
#endif // ASYS_UNIX_BASED

#ifdef ASYS_WINDOWS
//#define gethostname screwedUpGetHostname
#ifdef _SYS_TYPES_FD_SET
#undef _SYS_TYPES_FD_SET
#endif // _SYS_TYPES_FD_SET
#include <winsock.h>
//#undef gethostname
#endif // ASYS_WINDOWS

#ifdef ASYS_MAC
#ifdef MAC_USE_OT
#ifdef __MWERKS__
#include <OpenTransportProviders.h>
#endif // __MWERKS__
#endif // MAC_USE_OT
#endif // ASYS_MAC


class ANetAbstract
{
public:
  ANetAbstract();
  virtual ~ANetAbstract();
  bool isActive() const { return active; }
  char *getName() const { return name; }
  unsigned long getNumber() const { return number; }
  unsigned long getAddress() const { return number; }
  char *getAddressString();
  //
  AError err;
protected:
  void init();
  char *name;
  unsigned long number;
  bool active;
};


#endif // NETLIB_ANETABSTRACT_H


