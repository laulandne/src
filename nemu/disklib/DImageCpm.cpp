
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#include <disklib/DImageCpm.h>

#include <portable/nickcpp/ANullStream.h>

#include <portable/nickcpp/AStringHelper.h>


/*
NOTES:

Originally only IBM 3740 8" floppy
single sided, single density
~250k capacity

80 tracks, 26 sectors, 1 side, 128 byte blocks
tracks start at 0
sectors start at 1

64 files per disk max
32 bytes per entry (FCB)

first 2.5 tracks "system area" (starting with 0)
OS is first 2 tracks (including boot block)
directory is 1/2 of track 2

boot block is track 0 sector 1

FCB Format:
0: BF busy/free (0=in use, 0xe5=free)
1-8: FN filename
9-11: FT file type
12: EX Extent (normally 0 except for really large files)
13-14: not used
15: RC record count # of recs for this extent
16-31: DM disk allocation map, shows clusters used by file

If file is under 16k in size, then only one FCB/extent is needed.

There are 241 clusters per disk (starting with 1)
First cluster is track 2 sector 17

cluster pattern is:
(starting after dir on track 2)
(skip 16),1-8, 2-2
2-6,3-8,4-8,5-4
5-4,6-8,7-8,8-6
8-2,9-8,10-8,11-8
12-8,13-8,(skip 10)
(repeat)

In memory "Allocation Vector" keeps track of clusters
(241 bytes, each bit is 1 if sector is used)

sector skew is:
1,14,10,23,6,19,2,15,11,24,7,20,3,16,12,25,8,21,4,17,13,26,9,22,5,18

*/


#define DCPM_FORMAT_UNKNOWN 0
#define DCPM_FORMAT_RAW     1
#define DCPM_FORMAT_EXTDSK  2
#define DCPM_FORMAT_CPCEMU  3
#define DCPM_FORMAT_IMD     4


////////////////////////////////////////////////////////////////////////////////
//  DImageCpm Class
////////////////////////////////////////////////////////////////////////////////

DImageCpm::DImageCpm(DVfs *parent) : DDiskImageMem(parent)
{
  init();
  dos=parent;
  constructorHelper();
}


DImageCpm::DImageCpm(DVfs *parent,const char *fname) : DDiskImageMem(parent,fname)
{
  init();
  dos=parent;
  constructorHelper();
  MountDiskFromFile(fname);
}


void DImageCpm::constructorHelper()
{
  noFileSystem=false;
  allTracksSameSectors=true;
  noSectorZero=true;
  // Start with original cp/m geometry, most likely will change as we decode the image...
  numTracks=80; numSectors=26; numSides=1; blockSize=128;
  unpackedSize=0;
  unpackedData=NULL;
  totalSectors=0;
  skipOffset=0;
  cpcTrackSize=0;
  imageFormat=DCPM_FORMAT_UNKNOWN;
}


DImageCpm::~DImageCpm()
{
  freeDirCache();
  diskMounted=false;
}


/* STATIC */
unsigned int DImageCpm::recognize(BYTE *tbuf,FILE *f,unsigned int fsize)
{
  unsigned int ret=DDISK_FORMAT_UNKNOWN;
  char *tstring=NULL;
  tstring="EXTENDED CPC DSK File";
  if(!strncmp((char *)tbuf,tstring,strlen(tstring))) ret=DCPM_FORMAT_EXTDSK;
  tstring="MV - CPCEMU";
  if(!strncmp((char *)tbuf,tstring,strlen(tstring))) ret=DCPM_FORMAT_CPCEMU;
  tstring="IMD ";
  if(!strncmp((char *)tbuf,tstring,strlen(tstring))) ret=DCPM_FORMAT_IMD;
  /*
  if(size==737280) {
    // a 720k disk, check further, but could be a samcoupe disk...
    ret=DDISK_FORMAT_RAW;
  }
  */
  // TODO: Check for other formats here...
  return ret;
}


