
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


////////////////////////////////////////////////////////////////////////////////
//  DDiskAbstract Class
////////////////////////////////////////////////////////////////////////////////

DDiskAbstract::DDiskAbstract()
{
  init();
  constructorHelper();
}


DDiskAbstract::DDiskAbstract(DVfs *parent)
{
  init();
  dos=parent;
  constructorHelper();
}


DDiskAbstract::~DDiskAbstract()
{
#ifdef USE_FAKE_CHDIR
  if(prefixPath) free(prefixPath);
  prefixPath=(char *)NULL;
#endif // USE_FAKE_CHDIR
}


void DDiskAbstract::constructorHelper()
{
  // nothing here
}


void DDiskAbstract::init()
{
  dos=(DVfs *)NULL;
  curLoc=0;
  curTrack=0; curSector=0; curSide=0;
  diskError=false;
  writeProtected=true;  // For now everything is read-only
  allTracksSameSectors=true;
  noTrackZero=false;  noSectorZero=false;
  numTracks=0;  numSectors=0;  numSides=0;
  partitionTable=(DPartitionTable *)NULL;
  partitionNumber=0;
  unsigned int t=0;
  for(t=0;t<DDISK_MAX_NAMELEN;t++) diskName[t]=0;
  for(t=0;t<DDISK_MAX_NAMELEN;t++) diskID[t]=0;
  for(t=0;t<DDISK_MAX_NAMELEN;t++) matchName[t]=0;
  for(t=0;t<DDISK_MAX_NAMELEN;t++) curDir[t]=0;
#ifdef USE_FAKE_CHDIR
  prefixPath=(char *)NULL;
#endif // USE_FAKE_CHDIR
  blockSize=256;  // Just a default...
  for(unsigned int t=0;t<EDISK_MAX_SECTORS;t++) sectorSkew[t]=t;
  noFileSystem=false;
  cachesData=false;
  cachesFiles=false;
  cachesDir=false;
  doNotCacheDir=false;
  doNotCacheFiles=false;
#ifdef USE_FINDER_DAT
  ignoreFinderDat=false;
#endif // USE_FINDER_DAT
}


// NOTE: Remember to override this if you're caching the dir in a subclass!
void DDiskAbstract::readDirectory()
{
  cachesDir=false;
}


void DDiskAbstract::refresh()
{
  if(cachesDir) {
    freeDirCache();
    readDirectory();
  }
}


bool DDiskAbstract::isDir(const char *filename)
{
  DDirEntry *de=SearchForFirstFile(filename);
  if(de) {
    if(de->type==DDISK_TYPE_DIR) return true;
  }
  return false;
}


bool DDiskAbstract::isSpecial(const char *filename)
{
  DDirEntry *de=SearchForFirstFile(filename);
  if(de) {
    if(de->type==DDISK_TYPE_DIR) return true;
    if(de->type==DDISK_TYPE_DEVICE) return true;
  }
  return false;
}


// NOTE: See below function, which is the version for real files on the host fs
ULONG DDiskAbstract::getFileSize(const char *filename)
{
  bool done=false;
  ULONG rval=0;
#ifdef ALIB_HAVE_STAT
  struct awstat_struct s;
  if(!Stat(filename,&s)) {
    rval=s.st_size;
    done=true;
  }
#endif // ALIB_HAVE_STAT
  if(!done) {
    FILE *fd=(FILE *)Fopen(filename,"rb");
    if(fd) {
      Fseek(fd,0,SEEK_END);
      rval=Ftell(fd);
      Fclose(fd);
    }
  }
  return rval;
}


// NOTE: See above function, which is the version for files on this emulated fs
ULONG DDiskAbstract::getSize(const char *fname)
{
  bool done=false;
  ULONG rval=0;
  // TODO: use stat or equiv instead of seeking if we can here.
  if(!done) {
#ifdef USE_FAKE_CHDIR
    DEBUG_OUT<<"Warning! Couldn't fake path to find file size!!!\n";
    //fname=fakeFullPath(fname);
#endif // USE_FAKE_CHDIR
    FILE *fd=fopen(fname,"rb");
    if(fd) {
      fseek(fd,0,SEEK_END);
      rval=ftell(fd);
      fclose(fd);
    }
  }
  return rval;
}


