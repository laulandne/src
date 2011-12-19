#ifndef ELIB_EROM_H
#define ELIB_EROM_H


#include "ERam.h"


class ERom : public ERam {
public:
  // TODO: The parameter order is to avoid collisions, not to make sense...
  // If you still have trouble under UNICODE, use the flag parameter.
  ERom(char *qname,ULONG addr,BYTE *data,unsigned int size);
  ERom(ULONG addr,char *fname,ULONG offset,char *mname,bool flag=true);
  virtual BYTE write8(ULONG addr,BYTE data);
  virtual BYTE reallyWrite8(ULONG addr,BYTE data);
protected:
};



#endif
