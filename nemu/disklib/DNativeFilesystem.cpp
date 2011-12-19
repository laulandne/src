
//#define DEBUG_OUT std::cerr
#define DEBUG_OUT *aNullStream


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


#ifdef ASYS_DOS
#ifdef __SC__
#include <dos.h>
#endif
#endif // ASYS_DOS


////////////////////////////////////////////////////////////////////////////////
//  DNativeFilesystem Class
////////////////////////////////////////////////////////////////////////////////

DNativeFilesystem::DNativeFilesystem(DVfs *parent) : DDiskAbstract(parent)
{
  DEBUG_OUT<<"DNativeFilesystem::DNativeFilesystem...\n";
  dos=parent;
  constructorHelper();
#ifdef ASYS_WIN32CE
  chdir("/My Documents"));
#endif // ASYS_WIN32CE
}


DNativeFilesystem::~DNativeFilesystem()
{
}


void DNativeFilesystem::constructorHelper()
{
  curLoc=0;
  curTrack=0; curSector=0; curSide=0;
  diskError=false;
  // in all caps for those lowercase handicapped old OS's
  char *tname="HOST DIRECTORY";
  for(unsigned int t=0;t<strlen(tname);t++) diskName[t]=tname[t];
  //diskType="HOST FS");
#ifdef USE_FINDER_DAT
  // TODO: ignoreFinderDat is always false at this point...
  if(!ignoreFinderDat) {
    DEBUG_OUT<<"Going to look for a finder.dat...\n";
    gotFinderDat=false;
    FILE *finderDatfd=(FILE *)fopen("FINDER.DAT"),"rb"));
    if(!finderDatfd) finderDatfd=(FILE *)fopen("finder.dat"),"rb"));
    if(finderDatfd) {
      DEBUG_OUT<<"(Opened finder.dat, reading it...)\n";
      gotFinderDat=true;
      readFinderDat(finderDatfd);
      fclose(finderDatfd);
    }
  }
#endif // USE_FINDER_DAT
  cachesDir=true;
  if(cachesDir) readDirectory();
}


#ifdef USE_FINDER_DAT
// TODO: this reads past the end of the real data in fidner.dat
// may be a problem with the working flag
void DNativeFilesystem::readFinderDat(FILE *f)
{
  if(!f) { DEBUG_OUT<<"DNativeFilesystem::NULL f!\n"; return; }
  bool working=true;
  BYTE *buf=(BYTE *)malloc(92*sizeof(BYTE));
  DFinderDatEntry *en=(DFinderDatEntry *)NULL;
  if(!buf) return;
  DEBUG_OUT<<"(DHardDisk found a finder.dat, reading it in and using it)\n";
  while(working) {
    unsigned int ret=fread(buf,1,92,f);
    if(ret!=92) working=false;
    if(buf[0]&&working) {
      en=new DFinderDatEntry;
      if(en) {
        unsigned int t=0;
        unsigned int len=buf[0];
        if(len>92) len=92;
        for(t=0;t<len;t++) en->longName[t]=buf[t+1];
        en->longName[len]=0;
        for(t=0;t<4;t++) en->type[t]=buf[t+32];
        for(t=0;t<4;t++) en->creator[t]=buf[t+36];
        for(t=0;t<8;t++) en->shortName[t]=buf[t+80];
        en->shortName[8]='.';
        for(t=0;t<3;t++) en->shortName[t+9]=buf[t+88];
        en->shortName[12]=0;
        // get rid of any trailing spaces or dots
        bool nonSpace=false;
        for(t=11;t>8;t--) {
          if(en->shortName[t]==' ') en->shortName[t]=0;
          else nonSpace=true;
        }
        if(!nonSpace) en->shortName[8]=0;
        finderDat.append((void *)en);
        //DEBUG_OUT<<"'"<<en->longName<<"' -> '"<<en->shortName<<"'\n";
      }
    }
  }
}
#endif // USE_FINDER_DAT


