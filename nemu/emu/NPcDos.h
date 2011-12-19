#ifndef EMU_NPCDOS_H
#define EMU_NPCDOS_H


#include <elib/EOSHelper.h>


class NPcDos : public EOSHelper {
public:
  // public member functions
  NPcDos(EHandler *parent);
  virtual ~NPcDos();
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
