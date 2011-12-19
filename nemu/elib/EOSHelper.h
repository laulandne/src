#ifndef ELIB_EOSHELPER_H
#define ELIB_EOSHELPER_H


#include <elib/EMemAbstract.h>


class AList;
class EHandler;
class EOSEntry;
class EMemMapAbstract;
class DVfs;
class ECPUAbstract;


#define EOSHELPER_BUFSIZE 1024


class EOSHelper {
public:
  // public member functions
  EOSHelper(EHandler *parent);
  virtual ~EOSHelper();
  //
  virtual void patchAll(ULONG *nDos);
  //
  virtual bool systemCall(ULONG loc);
  virtual void reset();
  virtual unsigned int load(const char *filename,ULONG loc,ULONG off,unsigned int maxBytes);
  virtual unsigned int save(const char *filename,ULONG loc,unsigned int size);
  virtual unsigned int loadDirectory(const char *filename,ULONG loc,unsigned int maxBytes);
  //
protected:
  // private member functions
  void init();
  EOSEntry *findOriginal(ULONG loc);
  EOSEntry *findActual(ULONG loc);
  // private data members
  unsigned int outFile,inFile;
  unsigned int nameAddr,nameLen;
  unsigned int p1,p2,p3;  // parameters
  AList *osCalls;
  BYTE wbuf[EOSHELPER_BUFSIZE];
  BYTE rbuf[EOSHELPER_BUFSIZE];
  unsigned int rbufPtr,wbufPtr;
  EHandler *io;
  EMemMapAbstract *map;
  ECPUAbstract *cpu;
  DVfs *dos;
};


#endif