// TODO: Since the finder.dat is only read from the current dir, it
// doesn't work for any full pathnames, only files in the current dir!
const char *DNativeFilesystem::getShortName(const char *longName)
{
  if(!longName) { DEBUG_OUT<<"DNativeFilesystem::NULL longName!\n"; return longName; }
#ifdef USE_FINDER_DAT
  if(!gotFinderDat) return longName;
#endif // USE_FINDER_DAT
  char tlongName[DDISK_MAX_NAMELEN];
  tlongName[0]=0;
  AStringHelper::fillInNameFromFullName(tlongName,longName);
#ifdef USE_FINDER_DAT
  if(!ignoreFinderDat) {
    DEBUG_OUT<<"Looking in finder.dat for "<<tlongName<<"...\n";
    finderDat.reset();
    if(finderDat.atEnd()) {
      // Nothing in finderDat...could use AApp::getShortName here...
      return longName;
    }
    DFinderDatEntry *en=(DFinderDatEntry *)NULL;
    bool working=true;
    while(working) {
      if(finderDat.atEnd()) working=false;
      else {
        en=(DFinderDatEntry *)finderDat.info();
        if(en->longName) {
          if(!strcmp(tlongName,en->longName)) return en->shortName;
        }
        finderDat.advance();
      }
    }
  }
#endif // USE_FINDER_DAT
  DEBUG_OUT<<"(Didn't find it)\n";
  return longName;
}


void *DNativeFilesystem::Fopen(const char *filename,const char *mode)
{
  if(!filename) { DEBUG_OUT<<"DNativeFilesystem::NULL fname!\n"; return NULL; }
  if(!mode) { DEBUG_OUT<<"DNativeFilesystem::NULL mode!\n"; return NULL; }
  DEBUG_OUT<<"DNativeFilesystem::fopen("<<filename<<","<<mode<<")\n";
  void *ret=NULL;
  void *fd=NULL;
#ifdef USE_FAKE_CHDIR
#endif // USE_FAKE_CHDIR
  fd=(void *)fopen(filename,mode);
#ifdef USE_FINDER_DAT
  if(!fd) {
    // Look and see if we can figure out the "short name" by using a finder.dat file...
    // NOTE: This lets you use long filenames even on 16-bit dos if you've got a finder.dat
    if(!gotFinderDat) return NULL;
#ifdef USE_FAKE_CHDIR
#endif // USE_FAKE_CHDIR
    fd=(void *)fopen(getShortName(filename),mode);
  }
#endif // USE_FINDER_DAT
  if(fd) {
    ret=fd;
  }
  return ret;
}


unsigned int DNativeFilesystem::Fclose(void *fp)
{
  if(!fp) { DEBUG_OUT<<"DNativeFilesystem::NULL fp!\n"; return 0; }
  return fclose((FILE *)fp);
}


ULONG DNativeFilesystem::Fread(void *ptr,ULONG sizelem,ULONG n,void *fp)
{
  return fread(ptr,sizelem,n,(FILE *)fp);
}


ULONG DNativeFilesystem::Fwrite(void *ptr,ULONG sizelem,ULONG n,void *fp)
{
  return fwrite(ptr,sizelem,n,(FILE *)fp);
}


ULONG DNativeFilesystem::Fseek(void *fp,long offset,int origin)
{
#ifdef ASYS_PALM
  return fseek((FILE *)fp,offset,(FileOriginEnum)origin);
#else
  return fseek((FILE *)fp,offset,origin);
#endif // ASYS_PALM
}


ULONG DNativeFilesystem::Ftell(void *fp)
{
  return ftell((FILE *)fp);
}


int DNativeFilesystem::Fstat(int fd,struct awstat_struct *st)
{
  // Because some compilers (mwerks) don't have it...
  return fstat(fd,st);
}


int DNativeFilesystem::Stat(const char *filename,struct awstat_struct *st)
{
  return awstat(filename,st);
}


void DNativeFilesystem::refresh()
{
  if(cachesDir) {
    freeDirCache();
    readDirectory();
  }
}


#ifdef ASYS_MAC
#ifdef MAC_USE_MOREFILES
// prototype...
pascal void myIterateFilterProc(const CInfoPBRec * const cpbPtr, Boolean *quitFlag,
  void *yourDataPtr);