unsigned int DDiskAbstract::getFileVersionMajor(const char *filename)
{
  unsigned int ret=0;
#ifndef ALIB_16BIT_INTS
  bool foundIt;
  // First try Mac style 'vers' resource
  EResourceFile mres(filename,dos);
  if(!mres.err.error()) {
    unsigned int rcount=mres.count("vers");
    if(rcount) {
      foundIt=true;
#ifdef DEBUG_VERBOSE
      DEBUG_OUT<<"Got a Mac vers resource...\n";
#endif // DEBUG_VERBOSE
      unsigned int num=0;
      if(rcount>1) num=2;  // look at 2nd vers if there's more than 1
      unsigned int rsize=mres.getSizeFromNum("vers",num);
#ifdef DEBUG_VERBOSE
      DEBUG_OUT<<"size is "<<rsize<<"\n";
#endif // DEBUG_VERBOSE
      BYTE *buf=(BYTE *)malloc(rsize*sizeof(BYTE));
      mres.readFromNum("vers",num,buf);
      if(err.error()) DEBUG_OUT<<"Something went wrong...\n";
#ifdef DEBUG_VERBOSE
      else DEBUG_OUT<<"Looks like it read fine.\n";
#endif // DEBUG_VERBOSE
      free(buf);
    }
  }
  // TODO: now try a Windows style version resource
#endif // ALIB_16BIT_INTS
  return ret;
}


unsigned int DDiskAbstract::getFileVersionMinor(const char *filename)
{
  DEBUG_OUT<<"DDiskAbstract::getFileVersionMinor not implemented!!!\n";
  return 0;
}


const char *DDiskAbstract::SearchForFirst(const char *fname)
{
  return SearchForFirstCached(fname);
}


const char *DDiskAbstract::SearchForFirstCached(const char *fname)
{
  if(!cachesDir) {
    DEBUG_OUT<<"DDiskAbstract::SearchForFirstCached can't work if dir isn't cached!\n";
    return (char *)NULL;
  }
  theDir.reset();
  if(!fname) fname="*";
  // TODO: problem here if fname is longer than DDISK_MAX_NAMELEN chars
  strcpy(matchName,fname);
  return SearchForNext();
}


const char *DDiskAbstract::SearchForNext()
{
  return SearchForNextCached();
}


// NOTE: don't TOUCH the dir list if you expect to use this...
const char *DDiskAbstract::SearchForNextCached()
{
  if(!cachesDir) {
    DEBUG_OUT<<"DDiskAbstract::SearchForNextCached can't work if dir isn't cached!\n";
    return (char *)NULL;
  }
  DDirEntry *de=SearchForNextFile();
  if(de) return de->name;
  else return (char *)NULL;
}


DDirEntry *DDiskAbstract::SearchForFirstFile(const char *fname)
{
  return SearchForFirstFileCached(fname);
}


DDirEntry *DDiskAbstract::SearchForFirstFileCached(const char *fname)
{
  if(!cachesDir) {
    DEBUG_OUT<<"DDiskAbstract::SearchForFirstFileCahced can't work if dir isn't cached!\n";
    return (DDirEntry *)NULL;
  }
  theDir.reset();
  if(!fname) fname="*";
  // TODO: problem here if fname is longer than DDISK_MAX_NAMELEN chars
  strcpy(matchName,fname);
  return SearchForNextFile();
}


DDirEntry *DDiskAbstract::SearchForNextFile()
{
  return SearchForNextFileCached();
}


// NOTE: don't TOUCH the dir list if you expect to use this...
DDirEntry *DDiskAbstract::SearchForNextFileCached()
{
  if(!cachesDir) {
    DEBUG_OUT<<"DDiskAbstract::SearchForNextFileCached can't work if dir isn't cached!\n";
    return (DDirEntry *)NULL;
  }
  DDirEntry *de=(DDirEntry *)NULL;
  DDirEntry *foundDe=(DDirEntry *)NULL;
  bool done=false;
  while(!done) {
    de=(DDirEntry *)theDir.info();
    if(de) {
      //DEBUG_OUT<<"findNextFile looking at "<<de->name<<"\n";
      unsigned int t=0;
      bool c=true,bad=false,wild=false;
      while(c) {
        //DEBUG_OUT<<t<<" comparing "<<matchName[t]<<" and "<<de->name[t]<<"\n";
        if(matchName[t]!=de->name[t]) {
          // the letters didn't match, but we might have a wild card
          bad=true;  wild=false;
          if(matchName[t]=='*') { bad=false; wild=true; }
          if(matchName[t]=='?') { bad=false; wild=true; }
          if(bad) c=false;
        }
        t++;
        if(t==DDISK_MAX_NAMELEN) c=false;  // hit max file len
        if(t==strlen(matchName)) {
          // we're past the end of the name we're using to match
          if(strlen(de->name)>strlen(matchName)) {
            // if the last letter we looked at wasn't wild, we failed
            if(!wild) bad=true;
          }
          c=false;
        }
        if(t==strlen(de->name)) {
          // we're past the end of the name we're looking at
          if(strlen(de->name)<strlen(matchName)) {
            bad=true;
            // if the letter past what we looked at wasn't wild, we failed
            if(matchName[t]=='*') { bad=false; wild=true; }
            if(matchName[t]=='?') { bad=false; wild=true; }
          }
          c=false;
        }
      }
      if(!bad) { foundDe=de; done=true; }
      theDir.advance();
      if(theDir.atEnd()) done=true;
    }
    else done=true;
  }
  //if(foundDe) DEBUG_OUT<<foundDe->name<<" looks good!\n";
  return foundDe;
}


