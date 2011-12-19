
//#define DEBUG_OUT std::cerr
#define DEBUG_OUT *aNullStream


#include <iostream>

#include <portable/nickcpp/ANullStream.h>

#include "AHost.h"
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


////////////////////////////////////////////////////////////////////////////////
//  AHost Class
////////////////////////////////////////////////////////////////////////////////

AHost::AHost(ANetwork *n, char *path)
{
  init();
  active=false;
  name=path;
  number=0;
  if(!n->isActive()) { DEBUG_OUT<<"Net isn't active.\n"; return; }
  net=n;
#ifndef ALIB_NO_NET
#ifdef ASYS_WINDOWS
  hostEnt=gethostbyname(path);
  if(hostEnt) {
    number=ntohl(*(unsigned long *)hostEnt->h_addr_list[0]);
    active=true;  // Doesn't REALLY mean anything...
  }
  else DEBUG_OUT<<"Didn't get host\n";
#endif // ASYS_WINDOWS
#ifdef ASYS_UNIX_BASED
  hostEnt=gethostbyname(path);
  if(hostEnt) {
    number=ntohl(*(unsigned long *)hostEnt->h_addr_list[0]);
    active=true;  // Doesn't REALLY mean anything...
  }
  else {
    DEBUG_OUT<<"Didn't get host\n";
  }
#endif // ASYS_UNIX_BASED
#ifdef ASYS_MAC
#ifdef MAC_USE_OT
  InetHostInfo hinfo;
  OSStatus err=OTInetStringToAddress(net->refOT,path,&hinfo);
  if(err==kOTNoError) {
    DEBUG_OUT<<"Waiting for async to finish...\n";
    while(!net->asyncDone) ;
    err=net->asyncResult;
  }
  if(err==kOTNoError) {
    number=hinfo.addrs[0];
    active=true;
  }
  else
  switch(err) {
    case kENOMEMErr:  DEBUG_OUT<<"Out of memory error!\n";  break;
    case kEINVALErr:  DEBUG_OUT<<"Address size error!\n";  break;
    case kOTNoDataErr:  DEBUG_OUT<<"No data error!\n";  break;
    case kOTBadNameErr:  DEBUG_OUT<<"Bad name error!\n";  break;
    default:  DEBUG_OUT<<"Unknown OT error!\n";  break;
  }
#endif // MAC_USE_OT
#endif // ASYS_MAC
#endif // ALIB_NO_NET
}


AHost::AHost(ANetwork *n, unsigned long addr)
{
  init();
  active=false;
  name=(char *)NULL;
  if(!n->isActive()) { DEBUG_OUT<<"Net isn't active.\n"; return; }
  net=n;
#ifndef ALIB_NO_NET
#ifdef ASYS_WINDOWS
  // NOTE: The address is 4 bytes long
  hostEnt=gethostbyaddr((LPSTR)&addr,4,PF_INET);
  if(hostEnt) {
    number=addr;
    active=true;  // Doesn't REALLY mean anything...
  }
  else DEBUG_OUT<<"Didn't get host\n";
#endif // ASYS_WINDOWS
#ifdef ASYS_UNIX_BASED
  // NOTE: The address is 4 bytes long
  hostEnt=gethostbyaddr((char *)&addr,4,PF_INET);
  if(hostEnt) {
    number=addr;
    active=true;  // Doesn't REALLY mean anything...
  }
  else DEBUG_OUT<<"Didn't get host\n";
#endif // ASYS_UNIX_BASED
#ifdef ASYS_MAC
#ifdef MAC_USE_OT
  InetHost ihost=number;
  name=(char *)malloc(256);
  OSStatus err=OTInetAddressToName(net->refOT,ihost,name);
  if(err==kOTNoError) {
    DEBUG_OUT<<"Waiting for async to finish...\n";
    while(!net->asyncDone) ;
    err=net->asyncResult;
  }
  if(err==kOTNoError) {
    active=true;
  }
  else
  switch(err) {
    case kENOMEMErr:  DEBUG_OUT<<"Out of memory error!\n";  break;
    case kEINVALErr:  DEBUG_OUT<<"Address size error!\n";  break;
    case kOTNoDataErr:  DEBUG_OUT<<"No data error!\n";  break;
    case kOTBadNameErr:  DEBUG_OUT<<"Bad name error!\n";  break;
    default:  DEBUG_OUT<<"Unknown OT error!\n";  break;
  }
#endif // MAC_USE_OT
#endif // ASYS_MAC
#endif // ALIB_NO_NET
}


AHost::~AHost()
{
  // FIXME: delete all sockets linked to this host
  active=false;
}


unsigned long AHost::lookup(char *addr)
{
  unsigned long t= (unsigned long)-1;
#ifndef ALIB_NO_NET
#ifdef ASYS_WINDOWS
  t=inet_addr(addr);
  if(t==INADDR_NONE) DEBUG_OUT<<"lookup failed for "<<addr<<"\n";
#endif // ASYS_WINDOWS
#ifdef ASYS_UNIX_BASED
  t=(unsigned long)-1;
  t=inet_addr(addr);
  if(t== -1) DEBUG_OUT<<"lookup failed for "<<addr<<"\n";
#endif // ASYS_UNIX_BASED
#endif // ALIB_NO_NET
  return t;
}


#ifdef ASYS_WIN32CE
// Fake getservbyname for Windows CE...
struct servent *AHost::my_getservbyname(char *,char *)
{
  return (struct servent *)NULL;
}
#endif // ASYS_WIN32CE