// the actual function...
pascal void myIterateFilterProc(const CInfoPBRec * const cpbPtr, Boolean *quitFlag,
  void *yourDataPtr)
{
  AList *theDir=(AList *)yourDataPtr;
  unsigned int fType=DDISK_TYPE_NONE;
  unsigned int theSize=0;
  DDirEntry *de=new DDirEntry;
  StringPtr theName=(StringPtr)NULL;
  if(cpbPtr->hFileInfo.ioFlAttrib&0x10) {
    // a folder
    theName=cpbPtr->dirInfo.ioNamePtr;
    fType=DDISK_TYPE_DIR;
  }
  else
  {
    // a file
    theName=cpbPtr->hFileInfo.ioNamePtr;
    fType=DDISK_TYPE_NONE;
    theSize=cpbPtr->hFileInfo.ioFlLgLen+cpbPtr->hFileInfo.ioFlRLgLen;
  }
  // TODO: problem here if filename is longer than DDISK_MAX_NAMELEN bytes.
  for(unsigned int t=0;t<theName[0];t++)
    de->name[t]=theName[t+1];
  de->name[theName[0]]=0;
  de->type=fType;
  de->nativeType=cpbPtr->hFileInfo.ioFlAttrib;
  de->size=theSize;
  //DEBUG_OUT<<"File: "<<de->name<<" "<<de->type<<"\n";
  theDir->insert(de);
}
#endif // MAC_USE_MOREFILES
#endif // ASYS_MAC


