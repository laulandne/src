#ifndef DLIB_DAPPLE2DISK_H
#define DLIB_DAPPLE2DISK_H


#include <disklib/DDiskImageMem.h>


#define EDISK2_NUM_TRACKS  35
#define EDISK2_NUM_SECTORS  16
#define EDISK2_ENCODED_SECTOR_SIZE  416
#define EDISK2_DECODED_SECTOR_SIZE  256
#define EDISK2_NIB_TRACK_SIZE (EDISK2_NUM_SECTORS*EDISK2_ENCODED_SECTOR_SIZE)
#define EDISK2_TRACK_SIZE (EDISK2_NUM_SECTORS*EDISK2_DECODED_SECTOR_SIZE)


class DImageApple2 : public DDiskImageMem {
public:
  DImageApple2(DVfs *parent);
  DImageApple2(DVfs *parent,const char *name);
  virtual ~DImageApple2();
  //
  static unsigned int recognize(BYTE *tbuf,FILE *f,unsigned int fsize);
  virtual const char *which() { return "DImageApple2"; }
  //
  virtual void ls(const char *dname);
  static bool encodeNibs(unsigned int tnum,BYTE *src,unsigned int ssize,BYTE *dest,unsigned int dsize);
  static bool decodeNibs(unsigned int tnum,BYTE *src,unsigned int ssize,BYTE *dest,unsigned int dsize);
  //
protected:
  //
  virtual void constructorHelper();
  virtual void readDirectory();
  virtual void cacheFile(void *fp);
  virtual void readImageDirEntry(ULONG offset);
  //
  void readDosDirEntry(ULONG offset);
  void readProdosDirEntry(ULONG offset);
  static void encodeNibSector(unsigned int tnum,unsigned int snum,BYTE *src,BYTE *dest);
  static void decodeNibSector(unsigned int tnum,unsigned int snum,BYTE *src,BYTE *dest);
  //
  bool prodos;
  bool thirteenSector;
  bool nibble;
  BYTE vtoc[256];  // NOTE: Raw block
};


#endif

