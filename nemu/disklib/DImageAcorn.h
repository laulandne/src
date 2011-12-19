#ifndef DLIB_DACORNDISK_H
#define DLIB_DACORNDISK_H


#include <disklib/DDiskImageMem.h>


class DImageAcorn : public DDiskImageMem {
public:
  DImageAcorn(DVfs *parent);
  DImageAcorn(DVfs *parent,const char *name);
  virtual ~DImageAcorn();
  //
  static unsigned int recognize(BYTE *tbuf,FILE *f,unsigned int fsize);
  virtual const char *which() { return "DImageAcorn"; }
  //
protected:
  //
  virtual void constructorHelper();
  virtual void readDirectory();
  virtual void cacheFile(void *fp);
  virtual void readImageDirEntry(ULONG offset);
  //
  bool adfs;
};


#endif