void DNativeFilesystem::readDirectory()
{
  DEBUG_OUT<<"DNativeFilesystem::readDirectory()...\n";
  if(!cachesDir) {
    DEBUG_OUT<<"(cachesDir is false)\n";
	return;
  }
  bool done=false;
  bool warnEm=true;
  DDirEntry *de=(DDirEntry *)NULL;
  unsigned int fType=DDISK_TYPE_NONE;
#ifdef ASYS_MAC
#ifdef MAC_USE_MOREFILES
  OSErr err=IterateDirectory((short)0,(long)0,(unsigned char *)nil,(unsigned short)1,myIterateFilterProc,(void *)&theDir);
  if(err) DEBUG_OUT<<"IterateDirectory returned "<<err<<"\n";
  done=true;
#endif // MAC_USE_MOREFILES
#endif // ASYS_MAC
#ifdef ASYS_DOS
#ifdef __SC__
  // TODO: with Symantec C++, findfirst makes a DOS call, so we'll only get the
  // short name if we use this on Windows...should special case Win32...
  FIND *theSearchFor=findfirst("*.*",_A_HIDDEN|_A_SYSTEM|_A_SUBDIR);
  while(theSearchFor) {
    //printf("%s\n",theSearchFor->name);
    de=new DDirEntry;
    // TODO: problem here if filename is longer than DDISK_MAX_NAMELEN bytes.
    strcpy(de->name,theSearchFor->name);
    fType=DDISK_TYPE_NONE;
    if(theSearchFor->attribute&_A_SUBDIR) fType=DDISK_TYPE_DIR;
    de->type=fType;
    de->nativeType=theSearchFor->attribute;
    de->size=theSearchFor->size;
    //DEBUG_OUT<<"File: "<<de->name<<" "<<de->size<<" "<<de->type<<"\n";
    //DEBUG_OUT<<"File: "<<theSearchFor->name<<" "<<(int)theSearchFor->attribute<<"\n";
    theDir.insert(de);
    theSearchFor=findnext();
  }
  done=true;
#endif // __SC__
#endif // ASYS_DOS
#ifdef _MSC_VER
#ifndef ASYS_WIN32CE
  _finddata_t theSearchFor;
  ULONG fhandle=_findfirst("*.*",&theSearchFor);
  bool more=true;
  while(more) {
    //printf("%s\n",theSearchFor.name);
    de=new DDirEntry;
    // TODO: problem here if filename is longer than DDISK_MAX_NAMELEN bytes.
    strcpy(de->name,theSearchFor.name);
    fType=DDISK_TYPE_NONE;
    if(theSearchFor.attrib&_A_SUBDIR) fType=DDISK_TYPE_DIR;
    de->type=fType;
    de->nativeType=theSearchFor.attrib;
    de->size=theSearchFor.size;
    //DEBUG_OUT<<"File: "<<de->name<<" "<<de->size<<" "<<de->type<<"\n";
    //DEBUG_OUT<<"File: "<<theSearchFor.name<<" "<<(int)theSearchFor.attrib<<"\n";
    theDir.insert(de);
    unsigned int ret=_findnext(fhandle,&theSearchFor);
        if(ret) more=false;
  }
  done=true;
#endif // ASYS_WIN32CE
#endif // _MSC_VER
#ifdef ASYS_WIN32
  if(!done) {
    WIN32_FIND_DATA theSearchFor;
    HANDLE fhandle=FindFirstFile("*.*"),&theSearchFor);
    bool more=true;
    while(more) {
      //printf("%s\n",theSearchFor.name);
      de=new DDirEntry;
      // TODO: problem here if filename is longer than DDISK_MAX_NAMELEN bytes.
      strcpy(de->name,theSearchFor.cFileName);
      fType=DDISK_TYPE_NONE;
      if(theSearchFor.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
        fType=DDISK_TYPE_DIR;
      de->type=fType;
      de->nativeType=theSearchFor.dwFileAttributes;
      de->size=theSearchFor.nFileSizeLow;  // TODO: Lost high 32 bits here
      DEBUG_OUT<<"File: "<<de->name<<" "<<de->size<<" "<<de->type<<"\n";
      //DEBUG_OUT<<"File: "<<theSearchFor.name<<" "<<(int)theSearchFor.attrib<<"\n";
      theDir.insert(de);
      unsigned int ret=FindNextFile(fhandle,&theSearchFor);
      if(!ret) more=false;
    }
    done=true;
  }
#endif // ASYS_WIN32
#ifdef ASYS_PALM
  unsigned int numdb=DmNumDatabases(0);  // 0 is the card number...
  // How can I tell how many cards there are?
  for(unsigned int t=0;t<numdb;t++) {
    LocalID lid=DmGetDatabase(0,t);
    Err err=0;
    if(lid) {
      de=new DDirEntry;
      UInt attr=0;
      err=DmDatabaseInfo(0,lid,(CharPtr)&(de->name),&attr,
        NULL,NULL,
        NULL,NULL,
        NULL,NULL,
        NULL,NULL,
        NULL);
      // TODO: check err
      fType=DDISK_TYPE_NONE;
      de->type=fType;
      de->nativeType=attr;
      ULong dsize=0;
      err=DmDatabaseSize(0,lid,NULL,NULL,&dsize);
      // TODO: check err
      de->size=dsize;
      //DEBUG_OUT<<"File: "<<de->name<<" "<<de->size<<" "<<de->type<<"\n";
      theDir.insert(de);
    }
  }
  done=true;
#endif // ASYS_PALM
  if(!done) {
    int n=0;
#ifdef ALIB_HAVE_SCANDIR
    DEBUG_OUT<<"About to scandir...\n";
    n=scandir(".",&theDirents,/*(int (*)(...))*/NULL,
#ifdef ALIB_HAVE_ALPHASORT
      /*(int(*)(...))*/alphasort
#else
      NULL
#endif // ALIB_HAVE_ALPHASORT
    );
#endif // ALIB_HAVE_SCANDIR
    if(n) {
      DEBUG_OUT<<"Got "<<n<<" from dir...\n";
      if(n==-1) n=0;
      while(n) {
        n--;
        //printf("%s\n",theDirents[n]->d_name);
        de=new DDirEntry;
#ifdef ALIB_HAVE_DIRENT
        // TODO: problem if filename is longer than DDISK_MAX_NAMELEN bytes.
        strcpy(de->name,theDirents[n]->d_name);
#endif // ALIB_HAVE_DIRENT
        fType=DDISK_TYPE_NONE;
        int r=1;
#ifdef ALIB_HAVE_STAT
        struct awstat_struct s;
        //DEBUG_OUT<<"Gonna awstat...\n";
        r=awstat(de->name,&s);
        //DEBUG_OUT<<"Back from awstat.\n";
        if(!r) {
          if(s.st_mode&S_IFDIR) fType=DDISK_TYPE_DIR;
          //if(S_ISDIR(s.st_mode)) fType=DDISK_TYPE_DIR;
          if(s.st_mode&S_IFCHR) fType=DDISK_TYPE_DEVICE;
          //if(S_ISCHR(s.st_mode)) fType=DDISK_TYPE_DEVICE;
                  //if(S_ISBLK(s.st_mode)) fType=DDISK_TYPE_DEVICE;
        }
#endif // ALIB_HAVE_STAT
        de->type=fType;
#ifdef ALIB_HAVE_DIRENT
#ifdef ALIB_HAVE_DIRENT_D_TYPE
        de->nativeType=theDirents[n]->d_type;
#else
        de->nativeType=0;
#endif // ALIB_HAVE_DIRENT_D_TYPE
#endif // ALIB_HAVE_DIRENT
        de->size=getFileSize(de->name);
        //DEBUG_OUT<<"File: "<<de->name<<" "<<de->size<<" "<<de->type<<"\n";
        //DEBUG_OUT<<"File: "<<theDirents[n]->d_name<<" ";
        //DEBUG_OUT<<theDirents[n]->d_type<<"\n";
        theDir.insert(de);
      }
      done=true;
    }
  }
  if(!done) {
    char msg[256];
        sprintf(msg,"%s %d: Need to implement readDirectory!\n",__FILE__,__LINE__);
    if(warnEm) std::cerr<<msg;
        else DEBUG_OUT<<msg<<"\n";
    cachesDir=false;
  }
  DEBUG_OUT<<"Done.\n";
}


