
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#include <disklib/DImageAcorn.h>

#include <portable/nickcpp/ANullStream.h>

#include <portable/nickcpp/AStringHelper.h>


////////////////////////////////////////////////////////////////////////////////
//  DImageAcorn Class
////////////////////////////////////////////////////////////////////////////////

DImageAcorn::DImageAcorn(DVfs *parent) : DDiskImageMem(parent)
{
  init();
  dos=parent;
  constructorHelper();
}


DImageAcorn::DImageAcorn(DVfs *parent,const char *fname) : DDiskImageMem(parent,fname)
{
  init();
  dos=parent;
  constructorHelper();
  MountDiskFromFile(fname);
}


void DImageAcorn::constructorHelper()
{
  noFileSystem=false;
  allTracksSameSectors=true;
  // Init for standard original disk
  numTracks=40;
  numSectors=10;
  blockSize=256;
  numSides=1;
  adfs=false;
}


DImageAcorn::~DImageAcorn()
{
  freeDirCache();
  diskMounted=false;
}


/* STATIC */
unsigned int DImageAcorn::recognize(BYTE *tbuf,FILE *f,unsigned int fsize)
{
  DEBUG_OUT<<"DImageAcorn::recognize checking disk image...\n";
  unsigned int t=0;
  // The first 7 chars of disk name must not have bit 8 set...
  for(t=0;t<7;t++) if(tbuf[t]&0x80) return DDISK_FORMAT_UNKNOWN;
  DEBUG_OUT<<"(passed name test)\n";
  unsigned int oneZeroSix=tbuf[0x106];
  unsigned int totalSectors=tbuf[0x107]+((oneZeroSix<<8)&0xf00);
  unsigned int totalSize=totalSectors*256;
  // TODO: Check file size against this, but we can't because we don't have the file handle!
  unsigned int numEntries=tbuf[0x105]/8;
  // Check num entries was divisible by 8, fail if not
  if((numEntries*8)!=tbuf[0x105]) return DDISK_FORMAT_UNKNOWN;
  DEBUG_OUT<<"(passed numEntries test)\n";
  // Check all dir entries for non-null...
  for(t=0;t<numEntries;t++) {
    unsigned int doff=0x100+(t*8);
    unsigned int loadAddress=(tbuf[doff+1]*256)+tbuf[doff];
    loadAddress+=(tbuf[doff+6]<<14)&0x30000;
    unsigned int execAddress=(tbuf[doff+3]*256)+tbuf[doff+2];
    execAddress+=(tbuf[doff+6]<<10)&0x30000;
    unsigned int length=(tbuf[doff+5]*256)+tbuf[doff+4];
    length+=(tbuf[doff+6]<<12)&0x30000;
    unsigned int startSector=tbuf[doff+7];
    startSector+=(tbuf[doff+6]<<8)&0x0300;
    if((!loadAddress)&&(!startSector)&&(!execAddress)&&(!length)) return DDISK_FORMAT_UNKNOWN;
  }
  DEBUG_OUT<<"(passed dir entries test)\n";
  // If we made it here, it passed all our tests...
  return DDISK_FORMAT_RAW;
}


