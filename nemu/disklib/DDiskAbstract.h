#ifndef DLIB_DDISKABSTRACT_H
#define DLIB_DDISKABSTRACT_H


//#include <alib/aplugin.h>
#include <disklib/DDirEntry.h>

#include <portable/nickcpp/AError.h>


#define awstat_struct stat


#define awstat(a,b) stat(a,b)


#define ALIB_HAVE_UNISTD_H 1
#define ALIB_HAVE_SYS_STAT_H 1
//#define ALIB_HAVE_STAT_H 1
#define ALIB_HAVE_DIRENT_H 1
//#define ALIB_HAVE_DIRECT_H 1
#define ALIB_HAVE_SYS_DIR_H 1


// The size was arbitrary...
#define EDISK_BUF_SIZE 1024

// These are arbitrary, and mostly only for floppy use (hence 255 tracks is enough...)
#define EDISK_MAX_TRACKS 16384
// ...also known as cylinders
#define EDISK_MAX_SECTORS 256
// ...also known as blocks
#define EDISK_MAX_SIDES 256
// ...also known as heads


class DVfs;
class DPartitionTable;


class DDiskAbstract
{
public:
  // Public Member Functions
  DDiskAbstract();
  DDiskAbstract(DVfs *parent);
  virtual ~DDiskAbstract();
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
  virtual bool Rename(const char *oldname,const char *newname);
  virtual bool CopyFile(const char *oldname,const char *newname);
  // I wanted to call this "delete"...C++, bah!
  virtual bool Remove(const char *filename);
  virtual bool isDir(const char *filename);
  virtual bool isSpecial(const char *filename);
  virtual unsigned int getFileVersionMajor(const char *filename);
  virtual unsigned int getFileVersionMinor(const char *filename);
  virtual ULONG getFileSize(const char *filename);
  virtual const char *Getcwd(char *buf,unsigned int len);
  //
  virtual const char *SearchForFirstCached(const char *fname);
  virtual const char *SearchForNextCached();
  virtual DDirEntry *SearchForFirstFileCached(const char *fname);
  virtual DDirEntry *SearchForNextFileCached();
  //
  virtual const char *SearchForFirst(const char *fname);
  virtual const char *SearchForNext();
  virtual DDirEntry *SearchForFirstFile(const char *fname);
  virtual DDirEntry *SearchForNextFile();
  //
  virtual void ls(const char *dname);
  virtual bool Chdir(const char *dname);
  virtual const char *Getcwd(const char *buf,unsigned int len);
  virtual void readTrackSector(unsigned int tt,unsigned int ss) { diskError=true; }
  virtual void readBlock(unsigned int blk) { diskError=true; }
  virtual void writeTrackSector(unsigned int tt,unsigned int ss) { diskError=true; }
  virtual void writeBlock(unsigned int blk) { diskError=true; }
  virtual void markBlockUsed(unsigned int blk) { }
  virtual void markBlockUnused(unsigned int blk) { }
  virtual BYTE *getBuffer() { return buf; }
  virtual void refresh(); // flush all caches
  virtual const char *getShortName(const char *longName);
  virtual const char *getDiskName() { return (const char *)diskName; }
  virtual const char *getDiskID() { return (const char *)diskID; }
  virtual const char *getCurDir() { return (const char *)curDir; }
  virtual const char *getDiskType() { return which(); }
  virtual bool getDiskError() { return diskError; }
  virtual unsigned int getCurTrack() { return curTrack; }
  virtual unsigned int getCurSector() { return curSector; }
  virtual unsigned int getCurSide() { return curSide; }
  virtual unsigned int getBlockSize() { return blockSize; }
  virtual bool getWriteProtected() { return writeProtected; }
#ifdef USE_FINDER_DAT
  virtual void setIgnoreFinderDat(bool t);
  virtual bool getIgnoreFinderDat() { return ignoreFinderDat; }
#endif // USE_FINDER_DAT
  // Don't mess with the list when you get it!!!
  virtual AList *getTheDir() { return &theDir; }
  virtual const char *which() { return "DDiskAbstract"; }
  //
  AError err;
protected:
  // Private Member Functions
  virtual void init();
  virtual void constructorHelper();
  virtual void readDirectory();
  virtual void freeDirCache();
  virtual void cacheFile(void *fp);
  virtual void freeFileCache(void *fp);
  virtual bool removeAllFilesinFolder(const char *foldername);
  //
  static ULONG getSize(const char *filename);
  unsigned int popCount(BYTE q);
  unsigned int verifyTrack(unsigned int ttrk);
  unsigned int verifySector(unsigned int tsec);
  unsigned int verifySide(unsigned int tsd);
  unsigned int verifyBlock(unsigned int tblk);
#ifdef USE_FAKE_CHDIR
  const char *fakeFullPath(const char *name);
#endif // USE_FAKE_CHDIR
  // Private Data Members
  DVfs *dos;
  BYTE buf[EDISK_BUF_SIZE];
  ULONG curLoc;
  bool diskError;
  bool writeProtected;
  unsigned int curTrack,curSector,curSide;
  unsigned int blockSize;
  // If allTracksSameSectors is false, numSectors is really maxSectors...
  unsigned int numTracks,numSectors,numSides;
  bool allTracksSameSectors;
  bool noTrackZero,noSectorZero;  // Instead start at one
  unsigned int sectorSkew[EDISK_MAX_SECTORS];
  DPartitionTable *partitionTable; // Non-NULL if we're a partition
  unsigned int partitionNumber; // Our number if we're a partition
  AList theDir;
  char matchName[DDISK_MAX_NAMELEN]; // for finds
  char diskName[DDISK_MAX_NAMELEN]; // can be left blank
  char diskID[DDISK_MAX_NAMELEN];   // can be left blank
  char curDir[DDISK_MAX_NAMELEN];   // can be left blank
#ifdef USE_FAKE_CHDIR
  char *prefixPath;  // can be left blank
  char fullPathName[DDISK_MAX_NAMELEN];   // can be left blank
#endif // USE_FAKE_CHDIR
  bool noFileSystem;
  bool cachesData;
  bool cachesFiles;
  bool cachesDir;
  bool doNotCacheDir;
  bool doNotCacheFiles;
#ifdef USE_FINDER_DAT
  bool ignoreFinderDat;
#endif // USE_FINDER_DAT
};


#endif

