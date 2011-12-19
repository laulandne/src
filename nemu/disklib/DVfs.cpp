
//#define DEBUG_OUT std::cerr
#define DEBUG_OUT *aNullStream


/*

TODO: look at nemu2.cpp and fix it so you can include different disk
formats as you choose and it'll check them just by the define'd symbol
from the include file...so you don't have to move the check in Mount()
by hand...

*/


#include <portable/nickcpp/AStringHelper.h>

#include <portable/nickcpp/ANullStream.h>

#include <elib/EResourceFile.h>
#include <elib/EMemMapAbstract.h>

#include <disklib/DVfs.h>
#include <disklib/DPartitionTable.h>
#include <disklib/DDiskImageFile.h>
#include <disklib/DDiskImageMem.h>
#include <disklib/DNativeFilesystem.h>


#ifdef ALIB_HAVE_UNISTD_H
//#include <unistd.h>
#endif // ALIB_HAVE_UNISTD_H

#ifdef ALIB_HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif

#ifdef ALIB_HAVE_STAT_H
#include <stat.h>
#endif

#ifdef ALIB_HAVE_DIRENT_H
//#include <dirent.h>
#endif // ALIB_HAVE_DIRENT_H

#ifdef ALIB_HAVE_DIRECT_H
#include <direct.h>
#endif // ALIB_HAVE_DIRECT_H

#ifdef ALIB_HAVE_SYS_DIR_H
//#include <sys/dir.h>
#endif // ALIB_HAVE_SYS_DIR_H


#ifdef ASYS_MAC
#ifndef __GNUC__
#define MAC_USE_MOREFILES 1
#include <IterateDirectory.h>
#endif // __GNUC__
#endif // ASYS_MAC


#include <disklib/DPartitionTableAmy.h>
#include <disklib/DPartitionTableHfs.h>
#include <disklib/DPartitionTableMbr.h>

#include <disklib/DImage1541.h>
#include <disklib/DImageAcorn.h>
#include <disklib/DImageAmy.h>
#include <disklib/DImageApple2.h>
#include <disklib/DImageAtari.h>
#include <disklib/DImageCpm.h>
#include <disklib/DImageFat.h>
#include <disklib/DImageHfs.h>
#include <disklib/DImageTrs80.h>


////////////////////////////////////////////////////////////////////////////////
//  DVfs Class
////////////////////////////////////////////////////////////////////////////////

DVfs::DVfs(EHandler *parent)
{
  DEBUG_OUT<<"DVFS created (with parent).\n";
  init();
  io=parent;
}


DVfs::DVfs()
{
  DEBUG_OUT<<"DVFS created.\n";
  init();
  io=(EHandler *)NULL;
}


DVfs::~DVfs()
{
  for(unsigned int t=0;t<DDISK_NUM_DISKS;t++) {
    if(disk[t]) delete disk[t];
    disk[t]=(DDiskAbstract *)NULL;
  }
}


void DVfs::init()
{
  DEBUG_OUT<<"DVfs::init()...\n";
  DDiskAbstract::init();
  for(unsigned int t=0;t<DDISK_NUM_DISKS;t++) disk[t]=(DDiskAbstract *)NULL;
  map=(EMemMapAbstract *)NULL;
  cpu=(ECPUAbstract *)NULL;
  io=(EHandler *)NULL;
  bookmark=(char *)NULL;
  reset();
}


void DVfs::reset()
{
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"DVfs::reset()...\n";
#endif // DEBUG_VERBOSE
  diskNum=0;
  diskError=false;
  readSize=0;  writeSize=0;
  if(!disk[0]) {
#ifdef DEBUG_VERBOSE
    DEBUG_OUT<<"Going to Mount NULL...\n";
#endif // DEBUG_VERBOSE
    mount((char *)NULL,0);
  }
}


void DVfs::disks()
{
  DEBUG_OUT<<"Disks:\n";
  for(unsigned int t=0;t<DDISK_NUM_DISKS;t++) {
    DEBUG_OUT<<t<<": ";
    if(disk[t]) DEBUG_OUT<<disk[t]->getDiskName()<<" (type is "<<disk[t]->getDiskType()<<")";
    else DEBUG_OUT<<"No Disk";
    // TODO: print curDir here...
    DEBUG_OUT<<"\n";
  }
}


// NOTE: Should this function be removed?  Doesn't Mount() do what this was meant to?
bool DVfs::recognize(const char *fname)
{
  if(!fname) return false;
  DEBUG_OUT<<"DVfs::recognize not implemented\n";
  return false;
}


