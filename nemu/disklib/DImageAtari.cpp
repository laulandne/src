
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#include <disklib/DImageAtari.h>

#include <portable/nickcpp/ANullStream.h>

#include <portable/nickcpp/AStringHelper.h>


/*
NOTES:

First 3 blocks are boot, and always single density regardless of rest of disk

"density" is not always what it means for other disk! (more a rule of thumb)

standard format:
720 sectors, 40 tracks
dos leaves 707 sectors free
128 btes per sector
last 3 bytes of each sector: bytes used, file number, next sector
(leaving 125 bytes for data)

single-sides, single density:
40 tracks, 18 sectors per track, 128 bytes per sector, 90k capacity.

single-sides, double density:
40 tracks, 18 sectors per track, 256 bytes per sector, 180k capacity.

single-sides, enhanced density:
40 tracks, 26 sectors per track, 128 bytes per sector, 127k capacity.

double-sides, double density: (called quad density sometimes)
80 tracks (40 per side), 18 sectors per track, 256 bytes per sector, 360k capacity.

1050 disks:
40 tracks 32 sectors each 128 bytes per sector total 160k space.

Percom, astral, micromainframe 3rd party disks:
40 tracks, 18 sectors each 256 bytes per sector ttal 180k

SD - 1050/XF551:
18 sectors per track
Interleave factor: 9
1, 3, 5, 7, 9, 11, 13, 15, 17, 19, 21, 23, 25,
2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26.

ED - 1050/XF551:
26 sectors per track
Interleave factor 13
1, 3, 5, 7, 9, 11, 13, 15, 17, 19, 21, 23, 25,
2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26.

I discovered two different interleave factors for DD (the interleave factor is sometimes revered to as 'sector skew'). The first one is 'fast' for 1050 read/write operations and the second makes things faster for the XF551 FD.

DD - 1050:
18 sectors per track
Interleave factor 15
6, 12, 18, 5, 11, 17, 4, 10, 16,
3, 9, 15, 2, 8, 14, 1, 7, 13.

DD - XF551:
18 sectors per track
Interleave factor 9
1, 3, 5, 7, 9, 11, 13, 15, 17,
2, 4, 6, 8, 10, 12, 14, 16, 18.


The 'standard' ATR file format is: (first byte listed as 1, not 0)

    * 01: NICKATARI Signature
    * 02: "
    ($0296)
    * 03: Paragraphs, low
    (size/$10)
    * 04: Paragraphs, high
    * 05: Sector Size, low
    * 06: Sector Size, high
    * 07: Extended paragraphs
    (extra hi byte)
    * 08..16: Unused
    * Remainder of image contains Atari data.


Current APE changes to the ATR header:

    * Byte 8 : 32bit Authentication CRC
    * 9 : 32bit Authentication CRC
    * 10: 32bit Authentication CRC
    * 11: 32bit Authentication CRC

    * 16: Bit 0: Write Protect
          o Bit 1: Sealed/Authenticated Atr
          o Bits 2..7: Unused


XFD format has no header and is just raw sectors...


directory is blocks 361-368
vtoc is 360
dir entries are 16 bytes
8 per sector

*/


////////////////////////////////////////////////////////////////////////////////
//  DImageAtari Class
////////////////////////////////////////////////////////////////////////////////

DImageAtari::DImageAtari(DVfs *parent) : DDiskImageMem(parent)
{
  init();
  dos=parent;
  constructorHelper();
}


DImageAtari::DImageAtari(DVfs *parent,const char *fname) : DDiskImageMem(parent,fname)
{
  init();
  dos=parent;
  constructorHelper();
  MountDiskFromFile(fname);
}


void DImageAtari::constructorHelper()
{
  noFileSystem=false;
  allTracksSameSectors=true;
  atrHeader=false;
  numTracks=40;
  numSectors=18;
  numSides=1;
  blockSize=128;
  for(unsigned int t=0;t<128;t++) { vtoc[t]=0; }
}


DImageAtari::~DImageAtari()
{
  freeDirCache();
  diskMounted=false;
}


