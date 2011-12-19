
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
//  DDiskImageMem Class
////////////////////////////////////////////////////////////////////////////////

DDiskImageMem::DDiskImageMem(DVfs *parent) : DDiskImageAbstract(parent)
{
  init();
  dos=parent;
  constructorHelper();
}


DDiskImageMem::DDiskImageMem(DVfs *parent,const char *fname) : DDiskImageAbstract(parent,fname)
{
  init();
  dos=parent;
  constructorHelper();
  //MountDiskFromFile(fname);
}


void DDiskImageMem::init()
{
  DDiskImageAbstract::init();
  imageDataSize=0;
  imageData=(BYTE *)NULL;
}


DDiskImageMem::~DDiskImageMem()
{
  // TODO: flush files if cached
  freeDirCache();
  if(imageData) delete imageData;
  imageData=(BYTE *)NULL;
  diskMounted=false;
}


// TODO: What if there's no block 0?!?
void DDiskImageMem::readBlock(unsigned int blk)
{
  diskError=true;
  if(!imageData) { std::cerr<<"No imageData for disk image!\n"; return; }
  curLoc=blk*blockSize+skipOffset;
  if(curLoc>imageDataSize) { std::cerr<<"Past end of imageData!\n"; return; }
  DEBUG_OUT<<"DDiskImageMem::readBlock("<<blk<<") at offset "<<curLoc<<"\n";
  //DEBUG_OUT<<(int)0<<": ";
  for(unsigned int t=0;t<blockSize;t++) {
    buf[t]=imageData[curLoc+t];
    //DEBUG_OUT<<(int)buf[t]<<",";
    //if(t&&(!(t&0xf))) DEBUG_OUT<<"\n"<<(int)t<<": ";
  }
  //DEBUG_OUT<<"\n";
  diskError=false;
}


void DDiskImageMem::writeBlock(unsigned int blk)
{
  std::cerr<<"DDiskImageMem::writeBlock not implemented!\n";
  diskError=true;
}


void DDiskImageMem::MountDiskFromFile(const char *fname)
{
  DEBUG_OUT<<"DDiskImageMem::MountDiskFromFile("<<fname<<")\n";
  if(imageData) free(imageData);
  diskMounted=false;
  unsigned int t=0;
  unsigned int err=0;
  imageDataSize=getSize(fname);
#ifdef USE_FAKE_CHDIR
#endif // USE_FAKE_CHDIR
  FILE *fd=fopen(fname,"rb");
  if(fd) {
    DEBUG_OUT<<"Going to allocate "<<imageDataSize<<" sized chunk...\n";
    imageData=(BYTE *)malloc(imageDataSize*sizeof(BYTE));
    if(imageData) {
      bool ww=true;
      BYTE c;
      t=0;
      while(ww) {
        err=fread(&c,1,1,fd);
        if(err!=1) ww=false;
        else {
          imageData[t]=c;
          t++;
        }
      }
      diskMounted=true;
      for(t=0;t<DDISK_MAX_NAMELEN;t++) diskName[t]=0;
      for(t=0;t<strlen(fname);t++) diskName[t]=fname[t];
      DEBUG_OUT<<"Mounted disk image "<<diskName<<"\n";
      // (Have to do this outside of DDiskImage due to overloaded readDir's)
      // Oops...probably not, but its working so leave it silly...
      //readDir();
      fclose(fd);
    }
    else {
      std::cerr<<"No mem for disk image!";
    }
  }
  else {
    std::cerr<<"Couldn't find disk image!";
  }
  readDirectory();
}



