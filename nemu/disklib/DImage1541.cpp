
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT *aNullStream


#include <disklib/DImage1541.h>
#include <portable/nickcpp/ANullStream.h>
#include <portable/nickcpp/AStringHelper.h>


#define HIGH_BYTDImage1541(x)  ((x&0xff00)>>8)
#define LOW_BYTDImage1541(x)  (x&0xff)


static unsigned int numSectors1541[]=
{
  21,21,21,21,21,21,21,21,21, // Tracks 1-9
  21,21,21,21,21,21,21,21,    // Tracks 10-17
  19,19,19,19,19,19,19,       // Tracks 18-24
  18,18,18,18,18,18,          // Tracks 25-30
  17,17,17,17,17              // Tracks 31-35
};


static unsigned int numSectors1571[]=
{
  21,21,21,21,21,21,21,21,21, // Tracks 1-9
  21,21,21,21,21,21,21,21,    // Tracks 10-17
  19,19,19,19,19,19,19,       // Tracks 18-24
  18,18,18,18,18,18,          // Tracks 25-30
  17,17,17,17,17,              // Tracks 31-35
  21,21,21,21,21,21,21,21,21, // Tracks 1-9 (other side)
  21,21,21,21,21,21,21,21,    // Tracks 10-17 (other side)
  19,19,19,19,19,19,19,       // Tracks 18-24 (other side)
  18,18,18,18,18,18,          // Tracks 25-30 (other side)
  17,17,17,17,17              // Tracks 31-35 (other side)
};


static unsigned int trackOffset1541[]=
{
  0,21,42,63,84,105,126,147,168,    // Tracks 1-9
  189,210,231,252,273,294,315,336,  // Tracks 10-17
  357,376,395,414,433,452,471,      // Tracks 18-24
  490,508,526,544,562,580,          // Tracks 25-30
  598,615,632,649,666               // Tracks 31-35
  // For a total of 683 blocks
};


static unsigned int trackOffset1571[]=
{
  0,21,42,63,84,105,126,147,168,    // Tracks 1-9
  189,210,231,252,273,294,315,336,  // Tracks 10-17
  357,376,395,414,433,452,471,      // Tracks 18-24
  490,508,526,544,562,580,          // Tracks 25-30
  598,615,632,649,666,              // Tracks 31-35 (other side)
  // Tracks 1-9 (other side)
  0+683,21+683,42+683,63+683,84+683,105+683,126+683,147+683,168+683,
  // Tracks 10-17 (other side)
  189+683,210+683,231+683,252+683,273+683,294+683,315+683,336+683,
  // Tracks 18-24 (other side)
  357+683,37+6836,395+683,414+683,433+683,452+683,471+683,
  // Tracks 25-30 (other side)
  490+683,508+683,526+683,544+683,562+683,580+683,
  // Tracks 31-35 (other side)
  598+683,615+683,632+683,649+683,666+683
  // For a total of 683*2 blocks
};


////////////////////////////////////////////////////////////////////////////////
//  DImage1541 Class
////////////////////////////////////////////////////////////////////////////////

DImage1541::DImage1541(DVfs *parent) : DDiskImageMem(parent)
{
  init();
  dos=parent;
  constructorHelper();
}


DImage1541::DImage1541(DVfs *parent,const char *fname) : DDiskImageMem(parent,fname)
{
  init();
  dos=parent;
  constructorHelper();
  MountDiskFromFile(fname);
}


void DImage1541::constructorHelper()
{
  //if(w) DEBUG_OUT<<"DImage1541 disk image.\n";
  id1=0;  id2=0;  dos1=0;  dos2=0;
  // Set up for a default 1541 image...
  // This will be overridden if its a 1581, etc...
  numSectorsPerTrack=numSectors1541;
  offsetToTrack=trackOffset1541;
  cbmDirTrack=18;
  allTracksSameSectors=false;
  numTracks=35;  numSectors=21;  numSides=1;
  noTrackZero=true;
  blockSize=256;
  geos=false;
  cpm=false;
  for(unsigned int t=0;t<256;t++) { bam[t]=0; }
}


