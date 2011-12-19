
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#include <disklib/DImageTrs80.h>

#include <portable/nickcpp/ANullStream.h>

#include <portable/nickcpp/AStringHelper.h>


/*
NOTES:

first sector is bootstrap
sectors 2-3 are "encoded copyright message"
sectors 4-5 are "tables"

directory is track 17
offset for that is 17*256*10=43520
sector 0 is "granuile allocation table"
sector 1 is "hash index table"
Remaining 8 are directory
32 bytes each, 8 per block, thus 64 files max

"hit" format
each filename has a byte
only first 8 bytes of each 32 byte segment is used
last hex digit plus 2 is sector of dir track where entry is
first hex digit times 16 is byte offset into that sector

32 byte entry format
either "file primary directory entry" or "file extention directory entry"

"fpde" format
0: type
1-2: unused
3: end of file
4: logical record length (only newer doses)
5-c: filename
d-f: file extension
10-13: "passwords"
14-15: "eof" relative sector
16-1f: five "extents"

Total of 89600 bytes 5.25in 35*10*256
Total of 509184 for 8in disk 77*26*256

*/


////////////////////////////////////////////////////////////////////////////////
//  DImageTrs80 Class
////////////////////////////////////////////////////////////////////////////////

DImageTrs80::DImageTrs80(DVfs *parent) : DDiskImageMem(parent)
{
  init();
  dos=parent;
  constructorHelper();
}


DImageTrs80::DImageTrs80(DVfs *parent,const char *fname) : DDiskImageMem(parent,fname)
{
  init();
  dos=parent;
  constructorHelper();
  MountDiskFromFile(fname);
}


void DImageTrs80::constructorHelper()
{
  noFileSystem=false;
  allTracksSameSectors=true;
  // Init for standard original disk
  numTracks=35;
  numSectors=10;
  blockSize=256;
  dmkImage=false;
  for(unsigned int t=0;t<256;t++) { gat[t]=0; hit[t]=0; }
}


DImageTrs80::~DImageTrs80()
{
  freeDirCache();
  diskMounted=false;
}


/* STATIC */
unsigned int DImageTrs80::recognize(BYTE *tbuf,FILE *f,unsigned int fsize)
{
  unsigned int ret=DDISK_FORMAT_UNKNOWN;
  /*
  if((size==228496)||(size==261136)||(size==267664)) {
    // possibly a dmk...should check more...
    ret=true;
  }
  if(size==204800) {
    // possibly an 80ssd jv1...should check more...
    ret=true;
  }
  */
  // TODO: Some more checking here before we recognize it
  return ret;
}


void DImageTrs80::readDirectory()
{
  DEBUG_OUT<<"DImageTrs80::readDirectory...\n";
  freeDirCache();
  switch(imageDataSize)
  {
    case 204800:
      // Let's guess an 80 track jv1 image...
      numTracks=80;
      DEBUG_OUT<<"DImageTrs80::readDirectory guessing an 80 track sssd jv1 image...\n";
      break;
    case 228496:
    case 261136:
    case 267664:
      dmkImage=true;
      std::cerr<<"DImageTrs80::readDirectory DMK images not implemented!\n";
      break;
    default:
      std::cerr<<"DImageTrs80::readDirectory can't figure out image format!\n";
      diskError=true;
      break;
  }
  unsigned int t=0;
  for(t=0;t<256;t++) diskName[t]=0;
  strcpy(diskName,"NO NAME");
  //
  // Read GAT
  DEBUG_OUT<<"DImageTrs80::readDirectory reading GAT...\n";
  readTrackSector(17,0);
  for(t=0;t<256;t++) gat[t]=buf[t];
  // TODO: Use info here...
  //
  // Read HIT
  DEBUG_OUT<<"DImageTrs80::readDirectory reading HHIT...\n";
  readTrackSector(17,1);
  for(t=0;t<256;t++) hit[t]=buf[t];
  // TODO: Use info here...
  //
  // Now read directory entries
  // Sectors 2-10
  for(t=0;t<8;t++) {
    readTrackSector(17,2+t);
    // 8 entries per block
    for(unsigned int i=0;i<8;i++) {
      readImageDirEntry(i*32);
    }
  }
}


void DImageTrs80::cacheFile(void *fp)
{
  std::cerr<<"DImageTrs80::cacheFile not implemented!\n";
  diskError=true;
}


// TODO: filenames have trailing spaces...
void DImageTrs80::readImageDirEntry(ULONG off)
{
  DEBUG_OUT<<"DImageTrs80::readImageDirEntry("<<off<<")...\n";
  // Only valid if type is non-zero
  if(buf[off]) {
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
      theFile->sizeBlocks=0;  // TODO: use GAT and HIT?!?
      theFile->nativeType=buf[0];
      DEBUG_OUT<<"nativeType="<<theFile->nativeType<<"\n";
      theFile->type=DDISK_TYPE_BINARY; // TODO
      theFile->track=0; // TODO: use GAT and HIT?!?
      theFile->sector=0; // TODO: use GAT and HIT?!?
      theFile->data=(BYTE *)NULL;
      theFile->size=theFile->sizeBlocks*blockSize;
      theFile->block=0;
      theFile->curSeek=0;
      theDir.append((void *)theFile);
    }
    else DEBUG_OUT<<"Couldn't alloc file entry!\n";
  }
}