/* STATIC */
unsigned int DImageAtari::recognize(BYTE *tbuf,FILE *f,unsigned int fsize)
{
  unsigned int ret=DDISK_FORMAT_UNKNOWN;
  if((tbuf[0]==0x96)&&(tbuf[1]==0x02)) {
    // ATR sig found!
    ret=DDISK_FORMAT_RAW;
  }
  /*
  switch(size) {
    case 92176:
      // probably atr with 16 byte header...
      ret=DDISK_FORMAT_RAW;
      break;
    case 92160:
      // probably atr w/o header...
      ret=DDISK_FORMAT_RAW;
      break;
    default:
      DEBUG_OUT<<"(recognize: size was wrong and no ATR header)\n";
      break;
  }
  */
  return ret;
}


void DImageAtari::readDirectory()
{
  DEBUG_OUT<<"DImageAtari::readDirectory...\n";
  freeDirCache();
  unsigned int t=0;
  for(t=0;t<256;t++) diskName[t]=0;
  strcpy(diskName,"NO NAME");
  readBlock(0);  // NOTE: This will get the ATR header if its there before we set skipOffset!
  switch(imageDataSize)
  {
    case 92176:
      // Let's guess an atr image with 16 byte header...
      skipOffset=16;
      atrHeader=true;
      DEBUG_OUT<<"DImageAtari::readDirectory guessing an atr image with header...\n";
      break;
    case 92160:
      // Let's guess an atr image w/o header...
      DEBUG_OUT<<"DImageAtari::readDirectory guessing an atr image w/o header...\n";
      break;
    default:
      if((buf[0]==0x96)&&(buf[1]==0x02)) {
        // ATR sig found!
        skipOffset=16;
        atrHeader=true;
        unsigned int wPars=(buf[3]*256)+buf[2];
        unsigned int wSecSize=(buf[5]*256)+buf[4];
        unsigned int btParsHigh=buf[6];
        DEBUG_OUT<<"wPars="<<wPars<<"("<<wPars*16<<" bytes) wSecSize="<<wSecSize<<" btParsHigh="<<btParsHigh<<"\n";
        blockSize=wSecSize;
        // NOTE: We aren't told what the # of tracks or sectors is, so we just make up something that'd fit the size we've got.
        // (The size values used are mostly arbitrary to fit between actual sizes)
        //
        // So...standard disks are 90k, we only change the geometry if the image is larger...
        if(imageDataSize>100000) {
          if(blockSize==128) { numSectors=26;  DEBUG_OUT<<"guessing 'enhanced density' for now, around 127k\n"; }
          else { DEBUG_OUT<<"NOTE: larger than ~100k, but 256 byte blocks...\n"; }
        }
        if(imageDataSize>150000) {
          if(blockSize==128) { numSectors=32;  DEBUG_OUT<<"guessing '1050' for now, around 160k\n"; }
          else { DEBUG_OUT<<"NOTE: larger than ~150k, but 256 byte blocks...\n"; }
        }
        if(imageDataSize>170000) {
          if(blockSize==256) { numSectors=26;  DEBUG_OUT<<"guessing 'double density' for now, around 180k\n"; }
          else { DEBUG_OUT<<"NOTE: larger than ~170k, but 128 byte blocks...\n"; }
        }
        if(imageDataSize>320000) {
          if(blockSize==256) { numSectors=26; numSides=2; DEBUG_OUT<<"guessing 'quad density' for now, around 360k\n"; }
          else { DEBUG_OUT<<"NOTE: larger than ~320k, but 128 byte blocks...\n"; }
        }
        DEBUG_OUT<<"Guessing ATR image geometry is...\n";
        DEBUG_OUT<<"numTracks="<<numTracks<<" numSectors="<<numSectors<<" numSides="<<numSides<<" blockSize="<<blockSize<<"\n";
      }
      else {
        std::cerr<<"DImageAtari::readDirectory can't figure out image format!\n"; diskError=true; return;
      }
      break;
  }
  // NOTE: We are one sector off for some reason...for now...
  //
  // Read VTOC
  DEBUG_OUT<<"DImageAtari::readDirectory reading VTOC...\n";
  readBlock(359);
  for(t=0;t<128;t++) vtoc[t]=buf[t];
  // TODO: Use info here...
  //
  // Now read directory entries
  for(t=360;t<368;t++) {
    readBlock(t);
    // 8 entries per block
    for(unsigned int i=0;i<8;i++) {
      readImageDirEntry(i*16);
    }
  }
}


