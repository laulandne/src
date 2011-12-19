
//#define DEBUG_OUT std::cerr
#define DEBUG_OUT *aNullStream


#include <iostream>

#include <portable/nickcpp/ANullStream.h>

#include "ANetwork.h"


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

#ifdef ASYS_MAC
#include <guilib/alib_carbon.h>
#endif // ASYS_MAC


////////////////////////////////////////////////////////////////////////////////
//  ANet Class
////////////////////////////////////////////////////////////////////////////////

ANetwork::ANetwork(char *path)
{
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"ANetwork::ANetwork...\n";
#endif // DEBUG_VERBOSE
  init();
  active=false;
  asyncDone=true;  asyncError=false;
  name=(char *)"localhost";
  number=0x7f000001L;
#ifndef ALIB_NO_NET
#ifdef ASYS_WINDOWS
  sockVer=0;
/*
#ifdef ASYS_WIN16
  winSockHandle=(HINSTANCE)NULLHANDLE;
  winSockHandle=LoadLibrary("winsock.dll");
  if((int)winSockHandle<33) {
    DEBUG_OUT<<"Couldn't load winsock.dll!\n";
    return;
  }
  // First dynamically resolve the functions we need
  if(!_dll_inet_addr) {
    _dll_inet_addr=(unsigned long (pascal *)(const char *))GET_PROC("WINSOCK","INET_ADDR");
    if(!_dll_inet_addr) {
      DEBUG_OUT<<"Couldn't getProc inet_addr!\n";
    }
  }
  if(!_dll_inet_ntoa) {
    _dll_inet_ntoa=(char *(pascal *)(in_addr))GET_PROC("WINSOCK","INET_NTOA");
    if(!_dll_inet_ntoa) {
      DEBUG_OUT<<"Couldn't getProc inet_ntoa!\n";
    }
  }
  if(!_dll_WSAStartup) {
    _dll_WSAStartup=(int (pascal *)(unsigned short,WSAData *))GET_PROC("WINSOCK","WSASTARTUP");
    if(!_dll_WSAStartup) {
      DEBUG_OUT<<"Couldn't getProc wsastartup!\n";
    }
  }
  if(!_dll_WSACleanup) {
    _dll_WSACleanup=(int (pascal *)(void))GET_PROC("WINSOCK","WSACLEANUP");
    if(!_dll_WSACleanup) {
      DEBUG_OUT<<"Couldn't getProc wsacleanup!\n";
    }
  }
  if(!_dll_gethostbyaddr) {
    _dll_gethostbyaddr=(hostent *(pascal *)(const char *,int,int))GET_PROC("WINSOCK","GETHOSTBYADDR");
    if(!_dll_gethostbyaddr) {
      DEBUG_OUT<<"Couldn't getProc gethostbyaddr!\n";
    }
  }
  if(!_dll_gethostbyname) {
    _dll_gethostbyname=(hostent FAR *(PASCAL *)(const char *))GET_PROC("WINSOCK","GETHOSTBYNAME");
    if(!_dll_gethostbyname) {
      DEBUG_OUT<<"Couldn't getProc gethostbyname!\n";
    }
  }
  if(!_dll_send) {
    _dll_send=(int (pascal *)(unsigned int,const char *,int,int))GET_PROC("WINSOCK","SEND");
    if(!_dll_send) {
      DEBUG_OUT<<"Couldn't getProc send!\n";
    }
  }
  if(!_dll_recv) {
    _dll_recv=(int (pascal *)(unsigned int,const char *,int,int))GET_PROC("WINSOCK","RECV");
    if(!_dll_recv) {
      DEBUG_OUT<<"Couldn't getProc recv!\n";
    }
  }
  if(!_dll_connect) {
    _dll_connect=(int (pascal *)(unsigned int,const sockaddr *,int))GET_PROC("WINSOCK","CONNECT");
    if(!_dll_connect) {
      DEBUG_OUT<<"Couldn't getProc connect!\n";
    }
  }
  if(!_dll_socket) {
    _dll_socket=(unsigned int (pascal *)(int,int,int))GET_PROC("WINSOCK","SOCKET");
    if(!_dll_socket) {
      DEBUG_OUT<<"Couldn't getProc socket!\n";
    }
  }
  if(!_dll_getservbyname) {
    _dll_getservbyname=(servent *(pascal *)(const char *,const char *))GET_PROC("WINSOCK","GETSERVBYNAME");
    if(!_dll_getservbyname) {
      DEBUG_OUT<<"Couldn't getProc getservbyname!\n";
    }
  }
  if(!_dll_closesocket) {
    _dll_closesocket=(void (pascal *)(unsigned int))GET_PROC("WINSOCK","CLOSESOCKET");
    if(!_dll_closesocket) {
      DEBUG_OUT<<"Couldn't getProc closesocket\n";
    }
  }
  if(!_dll_gethostname) {
    _dll_gethostname=(int (pascal *)(char *,int))GET_PROC("WINSOCK","GETHOSTNAME");
    if(!_dll_gethostname) {
      DEBUG_OUT<<"Couldn't getProc gethostname\n";
    }
  }
  if(!_dll_htonl) {
    _dll_htonl=(u_long (pascal *)(u_long))GET_PROC("WINSOCK","HTONL");
    if(!_dll_htonl) {
      DEBUG_OUT<<"Couldn't getProc htonl\n";
    }
  }
  if(!_dll_htons) {
    _dll_htons=(u_short (pascal *)(u_short))GET_PROC("WINSOCK","HTONS");
    if(!_dll_htons) {
      DEBUG_OUT<<"Couldn't getProc htons\n";
    }
  }
  if(!_dll_ntohl) {
    _dll_ntohl=(u_long (pascal *)(u_long))GET_PROC("WINSOCK","NTOHL");
    if(!_dll_ntohl) {
      DEBUG_OUT<<"Couldn't getProc ntohl\n";
    }
  }
  if(!_dll_ntohs) {
    _dll_ntohs=(u_short (pascal *)(u_short))GET_PROC("WINSOCK","NTOHS");
    if(!_dll_ntohs) {
      DEBUG_OUT<<"Couldn't getProc ntohs\n";
    }
  }
#endif  // ASYS_WIN16
*/
  // Now initialize WinSock
  if(1) {
    // We want at least version 1.1
    if(WSAStartup(0x0101,&wsaData)) {
      DEBUG_OUT<<"Couldn't WSAStartup!\n";
    }
    else {
      active=true;
#ifdef DEBUG_VERBOSE
      DEBUG_OUT<<"Got net (";
      DEBUG_OUT<<wsaData.szDescription<<").\n";
#endif // DEBUG_VERBOSE
      //unsigned int ver1=HIBYTE(wsaData.wVersion),ver2=LOBYTE(wsaData.wVersion);
      unsigned int ver1=HIBYTE(wsaData.wHighVersion),ver2=LOBYTE(wsaData.wHighVersion);
#ifdef DEBUG_VERBOSE
      DEBUG_OUT<<"WinSock version: "<<ver1<<"."<<ver2<<"\n";
#endif // DEBUG_VERBOSE
      sockVer=ver1;
      char buf[256];
      gethostname(buf,255);
      name=strdup(buf);
      hostEnt=gethostbyname(buf);
      if(hostEnt) number=ntohl(*(unsigned long *)hostEnt->h_addr_list[0]);
    }
  }
#endif // ASYS_WINDOWS
#ifdef ASYS_UNIX_BASED
  active=true;
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"Got net (since we're on unix).\n";
#endif // DEBUG_VERBOSE
  char buff[256];
  int err=1;
  err=gethostname(buff,255);
  if(err) name=(char *)"localhost";
  else name=strdup(buff);
  hostEnt=(struct hostent *)NULL;
  hostEnt=gethostbyname(name);
  if(hostEnt) number=ntohl(*(unsigned long *)hostEnt->h_addr_list[0]);
#endif // ASYS_UNIX_BASED
#ifdef ASYS_MAC
  usingOT=false;
#ifdef MAC_USE_OT
  OSStatus err=kOTNoError;
  if(alib_usingOT) {
    refOT=(InetSvcRef)NULL;
#if TARGET_CARBON
#ifndef _LATITUDE_
#define MAC_USE_IOTIC 1
#endif // _LATITUDE_
#endif // TARGET_CARBON
#ifdef MAC_USE_IOTIC
    err=InitOpenTransportInContext(kInitOTForApplicationMask,&myContext);
#else
    err=InitOpenTransport();
#endif // MAC_USE_IOTIC
    if(err==kOTNoError) {
      usingOT=true;
#ifdef DEBUG_VERBOSE
      DEBUG_OUT<<"Got net (OpenTransport).\n";
#endif // DEBUG_VERBOSE
      active=true;
#if TARGET_CARBON
      refOT=OTOpenInternetServicesInContext(kDefaultInternetServicesPath,0,&err,myContext);
#else
      refOT=OTOpenInternetServices(kDefaultInternetServicesPath,0,&err);
#endif // TARGET_CARBON
      if(!refOT) err=kOTBadNameErr;
      InetInterfaceInfo ifaceInfo;
      if(err==kOTNoError) err=OTInetGetInterfaceInfo(&ifaceInfo,kDefaultInetInterface);
      if(err==kOTNoError) number=ifaceInfo.fAddress;
      asyncResult=kOTNoError;
    }
    else DEBUG_OUT<<"OpenTransport isn't present!\n";
  }
  else DEBUG_OUT<<"Not usingOT!\n";
#else
#ifndef ASYS_UNIX_BASED
  DEBUG_OUT.warning("Not using OT, and classic networking isn't implemented!\n");
#endif // ASYS_UNIX_BASED
#endif // MAC_USE_OT
#endif // ASYS_MAC
#endif // ALIB_NO_NET
}


ANetwork::~ANetwork()
{
  active=false;
  // FIXME: Here delete all hosts linked to this net
#ifndef ALIB_NO_NET
#ifdef ASYS_WINDOWS
  if(active) WSACleanup();
#ifdef ASYS_WIN16
  if((int)winSockHandle>32) FreeLibrary(winSockHandle);
  winSockHandle=(HINSTANCE)NULLHANDLE;
#endif // ASYS_WIN16
#endif // ASYS_WINDOWS
#ifdef ASYS_MAC
#ifdef MAC_USE_OT
  if(usingOT) {
#if TARGET_CARBON
    CloseOpenTransportInContext(myContext);
#else
    CloseOpenTransport();
#endif // TARGET_CARBON
  }
#endif // MAC_USE_OT
  usingOT=false;
#endif // ASYS_MAC
#endif // ALIB_NO_NET
}