// NOTE: name can be NULL...
// NOTE: Be sure to add any other formats here...
void DVfs::mount(const char *name,unsigned int num)
{
  num=verifyDiskNum(num);
  if(!name) {
    unmount(num);
#ifdef DEBUG_VERBOSE
    DEBUG_OUT<<"Mount: Going to create new DNativeFilesystem...\n";
#endif // DEBUG_VERBOSE
    disk[num]=new DNativeFilesystem(this);
    diskNum=num;
    return;
  }
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"Attempting to Mount "<<name<<"\n";
#endif // DEBUG_VERBOSE
  //
  FILE *f=fopen(name,"rb");
  if(!f) { DEBUG_OUT<<"recognize couldn't open file!\n"; return; }
  struct stat mystat;
  stat(name,&mystat);
  unsigned int fsize=mystat.st_size;
  unsigned int readsize=100*1024;  // read only first 100k
  BYTE *tbuf=(BYTE *)malloc(readsize);
  if(!tbuf) {
    ::fclose(f); DEBUG_OUT<<"recognize couldn't get tbuf!\n"; return;
  }
  unsigned int nread=::fread(tbuf,1,readsize,f);
  DDiskAbstract *d=(DDiskAbstract *)NULL;
  //
  if(!d) { if(DPartitionTableAmy::recognize(tbuf,f,fsize))  d=new DPartitionTableAmy(this,name); }
  if(!d) { if(DPartitionTableHfs::recognize(tbuf,f,fsize))  d=new DPartitionTableHfs(this,name); }
  if(!d) { if(DPartitionTableMbr::recognize(tbuf,f,fsize))  d=new DPartitionTableMbr(this,name); }
  //
  if(!d) { if(DImageAmy::recognize(tbuf,f,fsize))  d=new DImageAmy(this,name); }
  if(!d) { if(DImageFat::recognize(tbuf,f,fsize))  d=new DImageFat(this,name); }
  if(!d) { if(DImageAtari::recognize(tbuf,f,fsize))  d=new DImageAtari(this,name); }
  if(!d) { if(DImageTrs80::recognize(tbuf,f,fsize))  d=new DImageTrs80(this,name); }
  // We do these last because they don't have well implemented recognizers...
  if(!d) { if(DImage1541::recognize(tbuf,f,fsize))  d=new DImage1541(this,name); }
  if(!d) { if(DImageCpm::recognize(tbuf,f,fsize))  d=new DImageCpm(this,name); }
  if(!d) { if(DImageApple2::recognize(tbuf,f,fsize))  d=new DImageApple2(this,name); }
  if(!d) { if(DImageHfs::recognize(tbuf,f,fsize))  d=new DImageHfs(this,name); }
  if(!d) { if(DImageAcorn::recognize(tbuf,f,fsize))  d=new DImageAcorn(this,name); }
  //
  ::fclose(f);
  if(d) {
    if(d->err.error()) {
      d->which();
      DEBUG_OUT<<"Mount error!\n";
      delete d;
      d=(DDiskAbstract *)NULL;
    }
    else {
      DEBUG_OUT<<"Mounting "<<d->which()<<"image file...\n";
      unmount(num);  // (Old disk)
#ifdef DEBUG_VERBOSE
      DEBUG_OUT<<"Mount was successful.\n";
#endif // DEBUG_VERBOSE
      if(disk[num]) delete disk[num];
      disk[num]=d;
      diskNum=num;
    }
  }
  else {
    std::cerr<<"Couldn't read/recognize disk.\n";
  }
}


void DVfs::unmount(unsigned int num)
{
  num=verifyDiskNum(num);
  diskNum=num;
  if(disk[num]) {
    delete disk[num];
    disk[num]=(DDiskAbstract *)NULL;
    DEBUG_OUT<<"Unmounted disk "<<num<<"\n";
    if(num==0) {
#ifdef DEBUG_VERBOSE
      DEBUG_OUT<<"Unmount: Going to create new DNativeFilesystem...\n";
#endif // DEBUG_VERBOSE
      disk[0]=new DNativeFilesystem(this);
    }
  }
  else {
    //if(w) *w<<"No disk in drive "<<num<<"\n";
#ifdef DEBUG_VERBOSE
    DEBUG_OUT<<"unMount: No disk in drive "<<num<<"\n";
#endif // DEBUG_VERBOSE
  }
}


