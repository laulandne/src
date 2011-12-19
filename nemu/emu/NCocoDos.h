#ifndef EMU_NCOCODOS_H
#define EMU_NCOCODOS_H


#include <elib/EOSHelper.h>


class NCocoDos : public EOSHelper {
public:
  // public member functions
  NCocoDos(EHandler *parent);
  virtual ~NCocoDos();
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
