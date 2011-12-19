#ifndef NETLIB_ASOCKETNONBLOCKING_H
#define NETLIB_ASOCKETNONBLOCKING_H


#include "ASocket.h"


class ASocketNonBlocking : public ASocket
{
public:
  ASocketNonBlocking(AHost *h,char *path=(char *)NULL);
  ASocketNonBlocking(AHost *h,unsigned long port);
  virtual ~ASocketNonBlocking();
  long send(void *data,unsigned long size);
  long recv(void *data,unsigned long size);
private:
};


#endif // NETLIB_ASOCKETNONBLOCKING_H


