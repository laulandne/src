#ifndef DLIB_DFATDISK_H
#define DLIB_DFATDISK_H


#include <disklib/DDiskImageMem.h>


class DImageFat : public DDiskImageMem {
public:
  DImageFat(DVfs *parent);
  DImageFat(DVfs *parent,const char *name);
  virtual ~DImageFat();
  //
  static unsigned int recognize(BYTE *tbuf,FILE *f,unsigned int fsize);
  virtual const char *which() { return "DImageFat"; }
  //
protected:
  //
  virtual void constructorHelper();
  virtual void readDirectory();
  virtual void cacheFile(void *fp);
  virtual void readImageDirEntry(ULONG offset);
  //
  unsigned int imageFormat;
  unsigned int fatSectorsPerCluster;
  unsigned int fatReservedSectors;
  unsigned int fatTotalFats;
  unsigned int fatMaxRootEntries;
  unsigned int fatTotalSectors;
  unsigned int fatMediaDescriptor;
  unsigned int fatSectorsPerFat;
  unsigned int fatHiddenSectors;
  unsigned int fatTotalSectorsLarge;
  unsigned int fatDriveNumber;
  unsigned int fatFlags;
  unsigned int fatSignature;
};


#endif

