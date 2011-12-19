#ifndef DLIB_DDISKIMAGEFILE_H
#define DLIB_DDISKIMAGEFILE_H


#include <disklib/DDiskImageAbstract.h>


class DDiskImageFile : public DDiskImageAbstract
{
public:
  DDiskImageFile(DVfs *parent);
  DDiskImageFile(DVfs *parent,const char *name);
  virtual ~DDiskImageFile();
  virtual void readBlock(unsigned int blk);
  virtual void writeBlock(unsigned int blk);
  virtual const char *which() { return "DDiskImageFile"; }
protected:
//
  virtual void init();
  bool openTheFile();
  void MountDiskFromFile(const char *fname);
  //
  FILE *theImageFile;
  ULONG theImageFileSize;
  const char *theImageFileName;
};


#endif // DLIB_DDISKIMAGEFILE_H

