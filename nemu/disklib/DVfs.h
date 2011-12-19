#ifndef DLIB_DDOS_H
#define DLIB_DDOS_H


#include <portable/nickcpp/AList.h>
#include <disklib/DDiskAbstract.h>


class EHandler;
class ECPUAbstract;
class EMemMapAbstract;


#define DDISK_NUM_DISKS 4


class DVfs  : public DDiskAbstract
{
public:
  DVfs(EHandler *parent);  // NOTE: parent can be NULL
  DVfs();
  virtual ~DVfs();
  virtual void *Fopen(const char *filename,const char *mode);
  virtual unsigned int Fclose(void *fp);
  // NOTE, addr in readMany is an address in a map, not an actual pointer
  virtual ULONG readMany(void *fp,ULONG addr,ULONG n);
  virtual ULONG Fread(void *ptr,ULONG sizelem,ULONG n,void *fp);
  virtual ULONG Fwrite(void *ptr,ULONG sizelem,ULONG n,void *fp);
  virtual ULONG Fseek(void *fp,long offset,int origin);
  virtual ULONG Ftell(void *fp);
  virtual int Fstat(int fd,struct awstat_struct *st);
  virtual int Stat(const char *filename,struct awstat_struct *st);
  virtual void *Open(const char *filename,unsigned int mode);
  virtual unsigned int Close(void *fp);
  virtual ULONG Read(void *fp,void *ptr,ULONG n);
  virtual ULONG Write(void *fp,void *ptr,ULONG n);
  virtual ULONG Lseek(void *fp,long offset,int origin);
  virtual bool CopyFile(const char *oldname,const char *newname);
  virtual bool Rename(const char *oldname,const char *newname);
  // I wanted to call this "delete"...C++, bah!
  virtual bool Remove(const char *filename);
  virtual bool isDir(const char *filename);  // is it a dir and not a normal file?
  virtual bool isSpecial(const char *filename);  // is it "special" and not a file?
  virtual unsigned int getFileVersionMajor(const char *filename);
  virtual unsigned int getFileVersionMinor(const char *filename);
  virtual ULONG getFileSize(const char *filename);
  virtual const char *SearchForFirst(const char *fname=(const char *)NULL);
  virtual const char *SearchForNext();
  virtual DDirEntry *SearchForFirstFile(const char *fname=(const char *)NULL);
  virtual DDirEntry *SearchForNextFile();
  virtual void ls(const char *dname);
  virtual bool Chdir(const char *dname);
  virtual const char *Getcwd(char *buf,unsigned int len);
  virtual void readTrackSector(unsigned int tt,unsigned int ss);
  virtual void readBlock(unsigned int blk);
  virtual void writeTrackSector(unsigned int tt,unsigned int ss);
  virtual void writeBlock(unsigned int blk);
  virtual void markBlockUsed(unsigned int blk);
  virtual void markBlockUnused(unsigned int blk);
  virtual BYTE *getBuffer();
  virtual void refresh(); // flush all caches
  virtual unsigned int getStartAddress(const char *filename);
  virtual const char *getShortName(const char *longName);
  virtual const char *getDiskName();
  virtual const char *getDiskID();
  virtual bool getDiskError();
  virtual const char *getDiskType();
  virtual unsigned int getCurTrack();
  virtual unsigned int getCurSector();
  virtual unsigned int getCurSide();
  virtual unsigned int getBlockSize();
  virtual bool getWriteProtected();
#ifdef USE_FINDER_DAT
  virtual void setIgnoreFinderDat(bool t);
#endif // USE_FINDER_DAT
  virtual bool bookmarkDir();
  virtual bool gotoBookmark();
  // Don't mess with the list when you get it!!!
  virtual AList *getTheDir();
  virtual const char *which() { return "DDOS"; }
  // the rest are calls unique to DDOS (not in DDiskAbstract)
  void mount(const char *name,unsigned int num);
  void unmount(unsigned int num);
  bool recognize(const char *fname);  // check file, but don't Mount
  void disks();
  bool systemCall(ULONG loc);
  void reset();
  void setDiskNum(unsigned int num);
  EMemMapAbstract *getMap() { return map; }
  void setMap(EMemMapAbstract *m) { map=m; }
  ECPUAbstract *getCPU() { return cpu; }
  void setCPU(ECPUAbstract *c) { cpu=c; }
  DDiskAbstract *getDisk(unsigned int num);
protected:
  virtual void init();
  unsigned int verifyDiskNum(unsigned int maybeNum);
  unsigned int verifyDiskNum();
  void noSuchDisk();
  EHandler *io;
  EMemMapAbstract *map;
  ECPUAbstract *cpu;
  char *bookmark;
  DDiskAbstract *disk[DDISK_NUM_DISKS];
  unsigned int diskNum;
  ULONG readSize,writeSize;
};


#endif // DLIB_DDOS_H