/*
unsigned int DVfs::Load(char *filename,ULONG loc,ULONG off,unsigned int maxBytes)
{
  if(!filename) return 0;
  diskError=false;
  if(disk[diskNum]) {
    if(w) *w<<"Attempting to load "<<filename<<"\n";
    if(w) *w<<"at "<<loc<<"\n";
    // NOTE: fopen, etc are OUR versions, not the standard C ones...
    void *fd=fopen(filename,"rb"));
    if(!fd) {
      if(w) *w<<"DVfs::load Can't open file "<<filename<<"!\n";
      return 0;
    }
    //ULONG tfsize=getSize(filename);
    ULONG tfsize=getFileSize(filename);
    if(!tfsize) {
      if(w) {
        *w<<"Got a size of zero for "<<filename;
        *w<<", probably a broken fseek...\n";
      }
      else {
        DEBUG_OUT<<"Got a size of zero for "<<filename;
        DEBUG_OUT<<", probably a broken fseek...\n";
      }
      return 0;
    }
    //if(w) *w<<"curSeek is "<<ftell(fd)<<", should be 0\n";
    DEBUG_OUT<<"offset is "<<off<<"...\n";
    //for(unsigned int t=0;t<off;t++) fread(&c,1,1,fd);
    fseek(fd,off,SEEK_SET);
    //if(w) *w<<"curSeek is "<<ftell(fd)<<", should be "<<off<<"\n";
    unsigned int rsize=tfsize-off;
    if(rsize>maxBytes) rsize=maxBytes;
    readMany(fd,loc,rsize);
    //if(w) *w<<"curSeek is "<<ftell(fd)<<", should be "<<(off+rsize)<<"\n";
    fclose(fd);
    return rsize;
  }
  else noSuchDisk();
  return 0;
}
*/


/*
unsigned int DVfs::Save(char *filename,ULONG loc,unsigned int size)
{
  if(w) *w<<"save not implemented for this dos.\n";
  else DEBUG_OUT<<"save not implemented for this dos.\n";
  verifyDiskNum();
  diskError=false;
  if(disk[diskNum]) return disk[diskNum]->Save(filename,loc,size);
  else noSuchDisk();
  return 0;
}
*/


void DVfs::ls(const char *dname)
{
  verifyDiskNum();
  diskError=false;
  if(disk[diskNum]) disk[diskNum]->ls(dname);
  else noSuchDisk();
}


bool DVfs::Chdir(const char *dname)
{
  if(!dname) return false;
  verifyDiskNum();
  diskError=false;
  if(disk[diskNum]) return disk[diskNum]->Chdir(dname);
  else noSuchDisk();
  return false;
}


void *DVfs::Fopen(const char *filename,const char *mode)
{
  if(!filename) return NULL;
  if(!mode) return NULL;
  verifyDiskNum();
  diskError=false;
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"DVfs::fopen "<<filename<<"...\n";
#endif // DEBUG_VERBOSE
  if(disk[diskNum]) return disk[diskNum]->Fopen(filename,mode);
  else noSuchDisk();
  return NULL;
}


unsigned int DVfs::Fclose(void *fp)
{
  if(!fp) return 0;
  verifyDiskNum();
  diskError=false;
  if(disk[diskNum]) return disk[diskNum]->Fclose(fp);
  else noSuchDisk();
  return 0;
}


ULONG DVfs::readMany(void *fp,ULONG addr,ULONG n)
{
  verifyDiskNum();
  diskError=false;
  if(disk[diskNum]) return disk[diskNum]->readMany(fp,addr,n);
  else noSuchDisk();
  return 0;
}


ULONG DVfs::Fread(void *ptr,ULONG sizelem,ULONG n,void *fp)
{
  verifyDiskNum();
  diskError=false;
  if(disk[diskNum]) return disk[diskNum]->Fread(ptr,sizelem,n,fp);
  else noSuchDisk();
  return 0;
}


ULONG DVfs::Fwrite(void *ptr,ULONG sizelem,ULONG n,void *fp)
{
  verifyDiskNum();
  diskError=false;
  if(disk[diskNum]) return disk[diskNum]->Fwrite(ptr,sizelem,n,fp);
  else noSuchDisk();
  return 0;
}


ULONG DVfs::Fseek(void *fp,long offset,int origin)
{
  verifyDiskNum();
  diskError=false;
  if(disk[diskNum]) return disk[diskNum]->Fseek(fp,offset,origin);
  else noSuchDisk();
  return 0;
}