void DDiskAbstract::ls(const char *dname)
{
  if(noFileSystem) {
    DEBUG_OUT<<"DDiskAbstract::ls dir isn't cached!\n";
    return;
  }
  if(!cachesDir) {
    DEBUG_OUT<<"DDiskAbstract::ls dir isn't cached!\n";
    return;
  }
  if(dname) Chdir(dname);
  if(diskName[0]) DEBUG_OUT<<"Disk: "<<diskName<<"\n";
#ifdef USE_FAKE_CHDIR
  if(prefixPath) DEBUG_OUT<<prefixPath<<"\n";
#endif // USE_FAKE_CHDIR
  theDir.reset();
  DDirEntry *aFile=(DDirEntry *)NULL;
  aFile=(DDirEntry *)theDir.info();
  bool someFiles=false;
  char tchar=' ';
  while(aFile) {
    someFiles=true;
    ULONG afsize=aFile->size;
    ULONG afbsize=aFile->sizeBlocks;
    DEBUG_OUT<<afsize;
    if(afsize<100) DEBUG_OUT<<" ";
    if(afsize<10) DEBUG_OUT<<" ";
    DEBUG_OUT<<"  ";
    DEBUG_OUT<<aFile->name;
    tchar=' ';
    if(aFile->type==DDISK_TYPE_DIR) tchar='/';
    DEBUG_OUT<<tchar;
    unsigned int afnlen=strlen(aFile->name);
    DEBUG_OUT<<"\n";
    theDir.advance();
    aFile=(DDirEntry *)theDir.info();
  }
  if(!someFiles) DEBUG_OUT<<"No files.";
  DEBUG_OUT<<"\n";
}


bool DDiskAbstract::Chdir(const char *dname)
{
  if(!dname) return false;
#ifdef USE_FAKE_CHDIR
  if(prefixPath) free(prefixPath);
  prefixPath=strdup(dname);
  DEBUG_OUT<<"CWD is now: "<<prefixPath<<"\n";
  return true;
#else
  DEBUG_OUT<<"cd not implemented for this format!\n";
#endif // USE_FAKE_CHDIR
  return false;
}


const char *DDiskAbstract::Getcwd(const char *buf,unsigned int len)
{
#ifdef USE_FAKE_CHDIR
  awstrncpy(buf,prefixPath,len);
  return prefixPath;
#else
  DEBUG_OUT<<"getcwd not implemented for this format!\n";
  return (char *)NULL;
#endif // USE_FAKE_CHDIR
}


void *DDiskAbstract::Fopen(const char *filename,const char *mode)
{
  return (void *)NULL;
}


unsigned int DDiskAbstract::Fclose(void *fp)
{
  return 0;
}


ULONG DDiskAbstract::Fread(void *ptr,ULONG sizelem,ULONG n,void *fp)
{
  return 0;
}


ULONG DDiskAbstract::Fwrite(void *ptr,ULONG sizelem,ULONG n,void *fp)
{
  return 0;
}


ULONG DDiskAbstract::readMany(void *fp,ULONG addr,ULONG n)
{
  if(!dos) return 0;
  ULONG num=0;
  BYTE c;
  EMemMapAbstract *map=dos->getMap();
  for(ULONG t=0;t<n;t++) {
    num+=Fread(&c,1,1,(FILE *)fp);
    map->write8(addr+t,c);
  }
  return num;
}


ULONG DDiskAbstract::Fseek(void *fp,long offset,int origin)
{
  return 0;
}


ULONG DDiskAbstract::Ftell(void *fp)
{
  return 0;
}


int DDiskAbstract::Fstat(int fd,struct awstat_struct *st)
{
  // TODO: implement this
  return -1;
}


int DDiskAbstract::Stat(const char *filename,struct awstat_struct *st)
{
  // TODO: implement this
  return -1;
}


