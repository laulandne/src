
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#include <disklib/DImageApple2.h>

#include <portable/nickcpp/ANullStream.h>

#include <portable/nickcpp/AStringHelper.h>


////////////////////////////////////////////////////////////////////////////////
//  DImageApple2 Class
////////////////////////////////////////////////////////////////////////////////

DImageApple2::DImageApple2(DVfs *parent) : DDiskImageMem(parent)
{
  init();
  dos=parent;
  constructorHelper();
}


DImageApple2::DImageApple2(DVfs *parent,const char *fname) : DDiskImageMem(parent,fname)
{
  init();
  dos=parent;
  constructorHelper();
  MountDiskFromFile(fname);
}


void DImageApple2::constructorHelper()
{
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"DImageApple2 disk image.\n";
#endif // DEBUG_VERBOSE
  prodos=false;
  thirteenSector=false;
  nibble=false;
  noFileSystem=false;
  numTracks=35;  numSectors=16;  numSides=1;  // Standard Disk2 floppy...
  blockSize=256;
  for(unsigned int t=0;t<256;t++) { vtoc[t]=0; }
}


DImageApple2::~DImageApple2()
{
  freeDirCache();
  if(imageData) delete imageData;
  imageData=(BYTE *)NULL;
  diskMounted=false;
}


/* STATIC */
unsigned int DImageApple2::recognize(BYTE *tbuf,FILE *f,unsigned int fsize)
{
  unsigned int ret=DDISK_FORMAT_UNKNOWN;
  /*
  if(size==143360) {
    // possibly a standard disk2 image...should check more...
    ret=DDISK_FORMAT_RAW;
  }
  if(size==232960) {
    // possibly a nybblized disk2 image...should check more...
    ret=DDISK_FORMAT_RAW;
  }
  */
  // TODO: Some more checking here before we recognize it
  return ret;
}


// NOTE: Is perfectly legit for this to fail...
void DImageApple2::readDirectory()
{
  DEBUG_OUT<<"DImageApple2::readDirectory...\n";
  freeDirCache();
  unsigned int t=0,tt=0,ss=0;
  for(t=0;t<256;t++) diskName[t]=0;
  strcpy(diskName,"NO NAME");
  //
  // Read VTOC
  DEBUG_OUT<<"DImageApple2::readDirectory reading VTOC...\n";
  readTrackSector(17,0);
  for(t=0;t<256;t++) vtoc[t]=buf[t];
  // TODO: Use info here...
  //
  bool ww=true;
  while(ww) {
    tt=buf[1];  ss=buf[2];
    DEBUG_OUT<<"Next Dir sector at "<<tt<<","<<ss<<"\n";
    if((tt==0)&&(ss==0)) ww=false;
    else {
      readTrackSector(tt,ss);
      if(diskError) {
        DEBUG_OUT<<"Can't read dir, disk error!\n"; noFileSystem=true; return;
      }
      ULONG off;
      for(t=0;t<6;t++) {
        off=t*35+11;
        readImageDirEntry(off);
      }
    }
  }
}


void DImageApple2::readImageDirEntry(ULONG off)
{
  if(prodos) readProdosDirEntry(off);
  else readDosDirEntry(off);
}


void DImageApple2::readDosDirEntry(ULONG off)
{
  DEBUG_OUT<<"DImageApple2::readDosDirEntry("<<off<<")...\n";
  unsigned int t=0;
  unsigned int fsize=buf[off+33]+(buf[off+34]*256);
  if(fsize) {
    DDirEntry *theFile=new DDirEntry;
    if(theFile) {
      theFile->signature=0xface;
      for(t=0;t<256;t++) theFile->name[t]=0;
      for(t=3;t<32;t++) {
        theFile->name[t-3]=buf[off+t]-128;
      }
      theFile->nativeType=buf[off+2]&0x7f;
      theFile->track=buf[off];
      theFile->sector=buf[off+1];
      theFile->sizeBlocks=fsize;
      theFile->data=(BYTE *)NULL;
      theFile->size=theFile->sizeBlocks*256;
      theFile->block=0;
      theFile->curSeek=0;
      theFile->type=DDISK_TYPE_TEXT;
      if(theFile->nativeType&1) {
        theFile->type=DDISK_TYPE_BASIC;
      }
      if(theFile->nativeType&2) {
        theFile->type=DDISK_TYPE_BASIC;
      }
      if(theFile->nativeType&4) {
        theFile->type=DDISK_TYPE_BINARY;
      }
      theDir.insert((void *)theFile);
    }
    else DEBUG_OUT<<"Couldn't alloc file entry!\n";
  }
}


void DImageApple2::readProdosDirEntry(ULONG off)
{
  DEBUG_OUT<<"DImageApple2::readProdosDirEntry("<<off<<")...\n";
  std::cerr<<"DImageApple2::readProdosDirEntry not implemented!\n";
  diskError=true;
}


