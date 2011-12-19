
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


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
#include <disklib/DDiskImageAbstract.h>
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


////////////////////////////////////////////////////////////////////////////////
//  DDiskImageAbstract Class
////////////////////////////////////////////////////////////////////////////////


DDiskImageAbstract::DDiskImageAbstract(DVfs *parent) : DDiskAbstract(parent)
{
  init();
  dos=parent;
  constructorHelper();
}


DDiskImageAbstract::DDiskImageAbstract(DVfs *parent,const char *fname) : DDiskAbstract(parent)
{
  init();
  dos=parent;
  constructorHelper();
  //MountDiskFromFile(fname);
}


DDiskImageAbstract::~DDiskImageAbstract()
{
  // TODO: flush files if cached
  freeDirCache();
  diskMounted=false;
}


void DDiskImageAbstract::init()
{
  DDiskAbstract::init();
  curLoc=0;
  // These next 3 might not be used, depending on the format...
  curTrack=0; curSector=0; curSide=0;
  diskError=false;
  char *tname="Generic Untitled Disk Image";
  unsigned int t=0;
  for(t=0;t<strlen(tname);t++) diskName[t]=tname[t];
  diskMounted=false;
  writeProtected=true;
  skipOffset=0;
  for(t=0;t<EDISK_BUF_SIZE;t++) buf[t]=0;
  offsetToTrack=(unsigned int *)NULL;
  numSectorsPerTrack=(unsigned int *)NULL;
  cachesData=true;
  cachesFiles=true;
  cachesDir=true;
}


void DDiskImageAbstract::readBlock(unsigned int blk)
{
  std::cerr<<"DDiskImageAbstract::readBlock not implemented!\n";
  diskError=true;
}


void DDiskImageAbstract::readTrackSector(unsigned int tt,unsigned int ss)
{
  DEBUG_OUT<<"DDiskImageAbstractMem::readTrackSector("<<tt<<","<<ss<<")\n";
  diskError=true;
  curTrack=tt;  curSector=ss;  curSide=0;
  if(!diskMounted) {
    std::cerr<<"No disk!\n";
    return;
  }
  if(curTrack>numTracks) {
    DEBUG_OUT<<"tt="<<tt<<" numTracks="<<numTracks<<"\n";
    std::cerr<<"Track too high!\n";
    return;
  }
  if((!curTrack)&&noTrackZero) {
    std::cerr<<"No track zero!\n";
    return;
  }
  if((!curSector)&&noSectorZero) {
    std::cerr<<"No sector zero!\n";
    return;
  }
  unsigned int blk=0;
  unsigned int ttrack=curTrack,tsector=curSector,tMaxSector=numSectors;
  if(noTrackZero) ttrack--;  // NOTE: We already checked it wasn't zero in this case...
  if(noSectorZero) tsector--;  // NOTE: We already checked it wasn't zero in this case...
  //else tMaxSector++;
  if(allTracksSameSectors) {
    if(tsector>numSectors) {
      DEBUG_OUT<<"Attempted to read sector "<<tsector<<"...too high!\n";
      return;
    }
    blk=tsector+((tMaxSector)*ttrack);
    DEBUG_OUT<<"tsector="<<tsector<<" tMaxSector="<<tMaxSector<<" ttrack="<<ttrack<<"\n";
  }
  else {
    if(!numSectorsPerTrack) { std::cerr<<"No numSectorsPerTrack!\n"; return; }
    if(!offsetToTrack) { std::cerr<<"No offsetToTrack!\n"; return; }
    if(numSectorsPerTrack[ttrack]<tsector) {
      std::cerr<<"Sector too high!\n"; return;
    }
    blk=tsector+offsetToTrack[ttrack];
  }
  DEBUG_OUT<<"(Reading "<<curTrack<<","<<curSector<<")\n";
  readBlock(blk);
  diskError=false;
}


void DDiskImageAbstract::writeBlock(unsigned int blk)
{
  std::cerr<<"DDiskImageAbstract::writeBlock not implemented!\n";
  diskError=true;
}


void DDiskImageAbstract::writeTrackSector(unsigned int tt,unsigned int ss)
{
  std::cerr<<"DDiskImageAbstract::writeTrackSector not implemented!\n";
  diskError=true;
}


void *DDiskImageAbstract::Fopen(const char *filename,const char *mode)
{
  diskError=false;
  void *ret=NULL;
  DEBUG_OUT<<"Fopen("<<filename<<")...\n";
  if(!diskMounted) {
    std::cerr<<"DDiskImageAbstract::load no disk in drive!\n";
    diskError=true;
    return NULL;
  }
  if(noFileSystem) {
    std::cerr<<"DDiskImageAbstract::fopen Non-DOS Disk!\n";
    return NULL;
  }
  const char *thename=SearchForFirst(filename);
  if(!thename) {
    std::cerr<<"DDiskImageAbstract::fopen SearchForFirst returned NULL!\n";
    return NULL;
  }
  DDirEntry *aFile=SearchForFirstFile(thename);
  if(!aFile) {
    std::cerr<<"DDiskImageAbstract::fopen SearchForFirstFile returned NULL!\n";
    return NULL;
  }
  cacheFile(aFile);
  aFile->curSeek=0;
  return (void *)aFile;
}


unsigned int DDiskImageAbstract::Fclose(void *fp)
{
  DDirEntry *aFile=(DDirEntry *)fp;
  aFile->curSeek=0;
  return 0;
}


ULONG DDiskImageAbstract::Fread(void *ptr,ULONG sizelem,ULONG n,void *fp)
{
  if(!fp) return 0;
  if(!ptr) return 0;
  DDirEntry *f=(DDirEntry *)fp;
  if(!f->data) cacheFile(fp);
  if(!f->data) return 0;  // we tried...
  BYTE *p=(BYTE *)ptr;
  ULONG rsize=sizelem*n;
  //if(w) *w<<"Going to fread "<<rsize<<"\n";
  if(!rsize) return 0;
  for(ULONG t=0;t<rsize;t++) {
    p[t]=f->data[f->curSeek+t];
  }
  f->curSeek+=rsize;
  return n;
  // TODO: Check that we actually read that much and return accordingly
}


ULONG DDiskImageAbstract::Fwrite(void *ptr,ULONG sizelem,ULONG n,void *fp)
{
  // TODO: writing isn't implemented!
  return 0;
}


ULONG DDiskImageAbstract::Fseek(void *fp,long offset,int origin)
{
  DDirEntry *aFile=(DDirEntry *)fp;
  unsigned int seekOffset=0;
  switch(origin) {
    case SEEK_SET:
      seekOffset=offset;
      break;
    case SEEK_END:
      seekOffset=aFile->size+offset;
      break;
    case SEEK_CUR:
      seekOffset=aFile->curSeek+offset;
      break;
    default:
      break;
  }
  aFile->curSeek=seekOffset;
  return 0;
}


ULONG DDiskImageAbstract::Ftell(void *fp)
{
  DDirEntry *aFile=(DDirEntry *)fp;
  return aFile->curSeek;
}


int DDiskImageAbstract::Fstat(int fd,struct awstat_struct *st)
{
  // TODO: implement this
  return -1;
}


int DDiskImageAbstract::Stat(const char *filename,struct awstat_struct *st)
{
  // TODO: implement this
  return -1;
}


void DDiskImageAbstract::readImageDirEntry(ULONG off)
{
  std::cerr<<"DDiskImageAbstract::readImageDirEntry not implemented!\n";
  diskError=true;
}
