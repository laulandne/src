#ifndef DLIB_DCPMDISK_H
#define DLIB_DCPMDISK_H


#include <disklib/DDiskImageMem.h>


class DImageCpm : public DDiskImageMem {
public:
  DImageCpm(DVfs *parent);
  DImageCpm(DVfs *parent,const char *name);
  virtual ~DImageCpm();
  //
  static unsigned int recognize(BYTE *tbuf,FILE *f,unsigned int fsize);
  virtual const char *which() { return "DImageCpm"; }
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
  void unpackExtDsk();
  void readExtDskHeader();
  void unpackExtDskTrack(unsigned int t);
  void unpackCpcEmu();
  void readCpcEmuHeader();
  void unpackCpcEmuTrack(unsigned int t);
  //
  BYTE *unpackedData;
  unsigned int unpackedSize;
  unsigned int *trackDataSize;
  unsigned int cpcTrackSize;
  unsigned int imageFormat;
  unsigned int totalSectors;
  unsigned int offset,dOffset;  // source and desk offsets for unpacking data
  unsigned int skippedLast;  // dcount how many tracks we skip
};


#endif

