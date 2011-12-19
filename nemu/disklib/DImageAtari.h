#ifndef DLIB_DATARIDISK_H
#define DLIB_DATARIDISK_H


#include <disklib/DDiskImageMem.h>


class DImageAtari : public DDiskImageMem {
public:
  DImageAtari(DVfs *parent);
  DImageAtari(DVfs *parent,const char *name);
  virtual ~DImageAtari();
  //
  static unsigned int recognize(BYTE *tbuf,FILE *f,unsigned int fsize);
  virtual const char *which() { return "DImageAtari"; }
  //
  virtual void readBlock(unsigned int blk);
  virtual void writeBlock(unsigned int blk);
  //
protected:
  //
  virtual void constructorHelper();
  virtual void readDirectory();
  virtual void cacheFile(void *fp);
  virtual void readImageDirEntry(ULONG offset);
  //
  bool atrHeader;
  BYTE vtoc[128];  // NOTE: Row Block...and this is 128 bytes on single density...larger for double+ maybe?
};


#endif