void DImageCpm::readDirectory()
{
  DEBUG_OUT<<"DImageCpm::readDirectory...\n";
  unsigned int t=0;
  freeDirCache();
  for(t=0;t<256;t++) diskName[t]=0;
  strcpy(diskName,"NO NAME");
  //
  imageFormat=recognize(imageData,NULL,imageDataSize);
  switch(imageFormat) {
    case DCPM_FORMAT_RAW:
      unpackedData=imageData;
      unpackedSize=imageDataSize;
      switch(imageDataSize) {
        case 737280:
          // 720k
          numSectors=18;
          // NOTE: blind guess that seems to work...
          // So that directory shows up at 2*128*18, which is 4608
          break;
        default:
          DEBUG_OUT<<"Can't guess disk geometry, so leaving at defaults...\n";
          break;
      }
      break;
    case DCPM_FORMAT_EXTDSK:
      readExtDskHeader();
      unpackExtDsk();
      break;
     case DCPM_FORMAT_CPCEMU:
      readCpcEmuHeader();
      unpackCpcEmu();
      break;
    default:
      DEBUG_OUT<<"Unimplemented format, sorry!\n";
      diskError=true;
      return;
      break;
  }
  //
  DEBUG_OUT<<"numTracks="<<numTracks<<" numSectors="<<numSectors<<" numSides="<<numSides<<" blockSize="<<blockSize<<"\n";
  if(!numSectors) {
    DEBUG_OUT<<"numSectors is zero, we messed up someplace, bailing!\n";
    diskError=true;
    exit(5);
  }
  //
  // NOTE: We should do some sanity checking on the directory entries, etc...
  // Now read directory entries
  // TODO: Should we read less sectors if their size is larger than 128 bytes?!?
  for(t=1;t<16;t++) {
    readTrackSector(2,t);
    for(unsigned int i=0;i<(blockSize/32);i++) {
      readImageDirEntry(i*32);
    }
  }
}


void DImageCpm::readCpcEmuHeader()
{
  DEBUG_OUT<<"DImageCpm::readCpcEmuHeader...\n";
  numTracks=imageData[0x30];
  numSides=imageData[0x31];
  DEBUG_OUT<<"numTracks="<<numTracks<<" numSizes="<<numSides<<"\n";
  cpcTrackSize=(imageData[0x33]*256)+imageData[0x32];
  DEBUG_OUT<<"cpcTrackSize="<<cpcTrackSize<<"\n";
  totalSectors=(numTracks*cpcTrackSize)/blockSize;
  DEBUG_OUT<<"totalSectors="<<totalSectors<<" (probably completely wrong)\n";
  // TODO: numSectors here is also probably completely wrong...
  numSectors=cpcTrackSize/blockSize;
}


void DImageCpm::readExtDskHeader()
{
  DEBUG_OUT<<"DImageCpm::readExtDskHeader...\n";
  numTracks=imageData[0x30];
  numSides=imageData[0x31];
  DEBUG_OUT<<"numTracks="<<numTracks<<" numSizes="<<numSides<<"\n";
  trackDataSize=(unsigned int *)malloc(sizeof(unsigned int)*numTracks);
  // TODO: This is the coded size with all the headers, right?  So totalSectors is WRONG.
  for(unsigned int t=0;t<numTracks;t++) {
    trackDataSize[t]=imageData[0x34+t]*256;
    DEBUG_OUT<<"trackDataSize("<<t<<")="<<trackDataSize[t]<<" bytes\n";
    totalSectors+=trackDataSize[t]/blockSize;  // TODO: probably not right
  }
  DEBUG_OUT<<"totalSectors="<<totalSectors<<" (probably completely wrong)\n";
  // TODO: numSectors here is also probably completely wrong...
  numSectors=trackDataSize[0]/blockSize;  // TODO: For now assume all tracks the same...
}


void DImageCpm::cacheFile(void *fp)
{
  std::cerr<<"DImageCpm::cacheFile not implemented!\n";
  diskError=true;
}


// TODO: What if there's no block 0?!?
// NOTE: We have our own version because we may have unpackedData a weird format
void DImageCpm::readBlock(unsigned int blk)
{
  diskError=true;
  if(!unpackedData) { std::cerr<<"No unpackedData for disk image!\n"; return; }
  curLoc=blk*blockSize+skipOffset;
  if(curLoc>unpackedSize) { std::cerr<<"Past end of unpackedData!\n"; return; }
  DEBUG_OUT<<"DImageCpm::readBlock("<<blk<<") at offset "<<curLoc<<"\n";
  //DEBUG_OUT<<(int)0<<": ";
  for(unsigned int t=0;t<blockSize;t++) {
    buf[t]=unpackedData[curLoc+t];
    //DEBUG_OUT<<(int)buf[t]<<",";
    //if(t&&(!(t&0xf))) DEBUG_OUT<<"\n"<<(int)t<<": ";
  }
  //DEBUG_OUT<<"\n";
  diskError=false;
}


