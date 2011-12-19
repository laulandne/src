#ifndef DLIB_D1541_H
#define DLIB_D1541_H


#include <disklib/DDiskImageMem.h>
#include <portable/nickcpp/AList.h>


class DImage1541 : public DDiskImageMem {
public:
  DImage1541(DVfs *parent);
  DImage1541(DVfs *parent,const char *name);
  virtual ~DImage1541();
  //
  static unsigned int recognize(BYTE *tbuf,FILE *f,unsigned int fsize);
  virtual const char *which() { return "D1541"; }
  //
  virtual void ls(const char *dname);
  //
protected:
  //
  virtual void constructorHelper();
  virtual void readDirectory();
  virtual void cacheFile(void *fp);
  virtual void readImageDirEntry(ULONG offset);
  //
  char id1,id2,dos1,dos2;
  unsigned int blocksFree;
  unsigned int cbmDirTrack;
  bool geos;
  bool cpm;
  BYTE bam[256];  // NOTE: Raw block
};


#endif

