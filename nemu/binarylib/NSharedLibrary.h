#ifndef ELIB_NSHAREDLIBRARY_H
#define ELIB_NSHAREDLIBRARY_H


#include <elib/EMapSymbol.h>


class NSharedLibrary : public EMapSymbol {
public:
  NSharedLibrary(const char *tname,unsigned int tnum,unsigned int ttype,ULONG taddr,ULONG tsize,ULONG toff, unsigned int tflags,BYTE *tdata=(BYTE *)NULL,ULONG tassoc=0,ULONG tassoc2=0);
protected:
  virtual void init();
};


#endif