ULONG DVfs::Ftell(void *fp)
{
  verifyDiskNum();
  diskError=false;
  if(disk[diskNum]) return disk[diskNum]->Ftell(fp);
  else noSuchDisk();
  return 0;
}


int DVfs::Fstat(int fd,struct awstat_struct *st)
{
  verifyDiskNum();
  diskError=false;
  if(disk[diskNum]) return disk[diskNum]->Fstat(fd,st);
  else noSuchDisk();
  return -1;
}


int DVfs::Stat(const char *filename,struct awstat_struct *st)
{
  verifyDiskNum();
  diskError=false;
  if(disk[diskNum]) return disk[diskNum]->Stat(filename,st);
  else noSuchDisk();
  return -1;
}


// TODO: we return NULL on fail, not -1 as the standard open() does...
// In extremely rare cases this could cause a problem...
// but since the only reason we have unix-style file io is for
// EResource's benefit, it shouldn't matter...
void *DVfs::Open(const char *name,unsigned int mode)
{
  if(!name) return NULL;
  verifyDiskNum();
  diskError=false;
  if(disk[diskNum]) return disk[diskNum]->Open(name,mode);
  else noSuchDisk();
  return 0;
}


unsigned int DVfs::Close(void *fp)
{
  verifyDiskNum();
  diskError=false;
  if(disk[diskNum]) return disk[diskNum]->Close(fp);
  else noSuchDisk();
  return 0;
}


ULONG DVfs::Read(void *fp,void *ptr,ULONG n)
{
  verifyDiskNum();
  diskError=false;
  if(disk[diskNum]) return disk[diskNum]->Read(fp,ptr,n);
  else noSuchDisk();
  return 0;
}


ULONG DVfs::Write(void *fp,void *ptr,ULONG n)
{
  verifyDiskNum();
  diskError=false;
  if(disk[diskNum]) return disk[diskNum]->Write(fp,ptr,n);
  else noSuchDisk();
  return 0;
}


ULONG DVfs::Lseek(void *fp,long offset,int origin)
{
  verifyDiskNum();
  diskError=false;
  if(disk[diskNum]) return disk[diskNum]->Lseek(fp,offset,origin);
  else noSuchDisk();
  return 0;
}


bool DVfs::isDir(const char *filename)
{
  if(!filename) return false;
  verifyDiskNum();
  diskError=false;
  if(disk[diskNum]) return disk[diskNum]->isDir(filename);
  else noSuchDisk();
  return 0;
}


bool DVfs::isSpecial(const char *filename)
{
  if(!filename) return false;
  verifyDiskNum();
  diskError=false;
  if(disk[diskNum]) return disk[diskNum]->isSpecial(filename);
  else noSuchDisk();
  return 0;
}


DDirEntry *DVfs::SearchForFirstFile(const char *fname)
{
  verifyDiskNum();
  diskError=false;
  if(disk[diskNum]) return disk[diskNum]->SearchForFirstFile(fname);
  else noSuchDisk();
  return (DDirEntry *)NULL;
}


DDirEntry *DVfs::SearchForNextFile()
{
  verifyDiskNum();
  diskError=false;
  if(disk[diskNum]) return disk[diskNum]->SearchForNextFile();
  else noSuchDisk();
  return (DDirEntry *)NULL;
}


const char *DVfs::SearchForFirst(const char *fname)
{
  verifyDiskNum();
  diskError=false;
  if(disk[diskNum]) return disk[diskNum]->SearchForFirst(fname);
  else noSuchDisk();
  return (char *)NULL;
}


const char *DVfs::SearchForNext()
{
  verifyDiskNum();
  diskError=false;
  if(disk[diskNum]) return disk[diskNum]->SearchForNext();
  else noSuchDisk();
  return (char *)NULL;
}


unsigned int DVfs::getFileVersionMajor(const char *filename)
{
  if(!filename) return 0;
  verifyDiskNum();
  diskError=false;
  if(disk[diskNum]) return disk[diskNum]->getFileVersionMajor(filename);
  else noSuchDisk();
  return 0;
}


unsigned int DVfs::getFileVersionMinor(const char *filename)
{
  if(!filename) return 0;
  verifyDiskNum();
  diskError=false;
  if(disk[diskNum]) return disk[diskNum]->getFileVersionMinor(filename);
  else noSuchDisk();
  return 0;
}


