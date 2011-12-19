
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#include <disklib/DImageAmy.h>

#include <portable/nickcpp/ANullStream.h>

#include <portable/nickcpp/AStringHelper.h>


////////////////////////////////////////////////////////////////////////////////
//  DImageAmy Class
////////////////////////////////////////////////////////////////////////////////

DImageAmy::DImageAmy(DVfs *parent) : DDiskImageFile(parent)
{
  init();
  dos=parent;
  constructorHelper();
}


DImageAmy::DImageAmy(DVfs *parent,const char *fname) : DDiskImageFile(parent,fname)
{
  init();
  dos=parent;
  constructorHelper();
  MountDiskFromFile(fname);
}


void DImageAmy::constructorHelper()
{
  dosType=0;
  noFileSystem=false;
  allTracksSameSectors=true;
}


DImageAmy::~DImageAmy()
{
  freeDirCache();
  diskMounted=false;
}


/* STATIC */
unsigned int DImageAmy::recognize(BYTE *tbuf,FILE *f,unsigned int fsize)
{
  unsigned int ret=DDISK_FORMAT_UNKNOWN;
  // Look for UAE's ADF format signature...
  if(!strncmp((char *)tbuf,"UAE--ADF",8)) ret=DDISK_FORMAT_RAW;
  // Look for AmigaDOS's signature...
  // NOTE: This gives a false positive with some FAT partitions...(like DOSEMU)
  if(!strncmp((char *)tbuf,"DOS",3)) ret=DDISK_FORMAT_RAW;
  return ret;
}


void DImageAmy::readDirectory()
{
  DEBUG_OUT<<"DImageAmy::readDirectory...\n";
  freeDirCache();
  unsigned int t=0;
  for(t=0;t<256;t++) diskName[t]=0;
  strcpy(diskName,"NO NAME");
  readRootBlock();
  std::cerr<<"DImageAmy::readDirectory not implemented!\n";
  diskError=true;
}


void DImageAmy::cacheFile(void *fp)
{
  std::cerr<<"DImageAmy::cacheFile not implemented!\n";
  diskError=true;
}


void DImageAmy::readImageDirEntry(ULONG off)
{
  DEBUG_OUT<<"DImageAmy::readImageDirEntry("<<off<<")...\n";
  std::cerr<<"DImageAmy::readImageDirEntry not implemented!\n";
  diskError=true;
}


void DImageAmy::readRootBlock()
{
  blockSize=512;
  // set up defaults for floppy
  numTracks=79;  numSectors=10;  numSides=1;
  if(theImageFileSize!=901120) {
    std::cerr<<"DImageAmy::readRootBlock guessing geometry...\n";
    numSides=0; // one side
    numSectors=0; // one sector per track
    numTracks=theImageFileSize/512; // as many tracks as needed to hold entire disk...
    // NOTE: with this geomerty, track==block
  }
  ULONG offset=(theImageFileSize/2)/blockSize;
  DEBUG_OUT<<"Going to read root block "<<offset<<"\n";
  readBlock(offset);
  unsigned int nlen=alibReadBuf(blockSize-80);
  unsigned int t=0;
  for(t=0;t<DDISK_MAX_NAMELEN;t++) diskName[t]=0;
  for(t=0;t<nlen;t++) {
    //DEBUG_OUT<<t<<": "<<alibReadBuf(blockSize-80+t+1);
    //DEBUG_OUT<<" "<<(char)alibReadBuf(blockSize-80+t+1)<<"\n";
    diskName[t]=alibReadBuf(blockSize-80+t+1);
  }
  unsigned int type=alibReadBuf32b(0);
  DEBUG_OUT<<"block type "<<type<<"\n";
  if(type!=2) { std::cerr<<"DImageAmy: root type wrong!\n"; diskError=true; return; }
  unsigned int subtype=alibReadBuf32b(blockSize-4);
  DEBUG_OUT<<"block subtype "<<subtype<<"\n";
  if(subtype!=1) { std::cerr<<"DImageAmy: root subtype wrong!\n"; diskError=true; return; }
  unsigned int tableSize=alibReadBuf32b(12)*4;
  DEBUG_OUT<<"tableSize "<<tableSize<<"\n";
  if(tableSize!=(blockSize-56*4)) { std::cerr<<"DImageAmy: tableSize wrong!\n"; diskError=true; return; }
  unsigned int validated=alibReadBuf32b(blockSize-50*4);
  if(validated) std::cerr<<"DImageAmy: disk hasn't been validated...\n";
}