DImage1541::~DImage1541()
{
  freeDirCache();
  if(imageData) delete imageData;
  imageData=(BYTE *)NULL;
  diskMounted=false;
}


/* STATIC */
unsigned int DImage1541::recognize(BYTE *tbuf,FILE *f,unsigned int fsize)
{
  unsigned int ret=DDISK_FORMAT_RAW;  // We assume its good, and then fail it if we find inconsistancies
  if((fsize!=174848)&&(fsize!=533248)&&(fsize!=819200)) {
    DEBUG_OUT<<"(recognize: size was wrong for a 1541/1581/8050 image)\n";
    ret=DDISK_FORMAT_UNKNOWN;
  }
  else {
    DEBUG_OUT<<"(size looks right for 1541 or 1581 image...going to look more...)\n";
    unsigned int t=0;
    unsigned char sbuf[256];
    fseek(f,0x16500,SEEK_SET);
    fread(sbuf,256,1,f);
    DEBUG_OUT<<"first dir sect ";
    if((sbuf[0]==0x12)&&(sbuf[1]==0x1)) { DEBUG_OUT<<"looks good...\n"; }
    else { ret=DDISK_FORMAT_UNKNOWN; DEBUG_OUT<<"was "<<(unsigned int)sbuf[0]<<","<<(unsigned int)sbuf[1]<<"\n"; }
    DEBUG_OUT<<"disk dos version ";
    if(sbuf[2]==0x41) DEBUG_OUT<<"looks good...\n";
    else { ret=DDISK_FORMAT_UNKNOWN; DEBUG_OUT<<"was "<<(unsigned int)sbuf[2]<<"\n"; }
    DEBUG_OUT<<"first pad ";
    unsigned int p1=sbuf[0xa0],p2=sbuf[0xa1];
    if((p1==0xa0)&&(p2==0xa0)) DEBUG_OUT<<"looks good...\n";
    else { ret=DDISK_FORMAT_UNKNOWN; DEBUG_OUT<<"was "<<p1<<","<<p2<<"\n"; }
    bool flag=true;
    DEBUG_OUT<<"second pad ";
    for(t=0xa7;t<0xab;t++) { if(sbuf[t]!=0xa0) { ret=DDISK_FORMAT_UNKNOWN; DEBUG_OUT<<"mismatch at "<<t<<"\n"; flag=false; } }
    if(flag) DEBUG_OUT<<"looks good...\n";
    DEBUG_OUT<<"third pad ";
    for(t=0xd4;t<0xff;t++) { if(sbuf[t]!=0x00) { ret=DDISK_FORMAT_UNKNOWN; DEBUG_OUT<<"mismatch at "<<t<<"\n"; flag=false; } }
    if(flag) DEBUG_OUT<<"looks good...\n";
    if(ret==DDISK_FORMAT_RAW) DEBUG_OUT<<"Good enough, passed all our tests.\n";
  }
  return ret;
}