void DImageCpm::writeBlock(unsigned int blk)
{
  std::cerr<<"DImageCpm::writeBlock not implemented!\n";
  diskError=true;
}


void DImageCpm::unpackCpcEmu()
{
  DEBUG_OUT<<"DImageCpm::unpackCpcEmu...\n";
  unpackedSize=totalSectors*blockSize;
  unpackedData=(BYTE *)calloc(totalSectors,blockSize);
  offset=0x100; // offset to first track
  dOffset=0;  // Start writing at 0
  skippedLast=0;  // reset skip counter
  for(unsigned int t=0;t<numTracks;t++) {
    unpackCpcEmuTrack(t);
  }
}


void DImageCpm::unpackExtDsk()
{
  DEBUG_OUT<<"DImageCpm::unpackExtDsk...\n";
  unpackedSize=totalSectors*blockSize;
  unpackedData=(BYTE *)calloc(totalSectors,blockSize);
  offset=0x100; // offset to first track
  dOffset=0;  // Start writing at 0
  skippedLast=0;  // reset skip counter
  for(unsigned int t=0;t<numTracks;t++) {
    if(trackDataSize[t]) {
      unpackCpcEmuTrack(t);
    }
    else {
      DEBUG_OUT<<"Skipping track "<<t<<"...\n";
      skippedLast++;
    }
  }
}


void DImageCpm::unpackCpcEmuTrack(unsigned int t)
{
  DEBUG_OUT<<"DImageCpm::unpackCpcEmuTrack("<<t<<")...\n";
  unsigned int tNum,sNum,sSize,nSectors,gapLen,fillerByte;
  if(strncmp("Track-Info",(char *)&imageData[offset],10)) {
    DEBUG_OUT<<"Something went wrong with Track-Info, bailing!\n";
    diskError=true;
    exit(0);
  }
  else {
    tNum=imageData[offset+0x10];
    sNum=imageData[offset+0x11];
    if(sNum) {
      DEBUG_OUT<<"Dang, we only handle single sided disks, bailing!\n";
      diskError=true;
      exit(0);
    }
    sSize=imageData[offset+0x14];
    nSectors=imageData[offset+0x15];
    // TODO: Fill out track table from this like 1541 does
    gapLen=imageData[offset+0x16];
    fillerByte=imageData[offset+0x17];
    DEBUG_OUT<<"offset="<<offset<<" "<<t<<": tNum="<<tNum<<" sNum="<<sNum<<" sSize="<<sSize<<" nSectors="<<nSectors<<" gapLen="<<gapLen<<" fillerByte="<<fillerByte<<"\n";
    if(tNum!=t) {
      DEBUG_OUT<<"Something went wrong with tNum, bailing!\n";
      exit(0);
    }
    unsigned int sOffset=offset;
    unsigned int n765_c=imageData[sOffset+0x18];
    unsigned int n765_h=imageData[sOffset+0x19];
    unsigned int n765_r=imageData[sOffset+0x1a];
    unsigned int n765_n=imageData[sOffset+0x1b];
    unsigned int n765_1=imageData[sOffset+0x1c];
    unsigned int n765_2=imageData[sOffset+0x1d];
    unsigned int actualLen=(cpcTrackSize-0x100)/nSectors;
    blockSize=actualLen;
    // TODO: We assume all sectors are the same!
    numSectors=nSectors;
    // TODO: We assume all tracks have same number of sectors!
    if(skippedLast) {
      unsigned int sVal=(blockSize*numSectors)*skippedLast;
      DEBUG_OUT<<"(Skipped last "<<skippedLast<<" tracks, so advancing dOffset by "<<sVal<<" bytes)\n";
      skippedLast=0;
      dOffset+=sVal;
    }
    DEBUG_OUT<<"n765_c="<<n765_c<<" n765_h="<<n765_h<<" n765_r="<<n765_r<<" n765_n="<<n765_n<<" n765_1="<<n765_1<<" n765_2="<<n765_2<<" actualLen="<<actualLen<<"\n";
    for(unsigned int i=0;i<nSectors;i++) {
      DEBUG_OUT<<t<<","<<i<<": "<<dOffset<<" "<<sOffset<<"\n";
      memcpy(unpackedData+dOffset,imageData+sOffset+0x20,actualLen);
      sOffset+=actualLen;
      dOffset+=actualLen;
    }
  }
  offset+=cpcTrackSize;
}