void *DNativeFilesystem::Open(const char *name,unsigned int mode)
{
#ifdef DDISK_GLOBAL_UNIX_FILEIO
  void *ret=NULL;
  char *sname=strdups(name);
  int f=::open(sname,mode);
  if(f!=-1) ret=(void *)f;
  return ret;
#else
  char *cmode="rb";
  return Fopen(name,cmode);
#endif // DDISK_GLOBAL_UNIX_FILEIO
}


unsigned int DNativeFilesystem::Close(void *fp)
{
#ifdef DDISK_GLOBAL_UNIX_FILEIO
  return ::close((int)fp);
#else
  return Fclose(fp);
#endif // DDISK_GLOBAL_UNIX_FILEIO
}


ULONG DNativeFilesystem::Read(void *fp,void *ptr,ULONG n)
{
#ifdef DDISK_GLOBAL_UNIX_FILEIO
  return ::read((int)fp,(char *)ptr,n);
#else
  return Fread(ptr,n,1,fp);
#endif // DDISK_GLOBAL_UNIX_FILEIO
}


ULONG DNativeFilesystem::Write(void *fp,void *ptr,ULONG n)
{
#ifdef DDISK_GLOBAL_UNIX_FILEIO
  return ::write((int)fp,(char *)ptr,n);
#else
  return Fwrite(ptr,n,1,fp);
#endif // DDISK_GLOBAL_UNIX_FILEIO
}


ULONG DNativeFilesystem::Lseek(void *fp,long offset,int origin)
{
#ifdef DDISK_GLOBAL_UNIX_FILEIO
  return ::lseek((int)fp,offset,origin);
#else
  return Fseek(fp,offset,origin);
#endif // DDISK_GLOBAL_UNIX_FILEIO
}


bool DNativeFilesystem::Chdir(const char *dname)
{
  if(!dname) return false;
  // TODO: update curDir...
#ifdef USE_FAKE_CHDIR
  return DDisk::Chdir(dname);
#else
  const char *sname=dname;
  int ret=-1;
  chdir(sname);
  refresh();
  if(ret==-1) return false; else return true;
#endif // USE_FAKE_CHDIR
}


const char *DNativeFilesystem::Getcwd(char *buf,unsigned int len)
{
#ifdef USE_FAKE_CHDIR
  return DDisk::Getcwd(buf,len);
#else
  return getcwd(buf,len);
#endif // USE_FAKE_CHDIR
}


bool DNativeFilesystem::CopyFile(const char *oldname,const char *newname)
{
  if(!oldname) return false;
  if(!newname) return false;
  bool retVal=false;
  DEBUG_OUT<<"copyFile "<<oldname<<" to "<<newname<<"\n";
  return retVal;
}


bool DNativeFilesystem::Rename(const char *oldname,const char *newname)
{
  if(!oldname) return false;
  if(!newname) return false;
  bool retVal=false;
  DEBUG_OUT<<"rename "<<oldname<<" to "<<newname<<"\n";
#ifdef ASYS_WIN32
  retVal=MoveFileA(oldname,newname);
#endif // ASYS_WIN32
#ifdef ASYS_UNIX_BASED
#ifdef ALIB_HAVE_RENAME
  if(!::rename(oldname,newname)) retVal=true;
#endif // ALIB_HAVE_RENAME
#endif // ASYS_UNIX_BASED
#ifdef ASYS_PALM
  // TODO:
#endif // ASYS_PALM
  return retVal;
}


bool DNativeFilesystem::Remove(const char *filename)
{
  if(!filename) return false;
  bool retVal=false;
  DEBUG_OUT<<"remove "<<filename<<"\n";
#ifdef ASYS_WIN32
  if(isDir(filename)) {
    removeAllFilesinFolder(filename);
    retVal=RemoveDirectoryA(filename);
  }
  else retVal=DeleteFileA(filename);
#endif // ASYS_WIN32
#ifdef ASYS_UNIX_BASED
  if(!unlink(filename)) retVal=true;
#endif // ASYS_UNIX_BASED
#ifdef ASYS_PALM
  // TODO:
#endif // ASYS_PALM
  return retVal;
}





