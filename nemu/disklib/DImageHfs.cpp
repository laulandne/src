
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#include <disklib/DImageHfs.h>

#include <portable/nickcpp/ANullStream.h>

#include <portable/nickcpp/AStringHelper.h>


////////////////////////////////////////////////////////////////////////////////
//  DImageHfs Class
////////////////////////////////////////////////////////////////////////////////

DImageHfs::DImageHfs(DVfs *parent) : DDiskImageFile(parent)
{
  init();
  dos=parent;
  constructorHelper();
}


DImageHfs::DImageHfs(DVfs *parent,const char *fname) : DDiskImageFile(parent,fname)
{
  init();
  dos=parent;
  constructorHelper();
  MountDiskFromFile(fname);
}


void DImageHfs::constructorHelper()
{
  DEBUG_OUT<<"DImageHfs disk image.\n";
  hfs=false;
  lisa=false;
  partition=false;
  partNum=0;
  blockSize=512;
  partOffset=0;
}


DImageHfs::~DImageHfs()
{
  freeDirCache();
  diskMounted=false;
}


/* STATIC */
unsigned int DImageHfs::recognize(BYTE *tbuf,FILE *f,unsigned int fsize)
{
  unsigned int ret=DDISK_FORMAT_UNKNOWN;
  // Look for HFS MFS signature...
  if((tbuf[1024]==0xd2)&&(tbuf[1025]==0xd7)) ret=DDISK_FORMAT_RAW;
  // Look for HFS HFS "Big Disk" signature...
  if((tbuf[1024]==0x42)&&(tbuf[1025]==0x44)) ret=DDISK_FORMAT_RAW;
  if(!strncmp((char *)tbuf+1024+84,"BD",2)) ret=DDISK_FORMAT_RAW;
  // Look for driver descriptor signature...
  if((tbuf[0]==0x45)&&(tbuf[1]==0x52)) ret=DDISK_FORMAT_RAW;
  return ret;
}


void DImageHfs::readDirectory()
{
  DEBUG_OUT<<"DImageHfs::readDirectory...\n";
  freeDirCache();
  hfs=false;  noFileSystem=true;  skipOffset=0;
  lisa=false;  partition=false; partNum=0; partOffset=0;
  unsigned int t=0;
  for(t=0;t<256;t++) diskName[t]=0;
  strcpy(diskName,"NO NAME");
  // first look for a partition map...
  readBlock(0);
  if(lookAtFileSystem(0)) {
    if(partition) {
      DEBUG_OUT<<"Found a driver descriptor...\n";
      findFirstHFSPartition();
    }
  }
  // skip over boot info, if any...
  readBlock(2+partOffset);
  if(lookAtFileSystem(0)) {
    noFileSystem=false;
  }
  if(noFileSystem) {
    DEBUG_OUT<<"Can't readDirectory, disk format error!\n";
    return;
  }
  DEBUG_OUT<<"DImageHfs::readDirectory, got a good HFS filesystem...\n";
  if(partition) {
    DEBUG_OUT<<"partNum="<<partNum<<" partOffset="<<partOffset<<"\n";
  }
  if(hfs) DEBUG_OUT<<"hfs "; else DEBUG_OUT<<"mfs ";
  if(skipOffset) DEBUG_OUT<<"(offset is "<<skipOffset<<") ";
  DEBUG_OUT<<"\n";
  DEBUG_OUT<<"DImageHfs::readDirectory not implemented!\n";
}


bool DImageHfs::lookAtFileSystem(ULONG offset)
{
  bool ret=false;
  // Look for HFS MFS signature...
  if((buf[offset]==0xd2)&&(buf[offset+1]==0xd7)) {
    ret=true;  hfs=false;
  }
  // Look for HFS HFS "Big Disk" signature...
  if((buf[offset]==0x42)&&(buf[offset+1]==0x44)) {
    ret=true;  hfs=true;
  }
  // Look for driver descriptor signature...
  if((buf[offset]==0x45)&&(buf[offset+1]==0x52)) {
    ret=true;  partition=true;
  }
  return ret;
}


bool DImageHfs::findFirstHFSPartition()
{
  bool ret=false;
  std::cerr<<"DImageHfs::findFirstHFSPartition not implemented!\n";
  diskError=true;
  return ret;
}


void DImageHfs::cacheFile(void *fp)
{
  std::cerr<<"DImageHfs::cacheFile not implemented!\n";
  diskError=true;
}


void DImageHfs::readImageDirEntry(ULONG off)
{
  DEBUG_OUT<<"DImageHfs::readImageDirEntry("<<off<<")...\n";
  std::cerr<<"DImageHfs::readImageDirEntry not implemented!\n";
  diskError=true;
}