void DImage1541::ls(const char *dname)
{
  if(noFileSystem) { DEBUG_OUT<<"DImage1541::ls Non-DOS Disk!\n"; return; }
  //w->startMore();
  //w->dec();
  DEBUG_OUT<<"Disk: "<<diskName;
  DEBUG_OUT<<"  "<<id1<<id2<<" "<<dos1<<dos2<<"\n";
  theDir.reset();
  DDirEntry *aFile=(DDirEntry *)theDir.info();
  while(aFile) {
    unsigned int afsize=aFile->sizeBlocks;
    DEBUG_OUT<<afsize;
    if(afsize<100) DEBUG_OUT<<" ";
    if(afsize<10) DEBUG_OUT<<" ";
    DEBUG_OUT<<"  ";
    DEBUG_OUT<<aFile->name;
    unsigned int afnlen=strlen(aFile->name);
    for(unsigned int t=0;t<(24-afnlen);t++) DEBUG_OUT<<" ";
    switch(aFile->nativeType) {
      case 0:  DEBUG_OUT<<"DEL";  break;
      case 1:  DEBUG_OUT<<"SEQ";  break;
      case 2:  DEBUG_OUT<<"PRG";  break;
      case 3:  DEBUG_OUT<<"USR";  break;
      case 4:  DEBUG_OUT<<"REL";  break;
      case 5:  DEBUG_OUT<<"CBM";  break;
      case 6:  DEBUG_OUT<<"DIR";  break;
      default: DEBUG_OUT<<"???";  break;
    }
    DEBUG_OUT<<"\n";
    theDir.advance();
    aFile=(DDirEntry *)theDir.info();
  }
  // DEBUG_OUT<<blocksFree<<" blocks free.\n";
  //w->hex();
  //w->endMore();
}


void DImage1541::readDirectory()
{
  DEBUG_OUT<<"DImage1541::readDirectory...\n";
  freeDirCache();
  noFileSystem=false;
  // we might not have to do all this here...check out edisk.cpp
  numSectorsPerTrack=numSectors1541;
  offsetToTrack=trackOffset1541;
  allTracksSameSectors=false;
  numTracks=35;  numSectors=21;  numSides=1;
  cbmDirTrack=18;
  noTrackZero=true;
  blockSize=256;
  geos=false;
  cpm=false;
  switch(imageDataSize) {
    case 174848L:
      // A 1541, we're already set up...
      break;
    case 533248:
      // A 8050...fix things somewhat...
      // TODO: this isn't finished...
      break;
    case 819200L:
      // A 1581...fix things somewhat...
      allTracksSameSectors=true;
      numTracks=80;
      numSectors=39;
      numSides=2;
      cbmDirTrack=40;
      // TODO: this isn't finished...
      break;
    default:
     DEBUG_OUT<<"Can't read dir, disk size is weird!\n";
     noFileSystem=true;
     return;
     break;
  }
  blocksFree=0;
  unsigned int t=0,tt=0,ss=0;
  //
  // Read BAM (and header)
  DEBUG_OUT<<"DImage1541::readDirectory reading BAM (and header)...\n";
  readTrackSector(cbmDirTrack,0);  // At raw offset $16500
  for(t=0;t<256;t++) bam[t]=buf[t];
  // TODO: Use info here...
  /*
  // read bam
  // TODO: This is wrong for anything but a plain 1541 disk image...
  for(t=4;t<143;t+=4) {
    blocksFree+=popCount(buf[t+1]);
    blocksFree+=popCount(buf[t+2]);
    blocksFree+=popCount(buf[t+3]);
  }
  */
  //
  id1=buf[162];  id2=buf[163];  // At raw offset $165a2
  dos1=buf[165];  dos2=buf[166];  // At raw offset $165a5
  diskID[0]=id1;  diskID[1]=id2;  diskID[2]=32;
  diskID[3]=dos1;  diskID[4]=dos2;
  for(t=0;t<256;t++) diskName[t]=0;
  for(t=144;t<161;t++) {
    if(buf[t]!=160) diskName[t-144]=buf[t];
  }
  bool ww=true;
  while(ww) {
    tt=buf[0];  ss=buf[1];
    if(tt==0) ww=false;
    else {
      readTrackSector(tt,ss);
      if(diskError) {
        DEBUG_OUT<<"Can't read dir, disk error!\n";
        noFileSystem=true; return;
      }
      ULONG off;
      for(t=0;t<7;t++) {
        off=t*32+2;
        readImageDirEntry(off);
      }
    }
  }
}


