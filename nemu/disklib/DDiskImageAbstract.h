#ifndef DLIB_DDISKIMAGE_H
#define DLIB_DDISKIMAGE_H


#include <disklib/DDiskAbstract.h>


// Some useful macros for reading the disk buffer...
// TODO: These may be implemented as functions at some point...
#define alibReadBuf(x) (buf[x])
#define alibReadBuf16l(x) (buf[x]+(buf[x+1]*256))
#define alibReadBuf16b(x) ((buf[x]*256)+buf[x+1])
#define alibReadBuf32l(x) (alibReadBuf16l(x)+(alibReadBuf16l(x+2)*65536))
#define alibReadBuf32b(x) ((alibReadBuf16b(x)*65536)+alibReadBuf16b(x+2))


#define DDISK_FORMAT_UNKNOWN 0
#define DDISK_FORMAT_RAW     1


class DDiskImageAbstract : public DDiskAbstract
{
public:
  DDiskImageAbstract(DVfs *parent);
  DDiskImageAbstract(DVfs *parent,const char *name);
  virtual ~DDiskImageAbstract();
  //
  virtual const char *which() { return "DDiskImageAbstract"; }
  //
  // NOTE: Provided by child classes...
  //static unsigned int recognize(BYTE *tbuf,FILE *f,unsigned int fsize);
  //
  virtual void *Fopen(const char *filename,const char *mode);
  virtual unsigned int Fclose(void *fp);
  virtual ULONG Fread(void *ptr,ULONG sizelem,ULONG n,void *fp);
  virtual ULONG Fwrite(void *ptr,ULONG sizelem,ULONG n,void *fp);
  virtual ULONG Fseek(void *fp,long offset,int origin);
  virtual ULONG Ftell(void *fp);
  virtual int Fstat(int fd,struct awstat_struct *st);
  virtual int Stat(const char *filename,struct awstat_struct *st);
  //
  virtual void readTrackSector(unsigned int tt,unsigned int ss);
  virtual void writeTrackSector(unsigned int tt,unsigned int ss);
  //
  virtual void readBlock(unsigned int blk);
  virtual void writeBlock(unsigned int blk);
  //
protected:
  //
  virtual void init();
  virtual void readImageDirEntry(ULONG offset);
  //
  bool diskMounted;
  ULONG skipOffset; // Number of bytes (probably a header) to skip
  unsigned int *offsetToTrack;
  unsigned int *numSectorsPerTrack;
};


#endif // DLIB_DDISKIMAGE_H

