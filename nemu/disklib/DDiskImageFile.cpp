
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
//  DDiskImageFile Class
////////////////////////////////////////////////////////////////////////////////

DDiskImageFile::DDiskImageFile(DVfs *parent) : DDiskImageAbstract(parent)
{
  init();
  dos=parent;
  cachesData=false;
  constructorHelper();
}


DDiskImageFile::DDiskImageFile(DVfs *parent,const char *fname) : DDiskImageAbstract(parent,fname)
{
  init();
  dos=parent;
  cachesData=false;
  constructorHelper();
  //MountDiskFromFile(fname);
}


DDiskImageFile::~DDiskImageFile()
{
  // TODO: flush files if cached
  freeDirCache();
  if(theImageFile) fclose(theImageFile);
  theImageFile=(FILE *)NULL;
  diskMounted=false;
}


void DDiskImageFile::init()
{
  DDiskImageAbstract::init();
  theImageFile=(FILE *)NULL;
  theImageFileSize=0;
  theImageFileName=(char *)NULL;
}


bool DDiskImageFile::openTheFile()
{
  bool ret=false;
#ifdef USE_FAKE_CHDIR
#endif // USE_FAKE_CHDIR
  theImageFile=fopen(theImageFileName,"rb");
  if(theImageFile) ret=true;
  return ret;
}


void DDiskImageFile::readBlock(unsigned int blk)
{
  if(err.error()) return;
  if(!theImageFile) { if(!openTheFile()) return; }
  fseek(theImageFile,blk*blockSize+skipOffset,SEEK_SET);
  fread(buf,blockSize,1,theImageFile);
}


void DDiskImageFile::writeBlock(unsigned int blk)
{
  if(err.error()) return;
  if(!theImageFile) { if(!openTheFile()) return; }
  fseek(theImageFile,blk*blockSize+skipOffset,SEEK_SET);
  fwrite(buf,blockSize,1,theImageFile);
}


void DDiskImageFile::MountDiskFromFile(const char *fname)
{
  DEBUG_OUT<<"DDiskImageFile::MountDiskFromFile("<<fname<<")\n";
  theImageFileName=fname;
  if(openTheFile()) {
    // That's really all...I think.
  }
  else {
    DEBUG_OUT<<"MountDiskFromFile couldn't find disk image file!\n";
    diskError=true;
    err.set();
  }
}