void *DDiskAbstract::Open(const char *name,unsigned int mode)
{
  char *cmode="rb";
  // TODO: We don't actually use mode, since we're read-only right now...
  return Fopen(name,cmode);
}


unsigned int DDiskAbstract::Close(void *fp)
{
  return Fclose(fp);
}


ULONG DDiskAbstract::Read(void *fp,void *ptr,ULONG n)
{
  return Fread(ptr,n,1,fp);
}


ULONG DDiskAbstract::Write(void *fp,void *ptr,ULONG n)
{
  return Fwrite(ptr,n,1,fp);
}


ULONG DDiskAbstract::Lseek(void *fp,long offset,int origin)
{
  return Fseek(fp,offset,origin);
}


unsigned int DDiskAbstract::popCount(BYTE q)
{
  unsigned int ret=0;
  if(q&0x80) ret++;
  if(q&0x40) ret++;
  if(q&0x20) ret++;
  if(q&0x10) ret++;
  if(q&0x08) ret++;
  if(q&0x04) ret++;
  if(q&0x02) ret++;
  if(q&0x01) ret++;
  return ret;
}


void DDiskAbstract::freeDirCache()
{
  theDir.reset();
  DDirEntry *e=(DDirEntry *)NULL;
  while(!theDir.atEnd()) {
    e=(DDirEntry *)theDir.info();
    //DEBUG_OUT<<"freeDirCache removing "<<e->name<<" from theDir...\n";
    theDir.remove();
    delete e;
  }
}


void DDiskAbstract::freeFileCache(void *fp)
{
}


void DDiskAbstract::cacheFile(void *fp)
{
}


const char *DDiskAbstract::getShortName(const char *longName)
{
  return longName;
}


#ifdef USE_FINDER_DAT
void DDiskAbstract::setIgnoreFinderDat(bool t)
{
  ignoreFinderDat=t;
  refresh();
}
#endif // USE_FINDER_DAT


bool DDiskAbstract::Rename(const char *oldname,const char *newname)
{
  return false;
}


bool DDiskAbstract::CopyFile(const char *oldname,const char *newname)
{
  return false;
}


bool DDiskAbstract::Remove(const char *filename)
{
  return false;
}


const char *DDiskAbstract::Getcwd(char *buf,unsigned int len)
{
  return NULL;
}


bool DDiskAbstract::removeAllFilesinFolder(const char *foldername)
{
  bool retVal=false;
  if(!isDir(foldername)) return false;
  bool ok=dos->Chdir(foldername);
  if(!ok) {
    DEBUG_OUT<<"Couldn't cd to "<<foldername<<"\n";
    return false;
  }
  AList *theDir=dos->getTheDir();
  if(!theDir) {
    DEBUG_OUT<<"Couldn't getTheDir!\n";
    return false;
  }
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"Going to start...\n";
#endif // DEBUG_VERBOSE
  theDir->reset();
  bool working=true;
  DDirEntry *e=(DDirEntry *)NULL;
  while(working) {
    if(theDir->atEnd()) working=false;
    else {
      e=(DDirEntry *)theDir->info();
      DEBUG_OUT<<e->name;
      bool ignore=false;
      if((e->type==DDISK_TYPE_DIR)||(e->type==DDISK_TYPE_FS)) {
        DEBUG_OUT<<"/";
        DEBUG_OUT<<"\n";
        bool doit=true;
        if(!strcmp(".",e->name)) doit=false;
        if(!strcmp("..",e->name)) doit=false;
        if(doit) {
#ifdef DEBUG_VERBOSE
          DEBUG_OUT<<"Descending into sub-dir...\n";
#endif // DEBUG_VERBOSE
          Remove(e->name);
        }
        else ignore=true;
      }
      if(!ignore) {
        dos->Remove(e->name);
      }
      DEBUG_OUT<<"\n";
      theDir->next();
    }
  }
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"Done deleting files.\n";
#endif // DEBUG_VERBOSE
  ok=dos->Chdir("..");
  return retVal;
}


#ifdef USE_FAKE_CHDIR
const char *DDiskAbstract::fakeFullPath(const char *name)
{
  if(!prefixPath) return name;
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"Building fullPathName:\n";
#endif // DEBUG_VERBOSE
  fullPathName[0]=0;
  AApp::fillInString(fullPathName,prefixPath);
  AApp::fillInString(fullPathName,"/"));
  AApp::fillInString(fullPathName,name);
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"fullPathName is "<<fullPathName<<"\n";
#endif // DEBUG_VERBOSE
  return fullPathName;
}
#endif // USE_FAKE_CHDIR