void DImage1541::readImageDirEntry(ULONG off)
{
  DEBUG_OUT<<"DImage1541::readImageDirEntry("<<off<<")...\n";
  unsigned int t=0;
  if(buf[off]&0x7f) {
    DDirEntry *theFile=new DDirEntry;
    if(theFile) {
      theFile->signature=0xface;
      for(t=0;t<DDISK_MAX_NAMELEN;t++) theFile->name[t]=0;
      for(t=3;t<19;t++) {
        if(buf[off+t]!=160) theFile->name[t-3]=buf[off+t];
      }
      theFile->sizeBlocks=buf[off+28]+(buf[off+29]*256);
      theFile->nativeType=buf[off]&0x7f;
      switch(theFile->nativeType) {
        case 0:  theFile->type=DDISK_TYPE_NONE;  break;
        case 1:  theFile->type=DDISK_TYPE_TEXT;  break;
        case 2:  theFile->type=DDISK_TYPE_BASIC;  break;
        case 3:  theFile->type=DDISK_TYPE_BINARY;  break;
        case 4:  theFile->type=DDISK_TYPE_BINARY;  break;
        case 5:  theFile->type=DDISK_TYPE_DIR;  break;
        case 6:  theFile->type=DDISK_TYPE_DIR;  break;
        default:  theFile->type=DDISK_TYPE_NONE;  break;
      }
      theFile->track=buf[off+1];
      theFile->sector=buf[off+2];
      theFile->data=(BYTE *)NULL;
      theFile->size=theFile->sizeBlocks*256;
      theFile->block=0;
      theFile->curSeek=0;
      theDir.append((void *)theFile);
    }
    else DEBUG_OUT<<"Couldn't alloc file entry!\n";
  }
}


/*
// TODO: Redesign this...it only works if you read one and only one full sector at a time
unsigned int DImage1541::fread(void *ptr,unsigned int sizelem,unsigned int n,void *fp)
{
  if(!fp) return 0;
  if(!ptr) return 0;
  EMemMapAbstract *map=dos->getMap();
  DDirEntry *aFile=(DDirEntry *)fp;
  char *tptr=(char *)ptr;
  unsigned int tt=aFile->track,ss=aFile->sector;
  unsigned int wantSize=sizelem*n;
  unsigned int bytesRead=0,ssize,t;
  bool WW=true;
  ULONG offset=0,poff=0;
  while(WW) {
    if(bytesRead==wantSize) WW=false;
    else {
      readTrackSector(tt,ss);
      tt=buf[0];  ss=buf[1];
      ssize=254;
      offset=0;
      if(!tt) { ssize=ss;  WW=false; }  // At end of file...
      if(firstSector) { firstSector=false; offset=2; ssize-=2; }
      if((bytesRead+ssize)>wantSize) ssize=(bytesRead+ssize)-wantSize;
      for(t=0;t<ssize;t++) {
        tptr[t+poff]=buf[t+offset+2];
        //poff++;
        bytesRead++;
      }
    }
  }
  lastSeek+=bytesRead;
  return bytesRead;
}
*/


// NOTE: We read the whole file, start address included.
void DImage1541::cacheFile(void *fp)
{
  if(!fp) {
    //DEBUG_OUT<<"DImage1541 couldn't cache file, no fp!\n";
    return;
  }
  DDirEntry *aFile=(DDirEntry *)fp;
  if(aFile->data) free(aFile->data);
  aFile->data=(BYTE *)malloc(aFile->size);
  if(!aFile->data) {
     DEBUG_OUT<<"DImage1541 couldn't cache file, couldn't malloc!\n";
     return;
  }
  unsigned int tt=aFile->track,ss=aFile->sector;
  unsigned int ssize,t;
  unsigned int poff=0;
  bool working=true;
  while(working) {
    readTrackSector(tt,ss);
    if(diskError) {
      DEBUG_OUT<<"Can't cacheFile, disk error!\n";
      return;
    }
    tt=buf[0];  ss=buf[1];
    ssize=254;
    if(!tt) { ssize=ss;  working=false; }  // At end of file...
    for(t=0;t<ssize;t++) {
      aFile->data[poff]=buf[t+2];
      poff++;
    }
  }
}