void DImageAcorn::readDirectory()
{
  DEBUG_OUT<<"DImageAcorn::readDirectory...\n";
  freeDirCache();
  unsigned int t=0;
  unsigned int oneZeroSix=imageData[0x106];
  switch((oneZeroSix>>2)&0x3) {
    default:
      // NOTE: If these are non-null then it means this is an extended DFS...
      DEBUG_OUT<<"Byte 0x106 b3 and b2 are: "<<((oneZeroSix>>2)&0x3)<<"\n";
      break;
  }
  for(t=0;t<256;t++) diskName[t]=0;
  strcpy(diskName,"NO NAME");
  // NOTE: bit 7 of byte 0 is bit 10 of total size in extended DFS's...
  for(t=0;t<8;t++) diskName[t]=imageData[t]&0x7f;
  for(t=0;t<4;t++) diskName[t+8]=imageData[t+0x100]&0x7f;
  //
  unsigned int totalEntries=imageData[0x105]/8;
  DEBUG_OUT<<"totalEntries: "<<totalEntries<<"\n";
  unsigned int bootOption=(oneZeroSix>>4)&0x3;
  DEBUG_OUT<<"bootOption: "<<bootOption<<"\n";
  unsigned int totalSectors=imageData[0x107]+((oneZeroSix<<8)&0xf00);
  DEBUG_OUT<<"totalSectors: "<<totalSectors<<"\n";
  //
  DEBUG_OUT<<"numTracks="<<numTracks<<" numSectors="<<numSectors<<" numSides="<<numSides<<" blockSize="<<blockSize<<"\n";
  //
  for(t=1;t<totalEntries+1;t++) {
    readImageDirEntry(t*8);
  }
}


void DImageAcorn::cacheFile(void *fp)
{
  std::cerr<<"DImageAcorn::cacheFile not implemented!\n";
  diskError=true;
}


// TODO: uses imageData, not buf, since dir entries are spread over two sectors
void DImageAcorn::readImageDirEntry(ULONG off)
{
  DEBUG_OUT<<"DImageAcorn::readImageDirEntry("<<off<<")...\n";
  unsigned int doff=off+0x100;
  unsigned int loadAddress=(imageData[doff+1]*256)+imageData[doff];
  loadAddress+=(imageData[doff+6]<<14)&0x30000;
  unsigned int execAddress=(imageData[doff+3]*256)+imageData[doff+2];
  execAddress+=(imageData[doff+6]<<10)&0x30000;
  unsigned int length=(imageData[doff+5]*256)+imageData[doff+4];
  length+=(imageData[doff+6]<<12)&0x30000;
  unsigned int startSector=imageData[doff+7];
  startSector+=(imageData[doff+6]<<8)&0x0300;
  // NOTE: We assume a valid entry if first char of name is non-null
  if(imageData[off]) {
    unsigned int t=0;
    DDirEntry *theFile=new DDirEntry;
    if(theFile) {
      theFile->signature=0xface;
      for(t=0;t<DDISK_MAX_NAMELEN;t++) theFile->name[t]=0;
      theFile->name[DDISK_MAX_NAMELEN]=0;
      // NOTE: Extended DFS's use bit 7 of some of the filename bytes for extra data like perms, etc...
      for(t=0;t<7;t++) theFile->name[t]=imageData[off+t]&0x7f;
      DEBUG_OUT<<"name is \""<<theFile->name<<"\"\n";
      DEBUG_OUT<<"directory prefix is "<<(char)(imageData[off+7]&0x7f)<<"\n";
      theFile->nativeType=0;  // TODO:
      DEBUG_OUT<<"nativeType="<<theFile->nativeType<<"\n";
      theFile->type=DDISK_TYPE_BINARY; // TODO
      theFile->loadAddress=loadAddress;
      DEBUG_OUT<<"loadAddress="<<loadAddress<<"\n";
      theFile->execAddress=execAddress;
      DEBUG_OUT<<"execAddress="<<execAddress<<"\n";
      theFile->track=0; // TODO:
      theFile->sector=0; // TODO:
      theFile->data=(BYTE *)NULL;
      theFile->sizeBlocks=length/blockSize;
      theFile->size=length;
      DEBUG_OUT<<"size is "<<theFile->size<<" bytes\n";
      DEBUG_OUT<<"size is "<<theFile->sizeBlocks<<" blocks\n";
      theFile->block=startSector;
      DEBUG_OUT<<"startSector="<<startSector<<"\n";
      theFile->curSeek=0;
      theDir.append((void *)theFile);
    }
    else DEBUG_OUT<<"Couldn't alloc file entry!\n";
  }
}