ULONG DVfs::getFileSize(const char *filename)
{
  if(!filename) return 0;
  verifyDiskNum();
  diskError=false;
  if(disk[diskNum]) return disk[diskNum]->getFileSize(filename);
  else noSuchDisk();
  return 0;
}


void DVfs::refresh()
{
  verifyDiskNum();
  diskError=false;
  if(disk[diskNum]) disk[diskNum]->refresh();
  else noSuchDisk();
  return;
}


bool DVfs::getDiskError()
{
  verifyDiskNum();
  bool ret=diskError;
  for(unsigned int t=0;t<DDISK_NUM_DISKS;t++) {
    if(disk[t]) {
      if(disk[t]->getDiskError()) ret=true;
    }
  }
  return ret;
}


void DVfs::setDiskNum(unsigned int num)
{
  diskNum=num;
  verifyDiskNum();
}


unsigned int DVfs::verifyDiskNum()
{
  return verifyDiskNum(diskNum);
}


unsigned int DVfs::verifyDiskNum(unsigned int maybeNum)
{
  unsigned int ret=maybeNum;
  if(ret>DDISK_NUM_DISKS) ret=0;
  if(!disk[ret]) {
    ret=0;
    //*w<<"No such disk #"<<diskNum<<"!\n";
    diskError=true;
  }
  return ret;
}


bool DVfs::systemCall(ULONG loc)
{
  DEBUG_OUT<<"systemCall not implemented for this dos.\n";
  return false;
}


void DVfs::readTrackSector(unsigned int n1,unsigned int n2)
{
  verifyDiskNum();
  diskError=false;
  if(disk[diskNum]) disk[diskNum]->readTrackSector(n1,n2);
  else noSuchDisk();
}


void DVfs::readBlock(unsigned int n1)
{
  verifyDiskNum();
  diskError=false;
  if(disk[diskNum]) disk[diskNum]->readBlock(n1);
  else noSuchDisk();
}


void DVfs::writeTrackSector(unsigned int n1,unsigned int n2)
{
  verifyDiskNum();
  diskError=false;
  if(disk[diskNum]) disk[diskNum]->writeTrackSector(n1,n2);
  else noSuchDisk();
}


void DVfs::writeBlock(unsigned int n1)
{
  verifyDiskNum();
  diskError=false;
  if(disk[diskNum]) disk[diskNum]->writeBlock(n1);
  else noSuchDisk();
}


void DVfs::markBlockUsed(unsigned int n1)
{
  verifyDiskNum();
  diskError=false;
  if(disk[diskNum]) disk[diskNum]->markBlockUsed(n1);
  else noSuchDisk();
}


void DVfs::markBlockUnused(unsigned int n1)
{
  verifyDiskNum();
  diskError=false;
  if(disk[diskNum]) disk[diskNum]->markBlockUnused(n1);
  else noSuchDisk();
}


BYTE *DVfs::getBuffer()
{
  verifyDiskNum();
  diskError=false;
  if(disk[diskNum]) return disk[diskNum]->getBuffer();
  else noSuchDisk();
  return (BYTE *)NULL;
}


unsigned int DVfs::getBlockSize()
{
  verifyDiskNum();
  diskError=false;
  if(disk[diskNum]) return disk[diskNum]->getBlockSize();
  else noSuchDisk();
  return 0;
}


AList *DVfs::getTheDir()
{
  verifyDiskNum();
  diskError=false;
  if(disk[diskNum]) return disk[diskNum]->getTheDir();
  else noSuchDisk();
  return (AList *)NULL;
}


DDiskAbstract *DVfs::getDisk(unsigned int num)
{
  verifyDiskNum();
  diskError=false;
  if(disk[diskNum]) return disk[diskNum];
  else noSuchDisk();
  return (DDiskAbstract *)NULL;
}


const char *DVfs::getDiskName()
{
  verifyDiskNum();
  diskError=false;
  if(disk[diskNum]) return disk[diskNum]->getDiskName();
  else noSuchDisk();
  return (char *)NULL;
}


const char *DVfs::getDiskID()
{
  verifyDiskNum();
  diskError=false;
  if(disk[diskNum]) return disk[diskNum]->getDiskID();
  else noSuchDisk();
  return (char *)NULL;
}


const char *DVfs::getShortName(const char *longName)
{
  verifyDiskNum();
  diskError=false;
  if(disk[diskNum]) return disk[diskNum]->getShortName(longName);
  else noSuchDisk();
  return (char *)NULL;
}