void DImageAtari::cacheFile(void *fp)
{
  std::cerr<<"DImageAtari::cacheFile not implemented!\n";
  diskError=true;
}


void DImageAtari::readImageDirEntry(ULONG off)
{
  DEBUG_OUT<<"DImageAtari::readImageDirEntry("<<off<<")...\n";
  unsigned int firstBlock=(buf[off+4]*256)+buf[off+3];
  if(firstBlock) {
    unsigned int t=0;
    DDirEntry *theFile=new DDirEntry;
    if(theFile) {
      theFile->signature=0xface;
      for(t=0;t<DDISK_MAX_NAMELEN;t++) theFile->name[t]=0;
      theFile->name[DDISK_MAX_NAMELEN]=0;
      for(t=5;t<13;t++) theFile->name[t-5]=buf[off+t];
      theFile->name[8]='.';
      for(t=13;t<16;t++) theFile->name[t-4]=buf[off+t];
      DEBUG_OUT<<"name is \""<<theFile->name<<"\"\n";
      theFile->sizeBlocks=(buf[off+2]*256)+buf[off+1];
      DEBUG_OUT<<"size is "<<theFile->sizeBlocks<<" blocks\n";
      theFile->nativeType=buf[0];
      DEBUG_OUT<<"nativeType="<<theFile->nativeType<<"\n";
      theFile->type=DDISK_TYPE_BINARY; // TODO
      unsigned int firstTrack=firstBlock/numSectors;
      unsigned int firstSector=firstBlock-(firstTrack*numSectors);
      DEBUG_OUT<<"firstBlock="<<firstBlock<<" firstTrack="<<firstTrack<<" firstSector="<<firstSector<<"\n";
      DEBUG_OUT<<"(sanity check: block is "<<(firstTrack*numSectors)+firstSector<<")\n";
      theFile->track=firstTrack;
      theFile->sector=firstSector;
      theFile->data=(BYTE *)NULL;
      theFile->size=theFile->sizeBlocks*blockSize;
      theFile->block=0;
      theFile->curSeek=0;
      theDir.append((void *)theFile);
    }
    else DEBUG_OUT<<"Couldn't alloc file entry!\n";
  }
}


// NOTE: We override this because our first 3 sectors are ALWAYS 128 bytes long...
// TODO: What if there's no block 0?!?
void DImageAtari::readBlock(unsigned int blk)
{
  diskError=true;
  if(!imageData) {std::cerr<<"No imageData for disk image!\n"; return;}
  unsigned int tBlockSize=blockSize;
  unsigned int tBlk=blk;
  if(blk<4) {
    tBlockSize=128;
    curLoc=blk*tBlockSize+skipOffset;
  }
  else {
    tBlk=blk-3;
    curLoc=3*128;
    curLoc+=tBlk*blockSize+skipOffset;
  }
  if(curLoc>imageDataSize) { std::cerr<<"Past end of imageData!\n"; return; }
  DEBUG_OUT<<"DImageAtari::readBlock("<<blk<<") at offset "<<curLoc<<" tBlk="<<tBlk<<" tBlockSize="<<tBlockSize<<"\n";
  //DEBUG_OUT<<(int)0<<": ";
  for(unsigned int t=0;t<tBlockSize;t++) {
    buf[t]=imageData[curLoc+t];
    //DEBUG_OUT<<(int)buf[t]<<",";
    //if(t&&(!(t&0xf))) DEBUG_OUT<<"\n"<<(int)t<<": ";
  }
  //DEBUG_OUT<<"\n";
  diskError=false;
}


void DImageAtari::writeBlock(unsigned int blk)
{
  std::cerr<<"DImageAtari::writeBlock not implemented!\n";
  diskError=true;
}