void DImageApple2::ls(const char *dname)
{
  if(noFileSystem) { DEBUG_OUT<<"DImageApple2::ls Non-DOS Disk!\n"; return; }
  //w->startMore();
  //w->dec();
  DEBUG_OUT<<"Disk:\n";
  theDir.reset();
  DDirEntry *aFile=(DDirEntry *)theDir.info();
  while(aFile) {
    char tchar='T';
    if(aFile->nativeType&1) tchar='I';
    if(aFile->nativeType&2) tchar='A';
    if(aFile->nativeType&4) tchar='B';
    DEBUG_OUT<<tchar<<" ";
    unsigned int afsize=aFile->sizeBlocks;
    if(afsize<100) DEBUG_OUT<<"0";
    if(afsize<10) DEBUG_OUT<<"0";
    DEBUG_OUT<<afsize;
    DEBUG_OUT<<" "<<aFile->name;
    DEBUG_OUT<<"\n";
    theDir.advance();
    aFile=(DDirEntry *)theDir.info();
  }
  //w->hex();
  //w->endMore();
}


void DImageApple2::cacheFile(void *fp)
{
  DEBUG_OUT<<"DImageApple2::cacheFile not implemented!\n";
}


/* STATIC */
bool DImageApple2::encodeNibs(unsigned int tnum,BYTE *src,unsigned int ssize,BYTE *dest,unsigned int dsize)
{
  if(ssize!=EDISK2_TRACK_SIZE) {
    DEBUG_OUT<<"DImageApple2::encodeNibs, source buffer wrong size!\n";
    return false;
  }
  if(dsize!=EDISK2_NIB_TRACK_SIZE) {
    DEBUG_OUT<<"DImageApple2::encodeNibs, dest buffer wrong size!\n";
    return false;
  }
  if(tnum>35) {
    DEBUG_OUT<<"DImageApple2::encodeNibs, tnum too big!\n";
    return false;
  }
  unsigned int soffset=0,doffset=0;
  for(unsigned int t=0;t<16;t++) {
    encodeNibSector(tnum,t,src+soffset,dest+doffset);
    soffset+=EDISK2_DECODED_SECTOR_SIZE;
    doffset+=EDISK2_ENCODED_SECTOR_SIZE;
  }
  return true;
}


/* STATIC */
bool DImageApple2::decodeNibs(unsigned int tnum,BYTE *src,unsigned int ssize,BYTE *dest,unsigned int dsize)
{
  if(ssize!=EDISK2_NIB_TRACK_SIZE) {
    DEBUG_OUT<<"DImageApple2::encodeNibs, source buffer wrong size!\n";
    return false;
  }
  if(dsize!=EDISK2_TRACK_SIZE) {
    DEBUG_OUT<<"DImageApple2::encodeNibs, dest buffer wrong size!\n";
    return false;
  }
  if(tnum>35) {
    DEBUG_OUT<<"DImageApple2::decodeNibs, tnum too big!\n";
    return false;
  }
  unsigned int soffset=0,doffset=0;
  for(unsigned int t=0;t<16;t++) {
    decodeNibSector(tnum,t,src+soffset,dest+doffset);
    soffset+=EDISK2_ENCODED_SECTOR_SIZE;
    doffset+=EDISK2_DECODED_SECTOR_SIZE;
  }
  return true;
}


/* STATIC */
void DImageApple2::encodeNibSector(unsigned int tnum,unsigned int snum,BYTE *src,BYTE *dest)
{
  // NOTE: we write a gap BEFORE the address field, but none after the
  // data field...this is completely arbitrary.
  unsigned int t;
  // write gap 1, 40 bytes
  ULONG offset=0;
  for(t=0;t<40;t++) {
    dest[offset+t]=0xff;  // TODO: Is this right?  No 0 bits?
  }
  // write address field, 15 bytes
  offset=40;
  // prolog
  dest[offset]=0xd5; dest[offset+1]=0xaa; dest[offset+2]=0x96; offset+=3;
  // disk volume
  offset+=2;
  // track address
  offset+=2;
  // sector address
  offset+=2;
  // checksum
  offset+=2;
  // epilog
  dest[offset]=0xde; dest[offset+1]=0xaa; dest[offset+2]=0xeb; offset+=3;
  // write gap 2, 8 bytes
  offset=55;
  for(t=0;t<8;t++) {
    dest[offset+t]=0xff;  // TODO: Is this right?  No 0 bits?
  }
  // write data field, 349 bytes
  offset=63;
  // prolog
  dest[offset]=0xd5; dest[offset+1]=0xaa; dest[offset+2]=0xad; offset+=3;
  // the actual encoded data...
  for(t=0;t<342;t++) {
    offset++;
  }
  // checksum
  offset+=1;
  // epilog
  dest[offset]=0xde; dest[offset+1]=0xaa; dest[offset+2]=0xeb; offset+=3;
}


/* STATIC */
void DImageApple2::decodeNibSector(unsigned int tnum,unsigned int snum,BYTE *src,BYTE *dest)
{
  // TODO: maybe read gap 1, ? bytes
  // TODO: read address field, 15 bytes
  // TODO: read gap 2, ? bytes
  // TODO: read data field, 349 bytes
  // TODO: maybe read gap 3, ? bytes
}

