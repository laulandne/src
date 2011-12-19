#ifndef DLIB_DDISKIMAGEMEM_H
#define DLIB_DDISKIMAGEMEM_H


#include <disklib/DDiskImageAbstract.h>


class DDiskImageMem : public DDiskImageAbstract
{
public:
  DDiskImageMem(DVfs *parent);
  DDiskImageMem(DVfs *parent,const char *name);
  virtual ~DDiskImageMem();
  virtual void readBlock(unsigned int blk);
  virtual void writeBlock(unsigned int blk);
  virtual const char *which() { return "DDiskImageMem"; }
protected:
  //
  virtual void init();
  void MountDiskFromFile(const char *fname);
  //
  BYTE *imageData;
  ULONG imageDataSize;
};


#endif // DLIB_DDISKIMAGEMEM_H

