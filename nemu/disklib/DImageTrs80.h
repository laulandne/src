#ifndef DLIB_DTRS80DISK_H
#define DLIB_DTRS80DISK_H


#include <disklib/DDiskImageMem.h>


class DImageTrs80 : public DDiskImageMem {
public:
  DImageTrs80(DVfs *parent);
  DImageTrs80(DVfs *parent,const char *name);
  virtual ~DImageTrs80();
  //
  static unsigned int recognize(BYTE *tbuf,FILE *f,unsigned int fsize);
  virtual const char *which() { return "DImageTrs80"; }
  //
protected:
  //
  virtual void constructorHelper();
  virtual void readDirectory();
  virtual void cacheFile(void *fp);
  virtual void readImageDirEntry(ULONG offset);
  //
  bool dmkImage;
  BYTE gat[256];  // NOTE: Raw block
  BYTE hit[256];  // NOTE: Raw block
};


#endif

