#ifndef EMU_NATARIDOS_H
#define EMU_NATARIDOS_H


#include <elib/EOSHelper.h>


class NAtariDos : public EOSHelper {
public:
  // public member functions
  NAtariDos(EHandler *parent);
  virtual ~NAtariDos();
  //
  virtual bool systemCall(ULONG loc);
  virtual void reset();
  virtual unsigned int load(const char *filename,ULONG loc,ULONG off,unsigned int maxBytes);
  virtual unsigned int save(const char *filename,ULONG loc,unsigned int size);
  virtual unsigned int loadDirectory(const char *filename,ULONG loc,unsigned int maxBytes);
  //
protected:
  // private member functions
  // private data members
};


#endif
