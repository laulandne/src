#ifndef DLIB_DAMYDISK_H
#define DLIB_DAMYDISK_H


#include <disklib/DDiskImageFile.h>


class DImageAmy : public DDiskImageFile {
public:
  DImageAmy(DVfs *parent);
  DImageAmy(DVfs *parent,const char *name);
  virtual ~DImageAmy();
  //
  static unsigned int recognize(BYTE *tbuf,FILE *f,unsigned int fsize);
  virtual const char *which() { return "DImageAmy"; }
  //
protected:
  //
  virtual void constructorHelper();
  virtual void readDirectory();
  virtual void cacheFile(void *fp);
  virtual void readImageDirEntry(ULONG offset);
  //
  void readRootBlock();
  //
  unsigned int dosType; // 0=ofs, 1=ffs, etc (3.0 defines a bunch)
};


#endif // DLIB_DAMYDISK_H