void DImageCpm::unpackExtDskTrack(unsigned int t)
{
  DEBUG_OUT<<"DImageCpm::unpackExtDskTrack("<<t<<")...\n";
  unsigned int tNum,sNum,sSize,nSectors,gapLen,fillerByte;
  if(strncmp("Track-Info",(char *)&imageData[offset],10)) {
    DEBUG_OUT<<"Something went wrong with Track-Info, bailing!\n";
    diskError=true;
    exit(0);
  }
  else {
    tNum=imageData[offset+0x10];
    sNum=imageData[offset+0x11];
    if(sNum) {
      DEBUG_OUT<<"Dang, we only handle single sided disks, bailing!\n";
      diskError=true;
      exit(0);
    }
    sSize=imageData[offset+0x14];
    nSectors=imageData[offset+0x15];
    // TODO: Fill out track table from this like 1541 does
    gapLen=imageData[offset+0x16];
    fillerByte=imageData[offset+0x17];
    DEBUG_OUT<<"offset="<<offset<<" "<<t<<": tNum="<<tNum<<" sNum="<<sNum<<" sSize="<<sSize<<" nSectors="<<nSectors<<" gapLen="<<gapLen<<" fillerByte="<<fillerByte<<"\n";
    if(tNum!=t) {
      DEBUG_OUT<<"Something went wrong with tNum, bailing!\n";
      exit(0);
    }
    unsigned int sOffset=offset;
    unsigned int n765_c=imageData[sOffset+0x18];
    unsigned int n765_h=imageData[sOffset+0x19];
    unsigned int n765_r=imageData[sOffset+0x1a];
    unsigned int n765_n=imageData[sOffset+0x1b];
    unsigned int n765_1=imageData[sOffset+0x1c];
    unsigned int n765_2=imageData[sOffset+0x1d];
    unsigned int actualLen=(imageData[sOffset+0x1f]*256)+imageData[sOffset+0x1e];
    blockSize=actualLen;
    // TODO: We assume all sectors are the same!
    numSectors=nSectors;
    // TODO: We assume all tracks have same number of sectors!
    if(skippedLast) {
      unsigned int sVal=(blockSize*numSectors)*skippedLast;
      DEBUG_OUT<<"(Skipped last "<<skippedLast<<" tracks, so advancing dOffset by "<<sVal<<" bytes)\n";
      skippedLast=0;
      dOffset+=sVal;
    }
    DEBUG_OUT<<"n765_c="<<n765_c<<" n765_h="<<n765_h<<" n765_r="<<n765_r<<" n765_n="<<n765_n<<" n765_1="<<n765_1<<" n765_2="<<n765_2<<" actualLen="<<actualLen<<"\n";
    for(unsigned int i=0;i<nSectors;i++) {
      DEBUG_OUT<<t<<","<<i<<": "<<dOffset<<" "<<sOffset<<"\n";
      memcpy(unpackedData+dOffset,imageData+sOffset+0x20,actualLen);
      sOffset+=actualLen;
      dOffset+=actualLen;
    }
  }
  offset+=trackDataSize[t];
}


void DImageCpm::readImageDirEntry(ULONG off)
{
  DEBUG_OUT<<"DImageCpm::readImageDirEntry("<<off<<")...\n";
  if(true) {
    unsigned int t=0;
    DDirEntry *theFile=new DDirEntry;
    if(theFile) {
      theFile->signature=0xface;
      for(t=0;t<DDISK_MAX_NAMELEN;t++) theFile->name[t]=0;
      theFile->name[DDISK_MAX_NAMELEN]=0;
      for(t=1;t<9;t++) theFile->name[t-1]=buf[off+t];
      theFile->name[8]='.';
      for(t=9;t<12;t++) theFile->name[t]=buf[off+t];
      DEBUG_OUT<<"name is \""<<theFile->name<<"\"\n";
      theFile->sizeBlocks=buf[off+15];
      DEBUG_OUT<<"size is "<<theFile->sizeBlocks<<" blocks\n";
      theFile->nativeType=buf[0];
      DEBUG_OUT<<"nativeType="<<theFile->nativeType<<"\n";
      theFile->type=DDISK_TYPE_BINARY; // TODO
      theFile->track=0; // TODO:
      theFile->sector=0; // TODO:
      theFile->data=(BYTE *)NULL;
      theFile->size=theFile->sizeBlocks*blockSize;
      theFile->block=0;
      theFile->curSeek=0;
      theDir.append((void *)theFile);
    }
    else DEBUG_OUT<<"Couldn't alloc file entry!\n";
  }
}