// Return the CBM-style load address..
// TODO: figure out where this belongs!
unsigned int DVfs::getStartAddress(const char *filename)
{
  if(!filename) return 0;
  diskError=false;
  if(disk[diskNum]) {
    // NOTE: fopen, etc are OUR versions, not the standard C ones...
    void *fd=Fopen(filename,"rb");
    if(!fd) {
      return 0;
    }
    BYTE l,h;
    Fread(&l,1,1,fd);
    Fread(&h,1,1,fd);
    unsigned int saddr=(h*256)+l;
    Fclose(fd);
    return saddr;
  }
  else noSuchDisk();
  return 0;
}


void DVfs::noSuchDisk()
{
  std::cerr<<"No such disk #!";
  diskError=true;
}


unsigned int DVfs::getCurTrack()
{
  verifyDiskNum();
  diskError=false;
  if(disk[diskNum]) return disk[diskNum]->getCurTrack();
  else noSuchDisk();
  return 0;
}


unsigned int DVfs::getCurSector()
{
  verifyDiskNum();
  diskError=false;
  if(disk[diskNum]) return disk[diskNum]->getCurSector();
  else noSuchDisk();
  return 0;
}


unsigned int DVfs::getCurSide()
{
  verifyDiskNum();
  diskError=false;
  if(disk[diskNum]) return disk[diskNum]->getCurSide();
  else noSuchDisk();
  return 0;
}


bool DVfs::getWriteProtected()
{
  verifyDiskNum();
  diskError=false;
  if(disk[diskNum]) return disk[diskNum]->getWriteProtected();
  else noSuchDisk();
  return true;
}


const char *DVfs::getDiskType()
{
  verifyDiskNum();
  diskError=false;
  if(disk[diskNum]) return disk[diskNum]->getDiskType();
  else noSuchDisk();
  return (char *)NULL;
}


#ifdef USE_FINDER_DAT
void DVfs::setIgnoreFinderDat(bool t)
{
  ignoreFinderDat=t;
  refresh();
  verifyDiskNum();
  diskError=false;
  if(disk[diskNum]) disk[diskNum]->setIgnoreFinderDat(t);
  else noSuchDisk();
}
#endif // USE_FINDER_DAT


bool DVfs::CopyFile(const char *oldname,const char *newname)
{
  if(!newname) return false;
  if(!oldname) return false;
  verifyDiskNum();
  diskError=false;
  if(disk[diskNum]) return disk[diskNum]->CopyFile(oldname,newname);
  else noSuchDisk();
  return false;
}


bool DVfs::Rename(const char *oldname,const char *newname)
{
  if(!newname) return false;
  if(!oldname) return false;
  verifyDiskNum();
  diskError=false;
  if(disk[diskNum]) return disk[diskNum]->Rename(oldname,newname);
  else noSuchDisk();
  return false;
}


bool DVfs::Remove(const char *filename)
{
  if(!filename) return false;
  verifyDiskNum();
  diskError=false;
  if(disk[diskNum]) return disk[diskNum]->Remove(filename);
  else noSuchDisk();
  return false;
}


const char *DVfs::Getcwd(char *buf,unsigned int len)
{
  if(!buf) return NULL;
  verifyDiskNum();
  diskError=false;
  if(disk[diskNum]) return disk[diskNum]->Getcwd(buf,len);
  else noSuchDisk();
  return NULL;
}


bool DVfs::gotoBookmark()
{
  bool ret=false;
  if(bookmark) Chdir(bookmark);
  return ret;
}


bool DVfs::bookmarkDir()
{
  bool ret=false;
  if(bookmark) { free(bookmark); bookmark=(char *)NULL; }
  bookmark=(char *)malloc(DDISK_MAX_PATHLEN);
  char *err=(char *)NULL;
#ifdef UNICODE
  char *tempy=(char *)malloc(DDISK_MAX_PATHLEN);
  if(tempy) {
#ifdef ALIB_HAVE_GETCWD
    err=(char *)getcwd(tempy,DDISK_MAX_PATHLEN);
#endif
    strcpy(bookmark,tempy);
    free(tempy);
  }
  else err="foo";
#else
#ifdef ALIB_HAVE_GETCWD
  err=(char *)getcwd((char *)bookmark,DDISK_MAX_PATHLEN);
#endif
#endif // UNICODE
  if(err) return true;
  return ret;
}



