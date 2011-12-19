#ifndef DLIB_DHFSDISK_H
#define DLIB_DHFSDISK_H


#include <disklib/DDiskImageFile.h>


class DImageHfs : public DDiskImageFile {
public:
  DImageHfs(DVfs *parent);
  DImageHfs(DVfs *parent,const char *name);
  virtual ~DImageHfs();
  //
  static unsigned int recognize(BYTE *tbuf,FILE *f,unsigned int fsize);
  virtual const char *which() { return "DImageHfs"; }
  //
protected:
  //
  virtual void constructorHelper();
  virtual void readDirectory();
  virtual void cacheFile(void *fp);
  virtual void readImageDirEntry(ULONG offset);
  //
  bool lookAtFileSystem(ULONG offset);
  bool findFirstHFSPartition();
  //
  bool hfs; // if false then old-style mfs
  bool lisa; // a lisa disk (?!?)
  bool partition; // if false, plain fs, no partitions
  unsigned int partNum; // if partition, this is the one we're looking at
  ULONG partOffset; // if partition, offset to first block of fs
};


#endif // DLIB_DHFSDISK_H

